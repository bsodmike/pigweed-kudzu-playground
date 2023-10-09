#include "libkudzu/framecounter.h"

#define PW_LOG_LEVEL PW_LOG_LEVEL_DEBUG
#define PW_LOG_MODULE_NAME "FrameCounter"

#include <cstdint>
#include <cstring>

#include "pw_log/log.h"
#include "pw_ring_buffer/prefixed_entry_ring_buffer.h"
#include "pw_spin_delay/delay.h"

namespace kudzu {

FrameCounter::FrameCounter() {
  frame_start_millis = pw::spin_delay::Millis();
  frames = 0;
  frames_per_second = 0;
  draw_times.SetBuffer(draw_buffer);
  flush_times.SetBuffer(flush_buffer);
}

// TODO(b/304282368): Switch to pw_chrono and delete pw_spin_delay
void FrameCounter::StartFrame() { start = pw::spin_delay::Millis(); }

void FrameCounter::EndDraw() {
  uint32_t end = pw::spin_delay::Millis();
  uint32_t time = end - start;
  draw_times.PushBack(pw::as_bytes(pw::span{std::addressof(time), 1}));
  start = end;
}

void FrameCounter::EndFlush() {
  uint32_t time = pw::spin_delay::Millis() - start;
  flush_times.PushBack(pw::as_bytes(pw::span{std::addressof(time), 1}));
}

void FrameCounter::EndFrame() {
  frames++;
  if (pw::spin_delay::Millis() > frame_start_millis + 1000) {
    frames_per_second = frames;
    frames = 0;
    PW_LOG_INFO("FPS:%d, Draw:%dms, Flush:%dms",
                frames_per_second,
                (int)CalcAverageUint32Value(draw_times),
                (int)CalcAverageUint32Value(flush_times));
    frame_start_millis = pw::spin_delay::Millis();
  }
}

uint32_t CalcAverageUint32Value(
    pw::ring_buffer::PrefixedEntryRingBuffer& ring_buffer) {
  uint64_t sum = 0;
  uint32_t count = 0;
  for (const auto& entry_info : ring_buffer) {
    PW_ASSERT(entry_info.buffer.size() == sizeof(uint32_t));
    uint32_t val;
    std::memcpy(&val, entry_info.buffer.data(), sizeof(val));
    sum += val;
    count++;
  }
  return count == 0 ? 0 : sum / count;
}

}  // namespace kudzu
