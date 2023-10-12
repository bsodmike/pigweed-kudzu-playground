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

#include "pw_banner46x10.h"

#define PW_LOG_MODULE_NAME "Badge"
#define PW_LOG_LEVEL PW_LOG_LEVEL_DEBUG

#include "app_common/common.h"
#include "graphics/surface.hpp"
#include "heart_8x8.h"
#include "libkudzu/framecounter.h"
#include "libkudzu/random.h"
#include "pw_assert/assert.h"
#include "pw_assert/check.h"
#include "pw_banner46x10.h"
#include "pw_board_led/led.h"
#include "pw_color/color.h"
#include "pw_color/colors_endesga32.h"
#include "pw_color/colors_pico8.h"
#include "pw_display/display.h"
#include "pw_draw/draw.h"
#include "pw_framebuffer/framebuffer.h"
#include "pw_log/log.h"
#include "pw_logo5x7.h"
#include "pw_math/vector2.h"
#include "pw_spin_delay/delay.h"
#include "pw_string/string_builder.h"
#include "pw_sys_io/sys_io.h"
#include "pw_system/target_hooks.h"
#include "pw_system/work_queue.h"
#include "pw_thread/detached_thread.h"
#include "pw_touchscreen/touchscreen.h"

using pw::color::color_rgb565_t;
using pw::color::colors_pico8_rgb565;
using pw::display::Display;
using pw::framebuffer::Framebuffer;
using pw::math::Size;
using pw::math::Vector2;
using pw::touchscreen::Touchscreen;

namespace {

constexpr float pi = 3.14159265358979323846f;
constexpr float twopi = 2 * pi;
constexpr float angle_step = twopi / 120;
float angle = 0;

// Draw the pigweed text banner.
// Returns the bottom Y coordinate of the bottommost pixel set.
int DrawBanner(Framebuffer& framebuffer) {
  constexpr std::array<std::wstring_view, 2> banner = {
      L"Hello World from KUDZU!",
  };

  Vector2<int> tl = {12, 32};

  int color_index = 0;
  const float y_scale = 4.0;
  const float x_scale = 1.0;
  const float max_x_offset = 4.0;
  const float max_y_offset = pw::draw::font6x8.height * 2.0;

  for (auto text_row : banner) {
    int column = 0;
    for (auto text_char : text_row) {
      Vector2<int> position = {tl.x, tl.y};

      float final_angle = angle + (y_scale * column * angle_step);
      if (final_angle > twopi)
        final_angle -= twopi;
      if (final_angle < 0)
        final_angle += twopi;

      float offset_y = std::sin(final_angle);
      float offset_x = std::cos(angle + (x_scale * column * angle_step));

      position.x += round(max_x_offset * offset_x);
      position.y += round(max_y_offset * offset_y);

      pw::draw::DrawCharacter(text_char,
                              position,
                              pw::color::colors_pico8_rgb565[color_index + 8],
                              0,
                              pw::draw::font6x8,
                              framebuffer);

      if (text_char != ' ') {
        // Loop through these pico8 colors:
        // 0xf809,  // #ff004d 8 RED
        // 0xfd00,  // #ffa300 9 ORANGE
        // 0xff64,  // #ffec27 10 YELLOW
        // 0x0726,  // #00e436 11 GREEN
        // 0x2d7f,  // #29adff 12 BLUE
        // 0x83b3,  // #83769c 13 INDIGO
        // 0xfbb5,  // #ff77a8 14 PINK
        // 0xfe75,  // #ffccaa 15 PEACH
        color_index = (color_index + 1) % 7;
      }

      tl.x += pw::draw::font6x8.width;
      column += 1;
    }
    tl.x = 0;
    tl.y += pw::draw::font6x8.height;
  }

  return tl.y - pw::draw::font6x8.height;
}

void MainTask(void*) {
  kudzu::FrameCounter frame_counter = kudzu::FrameCounter();

  pw::board_led::Init();
  PW_CHECK_OK(Common::Init());

  Display& display = Common::GetDisplay();
  Framebuffer framebuffer = display.GetFramebuffer();
  PW_ASSERT(framebuffer.is_valid());

  blit::Surface screen = blit::Surface(
      (uint8_t*)framebuffer.data(),
      blit::PixelFormat::RGB565,
      blit::Size(framebuffer.size().width, framebuffer.size().height));
  screen.pen = blit::Pen(0, 0, 0, 255);
  screen.clear();

  display.ReleaseFramebuffer(std::move(framebuffer));

  Touchscreen& touchscreen = Common::GetTouchscreen();

  // The display loop.
  while (1) {
    frame_counter.StartFrame();

    angle += angle_step;
    if (angle >= twopi) {
      angle = 0;
    }

    pw::touchscreen::TouchEvent touch_event = touchscreen.GetTouchPoint();

    framebuffer = display.GetFramebuffer();
    PW_ASSERT(framebuffer.is_valid());
    screen.data = (uint8_t*)framebuffer.data();

    // Draw Phase
    // Clear the screen
    screen.pen = blit::Pen(0, 0, 0);
    screen.clear();

    // Draw animation
    DrawBanner(framebuffer);

    std::string text = "Nice to meet you\n Made with * by";
    auto text_size = screen.measure_text(text, blit::minimal_font, true);
    blit::Rect text_rect(
        blit::Point((screen.bounds.w / 2) - (text_size.w / 2),
                    (screen.bounds.h * .75) - (text_size.h / 2)),
        text_size);
    screen.pen = blit::Pen(0xFF, 0xFF, 0xFF);
    screen.text(
        text, blit::minimal_font, text_rect, true, blit::TextAlign::top_left);

    pw::draw::DrawSprite(framebuffer,
                         text_rect.x + text_rect.w - 29,
                         text_rect.y + text_rect.h - 9,
                         &heart_8x8_sprite_sheet,
                         1);
    pw::draw::DrawSprite(framebuffer,
                         text_rect.x + 10,
                         text_rect.y + text_rect.h + 2,
                         &pw_logo5x7_sprite_sheet,
                         1);
    pw::draw::DrawSprite(framebuffer,
                         text_rect.x + 10 + 7,
                         text_rect.y + text_rect.h,
                         &pw_banner46x10_sprite_sheet,
                         1);

    if (touch_event.type == pw::touchscreen::TouchEventType::Start ||
        touch_event.type == pw::touchscreen::TouchEventType::Drag) {
      pw::draw::DrawCircle(
          framebuffer,
          // TODO(tonymd): Touchscreen should understand how
          // the screen is rotated.
          round(((float)touch_event.point.y / 320.0) * 160.0),
          round(((240.0 - (float)touch_event.point.x) / 240.0) * 120.0),
          18,
          colors_pico8_rgb565[COLOR_BLUE],
          false);
    }

    // Update timers
    frame_counter.EndDraw();

    display.ReleaseFramebuffer(std::move(framebuffer));
    frame_counter.EndFlush();

    // Every second make a log message.
    frame_counter.EndFrame();
  }
}

}  // namespace

namespace pw::system {

void UserAppInit() {
  PW_LOG_INFO("UserAppInit");
  pw::thread::DetachedThread(Common::DisplayDrawThreadOptions(), MainTask);
}

}  // namespace pw::system