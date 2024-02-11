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

#pragma once
#include "voxy/chunk.hpp"

namespace voxy
{

// Side chunks including center.
const uint8_t chunkClusterSize = 7;

template<class C, typename V>
struct Cluster3
{
	typedef C Chunk;
	typedef V Voxel;

	Chunk *c, *nx, *px, *ny, *py, *nz, *pz;

	Cluster3(Chunk* c = nullptr,
		Chunk* nx = nullptr, Chunk* px = nullptr,
		Chunk* ny = nullptr, Chunk* py = nullptr,
		Chunk* nz = nullptr, Chunk* pz = nullptr)
	{
		this->c = c; this->nx = nx; this->px = px; this->ny = ny;
		this->py = py; this->nz = nz; this->pz = pz;
	}

	bool isComplete() const noexcept
	{
		return c && nx && px && ny && py && nz && pz;
	}

	Voxel get(int16_t x, int16_t y, int16_t z) const noexcept
	{
		assert(isComplete());
		if (x < 0) return nx->get(x + Chunk::SizeX, y, z);
		if (x >= Chunk::SizeX) return px->get(x - Chunk::SizeX, y, z);
		if (y < 0) return ny->get(x, y + Chunk::SizeY, z);
		if (y >= Chunk::SizeY) return py->get(x, y - Chunk::SizeY, z);
		if (z < 0) return nz->get(x, y, z + Chunk::SizeZ);
		if (z >= Chunk::SizeZ) return pz->get(x, y, z - Chunk::SizeZ);
		return c->get(x, y, z);
	}
	void set(uint8_t x, uint8_t y, uint8_t z, Voxel voxel) noexcept
	{
		assert(isComplete());
		if (x < 0) return nx->set(x + Chunk::SizeX, y, z, voxel);
		if (x >= Chunk::SizeX) return px->set(x - Chunk::SizeX, y, z, voxel);
		if (y < 0) return ny->set(x, y + Chunk::SizeY, z, voxel);
		if (y >= Chunk::SizeY) return py->set(x, y - Chunk::SizeY, z, voxel);
		if (z < 0) return nz->set(x, y, z + Chunk::SizeZ, voxel);
		if (z >= Chunk::SizeZ) return pz->set(x, y, z - Chunk::SizeZ, voxel);
		return c->set(x, y, z, voxel);
	}

	bool tryGet(uint8_t x, uint8_t y, uint8_t z, Voxel& voxel) const noexcept
	{
		if (x < 0) return nx ? nx->tryGet(x + Chunk::SizeX, y, z, voxel) : false;
		if (x >= Chunk::SizeX) return px ? px->tryGet(x - Chunk::SizeX, y, z, voxel) : false;
		if (y < 0) return ny ? ny->tryGet(x, y + Chunk::SizeY, z, voxel) : false;
		if (y >= Chunk::SizeY) return py ? py->tryGet(x, y - Chunk::SizeY, z, voxel) : false;
		if (z < 0) return nz ? nz->tryGet(x, y, z + Chunk::SizeZ, voxel) : false;
		if (z >= Chunk::SizeZ) return pz ? pz->tryGet(x, y, z - Chunk::SizeZ, voxel) : false;
		return c ? c->tryGet(x, y, z, voxel) : false;
	}
	bool trySet(uint8_t x, uint8_t y, uint8_t z, Voxel voxel) noexcept
	{
		if (x < 0) return nx ? nx->trySet(x + Chunk::SizeX, y, z, voxel) : false;
		if (x >= Chunk::SizeX) return px ? px->trySet(x - Chunk::SizeX, y, z, voxel) : false;
		if (y < 0) return ny ? ny->trySet(x, y + Chunk::SizeY, z, voxel) : false;
		if (y >= Chunk::SizeY) return py ? py->trySet(x, y - Chunk::SizeY, z, voxel) : false;
		if (z < 0) return nz ? nz->trySet(x, y, z + Chunk::SizeZ, voxel) : false;
		if (z >= Chunk::SizeZ) return pz ? pz->trySet(x, y, z - Chunk::SizeZ, voxel) : false;
		return c ? c->trySet(x, y, z, voxel) : false;
	}
};

};