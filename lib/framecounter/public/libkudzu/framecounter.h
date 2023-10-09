#pragma once

#include <stdint.h>

#include "pw_ring_buffer/prefixed_entry_ring_buffer.h"

namespace kudzu {

class FrameCounter {
 public:
  FrameCounter();

  void StartFrame();
  void EndDraw();
  void EndFlush();
  void EndFrame();

  uint32_t frame_start_millis;
  uint32_t frames;
  uint32_t start;
  float delta_time;
  int frames_per_second;
  std::byte draw_buffer[30 * sizeof(uint32_t)];
  std::byte flush_buffer[30 * sizeof(uint32_t)];
  pw::ring_buffer::PrefixedEntryRingBuffer draw_times;
  pw::ring_buffer::PrefixedEntryRingBuffer flush_times;
};

/// Given a ring buffer full of uint32_t values, return the average value or
/// zero if empty (or iteration error).
uint32_t CalcAverageUint32Value(
    pw::ring_buffer::PrefixedEntryRingBuffer& ring_buffer);

}  // namespace kudzu
