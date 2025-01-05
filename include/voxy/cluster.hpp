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
 * @brief Chunk cluster (group) functions.
 * 
 * @details
 * 
 * 2D chunk cluster representation:
 *      [X]
 *   [X][X][X]
 *      [X]
 */

#pragma once
#include "voxy/chunk.hpp"

namespace voxy
{

/***********************************************************************************************************************
 * @brief Nearby chunk 3D group container. (including center one)
 * 
 * @details
 * c  - central chunk (main)
 * nx - negative X-axis chunk (-x)
 * px - positive X-axis chunk (+x)
 * ny - negative Y-axis chunk (-y)
 * py - positive Y-axis chunk (+y)
 * nz - negative Z-axis chunk (-z)
 * pz - positive Z-axis chunk (+z)
 * 
 * @tparam C cluster chunk type
 * @tparam V chunk voxel ID type
 */
template<class C, typename V>
struct Cluster3D
{
	/**
	 * @brief Cluster chunk type.
	 */
	typedef C Chunk;
	/**
	 * @brief Chunk voxel ID type.
	 */
	typedef V Voxel;

	/**
	 * @brief Chunk group size around center chunk. (including center one)
	 */
	static constexpr uint8_t chunkSize = 7;

	C* c  = nullptr; /**< Central chunk instance. */
	C* nx = nullptr; /**< Negative X-axis chunk instance (-x) */
	C* px = nullptr; /**< Positive X-axis chunk instance (+x) */
	C* ny = nullptr; /**< Negative Y-axis chunk instance (-y) */
	C* py = nullptr; /**< Positive Y-axis chunk instance (+y) */
	C* nz = nullptr; /**< Negative Z-axis chunk instance (-z) */
	C* pz = nullptr; /**< Positive Z-axis chunk instance (+z) */

	/**
	 * @brief Creates a new chunk 3D cluster.
	 * 
	 * @param c central chunk instance
	 * @param nx negative X-axis chunk instance (-x)
	 * @param px positive X-axis chunk instance (+x)
	 * @param ny negative Y-axis chunk instance (-y)
	 * @param py positive Y-axis chunk instance (+y)
	 * @param nz negative Z-axis chunk instance (-z)
	 * @param pz positive Z-axis chunk instance (+z)
	 */
	constexpr Cluster3D(Chunk* c = nullptr, Chunk* nx = nullptr, Chunk* px = nullptr,
		Chunk* ny = nullptr, Chunk* py = nullptr, Chunk* nz = nullptr, Chunk* pz = nullptr) noexcept :
		c(c), nx(nx), px(px), ny(ny), py(py), nz(nz), pz(pz) { }

	/**
	 * @brief Are all cluster chunks not null.
	 */
	constexpr bool isComplete() const noexcept
	{
		return c && nx && px && ny && py && nz && pz;
	}

	/**
	 * @breaf Returns cluster voxel at specified 3D position.
	 * @note Use with care, it doesn't checks for out of cluster bounds!
	 * 
	 * @param x voxel position along X-axis
	 * @param y voxel position along Y-axis
	 * @param z voxel position along Z-axis
	 */
	Voxel get(int16_t x, int16_t y, int16_t z) const noexcept
	{
		assert(isComplete());
		if (x < 0)
			return nx->get(x + Chunk::sizeX, y, z);
		if (x >= Chunk::SizeX)
			return px->get(x - Chunk::sizeX, y, z);
		if (y < 0)
			return ny->get(x, y + Chunk::sizeY, z);
		if (y >= Chunk::SizeY)
			return py->get(x, y - Chunk::sizeY, z);
		if (z < 0)
			return nz->get(x, y, z + Chunk::sizeZ);
		if (z >= Chunk::sizeZ)
			return pz->get(x, y, z - Chunk::sizeZ);
		return c->get(x, y, z);
	}
	/**
	 * @breaf Sets cluster voxel at specified 3D position.
	 * @note Use with care, it doesn't checks for out of cluster bounds!
	 *
	 * @param x voxel position along X-axis
	 * @param y voxel position along Y-axis
	 * @param z voxel position along Z-axis
	 * @param voxel target voxel ID
	 */
	void set(uint16_t x, uint16_t y, uint16_t z, Voxel voxel) noexcept
	{
		assert(isComplete());
		if (x < 0)
			return nx->set(x + Chunk::sizeX, y, z, voxel);
		if (x >= Chunk::sizeX)
			return px->set(x - Chunk::sizeX, y, z, voxel);
		if (y < 0)
			return ny->set(x, y + Chunk::sizeY, z, voxel);
		if (y >= Chunk::sizeY)
			return py->set(x, y - Chunk::sizeY, z, voxel);
		if (z < 0)
			return nz->set(x, y, z + Chunk::sizeZ, voxel);
		if (z >= Chunk::sizeZ)
			return pz->set(x, y, z - Chunk::sizeZ, voxel);
		return c->set(x, y, z, voxel);
	}

	/**
	 * @brief Returns cluster voxel at specified 3D position if inside cluster bounds.
	 * @return True if voxel position is inside cluster bounds, otherwise false.
	 *
	 * @param x voxel position along X-axis
	 * @param y voxel position along Y-axis
	 * @param z voxel position along Z-axis
	 * @param[out] voxel target voxel ID
	 */
	bool tryGet(uint16_t x, uint16_t y, uint16_t z, Voxel& voxel) const noexcept
	{
		if (x < 0)
			return nx ? nx->tryGet(x + Chunk::sizeX, y, z, voxel) : false;
		if (x >= Chunk::sizeX)
			return px ? px->tryGet(x - Chunk::sizeX, y, z, voxel) : false;
		if (y < 0)
			return ny ? ny->tryGet(x, y + Chunk::sizeY, z, voxel) : false;
		if (y >= Chunk::sizeY)
			return py ? py->tryGet(x, y - Chunk::sizeY, z, voxel) : false;
		if (z < 0)
			return nz ? nz->tryGet(x, y, z + Chunk::sizeZ, voxel) : false;
		if (z >= Chunk::sizeZ)
			return pz ? pz->tryGet(x, y, z - Chunk::sizeZ, voxel) : false;
		return c ? c->tryGet(x, y, z, voxel) : false;
	}
	/**
	 * @brief Sets cluster voxel at specified 3D position if inside cluster bounds.
	 * @return True if voxel position is inside cluster bounds, otherwise false.
	 *
	 * @param x voxel position along X-axis
	 * @param y voxel position along Y-axis
	 * @param z voxel position along Z-axis
	 * @param voxel target voxel ID
	 */
	bool trySet(uint16_t x, uint16_t y, uint16_t z, Voxel voxel) noexcept
	{
		if (x < 0)
			return nx ? nx->trySet(x + Chunk::sizeX, y, z, voxel) : false;
		if (x >= Chunk::sizeX)
			return px ? px->trySet(x - Chunk::sizeX, y, z, voxel) : false;
		if (y < 0)
			return ny ? ny->trySet(x, y + Chunk::sizeY, z, voxel) : false;
		if (y >= Chunk::sizeY)
			return py ? py->trySet(x, y - Chunk::sizeY, z, voxel) : false;
		if (z < 0)
			return nz ? nz->trySet(x, y, z + Chunk::sizeZ, voxel) : false;
		if (z >= Chunk::sizeZ)
			return pz ? pz->trySet(x, y, z - Chunk::sizeZ, voxel) : false;
		return c ? c->trySet(x, y, z, voxel) : false;
	}
};

};