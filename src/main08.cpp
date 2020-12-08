#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <utility>

enum class Inst : int
{
	acc = 0,
	jmp = 1,
	nop = 2
};

struct Instruction
{
	Inst instruction{ Inst::nop };
	int value{ 0 };

	void negate()
	{
		if (instruction == Inst::jmp)
			instruction = Inst::nop;
		else if (instruction == Inst::nop)
			instruction = Inst::jmp;
	}
};

void flushVecBool(std::vector<bool>& vec)
{
	for (auto i = 0; i < vec.size(); ++i)
		vec[i] = false;
}

bool executeCode(std::vector<Instruction>& input, std::vector<bool>& instruction_cache, int& accumulator)
{
	Instruction instruction;
	int instruction_pointer{ 0 };
	accumulator = 0;
	bool executed_last_instruction{ false };
	while (true)
	{
		if (instruction_cache[instruction_pointer])
			break;
		instruction_cache[instruction_pointer] = true;
		if (instruction_pointer == input.size() - 1)
			executed_last_instruction = true;

		// Load current instruction
		instruction = input[instruction_pointer];

		// Execute instruction
		switch (instruction.instruction)
		{
		case Inst::jmp:
			instruction_pointer += instruction.value;
			break;
		case Inst::acc:
			accumulator += instruction.value;
			[[fallthrough]];
		default:
			++instruction_pointer;
			break;
		}

		if (executed_last_instruction)
			return true;
	}
	return false;
}

int main()
{
	std::cout << "Eigth day of Advent code!" << std::endl;
	std::fstream input("../../../data/08/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	std::vector<Instruction> input_data;
	std::string inst;
	int value;
	while (input >> inst, input >> value)
	{
		if (inst.compare("acc") == 0)
			input_data.push_back({Inst::acc, value});
		else if (inst.compare("jmp") == 0)
			input_data.push_back({ Inst::jmp, value });
		else if (inst.compare("nop") == 0)
			input_data.push_back({ Inst::nop, value });
		else
			std::cout << "Unrecogniced instruction" << std::endl;
	}

	std::vector<bool> instruction_cache(input_data.size());
	int instruction_pointer{ 0 };

	// First task
	int accumulator{ 0 };
	executeCode(input_data, instruction_cache, accumulator);

	std::cout << "Accumulator state was: " << accumulator << std::endl;

	// Second task
	flushVecBool(instruction_cache);
	if (!executeCode(input_data, instruction_cache, accumulator))
	{
		for (auto change_loc = 0; change_loc < input_data.size(); ++change_loc)
		{
			
			Instruction current_inst = input_data[change_loc];
			if (current_inst.instruction == Inst::acc)
			{
				continue;
			}
			else
			{
				input_data[change_loc].negate();
				// Acc is okay
				flushVecBool(instruction_cache);
				if (executeCode(input_data, instruction_cache, accumulator))
				{
					break;
				}
				input_data[change_loc].negate();
			}
		}
	}

	std::cout << "Accumulator state was: " << accumulator << std::endl;

	return 0;
}