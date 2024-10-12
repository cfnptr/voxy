# Voxy

A library providing API for working with **voxel** structures.

Voxel structures are used to store and manage 3D voxel data. Essentially, it's a three-dimensional grid, 
where each cell in the grid (voxel) contains information about a small volume of space in a 3D environment. 
The structure allows for efficient storage, retrieval, and manipulation of voxel data, such as position, 
color, density, or any other attributes tied to each voxel.

See the [documentation](https://cfnptr.github.io/voxy)

## Supported operating systems

* Windows (10/11)
* Ubuntu (22.04/24.04)
* macOS (14/15)

This list includes only those systems on which functionality testing is conducted.
However, you can also compile it under any other Linux distribution or operating system.

## Build requirements

* C++17 compiler
* [Git 2.30+](https://git-scm.com/)
* [CMake 3.16+](https://cmake.org/)

### CMake options

| Name                | Description                | Default value |
|---------------------|----------------------------|---------------|
| VOXY_BUILD_SHARED   | Build Voxy shared library  | `ON`          |
| VOXY_BUILD_TESTS    | Build Voxy library tests   | `ON`          |

### CMake targets

| Name        | Description            |
|-------------|------------------------|
| voxy        | Interface Voxy library |

## Cloning

```
git clone https://github.com/cfnptr/voxy
```

## Building ![CI](https://github.com/cfnptr/voxy/actions/workflows/cmake.yml/badge.svg)

* Windows: ```./scripts/build-release.bat```
* macOS / Ubuntu: ```./scripts/build-release.sh```