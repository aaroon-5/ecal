#include "ecal/os.h"

#include <map>
#include <chrono>
#include <string>

namespace eCAL {
   class ECAL_API_CLASS My_timestamps {
      public:
         // Reset index to zero
         ECAL_API_EXPORTED_MEMBER
            static void reset_index(void);
         // Increase index to count
         ECAL_API_EXPORTED_MEMBER
            static void increase_index(void);
         // Save a timestamp
         ECAL_API_EXPORTED_MEMBER
            static void make_timestamp(std::string tag);
         // Print all collected timestamps
         ECAL_API_EXPORTED_MEMBER
            static void print_timestamps(void);


      private:
         // Map to collect timestamps
         static std::map<std::string, long long> m_timestamps;
         // Index to count iterations
         static int m_idx;

         // Get the time in nanoseconds
         static long long get_time_ns(void);
   };
}