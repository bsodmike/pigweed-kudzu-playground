#define PW_LOG_MODULE_NAME "BLINKY"

#include "kudzu_blinky/service.h"

#include "pw_assert/check.h"

namespace kudzu::blinky {

void BlinkyService::Init(pw::async2::Dispatcher& dispatcher,
                         pw::Allocator& allocator,
                         MonochromeLed& monochrome_led,
                         PolychromeLed& polychrome_led) {
  blinky_.Init(dispatcher, allocator, monochrome_led, polychrome_led);
  // Start binking once every 1000ms.
  PW_CHECK_OK(blinky_.Blink(/*blink_count=*/0, /*interval_ms=*/1000));
}

pw::Status BlinkyService::ToggleLed(const pw_protobuf_Empty&,
                                    pw_protobuf_Empty&) {
  blinky_.Toggle();
  return pw::OkStatus();
}

pw::Status BlinkyService::SetLed(const blinky_SetLedRequest& request,
                                 pw_protobuf_Empty&) {
  blinky_.SetLed(request.on);
  return pw::OkStatus();
}

pw::Status BlinkyService::IsIdle(const pw_protobuf_Empty&,
                                 blinky_BlinkIdleResponse& response) {
  response.is_idle = blinky_.IsIdle();
  return pw::OkStatus();
}

pw::Status BlinkyService::Blink(const blinky_BlinkRequest& request,
                                pw_protobuf_Empty&) {
  uint32_t interval_ms = request.interval_ms == 0 ? Blinky::kDefaultIntervalMs
                                                  : request.interval_ms;
  uint32_t blink_count = request.has_blink_count ? request.blink_count : 0;
  return blinky_.Blink(blink_count, interval_ms);
}

pw::Status BlinkyService::Pulse(const blinky_CycleRequest& request,
                                pw_protobuf_Empty&) {
  uint32_t interval_ms = request.interval_ms == 0 ? 1000 : request.interval_ms;
  blinky_.Pulse(interval_ms);
  return pw::OkStatus();
}

pw::Status BlinkyService::SetRgb(const blinky_RgbRequest& request,
                                 pw_protobuf_Empty&) {
  uint8_t red = static_cast<uint8_t>(request.hex >> 16);
  uint8_t green = static_cast<uint8_t>(request.hex >> 8);
  uint8_t blue = static_cast<uint8_t>(request.hex);
  uint8_t brightness = static_cast<uint8_t>(request.brightness);
  blinky_.SetRgb(red, green, blue, brightness);
  return pw::OkStatus();
}

pw::Status BlinkyService::Rainbow(const blinky_CycleRequest& request,
                                  pw_protobuf_Empty&) {
  uint32_t interval_ms = request.interval_ms == 0 ? 1000 : request.interval_ms;
  blinky_.Rainbow(interval_ms);
  return pw::OkStatus();
}

}  // namespace kudzu::blinky