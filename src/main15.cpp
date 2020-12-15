#include <iostream>
#include <vector>
#include <map>

int main()
{
	std::cout << "Fifteenth day of Advent code!" << std::endl;
	std::vector<int> input{ 12,1,16,3,11,0 };
	input.reserve(2021);
	// Task 1
	for(int i = input.size() - 1; i < 2020; ++i)
	{
		int distance{ 0 };
		auto search_num{ input[i] };
		for(int back = i - 1; back >= 0; --back)
		{
			if(input[back] == search_num)
			{
				distance = i - back;
				break;
			}
		}
		input.push_back(distance);
	}

	std::cout << "Num is: " << input[2019] << std::endl;

	//Task 2
	// Lets switch the data structure for better speed
	std::map<int, int>input_map{ {12, 0}, { 1,1 }, { 16,2 }, { 3,3 }, { 11,4 }};
	int distance{ 0 };
	int current_element{ 0 };
	for (int i = input_map.size(); i < 30000000 - 1; ++i)
	{
		auto iter = input_map.find(current_element);
		if(iter != input_map.end())
		{
			distance = i - iter->second;
			iter->second = i;
		}
		else
		{
			input_map[current_element] = i;
			distance = 0;
		}
		current_element = distance;
	}
	std::cout << "Num is: " << distance << std::endl;
	return 0;
}