#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <array>

static constexpr int num_tiles{50000};
static constexpr int num_rounds{ 100 };

enum class Direction : int
{
	EAST = 0,
	SOUTH_EAST = 1,
	SOUTH_WEST = 2,
	WEST = 3,
	NORTH_WEST = 4,
	NORTH_EAST = 5,
};

enum class Colour : int
{
	WHITE = 0,
	BLACK = 1
};

using Directions = std::vector<Direction>;

struct Position
{
	int x{ 0 };
	int y{ 0 };

	bool operator ==(const Position& rhs) const
	{
		return (x == rhs.x) && (y == rhs.y);
	}
};

Position generatePositionFromDirections(const Directions& dirs)
{
	Position pos{0,0};

	for (auto dir : dirs)
	{
		switch (dir)
		{
		case Direction::EAST:
			pos.x += 2;
			break;
		case Direction::SOUTH_EAST:
			pos.x += 1;
			pos.y -= 1;
			break;
		case Direction::SOUTH_WEST:
			pos.x -= 1;
			pos.y -= 1;
			break;
		case Direction::WEST:
			pos.x -= 2;
			break;
		case Direction::NORTH_WEST:
			pos.x -= 1;
			pos.y += 1;
			break;
		case Direction::NORTH_EAST:
			pos.x += 1;
			pos.y += 1;
			break;
		default:
			std::cout << "Should not happen!" << std::endl;
			break;
		}
	}

	return pos;
}

struct Tile
{
	Tile() : pos{ Position{0,0} }, colour{ Colour::WHITE }{}

	void flipColour()
	{
		colour = (colour == Colour::WHITE) ? Colour::BLACK : Colour::WHITE;
	}

	std::vector<Position> generateNeighbours()
	{
		std::vector<Position> positions;
		positions.push_back(std::move(Position{pos.x + 1, pos.y + 1}));
		positions.push_back(std::move(Position{ pos.x + 2, pos.y }));
		positions.push_back(std::move(Position{ pos.x + 1, pos.y - 1 }));
		positions.push_back(std::move(Position{ pos.x - 1, pos.y - 1 }));
		positions.push_back(std::move(Position{ pos.x - 2, pos.y}));
		positions.push_back(std::move(Position{ pos.x - 1, pos.y + 1 }));
		return positions;
	}

	Tile(Position pos, Colour col = Colour::BLACK) : Tile() { this->pos = pos; this->colour = col; }
	Position pos;
	Colour colour{ Colour::WHITE };
};

struct Tiles
{
	void receiveTile(const Position& pos)
	{
		bool alreadyIn{ false };
		for (auto& tile : tile_mapping)
		{
			if (tile.pos == pos)
			{
				/*std::cout << "Tile at pos <" << tile.pos.x << ", " << tile.pos.y;
				std::cout << "> already here with colour: " << ((tile.colour == Colour::WHITE) ? "White" : "Black") << std::endl;*/
				tile.flipColour();
				alreadyIn = true;
				break;
			}
		}
		if (!alreadyIn)
		{
			//std::cout << "Add Tile at pos <" << pos.x << ", " << pos.y << ">" << std::endl;
			tile_mapping.push_back(std::move(Tile{ pos }));
		}
	}

	int countBlackTiles()
	{
		int num_black_tiles{ 0 };
		for (auto& tile : tile_mapping)
		{
			if (tile.colour == Colour::BLACK)
				++num_black_tiles;
		}
		return num_black_tiles;
	}

	void generatePerimeter()
	{
		auto current_size = tile_mapping.size();
		for (auto i = 0; i < current_size; ++i)
		{
			auto& current_tile = tile_mapping[i];
			if (current_tile.colour == Colour::WHITE)
				continue;
			auto neighbours = current_tile.generateNeighbours();
			for (auto& neighbour : neighbours)
			{
				bool alreadyIn{ false };
				for (auto& tile : tile_mapping)
				{
					if (tile.pos == neighbour)
					{
						alreadyIn = true;
						break;
					}
				}
				if (!alreadyIn)
				{
					tile_mapping.push_back(std::move(Tile{ neighbour, Colour::WHITE }));
				}
			}
		}
	}

	void flipTiles(Tiles& next_tiles)
	{
		auto& next_mapping = next_tiles.tile_mapping;
		for (auto i = 0; i < tile_mapping.size(); ++i)
		{
			auto neighbours = tile_mapping[i].generateNeighbours();
			int black_neighbours{ 0 };
			for (auto& neighbour : neighbours)
			{
				for (auto& tile : tile_mapping)
				{
					if (neighbour == tile.pos)
					{
						if (tile.colour == Colour::BLACK)
							++black_neighbours;
						break;
					}
				}
			}
			if (tile_mapping[i].colour == Colour::BLACK)
			{
				if (black_neighbours == 0 || black_neighbours > 2)
					next_mapping[i].flipColour();
			}
			else
			{
				if(black_neighbours == 2)
					next_mapping[i].flipColour();
			}
		}
	}

	std::vector<Tile> tile_mapping;
};

int main()
{
	std::cout << "Christmas - Advent of code!" << std::endl;
	std::fstream input("../../../data/24/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	std::vector<Directions> tile_directions;
	std::string line;
	while (getline(input, line))
	{
		Directions dirs;
		for (auto i = 0; i < line.length(); ++i)
		{
			if (line.at(i) == 'e')
			{
				dirs.push_back(Direction::EAST);
			}
			else if (line.at(i) == 'w')
			{
				dirs.push_back(Direction::WEST);
			}
			else if (line.at(i) == 's')
			{
				++i;
				if (line.at(i) == 'e')
				{
					dirs.push_back(Direction::SOUTH_EAST);
				}
				else
				{
					dirs.push_back(Direction::SOUTH_WEST);
				}
			}
			else if (line.at(i) == 'n')
			{
				++i;
				if (line.at(i) == 'e')
				{
					dirs.push_back(Direction::NORTH_EAST);
				}
				else
				{
					dirs.push_back(Direction::NORTH_WEST);
				}
			}
		}
		tile_directions.push_back(std::move(dirs));
	}

	// Task 1
	Tiles tiles;
	tiles.tile_mapping.push_back(std::move(Tile{ {0,0}, Colour::WHITE }));
	for (auto& dirs : tile_directions)
	{
		auto position = generatePositionFromDirections(dirs);
		tiles.receiveTile(position);
	}
	std::cout << "Task 1: Num Black tiles is " << tiles.countBlackTiles() << std::endl;

	// Task 2
	Tiles next_tiles;
	for (auto i = 0; i < num_rounds; ++i)
	{
		tiles.generatePerimeter();
		next_tiles = tiles;
		tiles.flipTiles(next_tiles);
		std::swap(tiles, next_tiles);
		tiles.countBlackTiles();
		std::cout << i << ": Task 2: Num Black tiles is " << tiles.countBlackTiles() << std::endl;
	}

	return 0;
}