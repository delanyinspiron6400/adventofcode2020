#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <utility>

#include "Timer.cuh"

struct Token
{
	int lower_bound{ 0 };
	int upper_bound{ 0 };
	char character;
};

int main()
{
	std::cout << "Second day of Advent code!" << std::endl;
	std::fstream input("../../../data/02/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	// Parse input data
	std::string line;
	std::vector<std::pair<Token, std::string>> input_data;
	while (std::getline(input, line))
	{
		std::string range = line.substr(0, line.find(" "));
		line = std::move(line.substr(line.find(" ") + 1));
		Token token{ std::stoi(range.substr(0,range.find("-"))), std::stoi(range.substr(range.find("-") + 1)), line.at(0) };
		input_data.push_back(std::make_pair(token, std::move(line.substr(line.find(" ") + 1))));
	}

	int num_valid_passwords{ 0 };
	int num_valid_passwords2{ 0 };
	for (auto& element : input_data)
	{
		Token token = std::move(element.first);
		std::string password = std::move(element.second);
		// Check if password is valid
		auto occurence = std::count(password.begin(), password.end(), token.character);
		if (occurence >= token.lower_bound && occurence <= token.upper_bound)
		{
			std::cout << "We found a valid password with condition: <" << token.lower_bound << "-" << token.upper_bound << " | " << token.character;
			std::cout << "> - " << password << std::endl;
			++num_valid_passwords;
		}
		// Check second output
		if (password.at(token.lower_bound - 1) == token.character)
		{
			if (password.at(token.upper_bound - 1) != token.character)
				++num_valid_passwords2;
		}
		else
		{
			if (password.at(token.upper_bound - 1) == token.character)
				++num_valid_passwords2;
		}
	}
	std::cout << "The number of valid passwords <old_standard> is: " << num_valid_passwords << std::endl;
	std::cout << "The number of valid passwords <new_standard> is: " << num_valid_passwords2 << std::endl;
	return 0;
}