#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include <unordered_map>

struct Rules
{
	Rules(std::string&& input) : class_name{ input.substr(0, input.find(':')) }
	{
		input = input.substr(input.find(':') + 2);
		while (true)
		{
			std::string range = input.substr(0, input.find(' '));
			ranges.push_back(std::make_pair(std::stoi(range.substr(0, range.find('-'))), std::stoi(range.substr(range.find('-') + 1))));

			if (input.find(' ') != std::string::npos)
				input = input.substr(input.find(' ') + 4);
			else
				break;
		}
		//std::cout << class_name << "|";
		//for (auto& range : ranges)
		//	std::cout << range.first << "-" << range.second << "|";
		//std::cout << std::endl;
	}

	bool checkValidEntry(int entry)
	{
		bool ret_val{ false };
		for (auto& range : ranges)
		{
			if ((entry >= range.first && entry <= range.second))
			{
				ret_val = true;
				break;
			}
		}
		return ret_val;
	}

	std::string class_name;
	std::vector<std::pair<int, int>> ranges;
};

struct Ticket
{
	void printTicket()
	{
		std::cout << "Ticket: " << std::endl;
		for (auto val : fields)
			std::cout << val << "|";
		std::cout << std::endl;
	}
	std::vector<int> fields;
};

int main()
{
	std::cout << "Sixteenth day of Advent code!" << std::endl;
	std::fstream input("../../../data/16/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	std::vector<Rules> rules;
	std::string line;
	while (getline(input, line))
	{
		if (line.empty())
		{
			break;
		}
		rules.push_back(std::move(Rules{ std::move(line) }));
	}

	// Skip 2 lines
	Ticket my_ticket;
	getline(input, line); getline(input, line);
	std::istringstream istream(line);
	std::string substr;
	while (getline(istream, substr, ','))
	{
		my_ticket.fields.push_back(std::stoi(substr));
	}

	// Skip 2 lines
	std::vector<Ticket> tickets;
	getline(input, line); getline(input, line);
	while (getline(input, line))
	{
		std::istringstream stream(line);
		Ticket ticket;
		while (getline(stream, substr, ','))
		{
			ticket.fields.push_back(std::stoi(substr));
		}
		tickets.push_back(std::move(ticket));
	}

	// Task 1
	int scanning_error{ 0 };
	std::vector<Ticket> valid_tickets;
	for(auto& ticket : tickets)
	{
		// Check if this ticket is valid
		bool valid_ticket{ true };
		for (auto val : ticket.fields)
		{
			bool fitsAnyRule{ false };
			for (auto& rule : rules)
			{
				fitsAnyRule = rule.checkValidEntry(val);
				if (fitsAnyRule)
					break;
			}
			if (!fitsAnyRule)
			{
				scanning_error += val;
				valid_ticket = false;
			}
		}
		if (valid_ticket)
			valid_tickets.push_back(ticket);
	}

	std::cout << "Scanning Error is: " << scanning_error << std::endl;

	// Task 2
	static constexpr int num_fields{ 20 };
	std::vector<std::bitset<num_fields>> locate_fields(num_fields);
	for (auto& bits : locate_fields)
		bits.set();

	for (auto& ticket : valid_tickets)
	{
		for (auto i = 0; i < num_fields; ++i)
		{
			std::bitset<num_fields> bits;
			for (auto j = 0; j < rules.size(); ++j)
			{
				if (rules[j].checkValidEntry(ticket.fields[i]))
					bits.set(j);
			}
			locate_fields[i] &= bits;
		}
	}

	std::unordered_map<int, std::string> field_mapping(num_fields);
	bool continue_work{ true };
	while (continue_work)
	{
		continue_work = false;
		std::bitset<num_fields> mask;
		for (auto i = 0; i < locate_fields.size(); ++i)
		{
			if (locate_fields[i].count() == 1)
			{
				continue_work = true;
				mask = locate_fields[i];
				mask.flip();
				for (auto j = 0; j < num_fields; ++j)
				{
					if (locate_fields[i].test(j))
					{
						//std::cout << "Rule " << i << ": is for class " << rules[j].class_name << std::endl;
						field_mapping.insert(std::make_pair(i, rules[j].class_name));
					}
				}
				break;
			}
		}

		// Remove this bit for all
		for (auto& bits : locate_fields)
			bits &= mask;
	}

	std::vector<int> fields_with_departure;
	for (auto& field : field_mapping)
	{
		if (field.second.find("departure") != std::string::npos)
			fields_with_departure.push_back(field.first);
	}

	unsigned long long task2_output{ 1ULL };
	for (auto field : fields_with_departure)
	{
		task2_output *= static_cast<unsigned long long>(my_ticket.fields[field]);
	}
		

	std::cout << "Task2 output is: " << task2_output << std::endl;

	return 0;
}
