#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include <numeric>

int main()
{
	std::cout << "Tenth day of Advent code!" << std::endl;
	std::fstream input("../../../data/10/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	std::vector<int> input_data;
	input_data.push_back(0);
	int val;
	while (input >> val)
	{
		input_data.push_back(val);
	}

	// Sort input data
	std::sort(input_data.begin(), input_data.end());

	// First task
	int one_jolt_difference{ 0 };
	int three_jolt_difference{ 1 };
	for (auto i = 0; i < input_data.size() - 1; ++i)
	{
		auto diff = input_data[i + 1] - input_data[i];
		if (diff == 1)
			++one_jolt_difference;
		else if (diff == 3)
			++three_jolt_difference;
	}

	std::cout << "Output of first task is: " << one_jolt_difference * three_jolt_difference << std::endl;

	// Second task
	std::vector<unsigned long long> num_paths_per_node(input_data.size());
	std::memset(num_paths_per_node.data(), 0, num_paths_per_node.size());
	num_paths_per_node[input_data.size() - 1] = 1;
	for (int i = input_data.size() - 1; i > 0; --i)
	{
		// Go over field in reverse order, potentially look back three elements
		
		unsigned long long* first_element = (i - 1 >= 0 && input_data[i] - input_data[i - 1] <= 3) ? &num_paths_per_node[i - 1] : nullptr;
		unsigned long long* second_element = (i - 2 >= 0 && input_data[i] - input_data[i - 2] <= 3) ? &num_paths_per_node[i - 2] : nullptr;
		unsigned long long* third_element = (i - 3 >= 0 && input_data[i] - input_data[i - 3] <= 3) ? &num_paths_per_node[i - 3] : nullptr;
		if (first_element)
		{
			*first_element += num_paths_per_node[i];
		}
		if (second_element)
		{
			*second_element += num_paths_per_node[i];
		}
		if (third_element)
		{
			*third_element += num_paths_per_node[i];
		}
	}

	std::cout << "Output of second task is: " << num_paths_per_node[0] << std::endl;

	return 0;

}