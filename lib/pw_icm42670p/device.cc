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

#include "pw_icm42670p/device.h"

#include <chrono>
#include <cstddef>
#include <cstdint>

#define PW_LOG_MODULE_NAME "pw_icm42670p"
#define PW_LOG_LEVEL PW_LOG_LEVEL_DEBUG

#include "pw_bytes/bit.h"
#include "pw_i2c/address.h"
#include "pw_i2c/register_device.h"
#include "pw_log/log.h"
#include "pw_status/status.h"

using ::pw::Status;
using namespace std::chrono_literals;

namespace pw::icm42670p {

namespace {

constexpr pw::i2c::Address kAddress = pw::i2c::Address::SevenBit<0x68>();

void ImuReadReg(pw::i2c::RegisterDevice& device,
                uint8_t addr,
                const char* name) {
  auto data =
      device.ReadRegister8(addr, pw::chrono::SystemClock::for_at_least(10ms));

  if (data.ok()) {
    PW_LOG_INFO("%s: %02x", name, *data);
  } else {
    PW_LOG_INFO("failed to read %s", name);
  }
}

void ImuReadReg16(pw::i2c::RegisterDevice& device,
                  uint8_t addr,
                  const char* name) {
  auto data =
      device.ReadRegister16(addr, pw::chrono::SystemClock::for_at_least(10ms));

  if (data.ok()) {
    PW_LOG_INFO("%s: %04x", name, *data);
  } else {
    PW_LOG_INFO("failed to read %s", name);
  }
}

}  // namespace

Device::Device(pw::i2c::Initiator& initiator)
    : initiator_(initiator),
      device_(initiator,
              kAddress,
              endian::little,
              pw::i2c::RegisterAddressSize::k1Byte) {}

Status Device::Enable() {
  device_.WriteRegister8(
      0x1f, 0x0f, pw::chrono::SystemClock::for_at_least(10ms));

  return OkStatus();
}

Status Device::Probe() {
  pw::Status probe_result(initiator_.ProbeDeviceFor(
      kAddress, pw::chrono::SystemClock::for_at_least(50ms)));

  if (probe_result != pw::OkStatus()) {
    PW_LOG_DEBUG("ICM-42670-P Probe Failed");
  } else {
    PW_LOG_DEBUG("ICM-42670-P Probe Ok");
  }
  return probe_result;
}

void Device::LogControllerInfo() {
  device_.WriteRegister8(
      0x1f, 0x0f, pw::chrono::SystemClock::for_at_least(10ms));

  ImuReadReg(device_, 0x75, "WHO_AM_I");
  ImuReadReg(device_, 0x1f, "PWR_MGMT0");
  ImuReadReg(device_, 0x0c, "X0");
  ImuReadReg(device_, 0x0b, "X1");
}

}  // namespace pw::icm42670p
