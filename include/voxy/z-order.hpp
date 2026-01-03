// Copyright 2023-2026 Nikita Fediuchin. All rights reserved.
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
 * @brief Z-order, Lebesgue curve, Morton space-filling curve functions.
 */

#pragma once
#include <cstdint>

#if defined(__BMI2__)
#include <immintrin.h>
#endif

namespace voxy
{

/**
 * @brief Expand bits for a Z-order (Morton) code.
 * @param v target value to expand
 */
static constexpr uint32_t expandBitsMorton(uint32_t v)
{
	v = (v | (v << 16u)) & 0x030000FFu;
	v = (v | (v <<  8u)) & 0x0300F00Fu;
	v = (v | (v <<  4u)) & 0x030C30C3u;
	v = (v | (v <<  2u)) & 0x09249249u;
	return v;
}
/**
 * @brief Expand bits for a Z-order (Morton) code.
 * @param v target value to expand
 */
static constexpr uint64_t expandBitsMorton(uint64_t v)
{
	v = (v | v << 32u) & 0x001F00000000FFFFu;
	v = (v | v << 16u) & 0x001F0000FF0000FFu;
	v = (v | v <<  8u) & 0x100F00F00F00F00Fu;
	v = (v | v <<  4u) & 0x10C30C30C30C30C3u;
	v = (v | v <<  2u) & 0x1249249249249249u;
	return v;
}

/**
 * @brief Compacts bits for a Z-order (Morton) code.
 * @param v target value to compact
 */
inline uint32_t compactBitsMorton(uint32_t v)
{
	v &= 0x09249249;
	v = (v | (v >>  2u)) & 0x030C30C3u;
	v = (v | (v >>  4u)) & 0x0300F00Fu;
	v = (v | (v >>  8u)) & 0x030000FFu;
	v = (v | (v >> 16u)) & 0x000003FFu;
	return v;
}
/**
 * @brief Compacts bits for a Z-order (Morton) code.
 * @param v target value to compact
 */
static constexpr uint64_t compactBitsMorton(uint64_t v)
{
	v &= 0x1249249249249249u;
	v = (v | v >> 2u)  & 0x10C30C30C30C30C3u;
	v = (v | v >> 4u)  & 0x100F00F00F00F00Fu;
	v = (v | v >> 8u)  & 0x001F0000FF0000FFu;
	v = (v | v >> 16u) & 0x001F00000000FFFFu;
	v = (v | v >> 32u) & 0x00000000001FFFFFu;
	return v;
}

/**
 * @brief Calculates volume 3D point index from the position. (Z-order curve)
 * @details Uses Morton space-filling curve encoding. (Lebesgue curve)
 *
 * @param x point position along X-axis
 * @param y point position along Y-axis
 * @param z point position along Z-axis
 */
static uint32_t posToIndex(uint32_t x, uint32_t y, uint32_t z) noexcept
{
	#if defined(__BMI2__) // Note: Y-fastest (Y is LSB)
	return _pdep_u32(x, 0x92492492u) | _pdep_u32(y, 0x49249249u) | _pdep_u32(z, 0x24924924u);
	#else
	return (expandBitsMorton(x) << 1u) | expandBitsMorton(y) | (expandBitsMorton(z) << 2u);
	#endif
}
/**
 * @brief Calculates volume point 3D position from the index. (Z-order curve)
 * @details Uses Morton space-filling curve encoding. (Lebesgue curve)
 *
 * @param index point index inside the volume (Z-order)
 * @param x point position along X-axis
 * @param y point position along Y-axis
 * @param z point position along Z-axis
 */
static void indexToPos(uint32_t index, uint32_t& x, uint32_t& y, uint32_t& z) noexcept
{
	#if defined(__BMI2__) // Note: Y-fastest (Y is LSB)
	x = _pext_u32(index, 0x92492492u);
	y = _pext_u32(index, 0x49249249u);
	z = _pext_u32(index, 0x24924924u);
	#else
	x = compactBitsMorton(index >> 1u);
	y = compactBitsMorton(index);
	z = compactBitsMorton(index >> 2u);
	#endif
}

/**
 * @brief Calculates volume point 3D X position from the index. (Z-order curve)
 * @details Uses Morton space-filling curve encoding. (Lebesgue curve)
 * @param index point index inside the volume (Z-order)
 */
static uint32_t indexToPosX(uint32_t index) noexcept
{
	#if defined(__BMI2__)
	return _pext_u32(index, 0x92492492u);
	#else
	return compactBitsMorton(index >> 1u);
	#endif
}
/**
 * @brief Calculates volume point 3D Y position from the index. (Z-order curve)
 * @details Uses Morton space-filling curve encoding. (Lebesgue curve)
 * @param index point index inside the volume (Z-order)
 */
static uint32_t indexToPosY(uint32_t index) noexcept
{
	#if defined(__BMI2__)
	return _pext_u32(index, 0x49249249u);
	#else
	return compactBitsMorton(index);
	#endif
}
/**
 * @brief Calculates volume point 3D Z position from the index. (Z-order curve)
 * @details Uses Morton space-filling curve encoding. (Lebesgue curve)
 * @param index point index inside the volume (Z-order)
 */
static uint32_t indexToPosZ(uint32_t index) noexcept
{
	#if defined(__BMI2__)
	return _pext_u32(index, 0x24924924u);
	#else
	return compactBitsMorton(index >> 2u);
	#endif
}

} // namespace voxy