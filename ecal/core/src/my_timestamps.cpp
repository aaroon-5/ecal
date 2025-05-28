#include "ecal/my_timestamps.h"

#include <iostream>

namespace eCAL {

  std::map<std::string, long long> My_timestamps::m_timestamps;
  int My_timestamps::m_idx;

  void My_timestamps::reset_index() {
    m_idx = 0;
  }

  void My_timestamps::increase_index() {
    m_idx++;
  }

  void My_timestamps::make_timestamp(std::string tag) {
    m_timestamps.insert({tag + std::to_string(m_idx), get_time_ns()});
  }

  void My_timestamps::print_timestamps() {
    for (auto itm : m_timestamps) {
      std::cout << itm.first << ";" << itm.second << std::endl;
    }
  }

  long long My_timestamps::get_time_ns(void) {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
  }
  
}