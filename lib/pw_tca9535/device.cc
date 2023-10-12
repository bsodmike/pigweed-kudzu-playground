// Copyright 2023 The Pigweed Authors
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License. You may obtain a copy of
// the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations under
// the License.

#include "pw_tca9535/device.h"

#include <chrono>
#include <cstddef>
#include <cstdint>

#define PW_LOG_MODULE_NAME "pw_tca9535"
#define PW_LOG_LEVEL PW_LOG_LEVEL_DEBUG

#include "pw_bytes/bit.h"
#include "pw_i2c/address.h"
#include "pw_i2c/register_device.h"
#include "pw_log/log.h"
#include "pw_status/status.h"

using ::pw::Status;
using namespace std::chrono_literals;

namespace pw::tca9535 {

namespace {

constexpr pw::i2c::Address kAddress = pw::i2c::Address::SevenBit<0x20>();

}  // namespace

Device::Device(pw::i2c::Initiator& initiator)
    : initiator_(initiator),
      device_(initiator,
              kAddress,
              endian::little,
              pw::i2c::RegisterAddressSize::k1Byte) {}

Status Device::Enable() {
  // Port 0 is all button input.
  device_.WriteRegister8(
      0x6, 0xff, pw::chrono::SystemClock::for_at_least(10ms));
  // Port 1 pins 6 and 7 are DISP_RESET and TOUCH_RESET which should be high.
  device_.WriteRegister8(
      0x3, 0xff, pw::chrono::SystemClock::for_at_least(10ms));
  device_.WriteRegister8(
      0x7, 0x3f, pw::chrono::SystemClock::for_at_least(10ms));

  return OkStatus();
}

Status Device::Probe() {
  pw::Status probe_result(initiator_.ProbeDeviceFor(
      kAddress, pw::chrono::SystemClock::for_at_least(10ms)));

  if (probe_result != pw::OkStatus()) {
    PW_LOG_DEBUG("TCA9535 Probe Failed");
  } else {
    PW_LOG_DEBUG("TCA9535 Probe Ok");
  }
  return probe_result;
}

void Device::LogControllerInfo() {
  auto port0 =
      device_.ReadRegister8(0x0, pw::chrono::SystemClock::for_at_least(10ms));
  if (port0.ok()) {
    PW_LOG_INFO("port 0: %02x", *port0);
  } else {
    PW_LOG_INFO("port 0 read failed");
  }

  auto port1 =
      device_.ReadRegister8(0x1, pw::chrono::SystemClock::for_at_least(10ms));
  if (port1.ok()) {
    PW_LOG_INFO("port 1: %02x", *port1);
  } else {
    PW_LOG_INFO("port 1 read failed");
  }
}

}  // namespace pw::tca9535
