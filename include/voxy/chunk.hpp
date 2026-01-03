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
 * @brief Voxel chunk (array) functions.
 */

#pragma once
#include "voxy/z-order.hpp"

#include <cstring>
#include <cassert>

namespace voxy
{

/**
 * @brief Voxel Z-order 3D container. (array)
 * 
 * @tparam S chunk size in voxels along one axis
 * @tparam V chunk voxel identifier type
 */
template<uint8_t S, typename V>
struct Chunk3Z
{
public:
	/**
	 * @brief Chunk length in voxels along one axis.
	 */
	static constexpr uint8_t length = S;
	/**
	 * @brief Chunk layer size in voxels. (length ^ 2)
	 */
	static constexpr uint16_t layerSize = (uint16_t)S * S;
	/**
	 * @brief Chunk array size in voxels, or chunk volume. (length ^ 3)
	 */
	static constexpr uint32_t size = (uint16_t)S * S * S;
	
	/**
	 * @brief Chunk voxel identifier type.
	 */
	typedef V Voxel;
protected:
	Voxel voxels[size];
public:
	/*******************************************************************************************************************
	 * @brief Creates a new uninitialized chunk.
	 * @warning Chunk may contain garbage voxels.
	 */
	Chunk3Z() noexcept = default;
	/**
	 * @brief Creates a new initialized chunk.
	 * @param voxel target voxel to fill chunk with
	 */
	Chunk3Z(Voxel voxel) noexcept { fill(voxel); }

	/**
	 * @brief Returns chunk voxel array. (Z-order)
	 */
	Voxel* getVoxels() noexcept { return voxels; }
	/**
	 * @brief Returns constant chunk voxel array. (Z-order)
	 */
	const Voxel* getVoxels() const noexcept { return voxels; }

	/*******************************************************************************************************************
	 * @brief Returns chunk voxel at specified 3D position.
	 * @note Use with care, it doesn't checks for out of chunk bounds!
	 * 
	 * @param x voxel position along X-axis
	 * @param y voxel position along Y-axis
	 * @param z voxel position along Z-axis
	 */
	Voxel get(uint32_t x, uint32_t y, uint32_t z) const noexcept
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
	 * @param voxel target voxel identifier
	 */
	void set(uint32_t x, uint32_t y, uint32_t z, Voxel voxel) noexcept
	{
		auto index = posToIndex(x, y, z);
		assert(index < size);
		voxels[index] = voxel;
	}

	/**
	 * @brief Returns chunk voxel at specified array Z-order index.
	 * @note Use with care, it doesn't checks for out of array bounds!
	 * @param index target voxel index inside array (Z-order)
	 */
	Voxel get(uint32_t index) const noexcept
	{
		assert(index < size);
		return voxels[index];
	}
	/**
	 * @brief Sets chunk voxel at specified array Z-order index.
	 * @note Use with care, it doesn't checks for out of array bounds!
	 * 
	 * @param index voxel index inside array (Z-order)
	 * @param voxel target voxel identifier
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
	 * @param[out] voxel target voxel identifier
	 *
	 * @return True if voxel 3D position is inside the chunk bounds, otherwise false.
	 */
	bool tryGet(uint32_t x, uint32_t y, uint32_t z, Voxel& voxel) const noexcept
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
	 * @param voxel target voxel identifier
	 *
	 * @return True if voxel 3D position is inside the chunk bounds, otherwise false.
	 */
	bool trySet(uint32_t x, uint32_t y, uint32_t z, Voxel voxel) noexcept
	{
		auto index = posToIndex(x, y, z);
		if (index >= size)
			return false;
		voxels[index] = voxel;
		return true;
	}

	/**
	 * @brief Returns chunk voxel at specified array Z-order index if inside array bounds.
	 *
	 * @param index voxel index inside array (Z-order)
	 * @param[out] voxel target voxel identifier
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
	 * @brief Sets chunk voxel at specified array Z-order index if inside array bounds.
	 *
	 * @param index voxel index inside array (Z-order)
	 * @param voxel target voxel identifier
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
	Voxel unsafeGet(uint32_t x, uint32_t y, uint32_t z) const noexcept
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
	 * @param voxel target voxel identifier
	 */
	void unsafeSet(uint32_t x, uint32_t y, uint32_t z, Voxel voxel) noexcept
	{
		voxels[posToIndex(x, y, z)] = voxel;
	}

	/**
	 * @brief Returns chunk voxel at specified array index.
	 * @warning Skips out of bounds check in Debug and Release builds!
	 * @param index target voxel index inside array (Z-order)
	 */
	Voxel unsafeGet(uint32_t index) const noexcept
	{
		return voxels[index];
	}
	/**
	 * @brief Sets chunk voxel at specified array index.
	 * @warning Skips out of bounds check in Debug and Release builds!
	 * 
	 * @param index voxel index inside array (Z-order)
	 * @param voxel target voxel identifier
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
	 * @param voxel target voxel identifier
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
	 * @param[in] otherVoxels target other voxel array
	 * @param countX voxel array part size along X-axis
	 * @param countY voxel array part size along Y-axis
	 * @param countZ voxel array part size along Z-axis
	 * @param otherLength other voxel array length along X-axis
	 * @param otherOffsetX other voxel array part offset along X-axis
	 * @param otherOffsetY other voxel array part offset along Y-axis
	 * @param otherOffsetZ other voxel array part offset along Z-axis
	 * @param thisOffsetX this voxel array part offset along X-axis
	 * @param thisOffsetY this voxel array part offset along Y-axis
	 * @param thisOffsetZ this voxel array part offset along Z-axis
	 */
	void copy(const Voxel* otherVoxels, uint32_t otherLength, uint32_t countX, uint32_t countY, uint32_t countZ,
		uint32_t otherOffsetX = 0, uint32_t otherOffsetY = 0, uint32_t otherOffsetZ = 0,
		uint32_t thisOffsetX = 0, uint32_t thisOffsetY = 0, uint32_t thisOffsetZ = 0) noexcept
	{
		assert(otherVoxels);
		assert(countX + thisOffsetX <= S);
		assert(countY + thisOffsetY <= S);
		assert(countZ + thisOffsetZ <= S);
		assert(countX + otherOffsetX <= otherLength);

		for (uint32_t z = 0; z < countZ; z++)
		{
			for (uint32_t x = 0; x < countX; x++)
			{
				for (uint32_t y = 0; y < countY; y++)
				{
					auto thisOffset = posToIndex(x + thisOffsetX, y + thisOffsetY, z + thisOffsetZ);
					auto otherOffset = posToIndex(x + otherOffsetX, y + otherOffsetY, z + otherOffsetZ);
					voxels[thisOffset] = otherVoxels[otherOffset];
				}
			}
		}
	}
};

} // namespace voxy