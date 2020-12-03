#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <utility>

int main()
{
	std::cout << "Third day of Advent code!" << std::endl;
	std::fstream input("../../../data/03/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	// Read in input data
	std::vector<std::string> input_data;
	std::string line;
	bool first_line{ true };
	while (getline(input, line))
	{
		// We do not need the first line
		if (first_line)
		{
			first_line = false;
			continue;
		}
		input_data.push_back(std::move(line));
	}

	auto countTrees = [&](int row_stride, int column_stride) -> int
	{
		int num_trees{ 0 }, row{ 1 }, position{ 0 };
		for (const auto& pattern : input_data)
		{
			if (row++ % row_stride == 0)
			{
				position += column_stride;
				// We simply have to check the pattern now
				if (pattern.at(position % pattern.length()) == '#')
					++num_trees;
			}
		}
		std::cout << "Num Tress for TC<" << row_stride << "|" << column_stride << "> is: " << num_trees << std::endl;
		return num_trees;
	};

	// Check path through the forest with pattern 3 right 1 down
	int num_trees{ countTrees(1, 3) };
	std::cout << "The number of trees hit is: " << num_trees << std::endl;
	
	// Different patterns
	unsigned long long num_trees_2{ static_cast<unsigned long long>(num_trees) };
	std::vector<std::pair<int, int>> further_testcases{ {1,1}, {1,5}, {1,7}, {2,1} };
	for (const auto [row_stride, column_stride] : further_testcases)
	{
		num_trees_2 *= static_cast<unsigned long long>(countTrees(row_stride, column_stride));
	}
	std::cout << "The number of trees hit the second time is: " << num_trees_2 << std::endl;

	return 0;
}