// Copyright 2022 The Pigweed Authors
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
#include "app_common/common.h"
#include "kudzu_buttons_null/buttons.h"
#include "pw_color/color.h"
#include "pw_display/display.h"
#include "pw_display_driver_null/display_driver.h"
#include "pw_status/try.h"
#include "pw_thread/thread.h"
#include "pw_thread_stl/options.h"
#include "pw_touchscreen_null/touchscreen.h"

using pw::Status;
using pw::color::color_rgb565_t;
using pw::framebuffer::PixelFormat;
using pw::framebuffer_pool::FramebufferPool;

using Buttons = kudzu::ButtonsNull;

namespace {

constexpr uint16_t kDisplayScaleFactor = 2;
constexpr pw::geometry::Size<uint16_t> kFramebufferDimensions = {
    .width = DISPLAY_WIDTH / kDisplayScaleFactor,
    .height = DISPLAY_HEIGHT / kDisplayScaleFactor,
};
constexpr size_t kNumPixels =
    kFramebufferDimensions.width * kFramebufferDimensions.height;
constexpr uint16_t kFramebufferRowBytes =
    sizeof(color_rgb565_t) * kFramebufferDimensions.width;
constexpr pw::geometry::Size<uint16_t> kDisplaySize = {DISPLAY_WIDTH,
                                                       DISPLAY_HEIGHT};

color_rgb565_t s_pixel_data[kNumPixels];
const pw::Vector<void*, 1> s_pixel_buffers{s_pixel_data};
FramebufferPool s_fb_pool({
    .fb_addr = s_pixel_buffers,
    .dimensions = kFramebufferDimensions,
    .row_bytes = kFramebufferRowBytes,
    .pixel_format = PixelFormat::RGB565,
});
pw::display_driver::DisplayDriverNULL s_display_driver;

}  // namespace

// static
Status Common::EndOfFrameCallback() { return pw::OkStatus(); }

Status Common::Init() { return s_display_driver.Init(); }

// static
pw::display::Display& Common::GetDisplay() {
  static pw::display::Display s_display(
      s_display_driver, kDisplaySize, s_fb_pool);
  return s_display;
}

pw::touchscreen::Touchscreen& Common::GetTouchscreen() {
  static pw::touchscreen::TouchscreenNull s_touchscreen =
      pw::touchscreen::TouchscreenNull();
  return s_touchscreen;
}

kudzu::Buttons& Common::GetButtons() {
  static Buttons s_buttons = Buttons();
  return s_buttons;
}

const pw::thread::Options& Common::DisplayDrawThreadOptions() {
  static pw::thread::stl::Options display_draw_thread_options;
  return display_draw_thread_options;
}

const pw::thread::Options& Common::TouchscreenThreadOptions() {
  static pw::thread::stl::Options display_draw_thread_options;
  return display_draw_thread_options;
}
