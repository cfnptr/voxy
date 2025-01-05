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
 * @brief Common volume iteration functions.
 */

#pragma once
#include <algorithm>
#include <functional>
#include <cassert>
#include <cstdint>

namespace voxy
{

/**
 * @brief Prepares 3D volume iterator variables.
 * 
 * @tparam T type of the center and positive integer
 * @param size size of the volume along one axis in points
 * @param[out] center returns 3D volume central point
 * @param[out] positive returns positive volume layer index
 * @param[out] isEven returns if volume size even or odd
 */
template<typename T /* = uint8_t */>
static constexpr void prepareIterator3D(T size, T& center, T& positive, bool& isEven)
{
	assert(size > 1);
	center = size / 2;
	isEven = size % 2 == 0;
	if (isEven) center -= 1;
	positive = isEven ? center + 2 : center + 1;
}
/**
 * @brief Iterates over target 3D volume layer.
 * 
 * @tparam T type of the positive and negative integer
 * @param[in] onIter target function to execute on volume point iteration
 * @param positive current positive volume layer index
 * @param negative current negative volume layer index
 */
template<typename T /* = uint8_t */>
static void runLayerIterator3D(const std::function<void(T, T, T)>& onIter, T positive, T negative)
{
	assert(onIter);
	assert(positive > negative);

	T negativeOne = negative + 1, positiveOne = positive - 1;

	// Iterate over X*Y negative/positive volume layers.
	for (T y = negative; y <= positive; y++)
	{
		for (T x = negative; x <= positive; x++)
			onIter(x, y, negative);
	}
	for (T y = negative; y <= positive; y++)
	{
		for (T x = negative; x <= positive; x++)
			onIter(x, y, positive);
	}

	// Iterate over X*Z negative/positive volume layers.
	for (T z = negativeOne; z <= positiveOne; z++)
	{
		for (T x = negative; x <= positive; x++)
			onIter(x, negative, z);
	}
	for (T z = negativeOne; z <= positiveOne; z++)
	{
		for (T x = negative; x <= positive; x++)
			onIter(x, positive, z);
	}

	// Iterate over Y*Z negative/positive volume layers.
	for (T z = negativeOne; z <= positiveOne; z++)
	{
		for (T y = negativeOne; y <= positiveOne; y++)
			onIter(negative, y, z);
	}
	for (T z = negativeOne; z <= positiveOne; z++)
	{
		for (T y = negativeOne; y <= positiveOne; y++)
			onIter(positive, y, z);
	}
}
/**
 * @brief Iterates over central 3D volume layer.
 * 
 * @tparam T type of the center and positive integer
 * @param[in] onIter target function to execute on volume point iteration
 * @param center 3D volume central point
 * @param positive current positive volume layer index
 * @param isEven is volume size even or odd
 */
template<typename T /* = uint8_t */>
static void runCenterIterator3D(const std::function<void(T, T, T)>& onIter, T center, T positive, bool isEven)
{
	assert(onIter);
	assert(center > 0);
	assert(positive > 0);

	if (isEven)
	{
		for (T z = center; z < positive; z++)
		{
			for (T y = center; y < positive; y++)
			{
				for (T x = center; x < positive; x++)
					onIter(x, y, z);
			}
		}
	}
	else
	{
		onIter(center, center, center);
	}
}

/***********************************************************************************************************************
 * @brief Begins 3D volume expanding iterator.
 * 
 * @tparam T type of the size and positive/negative integer
 * @param[in] onIter target function to execute on volume point iteration
 * @param size size of the volume along one axis in points
 * @param[out] positive returns positive volume layer index
 * @param[out] negative returns negative volume layer index
 */
template<typename T /* = uint8_t */>
static void beginExpandIterator3D(const std::function<void(T, T, T)>& onIter, T size, T& positive, T& negative)
{
	assert(onIter);
	assert(size > 1);

	T center; bool isEven;
	prepareIterator3D(size, center, positive, isEven);
	runCenterIterator3D(onIter, center, positive, isEven);
	negative = center - 1;
}
/**
 * @brief Checks if we can run expanding 3D volume layer iterator.
 * 
 * @tparam T type of the size and positive integer
 * @param size size of the volume along one axis in points
 * @param positive current positive layer index
 */
template<typename T /* = uint8_t */>
static constexpr bool checkExpandIterator3D(T size, T positive)
{
	return positive < size;
}
/**
 * @brief Advances expanding 3D volume positive and negative layer index.
 * 
 * @tparam T type of the size and positive integer
 * @param[out] positive target positive volume layer index
 * @param[out] negative target negative volume layer index
 */
template<typename T /* = uint8_t */>
static constexpr void advanceExpandIterator3D(T& positive, T& negative)
{
	assert(positive > negative);
	positive++;
	negative--;
}
/**
 * @brief Iterates over 3D volume layers expanding to the outer edges.
 * 
 * @tparam T type of the size integer
 * @param[in] onIter target function to execute on volume point iteration
 * @param size size of the volume along one axis in points
 */
template<typename T /* = uint8_t */>
static void expandIterator3D(const std::function<void(T, T, T)>& onIter, T size)
{
	assert(onIter);
	assert(size > 1);
	
	T positive, negative;
	beginExpandIterator3D(onIter, size, positive, negative);

	while (checkExpandIterator3D(size, positive))
	{
		runLayerIterator3D(onIter, positive, negative);
		advanceExpandIterator3D(positive, negative);
	}
}

/***********************************************************************************************************************
 * @brief Begins 3D volume shrinking iterator.
 * 
 * @tparam T type of the size and positive/negative integer
 * @param size size of the volume along one axis in points
 * @param[out] positive returns positive volume layer index
 * @param[out] negative returns negative volume layer index
 */
template<typename T /* = uint8_t */>
static constexpr void beginShrinkIterator3D(T size, T& positive, T& negative)
{
	assert(size > 1);
	negative = 0;
	positive = size - 1;
}
/**
 * @brief Checks if we can run shrinking 3D volume layer iterator.
 * 
 * @tparam T type of the size and positive integer
 * @param positive current positive layer index
 * @param negative current negative layer index
 */
template<typename T /* = uint8_t */>
static constexpr bool checkShrinkIterator3D(T positive, T negative)
{
	return positive - negative > 1;
}
/**
 * @brief Advances shrinking 3D volume positive and negative layer index.
 * 
 * @tparam T type of the size and positive integer
 * @param[out] positive target positive volume layer index
 * @param[out] negative target negative volume layer index
 */
template<typename T /* = uint8_t */>
static constexpr void advanceShrinkIterator3D(T& positive, T& negative)
{
	assert(positive > negative);
	positive--;
	negative++;
}
/**
 * @brief Ends 3D volume shrinking iterator.
 * 
 * @tparam T type of the size and positive/negative integer
 * @param[in] onIter target function to execute on volume point iteration
 * @param size size of the volume along one axis in points
 */
template<typename T /* = uint8_t */>
static void endShrinkIterator3D(const std::function<void(T, T, T)>& onIter, T size)
{
	assert(onIter);
	assert(size > 1);

	T center, positive; bool isEven;
	prepareIterator3D(size, center, positive, isEven);
	runCenterIterator3D(onIter, center, positive, isEven);
}
/**
 * @brief Iterates over 3D volume layers shrinking to the inner center.
 * 
 * @tparam T type of the size integer
 * @param[in] onIter target function to execute on volume point iteration
 * @param size size of the volume along one axis in points
 */
template<typename T /* = uint8_t */>
static void shrinkIterator3D(const std::function<void(T, T, T)>& onIter, T size)
{
	assert(onIter);
	assert(size > 1);
	
	T positive, negative;
	beginShrinkIterator3D(size, positive, negative);

	while (checkShrinkIterator3D(positive, negative))
	{
		runLayerIterator3D(onIter, positive, negative);
		advanceShrinkIterator3D(positive, negative);
	}

	endShrinkIterator3D(onIter, size);
}

};