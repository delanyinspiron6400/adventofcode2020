#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <cuda_runtime_api.h>

#include "Timer.cuh"

template <int ... NUMS>
struct LoopUnrolled;

template <int OFFSET, int BUS_ID, int ... NUMS>
struct LoopUnrolled<OFFSET, BUS_ID, NUMS...>
{
	using Next = LoopUnrolled<NUMS...>;

	static constexpr __device__ bool calcResult(unsigned long long start)
	{
		return ((start + OFFSET) % BUS_ID) ? false : Next::calcResult(start);
	}
};

template <>
struct LoopUnrolled<>
{
	static constexpr __device__ bool calcResult(unsigned long long start) { return true; }
};

template <int FIRST_NUM, int ... NUMS>
__global__ void d_brute_force(unsigned long long* early_time, unsigned int* global_stop)
{
	unsigned long long start = ((threadIdx.x + blockIdx.x * blockDim.x)) * FIRST_NUM;
	for (unsigned long long iter = 0; true; ++iter, start += gridDim.x * blockDim.x * FIRST_NUM)
	{
		if (iter % 10000 == 0)
		{
			if (threadIdx.x + blockIdx.x * blockDim.x == 0)
				printf("Iteration: %llu\n", start / 1000000000ULL);
			auto remainder = atomicCAS(global_stop, 0xFFFFFFFF, 0);
			if (remainder == 1)
				return;
		}
		if(LoopUnrolled<NUMS...>::calcResult(start))
			break;
	}

	*early_time = start;
	atomicExch(global_stop, 1);
}

int main()
{
	std::cout << "Thirteenth day of Advent code!" << std::endl;
	std::fstream input("../../../data/13/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	int earliest_departure{ 0 };
	std::vector<int> buslines;
	std::vector<int> indices;
	std::vector<unsigned long long> buslines_ll;
	std::vector<std::pair<int, unsigned long long>> bus_ordering;
	input >> earliest_departure;
	std::string table_entry;
	int index{ 0 };
	while (getline(input, table_entry, ','))
	{
		if (table_entry.compare("x") != 0)
		{
			buslines.push_back(std::stoi(table_entry));
			bus_ordering.push_back(std::make_pair(index, static_cast<unsigned long long>(std::stoi(table_entry))));
			indices.push_back(index);
			buslines_ll.push_back(static_cast<unsigned long long>(std::stoi(table_entry)));
		}
		++index;
	}

	// Task 1
	int min_bus_id{std::numeric_limits<int>::max()};
	int min_add_time{ std::numeric_limits<int>::max() };
	for (auto bus : buslines)
	{
		int min_after = bus - (earliest_departure % bus);
		if (min_after < min_add_time)
		{
			min_add_time = min_after;
			min_bus_id = bus;
		}
	}

	std::cout << "Bus : " << min_bus_id << " is available " << min_add_time << " minutes after earliest depature time, so result is: " << min_add_time * min_bus_id << std::endl;

	// Task 2
	unsigned long long* d_early_time{ nullptr };
	unsigned int* d_global_stop{ nullptr };
	cudaMalloc(reinterpret_cast<void**>(&d_early_time), sizeof(unsigned long long));
	cudaMalloc(reinterpret_cast<void**>(&d_global_stop), sizeof(unsigned int));
	cudaMemset(d_early_time, 0, sizeof(unsigned long long));
	cudaMemset(d_global_stop, 0, sizeof(unsigned int));
	
	int blockSize{ 256 };
	int num_blocks_per_sm = 0;
	cudaOccupancyMaxActiveBlocksPerMultiprocessor(&num_blocks_per_sm, d_brute_force<19, 13, 37, 19, 883, 27, 23, 32, 13, 36, 17, 50, 797, 60, 41, 79, 29>, blockSize, 0);
	int h_sm_count;
	cudaDeviceGetAttribute(&h_sm_count, cudaDevAttrMultiProcessorCount, 0);
	int gridSize = h_sm_count* num_blocks_per_sm;
	
	unsigned long long early_time{ 0ULL };
	CPUTimer timer;
	timer.start_clock();

	d_brute_force<19, 13, 37, 19, 883, 27, 23, 32, 13, 36, 17, 50, 797, 60, 41, 79, 29> << < gridSize, blockSize >> > (d_early_time, d_global_stop);
	cudaMemcpy(&early_time, d_early_time, sizeof(unsigned long long), cudaMemcpyDeviceToHost);
	float timing = timer.end_clock();
	if (early_time != 0ULL)
		std::cout << "Earliest timestamp according to criterion is: " << early_time << ", found in " << timing / 1000 << " seconds" << std::endl;
	else
		std::cout << "Did not find timestamp with this datatype" << std::endl;

	return 0;
}