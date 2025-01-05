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

#include "voxy/cluster.hpp"

#include <cstdlib>
#include <stdexcept>

using namespace std;
using namespace voxy;

typedef Chunk3D<16, 16, 16, uint8_t> Chunk;
typedef Cluster3D<Chunk, uint8_t> Cluster;

int main()
{
	Cluster::Chunk chunks[7] = {};

	Cluster cluster(&chunks[0], &chunks[1], &chunks[2],
		&chunks[3], &chunks[4], &chunks[5], &chunks[6]);
	
	return EXIT_SUCCESS;
}