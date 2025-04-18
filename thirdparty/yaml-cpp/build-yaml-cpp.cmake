include_guard(GLOBAL)

set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "My option" FORCE)
set(YAML_MSVC_SHARED_RT ON CACHE BOOL "My option" FORCE)
set(YAML_BUILD_SHARED_LIBS OFF CACHE BOOL "My option" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "My option" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "My option" FORCE)

# All targets made by yaml-cpp (including the uninstall target) will go in
# the correct IDE folder
set(CMAKE_FOLDER "thirdparty/yaml-cpp")

ecal_variable_push(CMAKE_POLICY_VERSION_MINIMUM)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/yaml-cpp thirdparty/yaml-cpp EXCLUDE_FROM_ALL SYSTEM)
ecal_variable_pop(CMAKE_POLICY_VERSION_MINIMUM)