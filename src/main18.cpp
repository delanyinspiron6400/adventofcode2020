#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include <functional>
#include <variant>
#include <limits>
#include <stack>
#include <numeric>

using DataType = unsigned int;

std::function<DataType(DataType, DataType)> Addition = [&](DataType a, DataType b) -> DataType {return a + b; };
std::function<DataType(DataType, DataType)> Multiplication = [&](DataType a, DataType b) -> DataType {return a * b; };
std::function<DataType(DataType, DataType)> Noop = [&]([[maybe_unused]] DataType a, DataType b) -> DataType {return b; };

int findClosingBracket(const std::string& input)
{
	if (input[0] != '(')
	{
		std::cout << "There is no opening bracket" << std::endl;
		return std::numeric_limits<int>::max();
	}

	std::stack <int> st;
	for (auto i = 0; i < input.length(); i++)
	{

		if (input[i] == '(')
		{
			st.push('(');
		}
		else if (input[i] == ')')
		{
			st.pop();
			if (st.empty())
			{
				return i;
			}
		}
	}

	std::cout << "Found no closing bracket" << std::endl;
	return std::numeric_limits<int>::max();
}

struct Operand
{
	Operand(DataType input) : val{input}{}
	
	// Process this input
	void processInput(std::string&& input)
	{
		val = std::numeric_limits<DataType>::max();
		//std::cout << "We got: " << input << std::endl;
		std::function<DataType(DataType, DataType)> op{ Noop };
		Operand operand{ std::numeric_limits<DataType>::max() };

		// First op is a bit different
		auto position{ 0 };
		if (input.at(0) == '(')
		{
			// Find closing bracket
			position = findClosingBracket(input);
			operand.processInput(std::move(input.substr(1, position - 1)));
		}
		else
		{
			// We get a number
			std::stringstream ss;
			ss << input.at(0);
			ss >> operand.val;
		}
		input = input.substr(position + 2);
		//std::cout << "Rest of line is: " << input << std::endl;

		// First instruction
		instructions.push_back(std::make_pair(operand, op));

		// Parse line (we always get operand first)
		while (true)
		{
			position = 0;
			if (input.at(0) == '(')
			{
				// Find closing bracket
				position = findClosingBracket(input);
				operand.processInput(std::move(input.substr(1, position - 1)));
			}
			else
			{
				// We get an operator first
				if (input.at(0) == '+')
				{
					op = Addition;
				}
				else if (input.at(0) == '*')
				{
					op = Multiplication;
				}
				input = input.substr(2);
				if (input.at(0) == '(')
				{
					// Find closing bracket
					position = findClosingBracket(input);
					operand.processInput(std::move(input.substr(1, position - 1)));
				}
				else
				{
					std::stringstream ss;
					ss << input.at(0);
					ss >> operand.val;
				}
			}

			instructions.push_back(std::make_pair(operand, op));
			//std::cout << "Remaining string: " << input;
			if (input.size() > position + 2)
			{
				input = input.substr(position + 2);
				//std::cout << " | Cut to: " << input << std::endl;
			}
			else
			{
				//std::cout << std::endl;
				break;
			}
				
		}
	}

	DataType get()
	{
		if (val == std::numeric_limits<DataType>::max())
		{
			DataType result{ 0 };
			std::cout << "Num instructions: " << instructions.size() << std::endl;
			for (auto& [operand, operation] : instructions)
			{
				result = operation(result, operand.get());
				std::cout << "Curr result: " << result << std::endl;
			}
			std::cout << "Sum Result is: " << result << std::endl;
			return result;
		}
		else
		{
			std::cout << "Val Result is: " << val << std::endl;
			return val;
		}
	}

	DataType val{ std::numeric_limits<DataType>::max() };
	std::vector<std::pair<Operand, std::function<DataType(DataType, DataType)>>> instructions;
};

struct Calculations
{
	Calculations(std::vector<std::string>&& input) : input_str{ std::move(input) } {}

	void processInput()
	{
		instructions.reserve(input_str.size());
		for (auto& line : input_str)
		{
			// Create a new instruction
			std::vector<std::pair<Operand, std::function<DataType(DataType, DataType)>>> inst;
			std::function<DataType(DataType, DataType)> op{ Noop };
			Operand operand{ std::numeric_limits<DataType>::max() };
			
			// First op is a bit different
			auto position{ 0 };
			if (line.at(0) == '(')
			{
				// Find closing bracket
				position = findClosingBracket(line);
				operand.processInput(std::move(line.substr(1, position - 1)));
			}
			else
			{
				// We get a number
				std::stringstream ss;
				ss << line.at(0);
				ss >> operand.val;
			}
			line = line.substr(position + 2);
			//std::cout << "*Rest of line is: " << line << std::endl;

			// First instruction
			inst.push_back(std::make_pair(operand, op));

			// Parse line (we always get operand first)
			while (true)
			{
				position = 0;
				if (line.at(0) == '(')
				{
					// Find closing bracket
					position = findClosingBracket(line);
					operand.processInput(std::move(line.substr(1, position - 1)));
				}
				else
				{
					// We get an operator first
					if (line.at(0) == '+')
					{
						op = Addition;
					}
					else if (line.at(0) == '*')
					{
						op = Multiplication;
					}
					line = line.substr(2);
					if (line.at(0) == '(')
					{
						// Find closing bracket
						position = findClosingBracket(line);
						operand.processInput(std::move(line.substr(1, position - 1)));
					}
					else
					{
						std::stringstream ss;
						ss << line.at(0);
						ss >> operand.val;
					}
				}
				
				inst.push_back(std::make_pair(operand, op));
				
				if (line.size() > position + 2)
					line = line.substr(position + 2);
				else
					break;
			}

			instructions.push_back(std::move(inst));
		}
	}

	void calculateOutput()
	{
		for (auto& instruction : instructions)
		{
			DataType result{ 0 };
			std::cout << "Num instructions: " << instruction.size() << std::endl;
			for (auto& [operand, operation] : instruction)
			{
				result = operation(result, operand.get());
				std::cout << "Current result: " << result << std::endl;
			}
			std::cout << "Final Result is: " << result << std::endl;
			std::cout << "-----------------------------" << std::endl;
			results.push_back(result);
		}
	}

	unsigned long long sumOutput()
	{
		unsigned long long result{ 0ULL };
		for (auto res : results)
			result += static_cast<unsigned long long>(res);
		return result;
	}

	std::vector<std::string> input_str;
	std::vector<std::vector<std::pair<Operand, std::function<DataType(DataType, DataType)>>>> instructions;
	std::vector<DataType> results;
};


int main()
{
	std::cout << "Eighteenth day of Advent code!" << std::endl;
	std::fstream input("../../../data/18/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	std::vector<std::string> input_data;
	std::string line;
	while (getline(input, line))
	{
		input_data.push_back(std::move(line));
	}

	// Receive input and process to executable format
	Calculations calc{ std::move(input_data) };
	calc.processInput();

	// Task 1
	calc.calculateOutput();
	std::cout << "Task1 Output is: " << calc.sumOutput() << std::endl;

	return 0;
}