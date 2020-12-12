#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include <numeric>
#include <bitset>

enum class Direction : unsigned char
{
	North = 'N',
	South = 'S',
	West = 'W',
	East = 'E',
	Left = 'L',
	Right = 'R',
	Forward = 'F'
};

struct Instruction
{
	Direction dir;
	int length;

	void printInstruction()
	{
		std::cout << "Heading " << static_cast<unsigned char>(dir) << " for " << length << " units\n";
	}
};

struct Position {
	int x{ 0 };
	int y{ 0 };

	void changePosition(const Instruction& inst)
	{
		switch (inst.dir)
		{
		case Direction::North:
			y += inst.length;
			break;
		case Direction::East:
			x += inst.length;
			break;
		case Direction::South:
			y -= inst.length;
			break;
		case Direction::West:
			x -= inst.length;
			break;
		}
	}

	void rotateClockwise()
	{
		auto tmp{ x };
		x = y;
		y = -tmp;
	}

	int manhattanDistance()
	{
		return std::abs(x) + std::abs(y);
	}
};

struct Ship
{
	Direction dir{ Direction::East };
	Position pos{ 0, 0 };
	Position way_point{ 10, 1 };

	void executeInstruction(Instruction inst)
	{
		//std::cout << "----------------------\n";
		//std::cout << "Current pos: (" << pos.x << ", " << pos.y << ") with heading: " << static_cast<unsigned char>(dir) << std::endl;
		//inst.printInstruction();
		switch (inst.dir)
		{
		case Direction::Left:
			changeDirection<false>(inst.length / 90);
			break;
		case Direction::Right:
			changeDirection<true>(inst.length / 90);
			break;
		case Direction::Forward:
			inst.dir = dir;
			[[fallthrough]];
		default:
			// Should get simple movement for all directions
			pos.changePosition(inst);
		}
		//std::cout << "Final pos: (" << pos.x << ", " << pos.y << ") with heading: " << static_cast<unsigned char>(dir) << std::endl;
		//getchar();
	}

	void executeInstruction2(Instruction inst)
	{
		//std::cout << "----------------------\n";
		//std::cout << "Current pos: (" << pos.x << ", " << pos.y << ") with heading: " << static_cast<unsigned char>(dir) << std::endl;
		//std::cout << "Current waypoint: (" << way_point.x << ", " << way_point.y << ")"<< std::endl;
		//inst.printInstruction();
		switch (inst.dir)
		{
		case Direction::Left:
			changeDirection2<false>(inst.length / 90);
			break;
		case Direction::Right:
			changeDirection2<true>(inst.length / 90);
			break;
		case Direction::Forward:
			pos.x += inst.length * way_point.x;
			pos.y += inst.length * way_point.y;
			break;
		default:
			// Should get simple movement for all directions
			way_point.changePosition(inst);
		}
		//std::cout << "Final pos: (" << pos.x << ", " << pos.y << ") with heading: " << static_cast<unsigned char>(dir) << std::endl;
		//std::cout << "Current waypoint: (" << way_point.x << ", " << way_point.y << ")" << std::endl;
		//getchar();
	}

	int dirToNum()
	{
		switch (dir)
		{
		case Direction::North:
			return 0;
		case Direction::East:
			return 1;
		case Direction::South:
			return 2;
		case Direction::West:
			return 3;
		default:
			return -1;
		}
	}

	void numToDir(int num)
	{
		switch (num)
		{
		case 0:
			dir = Direction::North;
			break;
		case 1:
			dir = Direction::East;
			break;
		case 2:
			dir = Direction::South;
			break;
		case 3:
			dir = Direction::West;
			break;
		}
	}

	template <bool Right>
	void changeDirection(int steps)
	{
		auto currentDir{ dirToNum() % 4};
		currentDir += (Right) ? steps : (4-steps);
		numToDir(currentDir % 4);
	}

	template <bool Right>
	void changeDirection2(int steps)
	{
		int rotation_steps = (Right) ? steps : (4 - steps);
		for (auto i = 0; i < rotation_steps; ++i)
		{
			way_point.rotateClockwise();
		}
	}

	int manhattanDistance()
	{
		return pos.manhattanDistance();
	}

	void clear()
	{
		dir = Direction::East;
		pos = std::move(Position{ 0,0 });
		way_point = std::move(Position{ 10, 1 });
	}
};

int main()
{
	std::cout << "Twelvth day of Advent code!" << std::endl;
	std::fstream input("../../../data/12/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	std::vector<Instruction> input_data;
	std::string line;
	while (getline(input, line))
	{
		input_data.push_back(std::move(Instruction{ static_cast<Direction>(line.at(0)), std::stoi(line.substr(1)) }));
	}

	Ship ship;
	for (auto& inst : input_data)
	{
		ship.executeInstruction(inst);
	}

	std::cout << "Manhattan distance traveled is: " << ship.manhattanDistance() << std::endl;

	ship.clear();
	for (auto& inst : input_data)
	{
		ship.executeInstruction2(inst);
	}

	std::cout << "Manhattan distance traveled is: " << ship.manhattanDistance() << std::endl;

	return 0;
}