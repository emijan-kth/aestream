#include <chrono>
#include <string>
#include <thread>

#include "../aedat4.hpp"
#include "../generator.hpp"
#include "file.hpp"

#include <iostream>

Generator<AEDAT::PolarityEvent> file_event_generator(const std::string filename,
                                                     const std::atomic<bool> &runFlag,
                                                     bool ignore_time) {
  AEDAT4 aedat_file;

  std::vector<AEDAT::PolarityEvent> polarity_events_from_txt_file;

  std::vector<AEDAT::PolarityEvent> *polarity_events = nullptr;

  if (filename.ends_with(".txt")) {
    std::ifstream input_stream(filename);

    std::string placeholder;

    while(input_stream >> placeholder)
    {
      uint64_t timestamp;
      uint16_t x;
      uint16_t y;
      bool polarity;
      input_stream >> timestamp >> x >> y >> polarity;
      polarity_events_from_txt_file.emplace_back(timestamp, x, y, true, polarity);
    }

    polarity_events = &polarity_events_from_txt_file;
  } else if (filename.ends_with(".aedat4")) {
    aedat_file.load(filename);
    polarity_events = &aedat_file.polarity_events;
  } else {
    std::stringstream error;
    error << "Unsupported file ending" << filename;
    throw std::invalid_argument(error.str());
  }

  const auto time_start = std::chrono::high_resolution_clock::now();
  const int64_t time_start_us = (*polarity_events)[0].timestamp;

  for (auto event : *polarity_events) {
    if (!runFlag.load()) break;
    
    // Sleep to align with real-time, unless ignore_time is set
    if (!ignore_time) {
      const int64_t time_diff =
          std::chrono::duration_cast<std::chrono::microseconds>(
              std::chrono::high_resolution_clock::now() - time_start)
              .count();
      const int64_t file_diff = event.timestamp - time_start_us;
      const int64_t time_offset = file_diff - time_diff;
      if (time_offset > 1000) {
        std::this_thread::sleep_for(std::chrono::microseconds(time_offset));
      }
    }
    co_yield event;
  }
}