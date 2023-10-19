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
#include <cstdint>

#define PW_LOG_MODULE_NAME "SnakeGame"
#define PW_LOG_LEVEL PW_LOG_LEVEL_DEBUG

#include "app_common/common.h"
#include "graphics/surface.hpp"
#include "libkudzu/framecounter.h"
#include "pw_assert/check.h"
#include "pw_board_led/led.h"
#include "pw_display/display.h"
#include "pw_framebuffer/framebuffer.h"
#include "pw_log/log.h"
#include "pw_spin_delay/delay.h"
#include "pw_system/target_hooks.h"
#include "pw_system/work_queue.h"
#include "pw_thread/detached_thread.h"
#include "pw_touchscreen/buttons.h"
#include "pw_touchscreen/touchscreen.h"
#include "snake/game.h"

namespace {

void MainTask(void*) {
  pw::board_led::Init();
  PW_CHECK_OK(Common::Init());

  pw::display::Display& display = Common::GetDisplay();
  pw::framebuffer::Framebuffer framebuffer = display.GetFramebuffer();
  PW_CHECK(framebuffer.is_valid());

  blit::Surface screen = blit::Surface(
      static_cast<uint8_t*>(framebuffer.data()),
      blit::PixelFormat::RGB565,
      blit::Size(framebuffer.size().width, framebuffer.size().height));
  screen.pen = blit::Pen(0, 0, 0, 255);
  screen.clear();

  const auto display_width = framebuffer.size().width;
  const auto display_height = framebuffer.size().height;
  display.ReleaseFramebuffer(std::move(framebuffer));

  snake::Game game(display_width, display_height);
  pw::touchscreen::DirectionTouchButtonsThread touch_buttons_thread{
      game, Common::GetTouchscreen(), display_width, display_height};
  pw::thread::DetachedThread(Common::TouchscreenThreadOptions(),
                             touch_buttons_thread);

  game.Start();

  // Display and app loop.
  kudzu::FrameCounter frame_counter = kudzu::FrameCounter();
  uint32_t frame_start_millis = pw::spin_delay::Millis();
  while (1) {
    frame_counter.StartFrame();

    // Get frame buffer.
    framebuffer = display.GetFramebuffer();
    PW_CHECK(framebuffer.is_valid());
    screen.data = static_cast<uint8_t*>(framebuffer.data());
    // Clear the screen
    screen.pen = blit::Pen(0, 0, 0);
    screen.clear();

    // Let game draw its frame.
    game.OnFrame(framebuffer);

    // Update timers
    frame_counter.EndDraw();

    display.ReleaseFramebuffer(std::move(framebuffer));
    frame_counter.EndFlush();

    // Every second make a log message.
    frame_counter.EndFrame();

    if (pw::spin_delay::Millis() > frame_start_millis + 10000) {
      Common::EndOfFrameCallback();
      frame_start_millis = pw::spin_delay::Millis();
    }
  }
}

}  // namespace

namespace pw::system {

void UserAppInit() {
  PW_LOG_INFO("UserAppInit");
  pw::thread::DetachedThread(Common::DisplayDrawThreadOptions(), MainTask);
}

}  // namespace pw::system
