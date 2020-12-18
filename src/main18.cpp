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

using DataType = unsigned long long;

std::function<DataType(DataType, DataType)> Addition = [&](DataType a, DataType b) -> DataType {return a + b; };
std::function<DataType(DataType, DataType)> Multiplication = [&](DataType a, DataType b) -> DataType {return a * b; };
std::function<DataType(DataType, DataType)> Noop = [&]([[maybe_unused]] DataType a, DataType b) -> DataType {return b; };

int findClosingBracket(const std::string& input)
{
	if (input[0] != '(')
	{
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

	return std::numeric_limits<int>::max();
}

struct Operand
{
	Operand(DataType input) : val{input}{}
	
	// Process this input
	void processInput(std::string&& input)
	{
		val = std::numeric_limits<DataType>::max();
		std::function<DataType(DataType, DataType)> op{ Noop };
		Operand operand{ std::numeric_limits<DataType>::max() };

		// First op is a bit different
		auto position{ 0 };
		if (input.at(0) == '(')
		{
			// Find closing bracket
			position = findClosingBracket(input);
			operand.val = std::numeric_limits<DataType>::max();
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

		// First instruction
		instructions.push_back(std::make_pair(operand, op));

		// Parse line (we always get operand first)
		while (true)
		{
			Operand operand2{ std::numeric_limits<DataType>::max() };
			position = 0;
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
				operand2.val = std::numeric_limits<DataType>::max();
				operand2.processInput(std::move(input.substr(1, position - 1)));
			}
			else
			{
				std::stringstream ss;
				ss << input.at(0);
				ss >> operand2.val;
			}

			instructions.push_back(std::make_pair(operand2, op));
			if (input.size() > position + 2)
				input = input.substr(position + 2);
			else
				break;
		}
	}

	DataType get()
	{
		if (val == std::numeric_limits<DataType>::max())
		{
			DataType result{ 0 };
			for (auto& [operand, operation] : instructions)
			{
				result = operation(result, operand.get());
			}
			return result;
		}
		else
		{
			return val;
		}
	}

	DataType get2(const std::string& prefix = "", bool printOutput=true)
	{
		if (val == std::numeric_limits<DataType>::max())
		{
			std::vector<std::pair<Operand, std::function<DataType(DataType, DataType)>>> tmp_inst;
			DataType result{ 0 };
			bool first_split{ true };
			for (auto& [operand, operation] : instructions)
			{
				if (operation(2, 3) == 6)
				{
					// Multiplication
					tmp_inst.push_back(std::make_pair(Operand{ result }, first_split ? Noop : Multiplication));
					result = operand.get2(prefix + std::string("-------- "), false);
					first_split = false;
				}
				else
				{
					// Addition or Noop
					result = operation(result, operand.get2(prefix + std::string("-------- ")));
				}
			}
			// Push last instruction
			if(result)
				tmp_inst.push_back(std::make_pair(Operand{ result }, (tmp_inst.size()) ? Multiplication : Noop));

			result = 0;
			for (auto& [operand, operation] : tmp_inst)
			{
				result = operation(result, operand.get2(prefix + std::string("-------- ")));
			}
			return result;
		}
		else
		{
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
			Operand op{std::numeric_limits<DataType>::max()};
			op.processInput(std::move(line));
			instructions.push_back(std::move(op));
		}
	}

	void calculateOutput()
	{
		results.resize(instructions.size());
		memset(results.data(), 0, sizeof(DataType) * results.size());
		for (int index = 0; auto & instruction : instructions)
		{
			results[index++] = instruction.get();
		}
	}

	void calculateOutput2()
	{
		results.resize(instructions.size());
		memset(results.data(), 0, sizeof(DataType) * results.size());
		for (int index = 0; auto& instruction : instructions)
		{
			results[index++] = instruction.get2();
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
	std::vector<Operand> instructions;
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

	// Task 2
	calc.calculateOutput2();
	std::cout << "Task2 Output is: " << calc.sumOutput() << std::endl;

	return 0;
}