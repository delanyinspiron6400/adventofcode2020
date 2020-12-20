#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <variant>
#include <sstream>
#include <regex>

using RuleSet = std::vector<std::vector<int>>;

struct Rules
{
	void receiveInput(std::string&& input)
	{
		auto pos = input.find(':');
		int index = std::stoi(input.substr(0, pos));
		auto rule_text = input.substr(pos + 2);

		RuleSet rule;
		if (rule_text.at(0) == '"')
		{
			std::vector<int> char_rule;
			char_rule.push_back(char_mapping);
			rule.push_back(std::move(char_rule));
			index_to_char_map.insert(std::make_pair(char_mapping--, rule_text.at(1)));
		}
		else
		{
			RuleSet mappings;
			while (true)
			{
				auto current_rule = rule_text.substr(0, rule_text.find('|'));
				std::stringstream ss(current_rule);
				int val;
				std::vector<int>mapping;
				while (ss >> val)
				{
					mapping.push_back(val);
				}
				mappings.push_back(std::move(mapping));

				if (rule_text.find('|') != std::string::npos)
					rule_text = rule_text.substr(rule_text.find('|') + 2);
				else
					break;
			}
			rule = std::move(mappings);
		}

		rules.insert(std::make_pair(index, std::move(rule)));
	}

	RuleSet generateRuleFromOldRule(const std::vector<int>& old_rule, bool& changeOccured)
	{
		//std::cout << "GenerateRuleFromOldRule called for: ";
		changeOccured = false;
		/*for (auto val : old_rule)
			std::cout << val << "-";
		std::cout << std::endl;*/
		RuleSet new_rules(1);
		for (auto num : old_rule)
		{
			//std::cout << "Look at num: " << num << std::endl;
			if (num < char_mapping)
			{
				changeOccured = true;
				auto& this_rules = rules.at(num);
				//std::cout << "New rules have size: " << this_rules.size() << std::endl;
				if (this_rules.size() == 1)
				{
					for (auto& new_rule : new_rules)
						new_rule.insert(new_rule.end(), this_rules[0].begin(), this_rules[0].end());
					printPossibleRules(new_rules);
				}
				else
				{
					auto current_size = new_rules.size();
					for (auto i = 1; i < this_rules.size(); ++i)
					{
						for (auto j = 0; j < current_size; ++j)
						{
							new_rules.push_back(new_rules[j]);
						}
					}

					printPossibleRules(new_rules);
					//std::cout << "------------------------------------------------\n";

					for (auto i = 0; i < this_rules.size(); ++i)
					{
						for (auto j = 0; j < current_size; ++j)
						{
							auto& curr_rul = new_rules[i * current_size + j];
							curr_rul.insert(curr_rul.end(), this_rules[i].begin(), this_rules[i].end());
						}
					}

					printPossibleRules(new_rules);
				}
			}
			else
			{
				//std::cout << "Just add it to all rules" << std::endl;
				// This is already a number, just append to the end
				for (auto& new_rule : new_rules)
					new_rule.push_back(num);
			}
		}
		return new_rules;
	}

	RuleSet generateRuleFromOldRule(const std::vector<int>& old_rule, bool& changeOccured, bool& foundCircle)
	{
		//std::cout << "GenerateRuleFromOldRule called for: ";
		changeOccured = false;
		/*for (auto val : old_rule)
			std::cout << val << "-";
		std::cout << std::endl;*/
		RuleSet new_rules(1);
		foundCircle = false;
		for (auto num : old_rule)
		{
			if (num == 8 || num == 11)
			{
				foundCircle = true;
				return new_rules;
			}
			//std::cout << "Look at num: " << num << std::endl;
			if (num < char_mapping)
			{
				changeOccured = true;
				auto& this_rules = rules.at(num);
				//std::cout << "New rules have size: " << this_rules.size() << std::endl;
				if (this_rules.size() == 1)
				{
					for (auto& new_rule : new_rules)
						new_rule.insert(new_rule.end(), this_rules[0].begin(), this_rules[0].end());
					printPossibleRules(new_rules);
				}
				else
				{
					auto current_size = new_rules.size();
					for (auto i = 1; i < this_rules.size(); ++i)
					{
						for (auto j = 0; j < current_size; ++j)
						{
							new_rules.push_back(new_rules[j]);
						}
					}

					printPossibleRules(new_rules);
					//std::cout << "------------------------------------------------\n";

					for (auto i = 0; i < this_rules.size(); ++i)
					{
						for (auto j = 0; j < current_size; ++j)
						{
							auto& curr_rul = new_rules[i * current_size + j];
							curr_rul.insert(curr_rul.end(), this_rules[i].begin(), this_rules[i].end());
						}
					}

					printPossibleRules(new_rules);
				}
			}
			else
			{
				//std::cout << "Just add it to all rules" << std::endl;
				// This is already a number, just append to the end
				for (auto& new_rule : new_rules)
					new_rule.push_back(num);
			}
		}
		return new_rules;
	}

	std::string generateResultString(const std::vector<int>& rule)
	{
		std::string ret_string(rule.size(), ' ');
		for (auto i = 0; i < rule.size(); ++i)
		{
			if(index_to_char_map.find(rule[i]) != index_to_char_map.end())
				ret_string[i] = index_to_char_map.at(rule[i]);
		}
		return ret_string;
	}

	bool tryGenerateRuleSet(int start_point = 0)
	{
		RuleSet current_rules;
		RuleSet current_rule = rules.at(start_point);
		for (auto& rule : current_rule)
		{
			current_rules.push_back(rule);
		}

		// Do this as long as we have something to replace
		bool change_occured{ true };
		bool foundCircle{ false };
		while (change_occured)
		{
			change_occured = false;
			RuleSet next_rules;
			// Go over vector and find rule to replace
			for (auto& rule : current_rules)
			{
				bool local_change{ false };
				auto generated_rules = generateRuleFromOldRule(rule, local_change, foundCircle);
				if (foundCircle)
				{
					possible_rules.resize(0);
					return false;
				}
				change_occured |= local_change;
				if (local_change)
				{
					//std::cout << "Append to next rules" << std::endl;
					//printPossibleRules(generated_rules);
					next_rules.insert(next_rules.end(), generated_rules.begin(), generated_rules.end());
				}
				else
				{
					//std::cout << "Append to possible rules" << std::endl;
					possible_rules.insert(possible_rules.end(), generated_rules.begin(), generated_rules.end());
				}
			}

			std::swap(current_rules, next_rules);
			next_rules.resize(0);
		}

		// Generate valid strings
		//std::cout << "Done generating rules" << std::endl;
		possible_strings.reserve(possible_rules.size());
		for (auto& possible_rule : possible_rules)
		{
			possible_strings.push_back(std::move(generateResultString(possible_rule)));
		}
		//std::cout << "Done generating strings" << std::endl;
		//printPossibleRules2(possible_rules);
		//rules[start_point] = std::move(possible_rules);
		return true;
	}

	void generateRuleSet(int max_length=std::numeric_limits<int>::max())
	{
		RuleSet current_rules;
		RuleSet current_rule = rules.at(0);
		for (auto& rule : current_rule)
		{
			current_rules.push_back(rule);
		}

		// Do this as long as we have something to replace
		bool change_occured{ true };
		while(change_occured)
		{
			change_occured = false;
			RuleSet next_rules;
			// Go over vector and find rule to replace
			//printPossibleRules2(current_rules);
			//getchar();
			for (auto& rule : current_rules)
			{
				if (rule.size() >= max_length)
				{
					possible_rules.insert(possible_rules.end(), rule.begin(), rule.end());
				}
				else
				{
					bool local_change{ false };
					auto generated_rules = generateRuleFromOldRule(rule, local_change);
					change_occured |= local_change;
					if (local_change)
					{
						//std::cout << "Append to next rules" << std::endl;
						//printPossibleRules(generated_rules);
						next_rules.insert(next_rules.end(), generated_rules.begin(), generated_rules.end());
					}
					else
					{
						//std::cout << "Append to possible rules" << std::endl;
						possible_rules.insert(possible_rules.end(), generated_rules.begin(), generated_rules.end());
					}
				}
			}

			std::swap(current_rules, next_rules);
			next_rules.resize(0);
		}

		// Generate valid strings
		std::cout << "Done generating rules" << std::endl;
		possible_strings.reserve(possible_rules.size());
		for (auto& possible_rule : possible_rules)
		{
			possible_strings.push_back(std::move(generateResultString(possible_rule)));
		}
		
		std::cout << "Done generating strings" << std::endl;
	}

	void printPossibleRules(const RuleSet& rules)
	{
		return;
		// Print current ruleset
		for (const auto& rule : rules)
		{
			for (const auto num : rule)
				std::cout << num << "-";
			std::cout << std::endl;
		}
	}

	void printPossibleRules2(const RuleSet& rules)
	{
		// Print current ruleset
		for (const auto& rule : rules)
		{
			for (const auto num : rule)
				std::cout << num << "-";
			std::cout << std::endl;
			std::cout << "------------------\n";
		}
	}

	bool verifyLine(const std::string& line)
	{
		for (auto& str : possible_strings)
		{
			if (str.compare(line) == 0)
				return true;
		}
		return false;
	}

	void printRules()
	{
		for (auto& [key, mapping] : rules)
		{
			std::cout << "Key: " << key << " : ";
			for (auto& mapping : mapping)
			{
				for (auto num : mapping)
					std::cout << num << "-";
				std::cout << " | ";
			}
			std::cout << std::endl;
		}
	}

	std::map<int, RuleSet> rules;
	RuleSet possible_rules;
	std::vector<std::string> possible_strings;
	int char_mapping{ std::numeric_limits<int>::max() };
	std::map <int, unsigned char> index_to_char_map;
};


void removeContent(std::vector<std::string>& content, std::vector<std::string>& rules)
{
	std::remove_if(content.begin(), content.end(), [&](const std::string& str)
		{
			bool found_match{ false };
			for (auto& rule : rules)
			{
				if (rule.compare(str) != std::string::npos)
				{
					found_match = true;
					break;
				}
			};
			return found_match;
		});
}

void generatePermutation(std::vector<std::string>& rules, std::vector<std::string>& new_rules)
{
	auto current_size = rules.size();
	for (auto i = 1; i < new_rules.size(); ++i)
	{
		for (auto j = 0; j < current_size; ++j)
		{
			rules.push_back(rules[j]);
		}
	}

	for (auto i = 0; i < new_rules.size(); ++i)
	{
		for (auto j = 0; j < current_size; ++j)
		{
			auto& curr_rul = rules[i * current_size + j];
			curr_rul.insert(curr_rul.end(), new_rules[i].begin(), new_rules[i].end());
		}
	}
}

bool testStringAgainstRules(const std::vector<std::string>& rules, const std::string& str)
{
	std::string tmp_str = str.substr(0, 8);
	for (auto& rule : rules)
	{
		if (rule.compare(tmp_str) == 0)
			return true;
	}
	return false;
}

void Task1()
{
	std::fstream input("../../../data/19/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	// Rules first
	Rules rules;
	std::string line;
	while (getline(input, line))
	{
		if (line.empty())
		{
			break;
		}
		rules.receiveInput(std::move(line));
	}

	// Content
	std::vector<std::string> content;
	while (getline(input, line))
	{
		if (line.empty())
		{
			break;
		}
		content.push_back(std::move(std::move(line)));
	}

	// --------------------------------
	// Rule 31
	rules.tryGenerateRuleSet(31);
	auto rules_31 = std::move(rules.possible_rules);
	std::vector<std::string> rules_31_str;
	rules_31_str.reserve(rules_31.size());
	for (auto& rule : rules_31)
	{
		rules_31_str.push_back(std::move(rules.generateResultString(rule)));
	}

	// --------------------------------
	// Rule 42
	rules.tryGenerateRuleSet(42);
	auto rules_42 = std::move(rules.possible_rules);
	std::vector<std::string> rules_42_str;
	rules_42_str.reserve(rules_42.size());
	for (auto& rule : rules_42)
	{
		rules_42_str.push_back(std::move(rules.generateResultString(rule)));
	}

	std::vector<std::vector<int>> rules_output;
	for (auto& str : content)
	{
		std::vector<int> output;
		while (true)
		{
			int val{ 0 };
			if (testStringAgainstRules(rules_31_str, str))
				val += 31;
			if (testStringAgainstRules(rules_42_str, str))
				val += 42;
			//std::cout << val << " - ";
			output.push_back(val);
			if (str.length() > 8)
			{
				str = str.substr(8);
			}
			else if (str.length() > 0 && str.length() != 8)
			{
				output.push_back(0);
				break;
			}
			else
				break;
		}
		rules_output.push_back(std::move(output));
	}

	int valid_rules{ 0 };
	for (auto& rule : rules_output)
	{
		bool success{ true };
		int pre_val{ 0 };
		if (rule.size() <= 3)
		{
			if ((rule[0] == 42 || rule[0] == 73) &&
				(rule[1] == 42 || rule[1] == 73) &&
				(rule[2] == 31 || rule[2] == 73))
				++valid_rules;
		}
	}

	std::cout << "Task 1 : Number of valid rules is: " << valid_rules << std::endl;
}



void Task2()
{
	std::fstream input("../../../data/19/input2.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	// Rules first
	Rules rules;
	std::string line;
	while (getline(input, line))
	{
		if (line.empty())
		{
			break;
		}
		rules.receiveInput(std::move(line));
	}

	// Content
	std::vector<std::string> content;
	int max_length{ 0 };
	while (getline(input, line))
	{
		if (line.empty())
		{
			break;
		}
		max_length = std::max(max_length, static_cast<int>(line.length()));
		content.push_back(std::move(std::move(line)));
	}

	// --------------------------------
	// Rule 31
	rules.tryGenerateRuleSet(31);
	auto rules_31 = std::move(rules.possible_rules);
	std::vector<std::string> rules_31_str;
	rules_31_str.reserve(rules_31.size());
	for (auto& rule : rules_31)
	{
		rules_31_str.push_back(std::move(rules.generateResultString(rule)));
	}

	// --------------------------------
	// Rule 42
	rules.tryGenerateRuleSet(42);
	auto rules_42 = std::move(rules.possible_rules);
	std::vector<std::string> rules_42_str;
	rules_42_str.reserve(rules_42.size());
	for (auto& rule : rules_42)
	{
		rules_42_str.push_back(std::move(rules.generateResultString(rule)));
	}

	std::vector<std::vector<int>> rules_output;
	for (auto& str : content)
	{
		std::vector<int> output;
		while (true)
		{
			int val{ 0 };
			if (testStringAgainstRules(rules_31_str, str))
				val += 31;
			if (testStringAgainstRules(rules_42_str, str))
				val += 42;
			//std::cout << val << " - ";
			output.push_back(val);
			if (str.length() > 8)
			{
				str = str.substr(8);
			}
			else if (str.length() > 0 && str.length() != 8)
			{
				//std::cout << "We got " << str << std::endl;
				output.push_back(0);
				break;
			}
			else
				break;
		}
		//std::cout << std::endl;
		rules_output.push_back(std::move(output));
	}

	int valid_rules{ 0 };
	int index = 0;
	for (auto& rule : rules_output)
	{
		bool success{ false };
		// Try to locate 42 42 31
		for (auto i = 0; i <= rule.size() - 3; ++i)
		{
			if ((rule[i] == 42) && (rule[i + 1] == 42) && (rule[i + 2] == 31))
			{
				// We found the sequence
				//std::cout << "We found the sequence" << std::endl;
				//getchar();

				// Count 42 before
				int num_42s{ 0 };
				for (auto j = i - 1; j >= 0; --j)
				{
					if (rule[j] == 42)
					{
						++num_42s;
					}
					else
					{
						num_42s = -1;
						break;
					}
				}

				// Count 31 after
				int num_31s{ 0 };
				for (auto j = i + 3; j < rule.size(); ++j)
				{
					if (rule[j] == 31)
					{
						++num_31s;
					}
					else
					{
						num_31s = -1;
						break;
					}
				}


				if ((num_31s <= num_42s) && (num_31s != -1) && (num_42s != -1))
					success = true;
				break;
			}
			if (success)
				break;
		}

		if (success)
		{
			++valid_rules;
			//std::cout << "OKAY: ";
			//for (auto num : rule)
			//	std::cout << num << " - ";
			//std::cout << std::endl;
		}
		//else
		//{
		//	std::cout << "NOT OKAY: ";
		//	for (auto num : rule)
		//		std::cout << num << " - ";
		//	std::cout << std::endl;
		//}
		//getchar();
		++index;
	}

	
	std::cout << "Task 2 : Number of valid rules is: " << valid_rules << std::endl;
}

int main()
{
	std::cout << "Nineteenth day of Advent code!" << std::endl;
	
	Task1();
	Task2();

	return 0;
}