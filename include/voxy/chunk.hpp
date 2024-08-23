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
 * @brief Calculates chunk index from chunk position and size.
 */
static size_t posToChunkIndex(uint8_t x, uint8_t y, uint8_t z, uint8_t sizeX, uint8_t sizeXY) noexcept
{
	return ((size_t)z * sizeXY + (size_t)y * sizeX + x);
}

/**
 * @brief Voxel 3D container. (array)
 * 
 * @tparam SX chunk size in voxels along X-axis
 * @tparam SY chunk size in voxels along Y-axis
 * @tparam SZ chunk size in voxels along Z-axis
 * @tparam V chunk voxel ID type
 */
template<uint8_t SX, uint8_t SY, uint8_t SZ, typename V>
struct Chunk3
{
public:
	/**
	 * @brief Chunk size in voxels along X-axis.
	 */
	static constexpr uint8_t SizeX = SX;
	/**
	 * @brief Chunk size in voxels along Y-axis.
	 */
	static constexpr uint8_t SizeY = SY;
	/**
	 * @brief Chunk size in voxels along Z-axis.
	 */
	static constexpr uint8_t SizeZ = SZ;
	/**
	 * @brief Chunk layer size in voxels. (SizeX * SizeY)
	 */
	static constexpr size_t SizeXY = SX * SY;
	/**
	 * @brief Chunk array size in voxels, or chunk volume. (SizeX * SizeY * SizeZ)
	 */
	static constexpr size_t Size = SX * SY * SZ;
	/**
	 * @brief Chunk voxel ID type.
	 */
	typedef V Voxel;
protected:
	Voxel voxels[Size];
public:
	/**
	 * @brief Creates a new uninitialized chunk.
	 * @note Chunk may contain garbage voxels.
	 */
	Chunk3() = default;
	/**
	 * @brief Creates a new initialized chunk.
	 * @param voxel target voxel to fill chunk with
	 */
	Chunk3(Voxel voxel) { fill(voxel); }

	/**
	 * @brief Returns chunk voxel array.
	 */
	Voxel* getVoxels() noexcept { return voxels; }
	/**
	 * @brief Returns constant chunk voxel array.
	 */
	const Voxel* getVoxels() const noexcept { return voxels; }

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
		return voxels[posToChunkIndex(x, y, z, SX, SizeXY)];
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
		voxels[posToChunkIndex(x, y, z, SX, SizeXY)] = voxel;
	}

	/**
	 * @brief Returns chunk voxel at specified array index.
	 * @note Use with care, it doesn't checks for out of array bounds!
	 * @param index target voxel index inside array
	 */
	Voxel get(size_t index) const noexcept
	{
		assert(index < Size);
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
		assert(index < Size);
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
		voxel = voxels[posToChunkIndex(x, y, z, SX, SizeXY)];
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
		voxels[posToChunkIndex(x, y, z, SX, SizeXY)] = voxel;
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
		if (index >= Size)
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
		if (index >= Size)
			return false;
		voxels[index] = voxel;
		return true;
	}

	/**
	 * @brief Fills chunk with specified voxel ID
	 * @param voxel target voxel ID
	 */
	void fill(Voxel voxel) noexcept
	{
		if (voxel == NULL_VOXEL)
		{
			memset(voxels, 0, Size * sizeof(V));
		}
		else
		{
			for (size_t i = 0; i < Size; i++)
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
		memcpy(this->voxels, voxels, Size * sizeof(Voxel));
	}
	/**
	 * @brief Copies voxels from specified array part to this chunk.
	 * @note Voxel array should have bigger or the same size as specified part!
	 * 
	 * @param[in] target voxel array
	 * @param sizeX voxel array part size along X-axis
	 * @param sizeY voxel array part size along Y-axis
	 * @param sizeZ voxel array part size along Z-axis
	 * @param offsetX voxel array part offset along X-axis
	 * @param offsetY voxel array part offset along Y-axis
	 * @param offsetZ voxel array part offset along Z-axis
	 */
	void copy(const Voxel* voxels, uint8_t sizeX, uint8_t sizeY, uint8_t sizeZ,
		uint8_t offsetX = 0, uint8_t offsetY = 0, uint8_t offsetZ = 0) noexcept
	{
		assert(voxels);
		assert(sizeX + offsetX <= SX);
		assert(sizeY + offsetY <= SY);
		assert(sizeZ + offsetZ <= SZ);

		auto sizeXY = sizeX * sizeY;
		for (uint8_t z = 0; z < sizeZ; z++)
		{
			for (uint8_t y = 0; y < sizeY; y++)
			{
				memcpy(this->voxels + posToChunkIndex(
					offsetX, offsetY + y, offsetZ + z, SX, SizeXY),
					voxels + posToChunkIndex(0, y, z, sizeX, sizeXY),
					sizeX * sizeof(Voxel));
			}
		}
	}
};

};