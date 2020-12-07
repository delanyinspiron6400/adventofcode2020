#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include <bitset>
#include <sstream>
#include <unordered_set>

struct Rule
{
	Rule(std::string&& container, std::vector<std::pair<int, std::string>>&& content)
		:
		container_bag{std::move(container)},
		content_bags{std::move(content)}
	{}

	Rule(std::string&& container)
		:
		container_bag{ std::move(container) }
	{}
	std::string container_bag;
	std::vector<std::pair<int, std::string>> content_bags;
};

void searchStringUnique(const std::string& search_string, const std::vector<Rule>& input, std::unordered_set<std::string>& bags_include_gold)
{
	for (const auto& item : input)
	{
		for (const auto& content : item.content_bags)
		{
			if (search_string.compare(content.second) == 0)
			{
				if (!bags_include_gold.contains(item.container_bag))
				{
					bags_include_gold.insert(item.container_bag);
					searchStringUnique(item.container_bag, input, bags_include_gold);
				}
			}
		}
	}
}

int searchContentOfBag(const std::string& search_string, const std::vector<Rule>& input, std::unordered_set<std::string>& bags_include_gold)
{
	if (bags_include_gold.contains(search_string))
		return 0;
	for (const auto& item : input)
	{
		if (item.container_bag.compare(search_string) == 0)
		{
			int num_bags{ 0 };
			for (const auto& content : item.content_bags)
			{
				num_bags += content.first + content.first * (searchContentOfBag(content.second, input, bags_include_gold));
			}
			return num_bags;
		}
	}
	return 0;
}

int main()
{
	std::cout << "Sixth day of Advent code!" << std::endl;
	std::fstream input("../../../data/07/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	std::vector<Rule> input_data;
	std::string line;
	while (getline(input, line))
	{
		std::string container_bag = line.substr(0, line.find("bags") - 1);
		std::string rules = line.substr(line.find("bags") + 13);
		if (rules.find("no other") != std::string::npos)
		{
			// We are already done
			input_data.push_back(std::move(Rule{ std::move(container_bag) }));
		}
		else
		{
			// Some rules follow
			std::stringstream rules_s(rules);
			std::string tmp_string;
			std::vector<std::pair<int, std::string>> contents;
			while (rules_s >> tmp_string)
			{
				try
				{
					int occurence = std::stoi(tmp_string);
					rules_s >> tmp_string;
					std::string colours = std::move(tmp_string);
					rules_s >> tmp_string;
					colours += std::string(" ") + tmp_string;
					contents.push_back(std::move(std::make_pair(occurence, colours)));
				}
				catch (...) {} // Empty catch
			}
			input_data.push_back(std::move(Rule{ std::move(container_bag), std::move(contents) }));
		}
	}

	std::string search_string{ "shiny gold" };
	std::unordered_set<std::string> bags_include_gold;
	searchStringUnique(search_string, input_data, bags_include_gold);

	std::cout << "We found it in " << bags_include_gold.size() << " bags." << std::endl;


	std::unordered_set<std::string> bags_in_gold_bag;
	int num_bags_in_gold_bag{ searchContentOfBag(search_string, input_data, bags_in_gold_bag) };

	std::cout << "We found " << num_bags_in_gold_bag << " bags in the shiny gold bag." << std::endl;

	return 0;
};