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
 * @brief Calculates volume 3D point index from the position and volume size.
 *
 * @tparam T type of the position integers
 * @param x point position along X-axis
 * @param y point position along Y-axis
 * @param z point position along Z-axis
 * @param length volume length in points along X-axis
 * @param layerSize volume layer size in points along X * Y
 */
template<typename T /* = uint8_t */>
static constexpr size_t posToIndex(T x, T y, T z, size_t length, size_t layerSize) noexcept
{
	return (size_t)z * layerSize + (size_t)y * length + (size_t)x;
}
/**
 * @brief Calculates volume point 3D position from the index and volume size.
 *
 * @tparam T type of the position integers
 * @param index point index inside the volume
 * @param length volume length in points along X-axis
 * @param layerSize volume layer size in points along X * Y
 * @param x point position along X-axis
 * @param y point position along Y-axis
 * @param z point position along Z-axis
 */
template<typename T /* = uint8_t */>
static constexpr void indexToPos(size_t index, size_t length, size_t layerSize, T& x, T& y, T& z) noexcept
{
	z = (T)(index / layerSize);
	index %= layerSize;
	y = (T)(index / length);
	x = (T)(index % length);
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
	 * @brief Chunk length in voxels along X-axis.
	 */
	static constexpr uint8_t lengthX = SX;
	/**
	 * @brief Chunk length in voxels along Y-axis.
	 */
	static constexpr uint8_t lengthY = SY;
	/**
	 * @brief Chunk length in voxels along Z-axis.
	 */
	static constexpr uint8_t lengthZ = SZ;
	/**
	 * @brief Chunk layer size in voxels. (sizeX * sizeY)
	 */
	static constexpr uint16_t layerSize = SX * SY;
	/**
	 * @brief Chunk array size in voxels, or chunk volume. (sizeX * sizeY * sizeZ)
	 */
	static constexpr uint32_t size = SX * SY * SZ;
	
	/**
	 * @brief Chunk voxel ID type.
	 */
	typedef V Voxel;
protected:
	Voxel voxels[size] = {};
public:
	/*******************************************************************************************************************
	 * @brief Creates a new uninitialized chunk.
	 * @warning Chunk may contain garbage voxels.
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
	 * @brief Calculates chunk voxel index from the 3D position.
	 *
	 * @param x voxel position along X-axis
	 * @param y voxel position along Y-axis
	 * @param z voxel position along Z-axis
	 */
	static constexpr uint32_t posToIndex(uint8_t x, uint8_t y, uint8_t z) noexcept
	{
		return voxy::posToIndex(x, y, z, lengthX, layerSize);
	}
	/**
	 * @brief Calculates chunk voxel 3D position from the index.
	 *
	 * @param index voxel index inside the chunk
	 * @param[out] x voxel position along X-axis
	 * @param[out] y voxel position along Y-axis
	 * @param[out] z voxel position along Z-axis
	 */
	static constexpr void indexToPos(uint32_t index, uint8_t& x, uint8_t& y, uint8_t& z) noexcept
	{
		voxy::indexToPos(index, lengthX, layerSize, x, y, z);
	}

	/*******************************************************************************************************************
	 * @brief Returns chunk voxel at specified 3D position.
	 * @note Use with care, it doesn't checks for out of chunk bounds!
	 * 
	 * @param x voxel position along X-axis
	 * @param y voxel position along Y-axis
	 * @param z voxel position along Z-axis
	 */
	Voxel get(uint8_t x, uint8_t y, uint8_t z) const noexcept
	{
		auto index = posToIndex(x, y, z);
		assert(index < size);
		return voxels[index];
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
		auto index = posToIndex(x, y, z);
		assert(index < size);
		voxels[index] = voxel;
	}

	/**
	 * @brief Returns chunk voxel at specified array index.
	 * @note Use with care, it doesn't checks for out of array bounds!
	 * @param index target voxel index inside array
	 */
	Voxel get(uint32_t index) const noexcept
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
	void set(uint32_t index, Voxel voxel) noexcept
	{
		assert(index < size);
		voxels[index] = voxel;
	}

	/*******************************************************************************************************************
	 * @brief Returns chunk voxel at specified 3D position if inside chunk bounds.
	 * 
	 * @param x voxel position along X-axis
	 * @param y voxel position along Y-axis
	 * @param z voxel position along Z-axis
	 * @param[out] voxel target voxel ID
	 *
	 * @return True if voxel 3D position is inside the chunk bounds, otherwise false.
	 */
	bool tryGet(uint8_t x, uint8_t y, uint8_t z, Voxel& voxel) const noexcept
	{
		auto index = posToIndex(x, y, z);
		if (index >= size)
			return false;
		voxel = voxels[index];
		return true;
	}
	/**
	 * @brief Sets chunk voxel at specified 3D position if inside chunk bounds.
	 *
	 * @param x voxel position along X-axis
	 * @param y voxel position along Y-axis
	 * @param z voxel position along Z-axis
	 * @param voxel target voxel ID
	 *
	 * @return True if voxel 3D position is inside the chunk bounds, otherwise false.
	 */
	bool trySet(uint8_t x, uint8_t y, uint8_t z, Voxel voxel) noexcept
	{
		auto index = posToIndex(x, y, z);
		if (index >= size)
			return false;
		voxels[index] = voxel;
		return true;
	}

	/**
	 * @brief Returns chunk voxel at specified array index if inside array bounds.
	 *
	 * @param index target voxel index inside array
	 * @param[out] voxel target voxel ID
	 *
	 * @return True if voxel index is inside array bounds, otherwise false.
	 */
	bool tryGet(uint32_t index, Voxel& voxel) const noexcept
	{
		if (index >= size)
			return false;
		voxel = voxels[index];
		return true;
	}
	/**
	 * @brief Sets chunk voxel at specified array index if inside array bounds.
	 *
	 * @param index target voxel index inside array
	 * @param target voxel ID
	 *
	 * @return True if voxel index is inside array bounds, otherwise false.
	 */
	bool trySet(uint32_t index, Voxel voxel) noexcept
	{
		if (index >= size)
			return false;
		voxels[index] = voxel;
		return true;
	}

	/*******************************************************************************************************************
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
	Voxel unsafeGet(uint32_t index) const noexcept
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
	void unsafeSet(uint32_t index, Voxel voxel) noexcept
	{
		voxels[index] = voxel;
	}

	/*******************************************************************************************************************
	 * @brief Fills chunk with zeros.
	 */
	void clear() noexcept
	{
		memset(voxels, 0, size * sizeof(Voxel));
	}
	/**
	 * @brief Fills chunk with specified voxel ID.
	 * @param voxel target voxel ID
	 */
	void fill(Voxel voxel) noexcept
	{
		for (uint32_t i = 0; i < size; i++)
			voxels[i] = voxel;
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
	 * @param[in] target other voxel array
	 * @param _sizeX other voxel array part size along X-axis
	 * @param _sizeY other voxel array part size along Y-axis
	 * @param _sizeZ other voxel array part size along Z-axis
	 * @param offsetX voxel array part offset along X-axis
	 * @param offsetY voxel array part offset along Y-axis
	 * @param offsetZ voxel array part offset along Z-axis
	 */
	void copy(const Voxel* otherVoxels, 
		uint8_t otherLength, uint16_t otherLayerSize, uint8_t countX, uint8_t countY, uint8_t countZ,
		uint8_t otherOffsetX = 0, uint8_t otherOffsetY = 0, uint8_t otherOffsetZ = 0,
		uint8_t thisOffsetX = 0, uint8_t thisOffsetY = 0, uint8_t thisOffsetZ = 0) noexcept
	{
		assert(otherVoxels);
		assert(countX + thisOffsetX <= SX);
		assert(countY + thisOffsetY <= SY);
		assert(countZ + thisOffsetZ <= SZ);
		assert(countX + otherOffsetX <= otherLength);

		for (uint8_t z = 0; z < countZ; z++)
		{
			for (uint8_t y = 0; y < countY; y++)
			{
				auto thisOffset = posToIndex(thisOffsetX, y + thisOffsetY, z + thisOffsetZ);
				auto otherOffset = voxy::posToIndex<uint8_t>(otherOffsetX, 
					y + otherOffsetY, z + otherOffsetZ, otherLength, otherLayerSize);
				memcpy(this->voxels + thisOffset, otherVoxels + otherOffset, countX * sizeof(Voxel));
			}
		}
	}
};

};