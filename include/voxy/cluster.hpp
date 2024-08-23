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

/**
 * @brief Chunk group size around center chunk. (including center one)
 */
const uint8_t chunkClusterSize = 7;

/***********************************************************************************************************************
 * @brief Nearby chunk group container. (including center one)
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
struct Cluster3
{
	/**
	 * @brief Cluster chunk type.
	 */
	typedef C Chunk;
	/**
	 * @brief Chunk voxel ID type.
	 */
	typedef V Voxel;

	Chunk *c, *nx, *px, *ny, *py, *nz, *pz;

	/**
	 * @brief Creates a new chunk cluster.
	 * 
	 * @param c central chunk pointer
	 * @param nx negative X-axis chunk pointer (-x)
	 * @param px positive X-axis chunk pointer (+x)
	 * @param ny negative Y-axis chunk pointer (-y)
	 * @param py positive Y-axis chunk pointer (+y)
	 * @param nz negative Z-axis chunk pointer (-z)
	 * @param pz positive Z-axis chunk pointer (+z)
	 */
	Cluster3(Chunk* c = nullptr, Chunk* nx = nullptr, Chunk* px = nullptr,
		Chunk* ny = nullptr, Chunk* py = nullptr, Chunk* nz = nullptr, Chunk* pz = nullptr)
	{
		this->c = c; this->nx = nx; this->px = px; this->ny = ny;
		this->py = py; this->nz = nz; this->pz = pz;
	}

	/**
	 * @brief Are all cluster chunks not null.
	 */
	bool isComplete() const noexcept
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
			return nx->get(x + Chunk::SizeX, y, z);
		if (x >= Chunk::SizeX)
			return px->get(x - Chunk::SizeX, y, z);
		if (y < 0)
			return ny->get(x, y + Chunk::SizeY, z);
		if (y >= Chunk::SizeY)
			return py->get(x, y - Chunk::SizeY, z);
		if (z < 0)
			return nz->get(x, y, z + Chunk::SizeZ);
		if (z >= Chunk::SizeZ)
			return pz->get(x, y, z - Chunk::SizeZ);
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
			return nx->set(x + Chunk::SizeX, y, z, voxel);
		if (x >= Chunk::SizeX)
			return px->set(x - Chunk::SizeX, y, z, voxel);
		if (y < 0)
			return ny->set(x, y + Chunk::SizeY, z, voxel);
		if (y >= Chunk::SizeY)
			return py->set(x, y - Chunk::SizeY, z, voxel);
		if (z < 0)
			return nz->set(x, y, z + Chunk::SizeZ, voxel);
		if (z >= Chunk::SizeZ)
			return pz->set(x, y, z - Chunk::SizeZ, voxel);
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
			return nx ? nx->tryGet(x + Chunk::SizeX, y, z, voxel) : false;
		if (x >= Chunk::SizeX)
			return px ? px->tryGet(x - Chunk::SizeX, y, z, voxel) : false;
		if (y < 0)
			return ny ? ny->tryGet(x, y + Chunk::SizeY, z, voxel) : false;
		if (y >= Chunk::SizeY)
			return py ? py->tryGet(x, y - Chunk::SizeY, z, voxel) : false;
		if (z < 0)
			return nz ? nz->tryGet(x, y, z + Chunk::SizeZ, voxel) : false;
		if (z >= Chunk::SizeZ)
			return pz ? pz->tryGet(x, y, z - Chunk::SizeZ, voxel) : false;
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
			return nx ? nx->trySet(x + Chunk::SizeX, y, z, voxel) : false;
		if (x >= Chunk::SizeX)
			return px ? px->trySet(x - Chunk::SizeX, y, z, voxel) : false;
		if (y < 0)
			return ny ? ny->trySet(x, y + Chunk::SizeY, z, voxel) : false;
		if (y >= Chunk::SizeY)
			return py ? py->trySet(x, y - Chunk::SizeY, z, voxel) : false;
		if (z < 0)
			return nz ? nz->trySet(x, y, z + Chunk::SizeZ, voxel) : false;
		if (z >= Chunk::SizeZ)
			return pz ? pz->trySet(x, y, z - Chunk::SizeZ, voxel) : false;
		return c ? c->trySet(x, y, z, voxel) : false;
	}
};

};