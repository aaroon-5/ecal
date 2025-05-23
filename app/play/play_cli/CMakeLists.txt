# ========================= eCAL LICENSE =================================
#
# Copyright (C) 2016 - 2019 Continental Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#      http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# ========================= eCAL LICENSE =================================

project(play)

find_package(tclap REQUIRED)
find_package(termcolor REQUIRED)

set(source_files
  src/ecal_play_cli.cpp
  src/ecal_play_service.cpp
  src/ecal_play_service.h
)

if(WIN32)
  set(win32_resource_files
      resources/resource.h
      resources/win32_resources.rc
  )
endif()

ecal_add_app_console(${PROJECT_NAME}
  ${source_files}
  ${win32_resource_files}
)

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_compile_definitions(${PROJECT_NAME}
  PRIVATE $<$<BOOL:${MSVC}>:PCRE_STATIC;_UNICODE>)

target_link_libraries(${PROJECT_NAME} PRIVATE
    termcolor::termcolor
    tclap::tclap
    eCAL::play_core
    eCAL::ecal-utils
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_14) 

# Create a source tree that mirrors the filesystem
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}"
    FILES
        ${source_files}
        ${win32_resource_files}
)

target_link_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/ignore:4098,4099>)

ecal_install_app(${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER app/play)
