# Copyright 2023-2023 Nikita Fediuchin. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.16)

if(${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})
	message(FATAL_ERROR "Prevented in-tree build")
endif()

if(TARGET voxy)
	return()
endif()

project(voxy VERSION 1.0.0 LANGUAGES CXX
	DESCRIPTION "Convenient library for working with \
		common voxel data structures written in C++"
	HOMEPAGE_URL "https://github.com/cfnptr/voxy")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

option(VOXY_BUILD_TESTS "Build Voxy library tests" ON)

add_library(voxy INTERFACE)
target_include_directories(voxy INTERFACE ${PROJECT_SOURCE_DIR}/include)

if(VOXY_BUILD_TESTS)
	enable_testing()

	add_executable(TestVoxyChunk tests/test-chunk.cpp)
	target_link_libraries(TestVoxyChunk PUBLIC voxy)
	add_test(NAME TestVoxyChunk COMMAND TestVoxyChunk)

	add_executable(TestVoxyCluster tests/test-cluster.cpp)
	target_link_libraries(TestVoxyCluster PUBLIC voxy)
	add_test(NAME TestVoxyCluster COMMAND TestVoxyCluster)
endif()