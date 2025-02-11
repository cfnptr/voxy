// Copyright 2023-2025 Nikita Fediuchin. All rights reserved.
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
 * @brief Voxel chunk (array) functions.
 */

#pragma once
#include "voxy/voxel.hpp"

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cassert>

namespace voxy
{

/**
 * @brief Calculates volume point index from the position and volume size.
 *
 * @tparam T type of the position integers
 * @param x point position along X-axis
 * @param y point position along Y-axis
 * @param z point position along Z-axis
 * @param sizeX volume size in points along X-axis
 * @param sizeXY volume size in points along X * Y
 */
template<typename T /* = uint8_t */>
static constexpr size_t posToIndex(T x, T y, T z, size_t sizeX, size_t sizeXY) noexcept
{
	return (size_t)z * sizeXY + (size_t)y * sizeX + x;
}
/**
 * @brief Calculates volume point position from the index and volume size.
 *
 * @tparam T type of the position integers
 * @param index point index inside the volume
 * @param sizeX volume size in points along X-axis
 * @param sizeXY volume size in points along X * Y
 * @param x point position along X-axis
 * @param y point position along Y-axis
 * @param z point position along Z-axis
 */
template<typename T /* = uint8_t */>
static constexpr void indexToPos(size_t index, size_t sizeX, size_t sizeXY, T& x, T& y, T& z) noexcept
{
	z = (T)(index / sizeXY);
	index %= sizeXY;
	y = (T)(index / sizeX);
	x = (T)(index % sizeX);
}

/***********************************************************************************************************************
 * @brief Voxel 3D container. (array)
 * 
 * @tparam SX chunk size in voxels along X-axis
 * @tparam SY chunk size in voxels along Y-axis
 * @tparam SZ chunk size in voxels along Z-axis
 * @tparam V chunk voxel ID type
 */
template<uint8_t SX, uint8_t SY, uint8_t SZ, typename V>
struct Chunk3D
{
public:
	/**
	 * @brief Chunk size in voxels along X-axis.
	 */
	static constexpr uint8_t sizeX = SX;
	/**
	 * @brief Chunk size in voxels along Y-axis.
	 */
	static constexpr uint8_t sizeY = SY;
	/**
	 * @brief Chunk size in voxels along Z-axis.
	 */
	static constexpr uint8_t sizeZ = SZ;
	/**
	 * @brief Chunk layer size in voxels. (sizeX * sizeY)
	 */
	static constexpr uint16_t sizeXY = SX * SY;
	/**
	 * @brief Chunk array size in voxels, or chunk volume. (sizeX * sizeY * sizeZ)
	 */
	static constexpr size_t size = SX * SY * SZ;
	/**
	 * @brief Chunk voxel ID type.
	 */
	typedef V Voxel;
protected:
	Voxel voxels[size];
public:
	/**
	 * @brief Creates a new uninitialized chunk.
	 * @note Chunk may contain garbage voxels.
	 */
	Chunk3D() = default;
	/**
	 * @brief Creates a new initialized chunk.
	 * @param voxel target voxel to fill chunk with
	 */
	Chunk3D(Voxel voxel) { fill(voxel); }

	/**
	 * @brief Returns chunk voxel array.
	 */
	Voxel* getVoxels() noexcept { return voxels; }
	/**
	 * @brief Returns constant chunk voxel array.
	 */
	const Voxel* getVoxels() const noexcept { return voxels; }

	/**
	 * @brief Calculates chunk voxel index from the position.
	 *
	 * @param x voxel position along X-axis
	 * @param y voxel position along Y-axis
	 * @param z voxel position along Z-axis
	 */
	static constexpr size_t posToIndex(uint8_t x, uint8_t y, uint8_t z) noexcept
	{
		return voxy::posToIndex(x, y, z, sizeX, sizeXY);
	}
	/**
	 * @brief Calculates chunk voxel position from the index.
	 *
	 * @param index voxel index inside the chunk
	 * @param[out] x voxel position along X-axis
	 * @param[out] y voxel position along Y-axis
	 * @param[out] z voxel position along Z-axis
	 */
	static constexpr void indexToPos(size_t index, uint8_t& x, uint8_t& y, uint8_t& z) noexcept
	{
		voxy::indexToPos(index, sizeX, sizeXY, x, y, z);
	}

	/**
	 * @brief Returns chunk voxel at specified 3D position.
	 * @note Use with care, it doesn't checks for out of chunk bounds!
	 * 
	 * @param x voxel position along X-axis
	 * @param y voxel position along Y-axis
	 * @param z voxel position along Z-axis
	 */
	Voxel get(uint8_t x, uint8_t y, uint8_t z) const noexcept
	{
		assert(x < SX);
		assert(y < SY);
		assert(z < SZ);
		return voxels[posToIndex(x, y, z)];
	}
	/**
	 * @brief Sets chunk voxel at specified 3D position.
	 * @note Use with care, it doesn't checks for out of chunk bounds!
	 * 
	 * @param x voxel position along X-axis
	 * @param y voxel position along Y-axis
	 * @param z voxel position along Z-axis
	 * @param voxel target voxel ID
	 */
	void set(uint8_t x, uint8_t y, uint8_t z, Voxel voxel) noexcept
	{
		assert(x < SX);
		assert(y < SY);
		assert(z < SZ);
		voxels[posToIndex(x, y, z)] = voxel;
	}

	/**
	 * @brief Returns chunk voxel at specified array index.
	 * @note Use with care, it doesn't checks for out of array bounds!
	 * @param index target voxel index inside array
	 */
	Voxel get(size_t index) const noexcept
	{
		assert(index < size);
		return voxels[index];
	}
	/**
	 * @brief Sets chunk voxel at specified array index.
	 * @note Use with care, it doesn't checks for out of array bounds!
	 * 
	 * @param index target voxel index inside array
	 * @param voxel target voxel ID
	 */
	void set(size_t index, Voxel voxel) noexcept
	{
		assert(index < size);
		voxels[index] = voxel;
	}

	/**
	 * @brief Returns chunk voxel at specified 3D position if inside chunk bounds.
	 * @return True if voxel position is inside chunk bounds, otherwise false.
	 * 
	 * @param x voxel position along X-axis
	 * @param y voxel position along Y-axis
	 * @param z voxel position along Z-axis
	 * @param[out] voxel target voxel ID
	 */
	bool tryGet(uint8_t x, uint8_t y, uint8_t z, Voxel& voxel) const noexcept
	{
		if (x >= SX || y >= SY || z >= SZ)
			return false;
		voxel = voxels[posToIndex(x, y, z)];
		return true;
	}
	/**
	 * @brief Sets chunk voxel at specified 3D position if inside chunk bounds.
	 * @return True if voxel position is inside chunk bounds, otherwise false.
	 *
	 * @param x voxel position along X-axis
	 * @param y voxel position along Y-axis
	 * @param z voxel position along Z-axis
	 * @param voxel target voxel ID
	 */
	bool trySet(uint8_t x, uint8_t y, uint8_t z, Voxel voxel) noexcept
	{
		if (x >= SX || y >= SY || z >= SZ)
			return false;
		voxels[posToIndex(x, y, z)] = voxel;
		return true;
	}

	/**
	 * @brief Returns chunk voxel at specified array index if inside array bounds.
	 * @return True if voxel index is inside array bounds, otherwise false.
	 *
	 * @param index target voxel index inside array
	 * @param[out] voxel target voxel ID
	 */
	bool tryGet(size_t index, Voxel& voxel) const noexcept
	{
		if (index >= size)
			return false;
		voxel = voxels[index];
		return true;
	}
	/**
	 * @brief Sets chunk voxel at specified array index if inside array bounds.
	 * @return True if voxel index is inside array bounds, otherwise false.
	 *
	 * @param index target voxel index inside array
	 * @param target voxel ID
	 */
	bool trySet(size_t index, Voxel voxel) noexcept
	{
		if (index >= size)
			return false;
		voxels[index] = voxel;
		return true;
	}

	/**
	 * @brief Returns chunk voxel at specified 3D position.
	 * @warning Skips out of bounds check in Debug and Release builds!
	 * 
	 * @param x voxel position along X-axis
	 * @param y voxel position along Y-axis
	 * @param z voxel position along Z-axis
	 */
	Voxel unsafeGet(uint8_t x, uint8_t y, uint8_t z) const noexcept
	{
		return voxels[posToIndex(x, y, z)];
	}
	/**
	 * @brief Sets chunk voxel at specified 3D position.
	 * @warning Skips out of bounds check in Debug and Release builds!
	 * 
	 * @param x voxel position along X-axis
	 * @param y voxel position along Y-axis
	 * @param z voxel position along Z-axis
	 * @param voxel target voxel ID
	 */
	void unsafeSet(uint8_t x, uint8_t y, uint8_t z, Voxel voxel) noexcept
	{
		voxels[posToIndex(x, y, z)] = voxel;
	}

	/**
	 * @brief Returns chunk voxel at specified array index.
	 * @warning Skips out of bounds check in Debug and Release builds!
	 * @param index target voxel index inside array
	 */
	Voxel unsafeGet(size_t index) const noexcept
	{
		return voxels[index];
	}
	/**
	 * @brief Sets chunk voxel at specified array index.
	 * @warning Skips out of bounds check in Debug and Release builds!
	 * 
	 * @param index target voxel index inside array
	 * @param voxel target voxel ID
	 */
	void unsafeSet(size_t index, Voxel voxel) noexcept
	{
		voxels[index] = voxel;
	}

	/**
	 * @brief Fills chunk with specified voxel ID.
	 * @param voxel target voxel ID
	 */
	void fill(Voxel voxel = voxel::null) noexcept
	{
		if (voxel == voxel::null)
		{
			memset(voxels, 0, size * sizeof(Voxel));
		}
		else
		{
			for (size_t i = 0; i < size; i++)
				voxels[i] = voxel;
		}
	}

	/**
	 * @brief Copies voxels from specified array to this chunk.
	 * @note Voxel array should have bigger or the same size as chunk!
	 * @param[in] voxels target voxel array
	 */
	void copy(const Voxel* voxels) noexcept
	{
		assert(voxels);
		memcpy(this->voxels, voxels, size * sizeof(Voxel));
	}
	/**
	 * @brief Copies voxels from specified array part to this chunk.
	 * @note Voxel array should have bigger or the same size as specified part!
	 * 
	 * @param[in] target voxel array
	 * @param _sizeX voxel array part size along X-axis
	 * @param _sizeY voxel array part size along Y-axis
	 * @param _sizeZ voxel array part size along Z-axis
	 * @param offsetX voxel array part offset along X-axis
	 * @param offsetY voxel array part offset along Y-axis
	 * @param offsetZ voxel array part offset along Z-axis
	 */
	void copy(const Voxel* voxels, uint8_t _sizeX, uint8_t _sizeY, uint8_t _sizeZ,
		uint8_t offsetX = 0, uint8_t offsetY = 0, uint8_t offsetZ = 0) noexcept
	{
		assert(voxels);
		assert(_sizeX + offsetX <= SX);
		assert(_sizeY + offsetY <= SY);
		assert(_sizeZ + offsetZ <= SZ);

		auto _sizeXY = _sizeX * _sizeY;
		for (uint8_t z = 0; z < _sizeZ; z++)
		{
			for (uint8_t y = 0; y < _sizeY; y++)
			{
				memcpy(this->voxels + posToIndex(offsetX, offsetY + y, offsetZ + z), voxels + 
					voxy::posToIndex((uint8_t)0, y, z, _sizeX, _sizeXY), _sizeX * sizeof(Voxel));
			}
		}
	}
};

};