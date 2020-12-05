#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

//#include "Timer.cuh"

int main()
{
	std::cout << "First day of Advent code!" << std::endl;
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

	//CPUTimer cpu_timer;
	int num_iterations{ 1 };

	// CPU version
	int first_num{ 0 }, second_num{ 0 };
	
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
	std::cout << "The product of these numbers is: " << first_num * second_num << std::endl;

	int third_num{ 0 };
	found_nums = false;
	for (const auto first : input_data)
	{
		for (const auto second : input_data)
		{
			for (const auto third : input_data)
			{
				if (first + second + third == 2020)
				{
					std::cout << "We found it with first num: " << first << " and second num: " << second << " and third num: " << third << std::endl;
					first_num = first;
					second_num = second;
					third_num = third;
					found_nums = true;
					break;
				}
			}
			if (found_nums)
				break;
		}
		if (found_nums)
			break;
	}

	std::cout << "The product of these numbers is: " << first_num * second_num * third_num << std::endl;

	return 0;
}