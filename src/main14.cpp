#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include <numeric>
#include <bitset>
#include <variant>
#include <map>

static constexpr int NUM_BITS_MASK{ 64 };
using BitMask = std::bitset<NUM_BITS_MASK>;

struct Mask
{
	Mask(std::string&& incoming)
	{
		std::string tmp = incoming;
		std::replace(tmp.begin(), tmp.end(), 'X', '1');
		and_mask = std::move(BitMask{tmp});

		tmp = incoming;
		std::replace(tmp.begin(), tmp.end(), 'X', '0');
		or_mask = std::move(BitMask{ tmp });

		tmp = incoming;
		std::replace(tmp.begin(), tmp.end(), '1', '0');
		std::replace(tmp.begin(), tmp.end(), 'X', '1');
		floating_mask = std::move(BitMask{ tmp });
	}
	BitMask and_mask;
	BitMask or_mask;
	BitMask floating_mask;
};

struct MemoryOperation
{
	int position{ 0 };
	unsigned long long value{ 0ULL };
};

using Instruction = std::variant<Mask, MemoryOperation>;

struct Emulator
{
	Emulator(int size) : memory(size + 1)
	{
		flushMemory();
	}

	void applyInstruction(const Instruction& inst)
	{
		if (std::holds_alternative<Mask>(inst))
		{
			// Lets apply the mask
			current_and_mask = std::get<Mask>(inst).and_mask;
			current_or_mask = std::get<Mask>(inst).or_mask;
		}
		else
		{
			// Lets write to memory
			auto& op = std::get<MemoryOperation>(inst);
			memory[op.position] = op.value | current_or_mask.to_ullong();
			memory[op.position] &= current_and_mask.to_ullong();
		}
	}

	void applyInstruction2(const Instruction& inst)
	{
		if (std::holds_alternative<Mask>(inst))
		{
			// Lets apply the mask
			current_or_mask = std::get<Mask>(inst).or_mask;
			current_floating_mask = std::get<Mask>(inst).floating_mask;
		}
		else
		{
			auto& op = std::get<MemoryOperation>(inst);
			BitMask address { static_cast<unsigned long long>(op.position) };

			// Modify address
			address |= current_or_mask.to_ullong(); // Now all 0 are unchanged and all 1 are set
			current_floating_mask.flip(); // Flip floating mask
			address &= current_floating_mask.to_ullong(); // Now all floating bits are 0
			current_floating_mask.flip(); // Flip it back
			writeAllPermutations<true>(memory, op.value, address, current_floating_mask);
		}
	}

	template <bool writeFirst>
	void writeAllPermutations(std::vector<unsigned long long>& memory, unsigned long long value, BitMask address, BitMask floating_mask)
	{
		for (auto i = 0; i < floating_mask.size(); ++i)
		{
			if (floating_mask.test(i))
			{
				if(writeFirst)
					memory_map[address.to_ullong()] = value;
				floating_mask.reset(i);
				if (floating_mask.count() > 0)
					writeAllPermutations<false>(memory, value, address, floating_mask);
				address.set(i);
				memory_map[address.to_ullong()] = value;
				if (floating_mask.count() > 0)
					writeAllPermutations<true>(memory, value, address, floating_mask);
				break;
			}
		}
	}

	unsigned long long sumOfMemory()
	{
		return std::accumulate(memory.begin(), memory.end(), 0ULL);
	}

	unsigned long long sumOfMap()
	{
		return std::accumulate(memory_map.begin(), memory_map.end(), 0ULL,
			[](unsigned long long value, const std::map<unsigned long long, unsigned long long>::value_type& mem) {
				return value + mem.second;
			});
	}

	void flushMemory()
	{
		std::memset(memory.data(), 0, sizeof(unsigned long long) * memory.size());
	}

	std::vector<unsigned long long> memory;
	std::map<unsigned long long, unsigned long long> memory_map;
	BitMask current_and_mask{};
	BitMask current_or_mask{};
	BitMask current_floating_mask{};
};



int main()
{
	std::cout << "Fourteenth day of Advent code!" << std::endl;
	std::fstream input("../../../data/14/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	std::vector<Instruction> input_data;
	std::string line;
	int size_of_memory{ 0 };
	while (getline(input, line))
	{
		if (line.substr(0, 4).compare("mask") == 0)
		{
			Instruction inst = std::move(Mask{std::move(line.substr(7))});
			input_data.push_back(std::move(inst));
		}
		else
		{
			auto position = line.substr(4, line.find(']') - 4);
			auto value = line.substr(line.find('=') + 2);
			Instruction inst = std::move(MemoryOperation{ std::stoi(position), std::stoull(value) });
			size_of_memory = std::max(size_of_memory, std::stoi(position));
			input_data.push_back(std::move(inst));
		}
	}

	// Task 1
	Emulator emulator{size_of_memory};

	// Execute instructions
	for (auto& instruction : input_data)
		emulator.applyInstruction(instruction);

	std::cout << "Task 1: Sum of Memory values is: " << emulator.sumOfMemory() << std::endl;

	// Task 2
	// Execute instructions
	for (auto& instruction : input_data)
		emulator.applyInstruction2(instruction);

	std::cout << "Task 2: Sum of Memory values is: " << emulator.sumOfMap() << std::endl;

	return 0;
}