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
#include "voxy/voxel.hpp"

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cassert>

namespace voxy
{

#define posToChunkIndex(x, y, z, sizeX, sizeXY) \
	((z) * (sizeXY) + (y) * (sizeX) + (x))

template<uint8_t SX, uint8_t SY, uint8_t SZ, typename V>
class Chunk3
{
public:
	static constexpr uint8_t SizeX = SX;
	static constexpr uint8_t SizeY = SY;
	static constexpr uint8_t SizeZ = SZ;
	static constexpr size_t SizeXY = SX * SY;
	static constexpr size_t Size = SX * SY * SZ;
	typedef V Voxel;
protected:
	Voxel voxels[Size];
public:
	Chunk3() = default;
	Chunk3(Voxel voxel) { fill(voxel); }
	Chunk3(const Voxel* voxels, uint8_t sizeX, uint8_t sizeY, uint8_t sizeZ,
		uint8_t offsetX = 0, uint8_t offsetY = 0, uint8_t offsetZ = 0)
	{
		fill(voxels, sizeX, sizeY, sizeZ, offsetX, offsetY, offsetZ);
	}

	Voxel* getVoxels() noexcept { return voxels; }
	const Voxel* getVoxels() const noexcept { return voxels; }

	Voxel get(uint8_t x, uint8_t y, uint8_t z) const noexcept
	{
		assert(x < SX);
		assert(y < SY);
		assert(z < SZ);
		return voxels[posToChunkIndex(x, y, z, SX, SizeXY)];
	}
	void set(uint8_t x, uint8_t y, uint8_t z, Voxel voxel) noexcept
	{
		assert(x < SX);
		assert(y < SY);
		assert(z < SZ);
		voxels[posToChunkIndex(x, y, z, SX, SizeXY)] = voxel;
	}

	Voxel get(size_t index) const noexcept
	{
		assert(index < Size);
		return voxels[index];
	}
	void set(size_t index, Voxel voxel) noexcept
	{
		assert(index < Size);
		voxels[index] = voxel;
	}

	bool tryGet(uint8_t x, uint8_t y, uint8_t z, Voxel& voxel) const noexcept
	{
		if (x >= SX || y >= SY || z >= SZ) return false;
		voxel = voxels[posToChunkIndex(x, y, z, SX, SizeXY)];
		return true;
	}
	bool trySet(uint8_t x, uint8_t y, uint8_t z, Voxel voxel) noexcept
	{
		if (x >= SX || y >= SY || z >= SZ) return false;
		voxels[posToChunkIndex(x, y, z, SX, SizeXY)] = voxel;
		return true;
	}

	bool tryGet(size_t index, Voxel& voxel) const noexcept
	{
		if (index >= Size) return false;
		voxel = voxels[index];
		return true;
	}
	bool trySet(size_t index, Voxel voxel) noexcept
	{
		if (index >= Size) return false;
		voxels[index] = voxel;
		return true;
	}

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
	void fill(const Voxel* voxels, uint8_t sizeX, uint8_t sizeY, uint8_t sizeZ,
		uint8_t offsetX = 0, uint8_t offsetY = 0, uint8_t offsetZ = 0) noexcept
	{
		assert(voxels);
		assert(sizeX + offsetX <= SX);
		assert(sizeY + offsetY <= SY);
		assert(sizeZ + offsetZ <= SZ);

		if (sizeX == SX && sizeY == SY && sizeZ == SZ)
		{
			memcpy(this->voxels, voxels, Size * sizeof(Voxel));
		}
		else
		{
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
	}
};

};