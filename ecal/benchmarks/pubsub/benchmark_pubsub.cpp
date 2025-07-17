/* ========================= eCAL LICENSE =================================
 *
 * Copyright (C) 2016 - 2025 Continental Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ========================= eCAL LICENSE =================================
*/

#include <ecal/ecal.h>
#include <ecal/pubsub/publisher.h>
#include <ecal/pubsub/subscriber.h>
#include <benchmark/benchmark.h>

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

#define REGISTRATION_DELAY_MS   2000
#define ACK_TIMEOUT_MS          50

#define RANGE_MULTIPLIER        1<<6
#define RANGE_START             1
#define RANGE_LIMIT             1<<24

#define WARMUP_TIME_S           2
#define REPETITIONS             10


/*
 *
 * Benchmarking the eCAL send process
 * 
*/
namespace Send {
  // Benchmark function
  static void BM_eCAL_Send(benchmark::State& state) {
    // Create payload to send, size depends on current argument
    size_t payload_size = state.range(0);
    std::vector<char> content_vector(payload_size);
    char* content_addr = content_vector.data();

    // Initialize eCAL and create sender
    eCAL::Initialize("Benchmark");
    eCAL::CPublisher publisher("benchmark_topic");

    // Create receiver in a different thread
    std::thread receiver_thread([]() { 
      eCAL::CSubscriber subscriber("benchmark_topic");
      while(eCAL::Ok()) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
    } );
    
    // Wait for eCAL synchronization
    std::this_thread::sleep_for(std::chrono::milliseconds(REGISTRATION_DELAY_MS));

    // This is the benchmarked section: Sending the payload
    for (auto _ : state) {
      publisher.Send(content_addr, payload_size);
    }

    // Finalize eCAL and wait for receiver thread to finish
    eCAL::Finalize();
    receiver_thread.join();
  }
  // Register the benchmark function
  BENCHMARK(BM_eCAL_Send)->RangeMultiplier(RANGE_MULTIPLIER)->Range(RANGE_START, RANGE_LIMIT)->UseRealTime()->Repetitions(REPETITIONS);
}


/*
 *
 * Benchmarking the eCAL send process in zero copy mode
 * 
*/
namespace Send_Zero_Copy {
  // Benchmark function
  static void BM_eCAL_Send_Zero_Copy(benchmark::State& state) {
    // Create payload to send, size depends on current argument
    size_t payload_size = state.range(0);
    std::vector<char> content_vector(payload_size);
    char* content_addr = content_vector.data();

    // Initialize eCAL
    eCAL::Initialize("Benchmark");

    // Create publisher config
    eCAL::Publisher::Configuration pub_config;
    pub_config.layer.shm.zero_copy_mode = true;

    // Create publisher with config
    eCAL::CPublisher publisher("benchmark_topic", eCAL::SDataTypeInformation(), pub_config);

    // Create receiver in a different thread
    std::thread receiver_thread([]() { 
      eCAL::CSubscriber subscriber("benchmark_topic");
      while(eCAL::Ok()) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
    } );
    
    // Wait for eCAL synchronization
    std::this_thread::sleep_for(std::chrono::milliseconds(REGISTRATION_DELAY_MS));

    // This is the benchmarked section: Sending the payload
    for (auto _ : state) {
      publisher.Send(content_addr, payload_size);
    }

    // Finalize eCAL and wait for receiver thread to finish
    eCAL::Finalize();
    receiver_thread.join();
  }
  // Register the benchmark function
  BENCHMARK(BM_eCAL_Send_Zero_Copy)->RangeMultiplier(RANGE_MULTIPLIER)->Range(RANGE_START, RANGE_LIMIT)->UseRealTime()->Repetitions(REPETITIONS);
}


/*
 *
 * Benchmarking the eCAL send process with handshake
 * 
*/
namespace Send_Handshake {
  // Benchmark function
  static void BM_eCAL_Send_Handshake(benchmark::State& state) {
    // Create payload to send, size depends on current argument
    size_t payload_size = state.range(0);
    std::vector<char> content_vector(payload_size);
    char* content_addr = content_vector.data();

    // Initialize eCAL
    eCAL::Initialize("Benchmark");

    // Create publisher config
    eCAL::Publisher::Configuration pub_config;
    pub_config.layer.shm.acknowledge_timeout_ms = ACK_TIMEOUT_MS;

    // Create publisher with config
    eCAL::CPublisher publisher("benchmark_topic", eCAL::SDataTypeInformation(), pub_config);

    // Create receiver in a different thread
    std::thread receiver_thread([]() { 
      eCAL::CSubscriber subscriber("benchmark_topic");
      while(eCAL::Ok()) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
    } );
    
    // Wait for eCAL synchronization
    std::this_thread::sleep_for(std::chrono::milliseconds(REGISTRATION_DELAY_MS));

    // This is the benchmarked section: Sending the payload
    for (auto _ : state) {
      publisher.Send(content_addr, payload_size);
    }

    // Finalize eCAL and wait for receiver thread to finish
    eCAL::Finalize();
    receiver_thread.join();
  }
  // Register the benchmark function
  BENCHMARK(BM_eCAL_Send_Handshake)->RangeMultiplier(RANGE_MULTIPLIER)->Range(RANGE_START, RANGE_LIMIT)->UseRealTime()->Repetitions(REPETITIONS);
}


/*
 *
 * Benchmarking the eCAL send process with double-buffering
 * 
*/
namespace Send_Double_Buffer {
  // Benchmark function
  static void BM_eCAL_Send_Double_Buffer(benchmark::State& state) {
    // Create payload to send, size depends on second argument
    size_t payload_size = state.range(0);
    std::vector<char> content_vector(payload_size);
    char* content_addr = content_vector.data();

    // Initialize eCAL
    eCAL::Initialize("Benchmark");

    // Create publisher config
    eCAL::Publisher::Configuration pub_config;
    pub_config.layer.shm.memfile_buffer_count = 2;

    // Create publisher with config
    eCAL::CPublisher publisher("benchmark_topic", eCAL::SDataTypeInformation(), pub_config);

    // Create receiver in a different thread
    std::thread receiver_thread([]() { 
      eCAL::CSubscriber subscriber("benchmark_topic");
      while(eCAL::Ok()) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
    } );
    
    // Wait for eCAL synchronization
    std::this_thread::sleep_for(std::chrono::milliseconds(REGISTRATION_DELAY_MS));

    // This is the benchmarked section: Sending the payload
    for (auto _ : state) {
      publisher.Send(content_addr, payload_size);
    }

    // Finalize eCAL and wait for receiver thread to finish
    eCAL::Finalize();
    receiver_thread.join();
  }
  // Register the benchmark function
  BENCHMARK(BM_eCAL_Send_Double_Buffer)->RangeMultiplier(RANGE_MULTIPLIER)->Range(RANGE_START, RANGE_LIMIT)->UseRealTime()->Repetitions(REPETITIONS);
}


/*
 *
 * Benchmarking the eCAL send process in zero-copy mode with double-buffering
 * 
*/
namespace Send_Zero_Copy_Double_Buffer {
  // Benchmark function
  static void BM_eCAL_Send_Zero_Copy_Double_Buffer(benchmark::State& state) {
    // Create payload to send, size depends on second argument
    size_t payload_size = state.range(0);
    std::vector<char> content_vector(payload_size);
    char* content_addr = content_vector.data();

    // Initialize eCAL
    eCAL::Initialize("Benchmark");

    // Create publisher config
    eCAL::Publisher::Configuration pub_config;
    pub_config.layer.shm.zero_copy_mode = true;
    pub_config.layer.shm.memfile_buffer_count = 2;

    // Create publisher with config
    eCAL::CPublisher publisher("benchmark_topic", eCAL::SDataTypeInformation(), pub_config);

    // Create receiver in a different thread
    std::thread receiver_thread([]() { 
      eCAL::CSubscriber subscriber("benchmark_topic");
      while(eCAL::Ok()) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
    } );
    
    // Wait for eCAL synchronization
    std::this_thread::sleep_for(std::chrono::milliseconds(REGISTRATION_DELAY_MS));

    // This is the benchmarked section: Sending the payload
    for (auto _ : state) {
      publisher.Send(content_addr, payload_size);
    }

    // Finalize eCAL and wait for receiver thread to finish
    eCAL::Finalize();
    receiver_thread.join();
  }
  // Register the benchmark function
  BENCHMARK(BM_eCAL_Send_Zero_Copy_Double_Buffer)->RangeMultiplier(RANGE_MULTIPLIER)->Range(RANGE_START, RANGE_LIMIT)->UseRealTime()->Repetitions(REPETITIONS);
}


/*
 *
 * Benchmarking the eCAL send and receive process
 * 
*/
namespace Send_and_Receive {
  // Define mutex and condition variable
  std::mutex mtx;
  std::condition_variable convar;
  bool msg_received = false;

  // Define callback function to register incoming message
  void callback(){
    std::lock_guard<std::mutex> lock(mtx);
    msg_received = true;
    convar.notify_one();
    return;
  };

  // Benchmark function
  static void BM_eCAL_Send_and_Receive(benchmark::State& state) {
    // Create payload to send, size depends on current argument
    size_t payload_size = state.range(0);
    std::vector<char> content_vector(payload_size);
    char* content_addr = content_vector.data();

    // Initialize eCAL, create sender
    eCAL::Initialize("Benchmark");
    eCAL::CPublisher publisher("benchmark_topic");

    // Create receiver in a different thread and register callback function
    std::thread receiver_thread([](){ 
      eCAL::CSubscriber subscriber("benchmark_topic");
      subscriber.SetReceiveCallback(std::bind(&callback));
      while(eCAL::Ok()) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
    });

    // Wait for eCAL synchronization
    std::this_thread::sleep_for(std::chrono::milliseconds(REGISTRATION_DELAY_MS));

    // This is the benchmarked section: Sending the payload and waiting for the receive callback
    for (auto _ : state) {
      msg_received = false;
      publisher.Send(content_addr, payload_size);
      std::unique_lock<std::mutex> lock(mtx);
      convar.wait(lock, [] {return msg_received;});
    }

    // Finalize eCAL and wait for receiver thread to finish
    eCAL::Finalize();
    receiver_thread.join();
  }
  // Register the benchmark function
  BENCHMARK(BM_eCAL_Send_and_Receive)->RangeMultiplier(RANGE_MULTIPLIER)->Range(RANGE_START, RANGE_LIMIT)->UseRealTime()->Repetitions(REPETITIONS);
}


/*
 * 
 * Benchmarking the eCAL receive latency (with manual timing)
 * 
*/
namespace Receive_Latency {
  // Define mutex and condition variable
  std::mutex mtx;
  std::condition_variable convar;
  bool msg_received = false;

  // Define variables for manual timing
  std::chrono::high_resolution_clock::time_point time_start;
  std::chrono::high_resolution_clock::time_point time_end;

  // Define callback function to register incoming message
  void callback_timed(){
    time_end = std::chrono::high_resolution_clock::now();

    std::lock_guard<std::mutex> lock(mtx);
    msg_received = true;
    convar.notify_one();
    return;
  };

  // Benchmark function
  static void BM_eCAL_Receive_Latency(benchmark::State& state) {
    // Create payload to send, size depends on current argument
    size_t payload_size = state.range(0);
    std::vector<char> content_vector(payload_size);

    // Initialize eCAL, create sender and receiver and register callback function
    eCAL::Initialize("Benchmark");
    eCAL::CPublisher publisher("benchmark_topic");

    // Create receiver in a different thread and register callback function
    std::thread receiver_thread([](){ 
      eCAL::CSubscriber subscriber("benchmark_topic");
      subscriber.SetReceiveCallback(std::bind(&callback_timed));
      while(eCAL::Ok()) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
    });
    
    // Wait for eCAL synchronization
    std::this_thread::sleep_for(std::chrono::milliseconds(REGISTRATION_DELAY_MS));

    // This is the benchmarked section: Sending the payload (untimed) and waiting for the receive callback
    for (auto _ : state) {
      msg_received = false;
      publisher.Send(content_vector.data(), payload_size);
      std::unique_lock<std::mutex> lock(mtx);

      time_start = std::chrono::high_resolution_clock::now();
      convar.wait(lock, [&]() {return msg_received;});

      // Calculate time difference between message sent and message received
      auto time_elapsed = std::chrono::duration_cast<std::chrono::duration<double>> (time_end - time_start);
      state.SetIterationTime(time_elapsed.count());
    }

    // Finalize eCAL and wait for receiver thread to finish
    eCAL::Finalize();
    receiver_thread.join();
  }
  // Register the benchmark function
  BENCHMARK(BM_eCAL_Receive_Latency)->RangeMultiplier(RANGE_MULTIPLIER)->Range(RANGE_START, RANGE_LIMIT)->UseManualTime()->Repetitions(REPETITIONS);
}


// Benchmark execution
BENCHMARK_MAIN();