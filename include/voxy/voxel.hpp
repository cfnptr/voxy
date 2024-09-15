// Copyright 2023-2023 Nikita Fediuchin. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/***********************************************************************************************************************
 * @file
 * @brief Voxel (block) functions.
 */

#pragma once
#include <cstdint>

namespace voxy::voxel
{

/**
 * @brief Predefined empty (air) voxel ID.
 */
const uint32_t null = 0;
/**
 * @brief Predefined unknown (missing) voxel ID.
 */
const uint32_t unknown = 1;
/**
 * @brief Predefined debugging voxel ID.
 */
const uint32_t debug = 2;
/**
 * @brief Predefined voxel ID count.
 */
const uint32_t predefinedCount = 3;

};