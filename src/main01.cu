#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "Timer.cuh"

int main()
{
	std::cout << "First of Advent code!" << std::endl;
	std::fstream input("../../../data/01/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}
	std::vector<int> input_data;
	int val;
	while (input >> val)
	{
		input_data.push_back(val);
	}

	CPUTimer cpu_timer;
	int num_iterations{ 1 };

	// CPU version
	int first_num{ 0 }, second_num{ 0 };
	
	for (auto iter = 0; iter < num_iterations; ++iter)
	{
		cpu_timer.start_clock();
		bool found_nums{ false };
		for (const auto first : input_data)
		{
			for (const auto second : input_data)
			{
				if (first + second == 2020)
				{
					std::cout << "We found it with first num: " << first << " and second num: " << second << std::endl;
					first_num = first;
					second_num = second;
					found_nums = true;
					break;
				}
			}
			if (found_nums)
				break;
		}
		cpu_timer.end_clock();
	}
	std::cout << "The product of these numbers is: " << first_num * second_num << std::endl;

	return 0;
}