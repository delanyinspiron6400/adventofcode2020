#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include <sstream>
#include <bitset>

struct Seat
{
	template<int N>
	Seat(const std::bitset<N> bits) :
		row{64U * bits[9] + 32U * bits[8] + 16U * bits[7] + 8U * bits[6] + 4U * bits[5] + 2U * bits[4] + 1U * bits[3] },
		col{ 4U * bits[2] + 2U * bits[1] + 1U * bits[0] }
	{}

	unsigned int row{ 0U };
	unsigned int col{ 0U };

	unsigned int computeLinearPlaceID()
	{
		return row * 8 + col;
	}
};

int main()
{
	std::cout << "Fifth day of Advent code!" << std::endl;
	std::fstream input("../../../data/05/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	std::vector<std::string> input_data;
	std::string val;
	while (input >> val)
	{
		input_data.push_back(std::move(val));
	}

	// Generate Bitsets from input data
	std::vector<std::bitset<10>> data;
	data.reserve(input_data.size());
	for (auto& item : input_data)
	{
		std::replace(item.begin(), item.end(), 'R', 'B');
		std::replace(item.begin(), item.end(), 'L', 'F');
		data.push_back(std::move(std::bitset<10>(item, 0, item.size(), 'F', 'B')));
	}

	// Sort data (higher value means further back
	std::sort(data.begin(), data.end(), [](const auto& lhs, const auto& rhs)
		{ return lhs.to_ulong() < rhs.to_ulong(); });

	std::vector<Seat> seats;
	seats.reserve(data.size());

	for (auto& item : data)
	{
		seats.push_back(std::move(Seat{ item }));
	}

	std::cout << "Highest seat ID is: " << seats[seats.size() - 1].computeLinearPlaceID() << std::endl;

	// Print all seats
	for (size_t i = 0U; i < seats.size() - 1; ++i)
	{
		if (seats[i].computeLinearPlaceID() + 1 != seats[i + 1ULL].computeLinearPlaceID())
		{
			std::cout << "Seat " << seats[i].computeLinearPlaceID() + 1 << " is empty!" << std::endl;
			break;
		}
	}

	return 0;
}