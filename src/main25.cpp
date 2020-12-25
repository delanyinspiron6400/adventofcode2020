#include <iostream>
#include <string>

// Test input
//static constexpr unsigned long long card_PK{ 5764801 };
//static constexpr unsigned long long door_PK{ 17807724 };

static constexpr unsigned long long subject_number{ 7 };
static constexpr unsigned long long card_PK{ 8252394 };
static constexpr unsigned long long door_PK{ 6269621 };
static constexpr unsigned long long divisor{ 20201227 };

int computeSecretLoopSize(unsigned long long PK)
{
	int loop_size{ 0 };
	unsigned long long value{ 1 };
	while (true)
	{
		++loop_size;
		value *= subject_number;
		value = value % divisor;
		if (value == PK)
			break;
	}
	return loop_size;
}

unsigned long long produceOutput(unsigned long long sn, int loop_size)
{
	unsigned long long value{ 1 };
	for (auto i = 0; i < loop_size; ++i)
	{
		value *= sn;
		value = value % divisor;
	}
	return value;
}

int main()
{
	std::cout << "Day after Christmas - Advent of code!" << std::endl;

	int card_loop_size{ computeSecretLoopSize(card_PK) };
	std::cout << "Card secret loop size is: " << card_loop_size << std::endl;

	int door_loop_size{ computeSecretLoopSize(door_PK) };
	std::cout << "Door secret loop size is: " << door_loop_size << std::endl;

	auto encryption_key_card{ produceOutput(card_PK, door_loop_size) };
	auto encryption_key_door{ produceOutput(door_PK, card_loop_size) };
	std::cout << "Task1: Encryption key is: " << encryption_key_card << " | " << encryption_key_door << std::endl;
	
	return 0;
}