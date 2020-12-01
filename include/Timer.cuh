#pragma once

#include <chrono>
#include <algorithm>
#include <numeric>
#include "cuda_runtime_api.h"

#include "UtilityFunctions.cuh"

struct Result
{
	float mean_{ 0.0f };
	float std_dev_{ 0.0f };
	float median_{ 0.0f };
	float min_{ 0.0f };
	float max_{ 0.0f };
	int num_{ 0 };
};

struct CPUTimer
{
	decltype(std::chrono::high_resolution_clock::now()) t0, t1;
	std::vector<float> measurements_;

	void inline start_clock()
	{
		t0 = std::chrono::high_resolution_clock::now();
	}

	float inline end_clock()
	{
		t1 = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count() / 1000000.0f;
	}

	void addMeasure(CPUTimer& measure)
	{
		measurements_.insert(measurements_.end(), measure.measurements_.begin(), measure.measurements_.end());
	}

	float mean()
	{
		return std::accumulate(measurements_.begin(), measurements_.end(), 0.0f) / static_cast<float>(measurements_.size());
	}

	float median()
	{
		std::vector<float> sorted_measurements(measurements_);
		std::sort(sorted_measurements.begin(), sorted_measurements.end());
		return sorted_measurements[sorted_measurements.size() / 2];
	}

	float std_dev(float mean)
	{
		std::vector<float> stdmean_measurements(measurements_);
		for (auto& elem : stdmean_measurements)
			elem = (elem - mean) * (elem - mean);
		return sqrt(std::accumulate(stdmean_measurements.begin(), stdmean_measurements.end(), 0.0f) / static_cast<float>(stdmean_measurements.size()));
	}

	Result generateResult()
	{
		if (measurements_.size() == 0)
			return Result{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0 };
		float mean_val = mean();
		return Result{
			mean_val,
			std_dev(mean_val),
			median(),
			*std::min_element(measurements_.begin(), measurements_.end()),
			*std::max_element(measurements_.begin(), measurements_.end()),
			static_cast<int>(measurements_.size())
		};
	}
};

struct GPUTimer
{
	cudaEvent_t ce_start, ce_stop;
	std::vector<float> measurements_;

	void start_clock() 
	{ 
		Utils::start_clock(ce_start, ce_stop);
	}

	float end_clock() 
	{ 
		auto timing = Utils::end_clock(ce_start, ce_stop);
		measurements_.push_back(timing);
		return timing;
	}

	void addMeasure(GPUTimer& measure)
	{
		measurements_.insert(measurements_.end(), measure.measurements_.begin(), measure.measurements_.end());
	}

	float mean()
	{
		return std::accumulate(measurements_.begin(), measurements_.end(), 0.0f) / static_cast<float>(measurements_.size());
	}

	float median()
	{
		std::vector<float> sorted_measurements(measurements_);
		std::sort(sorted_measurements.begin(), sorted_measurements.end());
		return sorted_measurements[sorted_measurements.size() / 2];
	}

	float std_dev(float mean)
	{
		std::vector<float> stdmean_measurements(measurements_);
		for (auto& elem : stdmean_measurements)
			elem = (elem - mean) * (elem - mean);
		return sqrt(std::accumulate(stdmean_measurements.begin(), stdmean_measurements.end(), 0.0f) / static_cast<float>(stdmean_measurements.size()));
	}

	Result generateResult()
	{
		if (measurements_.size() == 0)
			return Result{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0 };
		float mean_val = mean();
		return Result{
			mean_val,
			std_dev(mean_val),
			median(),
			*std::min_element(measurements_.begin(), measurements_.end()),
			*std::max_element(measurements_.begin(), measurements_.end()),
			static_cast<int>(measurements_.size())
		};
	}
};