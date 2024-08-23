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

#include "voxy/chunk.hpp"

#include <cstdlib>
#include <stdexcept>

using namespace std;
using namespace voxy;

typedef Chunk3<16, 16, 16, uint8_t> Chunk;

int main()
{
	Chunk chunk(nullVoxel);
	chunk.set(1, 2, 3, 100);

	if (chunk.get(1, 2, 3) != 100)
		throw runtime_error("Bad chunk voxel value.");
	
	chunk.fill(unknownVoxel);

	if (chunk.get(2, 2, 2) != unknownVoxel)
		throw runtime_error("Bad chunk voxel value.");
	
	return EXIT_SUCCESS;
}