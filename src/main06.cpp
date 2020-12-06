#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include <bitset>

int main()
{
	std::cout << "Sixth day of Advent code!" << std::endl;
	std::fstream input("../../../data/06/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	std::vector<std::vector<std::string>> input_data;
	std::string line;
	bool new_group{ true };
	while (getline(input, line))
	{
		// Empty lines indicate a new passport starting
		if (line.empty())
		{
			new_group = true;
			continue;
		}
		if (new_group)
		{
			std::vector<std::string> group(1);
			group[0] = std::move(line);
			input_data.push_back(std::move(group));
			new_group = false;
		}
		else
		{
			input_data[input_data.size() - 1].push_back(std::move(line));
		}
	}

	// Group Identifiers
	std::vector<std::bitset<26>> customs_decl;
	customs_decl.reserve(input_data.size());
	for (auto& group : input_data)
	{
		std::bitset<26> bits;
		for (auto& person : group)
		{
			// Go over string and set corresponding bits
			for (auto i = 0; i < person.length(); ++i)
			{
				bits.set(person.at(i) - 'a');
			}
		}
		customs_decl.push_back(std::move(bits));
	}

	size_t num_bits_set_per_group{ 0 };
	for (auto& bits : customs_decl)
		num_bits_set_per_group += bits.count();

	std::cout << "The number of questions answered as true is: " << num_bits_set_per_group << std::endl;

	// Second tasks

	// Group Identifiers for second task
	std::vector<std::bitset<26>> customs_decl_excl;
	customs_decl_excl.reserve(input_data.size());
	for (auto& group : input_data)
	{
		std::bitset<26> bits;
		bool first_person{ true };
		for (auto& person : group)
		{
			std::bitset<26> person_bits;
			// Go over string and set corresponding bits
			for (auto i = 0; i < person.length(); ++i)
			{
				person_bits.set(person.at(i) - 'a');
			}
			if (first_person)
			{
				bits = std::move(person_bits);
				first_person = false;
			}
			else
				bits &= person_bits;
		}
		customs_decl_excl.push_back(std::move(bits));
	}

	num_bits_set_per_group = 0;
	for (auto& bits : customs_decl_excl)
		num_bits_set_per_group += bits.count();

	std::cout << "The number of questions answered exclusively as true is: " << num_bits_set_per_group << std::endl;

	return 0;
}