#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include <numeric>

template <typename Iterator>
bool findTwoNumbersThatSumToNumber(Iterator begin, Iterator end, int num)
{
	for (auto range_start = begin; range_start != end - 1; ++range_start)
	{
		for (auto range_end = range_start + 1; range_end != end; ++range_end)
		{
			if (*range_start + *range_end == num)
			{
				return true;
			}
		}
	}
	return false;
}

int findXMASWeakness(const std::vector<int>& input, int invalid_num)
{
	for (auto range_start = input.begin(); range_start < input.end() - 1; ++range_start)
	{
		for (auto range_end = range_start + 1; range_end != input.end(); ++range_end)
		{
			int range_sum = std::accumulate(range_start, range_end + 1, 0);
			if (range_sum == invalid_num)
				return (*min_element(range_start, range_end + 1) + *max_element(range_start, range_end + 1));
			else if (range_sum > invalid_num)
				break;
		}
	}

	return 0;
}

int main()
{
	std::cout << "Ninth day of Advent code!" << std::endl;
	std::fstream input("../../../data/09/input.txt", std::ios_base::in);
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

	// First task
	static constexpr int preamble_length{ 25 };
	int i{ preamble_length };
	for (; i < input_data.size(); ++i)
	{
		// See if you can build input_data[i] from two numbers in [i-preamble_length, i-1] using addition
		if (!findTwoNumbersThatSumToNumber(std::next(input_data.begin(), i - preamble_length), std::next(input_data.begin(), i), input_data[i]))
			break;
	}
	
	int invalid_number = input_data[i];
	std::cout << "The invalid number is: " << invalid_number << std::endl;

	// Second task
	int weakness = findXMASWeakness(input_data, invalid_number);
	std::cout << "The weakness is: " << weakness << std::endl;

	return 0;
}