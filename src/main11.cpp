#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include <numeric>
#include <bitset>

static constexpr int SEATS_PER_ROW{ 99 };
using Bitset = std::bitset<SEATS_PER_ROW>;

enum class SeatState : bool
{
	EMPTY = false,
	OCCUPIED = true
};

struct Grid
{
	void receiveInputLine(Bitset&& bits)
	{
		current_state.push_back(std::make_pair(std::move(bits), std::move(Bitset())));
		next_state.push_back(current_state[current_state.size() - 1]);
		num_seats += SEATS_PER_ROW;
	}

	int checkAdjacenctSeats(int row_index, int col_index)
	{
		int num_occupied_seats{ 0 };
		for (int row = row_index - 1; (row <= row_index + 1) && (row < static_cast<int>(current_state.size())); ++row)
		{
			for (int col = col_index - 1; (col <= col_index + 1) && (col < SEATS_PER_ROW); ++col)
			{
				if (row < 0 || col < 0 || (row == row_index && col == col_index))
				{
					continue;
				}
				if (current_state[row].second.test(col))
					++num_occupied_seats;
			}
		}
		return num_occupied_seats;
	}

	int checkAdjacenctSeats2(int row_index, int col_index)
	{
		// Check eight directions
		int num_occupied_seats{ 0 };
		for (int vertical_step = -1; vertical_step <= 1; ++vertical_step)
		{
			for (int horizontal_step = -1; horizontal_step <= 1; ++horizontal_step)
			{
				// Step over no motion
				if (vertical_step == 0 && horizontal_step == 0)
					continue;

				int curr_row{ row_index + vertical_step }, curr_col{ col_index + horizontal_step };
				while (true)
				{
					// Boundary check
					if (curr_row < 0 || curr_col < 0 || curr_row >= static_cast<int>(current_state.size()) || curr_col >= SEATS_PER_ROW)
						break;

					// Check if floor, go to next element
					if (static_cast<SeatState>(current_state[curr_row].first.test(curr_col)) == SeatState::EMPTY)
					{
						curr_row += vertical_step;
						curr_col += horizontal_step;
						continue;
					}

					if (static_cast<SeatState>(current_state[curr_row].second.test(curr_col)) == SeatState::EMPTY)
						break;
					else
					{
						++num_occupied_seats;
						break;
					}
				}
			}
		}
		return num_occupied_seats;
	}

	template<int CRITERIA>
	bool generateSeatPattern_impl()
	{
		int num_changes{ 0 };
		for (int row_index = 0; auto & [row, seats] : next_state)
		{
			for (auto col_index = 0; col_index < SEATS_PER_ROW; ++col_index)
			{
				// Check if there is even a seat there
				if (!row.test(col_index))
					continue;

				// Check current state
				SeatState state{ static_cast<SeatState>(current_state[row_index].second.test(col_index)) };
				auto num_occupied_seats{ (CRITERIA == 4) ? checkAdjacenctSeats(row_index, col_index) : checkAdjacenctSeats2(row_index, col_index) };
				if (state == SeatState::EMPTY && num_occupied_seats == 0)
				{
					seats.set(col_index);
					++num_changes;
				}
				else if (state == SeatState::OCCUPIED)
				{
					if (num_occupied_seats >= CRITERIA)
					{
						seats.reset(col_index);
						++num_changes;
					}
					else
					{
						seats.set(col_index);
					}
				}
			}
			++row_index;
		}

		return num_changes != 0;
	}

	bool generateSeatPattern()
	{
		return generateSeatPattern_impl<4>();
	}

	bool generateSeatPattern2()
	{
		return generateSeatPattern_impl<5>();
	}

	void prepareNextRound()
	{
		std::swap(current_state, next_state);
		for (auto& [row, seats] : next_state)
			seats.reset();
	}

	int numOccupiedSeats()
	{
		int num_occupied_seats{ 0 };
		for (auto& [row, seats] : next_state)
			num_occupied_seats += static_cast<int>(seats.count());
		return num_occupied_seats;
	}

	std::vector<std::pair<Bitset, Bitset>> current_state;
	std::vector<std::pair<Bitset, Bitset>> next_state;
	int num_seats{ 0 };
};

int main()
{
	std::cout << "Eleventh day of Advent code!" << std::endl;
	std::fstream input("../../../data/11/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	Grid grid;
	std::string line;
	while (getline(input, line))
	{
		grid.receiveInputLine(std::move(Bitset(line, 0, line.size(), '.', 'L')));
	}
	// Keep a copy for the second round
	Grid grid2(grid);

	// Task 1
	bool people_switch_seats{ true };
	while(people_switch_seats)
	{
		people_switch_seats = grid.generateSeatPattern();
		if(!people_switch_seats)
			break;
		grid.prepareNextRound();
	}
	std::cout << "Number of occupied seats is: " << grid.numOccupiedSeats() << std::endl;

	// Task 2
	people_switch_seats = true;
	while (people_switch_seats)
	{
		people_switch_seats = grid2.generateSeatPattern2();
		if (!people_switch_seats)
			break;
		grid2.prepareNextRound();
	}
	std::cout << "Number of occupied seats is: " << grid2.numOccupiedSeats() << std::endl;

	return 0;
}