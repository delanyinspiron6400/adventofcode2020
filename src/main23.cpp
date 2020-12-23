#include <iostream>
#include <string>
#include <charconv>
#include <list>
#include <algorithm>
#include <vector>
#include <map>

static constexpr unsigned int num_rounds{ 10 };
static constexpr unsigned int num_rounds_2{ 10000000 };
//static constexpr unsigned int num_rounds_2{ 10 };
static constexpr unsigned int num_elements{ 1000000 };
//static constexpr unsigned int num_elements{ 9 };

char intToChar(int val)
{
	return '0' + val;
}

int charToInt(char val)
{
	return val - '0';
}

void task1()
{
	//std::string current_game{ "219748365" };
	std::string current_game{ "389125467" };
	for (auto i = 9; i < num_elements; ++i)
		current_game.push_back(i);
	std::string side_list(3, ' ');

	auto curr_element_pos{ 0 };
	auto curr_element = current_game.at(curr_element_pos);
	int destination_element_num = charToInt(curr_element) - 1;
	char destination_element = intToChar(destination_element_num);
	auto destination_pos = curr_element_pos;
	for (auto round = 0; round < num_rounds; ++round)
	{
		std::cout << "-- move " << round + 1 << " --" << std::endl;
		//std::cout << "Cups: ";
		//for (auto val : current_game)
		//{
		//	if (val == curr_element)
		//		std::cout << "(" << val << ") ";
		//	else
		//		std::cout << val << " ";
		//}
		//std::cout << std::endl;
		// --------------------------------------------------
		// Remove the 3 elements after the current element
		//std::cout << "pick up: ";
		for (auto i = 1; i <= 3; ++i)
		{
			//std::cout << current_game.at((curr_element_pos + i) % num_elements) << " ";
			side_list[i - 1] = current_game.at((curr_element_pos + i) % num_elements);
		}
		//std::cout << std::endl;
		if (curr_element_pos + 3 < num_elements)
		{
			current_game.erase((curr_element_pos + 1), 3);
		}
		else
		{
			// Delete from back
			current_game.erase((curr_element_pos + 1), num_elements - (curr_element_pos + 1));
			// Delete from front
			auto sub_value = 3 - (num_elements - (curr_element_pos + 1));
			current_game.erase(0, sub_value);
			curr_element_pos -= sub_value;
		}

		// --------------------------------------------------
		// Find destination
		while ((destination_pos = current_game.find(destination_element)) == std::string::npos)
		{
			destination_element_num = (destination_element_num == 0) ? num_elements : destination_element_num - 1;
			destination_element = intToChar(destination_element_num);
		}

		//std::cout << "destination: " << destination_element << std::endl;
		//getchar();

		// --------------------------------------------------
		// Insert list after destination
		current_game.insert(destination_pos + 1, side_list);

		// --------------------------------------------------
		// Select next curr_element_pos
		curr_element_pos = (current_game.find(curr_element) + 1) % num_elements;
		curr_element = current_game.at(curr_element_pos);
		//std::cout << curr_element_pos << " | " << curr_element << std::endl;
		destination_element_num = charToInt(curr_element) - 1;
		destination_element = intToChar(destination_element_num);
	}

	std::cout << "Task1: " << current_game << std::endl;
}

void task2()
{
	std::vector<int> current_game{ 2,1,9,7,4,8,3,6,5 };
	//std::map<int, int> mapping;
	//std::vector<int> current_game{ 3,8,9,1,2,5,4,6,7 };
	for (auto i = 10; i <= num_elements; ++i)
		current_game.push_back(i);
	//for (auto i = 0; i < num_elements; ++i)
	//{
	//	mapping[current_game[i]] = i;
	//}
	std::vector<int> side_list;

	auto curr_element_pos{ 0 };
	auto curr_element = current_game.front();
	auto destination_element = curr_element - 1;
	auto destination_iter = current_game.begin();
	for (auto round = 0; round < num_rounds_2; ++round)
	{
		if(round % 10000 == 0)
			std::cout << "-- move " << round + 1 << " --" << std::endl;
		//std::cout << "Cups: ";
		//for (auto val : current_game)
		//{
		//	if (val == curr_element)
		//		std::cout << "(" << val << ") ";
		//	else
		//		std::cout << val << " ";
		//}
		//std::cout << std::endl;
		// --------------------------------------------------
		// Remove the 3 elements after the current element
		auto iter_range_begin = current_game.begin();
		std::advance(iter_range_begin, curr_element_pos + 1);
		//std::cout << "pick up: ";
		if (curr_element_pos + 3 < num_elements)
		{
			auto iter_range_end = iter_range_begin;
			std::advance(iter_range_end, 3);
			//for(auto it = iter_range_begin; it != iter_range_end; ++it)
			//	std::cout << *it << " ";
			//std::cout << std::endl;
			side_list.insert(side_list.begin(),  iter_range_begin, iter_range_end);
			current_game.erase(iter_range_begin, iter_range_end);
		}
		else
		{
			auto elements_at_back{ num_elements - curr_element_pos - 1 };
			// Delete from back
			if (elements_at_back)
			{
				auto iter_range_end = iter_range_begin;
				std::advance(iter_range_end, elements_at_back);
				side_list.insert(side_list.begin(), iter_range_begin, iter_range_end);
				current_game.erase(iter_range_begin, iter_range_end);
			}

			// Delete from front
			auto elements_remaining = 3 - elements_at_back;
			iter_range_begin = current_game.begin();
			auto iter_range_end = iter_range_begin;
			std::advance(iter_range_end, elements_remaining);
			side_list.insert(side_list.end(), iter_range_begin, iter_range_end);
			current_game.erase(iter_range_begin, iter_range_end);
		}

		// --------------------------------------------------
		// Find destination
		while ((destination_iter = std::find(current_game.begin(), current_game.end(), destination_element)) == current_game.end())
		{
			destination_element = (destination_element == 0) ? num_elements : destination_element - 1;
		}

		//std::cout << "destination: " << *destination_iter << std::endl;
		//getchar();

		// --------------------------------------------------
		// Insert list after destination
		auto insert_iter = destination_iter;
		std::advance(insert_iter, 1);
		if (insert_iter == current_game.end())
			insert_iter = current_game.begin();
		current_game.insert(insert_iter, side_list.begin(), side_list.end());
		side_list.clear();

		// --------------------------------------------------
		// Select next curr_element_pos
		auto curr_element_iter{ std::find(current_game.begin(), current_game.end(), curr_element) };
		std::advance(curr_element_iter, 1);
		if (curr_element_iter == current_game.end())
			curr_element_iter = current_game.begin();
		curr_element_pos = (std::distance(current_game.begin(), curr_element_iter));
		
		curr_element = *curr_element_iter;
		//std::cout << curr_element_pos << " | " << curr_element << std::endl;
		destination_element = curr_element - 1;
	}

	auto one_element_iter{ std::find(current_game.begin(), current_game.end(), 1) };
	std::advance(one_element_iter, 1);
	if (one_element_iter == current_game.end())
		one_element_iter = current_game.begin();
	unsigned long long first_element_after = static_cast<unsigned long long>(*one_element_iter);
	std::cout << first_element_after << " | ";
	std::advance(one_element_iter, 1);
	if (one_element_iter == current_game.end())
		one_element_iter = current_game.begin();
	unsigned long long second_element_after = static_cast<unsigned long long>(*one_element_iter);
	std::cout << second_element_after << " | ";

	std::cout << "Task2: " << first_element_after * second_element_after << std::endl;
}

int main()
{
	std::cout << "Twentysecond day of Advent code!" << std::endl;

	//task1();
	std::cout << "---------------------------------------\n";
	std::cout << "---------------------------------------\n";
	task2();

	return 0;
}