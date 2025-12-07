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
 * @brief Chunk cluster (group) functions.
 */

#pragma once
#include "voxy/chunk.hpp"

namespace voxy
{

/***********************************************************************************************************************
 * @brief Nearby chunks 3x3x3 group container.
 * 
 * @tparam C cluster chunk type
 * @tparam V chunk voxel ID type
 */
template<class C, typename V>
struct Cluster3D
{
	typedef C Chunk; /**< Cluster chunk type. */
	typedef V Voxel; /**< Chunk voxel ID type. */
	
	static constexpr uint8_t length = 3;    /**< Cluster length in chunks. */
	static constexpr uint8_t layerSize = 9; /**< Cluster layer size in chunks. (length ^ 2) */
	static constexpr uint8_t size = 27;     /**< Cluster size in chunks. (length ^ 3) */

	/**
	 * @brief Cluster 3D position container.
	 */
	struct Position
	{
		int8_t x; int8_t y; int8_t z;
		constexpr Position(int8_t x = 0, int8_t y = 0, 
			int8_t z = 0) noexcept : x(x), y(y), z(z) { }
	};
	/**
	 * @brief Cluster 3D positions 3x3x3.
	 */
	static constexpr Position positions[size] = 
	{
		Position(-1, -1, -1), Position( 0, -1, -1), Position( 1, -1, -1), 
		Position(-1,  0, -1), Position( 0,  0, -1), Position( 1,  0, -1), 
		Position(-1,  1, -1), Position( 0,  1, -1), Position( 1,  1, -1), 
		Position(-1, -1,  0), Position( 0, -1,  0), Position( 1, -1,  0), 
		Position(-1,  0,  0), Position( 0,  0,  0), Position( 1,  0,  0), 
		Position(-1,  1,  0), Position( 0,  1,  0), Position( 1,  1,  0), 
		Position(-1, -1,  1), Position( 0, -1,  1), Position( 1, -1,  1), 
		Position(-1,  0,  1), Position( 0,  0,  1), Position( 1,  0,  1), 
		Position(-1,  1,  1), Position( 0,  1,  1), Position( 1,  1,  1)
	};
protected:
	Chunk* chunks[size]; /**< Cluster chunk array. */
public:
	/**
	 * @brief Creates a new chunk 3x3x3 cluster.
	 * @param chunks target cluster chunk array or null
	 */
	Cluster3D(Chunk* chunks[size] = nullptr) noexcept
	{
		if (chunks)
			memcpy(this->chunks, chunks, size * sizeof(Chunk*));
		else memset(this->chunks, 0, size * sizeof(Chunk*));
	}

	/*******************************************************************************************************************
	 * @brief Returns cluster chunk array.
	 */
	Chunk* getChunks() noexcept { return chunks; }
	/**
	 * @brief Returns constant cluster chunk array.
	 */
	const Chunk* getChunks() const noexcept { return chunks; }

	/**
	 * @brief Calculates cluster chunk index from the 3D position.
	 *
	 * @param x chunk position along X-axis
	 * @param y chunk position along Y-axis
	 * @param z chunk position along Z-axis
	 */
	static constexpr uint8_t posToIndex(uint8_t x, uint8_t y, uint8_t z) noexcept
	{
		return voxy::posToIndex(x, y, z, length, layerSize);
	}
	/**
	 * @brief Calculates cluster chunk 3D position from the index.
	 *
	 * @param index chunk index inside the chunk
	 * @param[out] x chunk position along X-axis
	 * @param[out] y chunk position along Y-axis
	 * @param[out] z chunk position along Z-axis
	 */
	template<typename T /* = uint8_t */>
	static constexpr void indexToPos(uint8_t index, uint8_t& x, uint8_t& y, uint8_t& z) noexcept
	{
		voxy::indexToPos(index, length, layerSize, x, y, z);
	}

	/**
	 * @brief Central cluster chunk index.
	 */
	static constexpr uint8_t centralIndex = posToIndex(1, 1, 1);

	/**
	 * @brief Are all cluster chunks not null.
	 */
	bool isComplete() const noexcept
	{
		size_t mask = 0;
		for (uint8_t i = 0; i < size; i++)
			mask |= (size_t)this->chunks[i];
		return mask;
	}

	/**
	 * @brief Returns cluster central chunk. (It can be null)
	 */
	Chunk* getCentralChunk() const noexcept
	{
		return chunks[centralIndex];
	}

	/*******************************************************************************************************************
	 * @brief Returns cluster chunk at specified 3D position. (It can be null)
	 * @note Use with care, it doesn't checks for out of cluster bounds!
	 * 
	 * @param x chunk position along X-axis
	 * @param y chunk position along Y-axis
	 * @param z chunk position along Z-axis
	 */
	Chunk* getChunk(uint8_t x, uint8_t y, uint8_t z) const noexcept
	{
		auto index = posToIndex(x, y, z);
		assert(index < size);
		return chunks[index];
	}
	/**
	 * @brief Returns cluster chunk at specified array index. (It can be null)
	 * @note Use with care, it doesn't checks for out of array bounds!
	 * @param index target chunk index inside array
	 */
	Chunk* getChunk(uint8_t index) const noexcept
	{
		assert(index < size);
		return chunks[index];
	}

	/**
	 * @brief Returns cluster chunk at specified 3D position. (It can be null)
	 * 
	 * @param x chunk position along X-axis
	 * @param y chunk position along Y-axis
	 * @param z chunk position along Z-axis
	 *
	 * @return Chunk instance if 3D position is inside array bounds.
	 */
	Chunk* tryGetChunk(uint8_t x, uint8_t y, uint8_t z) const noexcept
	{
		auto index = posToIndex(x, y, z);
		return index < size ? chunks[index] : nullptr;
	}
	/**
	 * @brief Returns cluster chunk at specified array index. (It can be null)
	 * @param index target chunk index inside array
	 * @return Chunk instance if index is inside array bounds.
	 */
	Chunk* tryGetChunk(uint8_t index) const noexcept
	{
		return index < size ? chunks[index] : nullptr;
	}

	/**
	 * @brief Returns cluster chunk at specified 3D position. (It can be null)
	 * @warning Skips out of bounds check in Debug and Release builds!
	 * 
	 * @param x chunk position along X-axis
	 * @param y chunk position along Y-axis
	 * @param z chunk position along Z-axis
	 */
	Chunk* unsafeGetChunk(uint8_t x, uint8_t y, uint8_t z) const noexcept { return chunks[posToIndex(x, y, z)]; }
	/**
	 * @brief Returns cluster chunk at specified array index. (It can be null)
	 * @warning Skips out of bounds check in Debug and Release builds!
	 * @param index target chunk index inside array
	 */
	Chunk* unsafeGetChunk(uint8_t index) const noexcept { return chunks[index]; }

	/*******************************************************************************************************************
	 * @brief Returns cluster chunk at specified voxel 3D position relative to the central chunk.
	 * @note Use with care, it doesn't checks for out of cluster bounds!
	 *
	 * @param[in,out] x voxel position along X-axis (Relative to the central chunk)
	 * @param[in,out] y voxel position along Y-axis (Relative to the central chunk)
	 * @param[in,out] z voxel position along Z-axis (Relative to the central chunk)
	 *
	 * @return Target voxel chunk and sets local x/y/z if it is not null.
	 */
	Chunk* getVoxelChunk(int16_t& x, int16_t& y, int16_t& z) const noexcept
	{
		assert(x >= -Chunk::length && x <= Chunk::length * 2 - 1);
		assert(y >= -Chunk::length && y <= Chunk::length * 2 - 1);
		assert(z >= -Chunk::length && z <= Chunk::length * 2 - 1);
		auto lx = x + Chunk::length, ly = y + Chunk::length, lz = z + Chunk::length;
		lx /= Chunk::length; ly /= Chunk::length; lz /= Chunk::length;
		auto index = posToIndex(lx, ly, lz);
		--lx; --ly; --lz;
		lx *= -Chunk::length; ly *= -Chunk::length; lz *= -Chunk::length;
		x += lx; y += ly; z += lz;
		return chunks[index];
	}
	/**
	 * @brief Returns cluster chunk at specified voxel 3D position relative to the central chunk.
	 *
	 * @param[in,out] x voxel position along X-axis (Relative to the central chunk)
	 * @param[in,out] y voxel position along Y-axis (Relative to the central chunk)
	 * @param[in,out] z voxel position along Z-axis (Relative to the central chunk)
	 *
	 * @return Target voxel chunk and sets local x/y/z if it is not null.
	 */
	Chunk* tryGetVoxelChunk(int16_t& x, int16_t& y, int16_t& z) const noexcept
	{
		if (x < -Chunk::length || x > Chunk::length * 2 - 1 ||
			y < -Chunk::length || y > Chunk::length * 2 - 1 ||
			z < -Chunk::length || z > Chunk::length * 2 - 1)
		{
			return nullptr;
		}

		auto lx = x + Chunk::length, ly = y + Chunk::length, lz = z + Chunk::length;
		lx /= Chunk::length; ly /= Chunk::length; lz /= Chunk::length;
		auto index = posToIndex(lx, ly, lz);
		--lx; --ly; --lz;
		lx *= -Chunk::length; ly *= -Chunk::length; lz *= -Chunk::length;
		x += lx; y += ly; z += lz;
		return chunks[index];
	}
	/**
	 * @brief Returns cluster chunk at specified voxel 3D position relative to the central chunk.
	 * @warning Skips out of bounds check in Debug and Release builds!
	 *
	 * @param[in,out] x voxel position along X-axis (Relative to the central chunk)
	 * @param[in,out] y voxel position along Y-axis (Relative to the central chunk)
	 * @param[in,out] z voxel position along Z-axis (Relative to the central chunk)
	 *
	 * @return Target voxel chunk and sets local x/y/z if it is not null.
	 */
	Chunk* unsafeGetVoxelChunk(int16_t& x, int16_t& y, int16_t& z) const noexcept
	{
		auto lx = x + Chunk::length, ly = y + Chunk::length, lz = z + Chunk::length;
		lx /= Chunk::length; ly /= Chunk::length; lz /= Chunk::length;
		auto index = posToIndex(lx, ly, lz);
		--lx; --ly; --lz;
		lx *= -Chunk::length; ly *= -Chunk::length; lz *= -Chunk::length;
		x += lx; y += ly; z += lz;
		return chunks[index];
	}

	/*******************************************************************************************************************
	 * @brief Returns voxel at specified 3D position relative to the central chunk.
	 * @note Use with care, it doesn't checks for out of cluster bounds!
	 * 
	 * @param x voxel position along X-axis (Relative to the central chunk)
	 * @param y voxel position along Y-axis (Relative to the central chunk)
	 * @param z voxel position along Z-axis (Relative to the central chunk)
	 * @param nullVoxel voxel ID returned if chunk instance is null
	 *
	 * @return Voxel at specified 3D position, or null if target chunk is null.
	 */
	Voxel getVoxel(int16_t x, int16_t y, int16_t z, Voxel nullVoxel = voxel::null) const noexcept
	{
		auto chunk = getVoxelChunk(x, y, z);
		return chunk ? chunk->get(x, y, z) : nullVoxel;
	}
	/**
	 * @brief Sets voxel at specified 3D position relative to the central chunk.
	 * @note Use with care, it doesn't checks for out of cluster bounds!
	 *
	 * @param x voxel position along X-axis
	 * @param y voxel position along Y-axis
	 * @param z voxel position along Z-axis
	 * @param voxel target voxel ID
	 */
	void setVoxel(uint16_t x, uint16_t y, uint16_t z, Voxel voxel) noexcept
	{
		auto chunk = getVoxelChunk(x, y, z);
		assert(chunk != nullptr);
		chunk->set(x, y, z, voxel);
	}

	/**
	 * @brief Returns voxel at specified 3D position relative to the central chunk.
	 * 
	 * @param x voxel position along X-axis (Relative to the central chunk)
	 * @param y voxel position along Y-axis (Relative to the central chunk)
	 * @param z voxel position along Z-axis (Relative to the central chunk)
	 * @param nullVoxel voxel ID returned if chunk instance is null
	 *
	 * @return True if specified 3D position is inside array bounds and chunk is not null.
	 */
	bool tryGetVoxel(int16_t x, int16_t y, int16_t z, Voxel& voxel) const noexcept
	{
		auto chunk = tryGetVoxelChunk(x, y, z);
		if (!chunk)
			return false;
		voxel = chunk->unsafeGet(x, y, z);
		return true;
	}
	/**
	 * @brief Sets voxel at specified 3D position relative to the central chunk.
	 *
	 * @param x voxel position along X-axis
	 * @param y voxel position along Y-axis
	 * @param z voxel position along Z-axis
	 * @param voxel target voxel ID
	 *
	 * @return True if specified 3D position is inside array bounds and chunk is not null.
	 */
	bool trySetVoxel(uint16_t x, uint16_t y, uint16_t z, Voxel voxel) noexcept
	{
		auto chunk = tryGetVoxelChunk(x, y, z);
		if (!chunk)
			return false;
		chunk->unsafeSet(x, y, z, voxel);
		return true;
	}

	/**
	 * @brief Returns voxel at specified 3D position relative to the central chunk.
	 * @warning Skips out of bounds check in Debug and Release builds!
	 * 
	 * @param x voxel position along X-axis (Relative to the central chunk)
	 * @param y voxel position along Y-axis (Relative to the central chunk)
	 * @param z voxel position along Z-axis (Relative to the central chunk)
	 * @param nullVoxel voxel ID returned if chunk instance is null
	 *
	 * @return Voxel at specified 3D position, or null if target chunk is null.
	 */
	Voxel unsafeGetVoxel(int16_t x, int16_t y, int16_t z, Voxel nullVoxel = voxel::null) const noexcept
	{
		auto chunk = unsafeGetVoxelChunk(x, y, z);
		return chunk ? chunk->unsafeGet(x, y, z) : nullVoxel;
	}
};

};