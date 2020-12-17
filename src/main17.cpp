#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include <unordered_map>

struct Extent
{
	void checkOddSize()
	{
		if (x % 2 == 0)
			x += 1;
		if (y % 2 == 0)
			y += 1;
		if (z % 2 == 0)
			z += 1;
		if (w % 2 == 0)
			w += 1;
	}

	Extent& operator+=(int num) {
		this->x += num;
		this->y += num;
		this->z += num;
		this->w += num;
		return *this;
	}

	int x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 };
};

struct Grid3D
{
	using DataType = unsigned char;

	Grid3D(int x, int y, int z) : x_dim{ x }, y_dim{ y }, z_dim{ z }, 
		x_off{x/2}, y_off{ y / 2 }, z_off{ z / 2 }
	{
		grid.resize(z);
		for (auto& vec : grid)
		{
			vec.resize(y);
			for (auto& inner_vec : vec)
			{
				inner_vec.resize(x);
				memset(inner_vec.data(), 0U, sizeof(DataType) * x_dim);
			}
		}
	}

	void flushGrid()
	{
		for (auto& vec : grid)
		{
			for (auto& inner_vec : vec)
			{
				memset(inner_vec.data(), 0U, sizeof(DataType) * x_dim);
			}
		}
	}

	Grid3D(Extent extent) : Grid3D(extent.x, extent.y, extent.z) {}

	void printGrid()
	{
		std::cout << "----------------------\n";
		int z_layer = -z_off;
		for (const auto& z_vec : grid)
		{
			for (const auto& y_vec : z_vec)
			{
				for (auto x_vec : y_vec)
				{
					if(x_vec == 0)
						std::cout << ".";
					else
						std::cout << "#";
				}
				std::cout << std::endl;
			}
			std::cout << z_layer++ << " ----------------------\n";
		}
	}

	void checkNeighbourhoodPerCube(Grid3D& next_grid)
	{
		if (next_grid.x_dim != x_dim || next_grid.y_dim != y_dim || next_grid.z_dim != z_dim)
		{
			std::cout << "Watch out, might be a problem!" << std::endl;
			exit(-1);
		}
		for (auto z = -z_off; z <= z_off; ++z)
		{
			for (auto y = -y_off; y <= y_off; ++y)
			{
				for (auto x = -x_off; x <= x_off; ++x)
				{
					if (checkNeighbourhood(x, y, z))
						next_grid.set(x, y, z, 1);
				}
			}
		}
	}

	bool checkNeighbourhood(int x_pos, int y_pos, int z_pos)
	{
		auto num_neighbours{ 0 };
		for (auto z = z_pos - 1; z <= z_pos + 1; ++z)
		{
			for (auto y = y_pos - 1; y <= y_pos + 1; ++y)
			{
				for (auto x = x_pos - 1; x <= x_pos + 1; ++x)
				{
					if ((x == x_pos && y == y_pos && z == z_pos) || outOfBounds(x,y,z))
						continue;
					if (test(x, y, z))
						++num_neighbours;
				}
			}
		}

		if (test(x_pos, y_pos, z_pos))
		{
			// Cube is active
			if (num_neighbours == 2 || num_neighbours == 3)
				return true;
		}
		else
		{
			// Cube is inactive
			if (num_neighbours == 3)
				return true;
		}
		return false;
	}

	bool outOfBounds(int x, int y, int z)
	{
		if (x > x_off || x < -x_off || y > y_off || y < -y_off || z > z_off || z < -z_off)
			return true;
		return false;
	}

	DataType at(int x, int y, int z)
	{
		return grid[z_off + z][y_off + y][x_off + x];
	}

	bool test(int x, int y, int z)
	{
		return grid[z_off + z][y_off + y][x_off + x] != 0;
	}

	void set(int x, int y, int z, DataType val)
	{
		grid[z_off + z][y_off + y][x_off + x] = val;
	}

	void set(int x, int y, const std::vector<DataType>& other, int off)
	{
		memcpy(&grid[z_off - off][y_off + y][x_off + x], other.data(), sizeof(DataType) * other.size());
	}

	int countActiveCubes()
	{
		int ret_val{ 0U };
		for (const auto& z_vec : grid)
		{
			for (const auto& y_vec : z_vec)
			{
				for (auto x_vec : y_vec)
				{
					if (x_vec != 0)
						++ret_val;
				}
			}
		}
		return ret_val;
	}

	int x_dim{ 0 }, y_dim{ 0 }, z_dim{ 0 }; // Current Size
	int x_off{ 0 }, y_off{ 0 }, z_off{ 0 }; // Offset for origin
	std::vector<std::vector<std::vector<DataType>>> grid; // Grid
};

struct Grid4D
{
	using DataType = unsigned char;

	Grid4D(int x, int y, int z, int w) : x_dim{ x }, y_dim{ y }, z_dim{ z }, w_dim{ w },
		x_off{ x / 2 }, y_off{ y / 2 }, z_off{ z / 2 }, w_off{ w / 2 }
	{
		grid.resize(w);
		for (auto& outer_vec : grid)
		{
			outer_vec.resize(z);
			for (auto& vec : outer_vec)
			{
				vec.resize(y);
				for (auto& inner_vec : vec)
				{
					inner_vec.resize(x);
					memset(inner_vec.data(), 0U, sizeof(DataType) * x_dim);
				}
			}
		}
	}

	void flushGrid()
	{
		for (auto& outervec : grid)
		{
			for (auto& vec : outervec)
			{
				for (auto& inner_vec : vec)
				{
					memset(inner_vec.data(), 0U, sizeof(DataType) * x_dim);
				}
			}
		}
	}

	Grid4D(Extent extent) : Grid4D(extent.x, extent.y, extent.z, extent.w) {}

	void printGrid()
	{
		std::cout << "----------------------\n";
		int w_layer = -w_off;
		for (const auto& w_vec : grid)
		{
			for (const auto& z_vec : w_vec)
			{
				for (const auto& y_vec : z_vec)
				{
					for (auto x_vec : y_vec)
					{
						if (x_vec == 0)
							std::cout << ".";
						else
							std::cout << "#";
					}
					std::cout << std::endl;
				}
				std::cout << "Z ----------------------\n";
			}
			std::cout << w_layer++ << " ----------------------\n";
		}
	}

	void checkNeighbourhoodPerCube(Grid4D& next_grid)
	{
		if (next_grid.x_dim != x_dim || next_grid.y_dim != y_dim || next_grid.z_dim != z_dim || next_grid.w_dim != w_dim)
		{
			std::cout << "Watch out, might be a problem!" << std::endl;
			exit(-1);
		}
		for (auto w = -w_off; w <= w_off; ++w)
		{
			for (auto z = -z_off; z <= z_off; ++z)
			{
				for (auto y = -y_off; y <= y_off; ++y)
				{
					for (auto x = -x_off; x <= x_off; ++x)
					{
						if (checkNeighbourhood(x, y, z, w))
							next_grid.set(x, y, z, w, 1);
					}
				}
			}
		}
	}

	bool checkNeighbourhood(int x_pos, int y_pos, int z_pos, int w_pos)
	{
		auto num_neighbours{ 0 };
		for (auto w = w_pos - 1; w <= w_pos + 1; ++w)
		{
			for (auto z = z_pos - 1; z <= z_pos + 1; ++z)
			{
				for (auto y = y_pos - 1; y <= y_pos + 1; ++y)
				{
					for (auto x = x_pos - 1; x <= x_pos + 1; ++x)
					{
						if ((x == x_pos && y == y_pos && z == z_pos && w == w_pos) || outOfBounds(x, y, z, w))
							continue;
						if (test(x, y, z, w))
							++num_neighbours;
					}
				}
			}
		}
		

		if (test(x_pos, y_pos, z_pos, w_pos))
		{
			// Cube is active
			if (num_neighbours == 2 || num_neighbours == 3)
				return true;
		}
		else
		{
			// Cube is inactive
			if (num_neighbours == 3)
				return true;
		}
		return false;
	}

	bool outOfBounds(int x, int y, int z, int w)
	{
		if (x > x_off || x < -x_off || y > y_off || y < -y_off || z > z_off || z < -z_off || w > w_off || w < -w_off)
			return true;
		return false;
	}

	DataType at(int x, int y, int z, int w)
	{
		return grid[w_off + w][z_off + z][y_off + y][x_off + x];
	}

	bool test(int x, int y, int z, int w)
	{
		return grid[w_off + w][z_off + z][y_off + y][x_off + x] != 0;
	}

	void set(int x, int y, int z, int w, DataType val)
	{
		grid[w_off + w][z_off + z][y_off + y][x_off + x] = val;
	}

	int countActiveCubes()
	{
		int ret_val{ 0U };
		for (const auto& w_vec : grid)
		{
			for (const auto& z_vec : w_vec)
			{
				for (const auto& y_vec : z_vec)
				{
					for (auto x_vec : y_vec)
					{
						if (x_vec != 0)
							++ret_val;
					}
				}
			}
		}
		
		return ret_val;
	}

	int x_dim{ 0 }, y_dim{ 0 }, z_dim{ 0 }, w_dim{ 0 }; // Current Size
	int x_off{ 0 }, y_off{ 0 }, z_off{ 0 }, w_off{ 0 }; // Offset for origin
	std::vector<std::vector<std::vector<std::vector<DataType>>>> grid; // Grid
};

void swap(Grid3D& current, Grid3D& next)
{
	std::swap(current.grid, next.grid);
}

void swap(Grid4D& current, Grid4D& next)
{
	std::swap(current.grid, next.grid);
}

int main()
{
	std::cout << "Seventeenth day of Advent code!" << std::endl;
	std::fstream input("../../../data/17/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	// Read in initial layer
	std::vector<std::string> input_data;
	std::string line;
	while (getline(input, line))
	{
		input_data.push_back(std::move(line));
	}

	// Find current maximum extent
	Extent extent{ static_cast<int>(input_data[0].length()), static_cast<int>(input_data.size()) , 1 };
	extent.checkOddSize();
	Extent string_extent = extent;
	std::cout << "Current extent is: " << extent.x << " | " << extent.y << " | " << extent.z << std::endl;

	// Task 1 
	static constexpr int rounds{ 6 };
	for (auto i = 0; i <= rounds; ++i)
	{
		extent += 2;
	}
	std::cout << "Current extent is: " << extent.x << " | " << extent.y << " | " << extent.z << std::endl;

	Grid3D grid{ extent };
	Grid3D next_grid{ extent };

	// Fill in current state
	int y_start{ -(string_extent.y / 2) };
	for (auto& str : input_data)
	{
		int x_start{ -(string_extent.x / 2) };
		for (auto ch : str)
		{
			grid.set(x_start, y_start, 0, (ch == '#') ? 1 : 0);
			++x_start;
		}
		++y_start;
	}

	// Build stuff
	for (auto i = 0; i < rounds; ++i)
	{
		grid.checkNeighbourhoodPerCube(next_grid);
		swap(grid, next_grid);
		next_grid.flushGrid();
	}

	std::cout << "Number of active cubes is: " << grid.countActiveCubes() << std::endl;

	// Task 2
	Grid4D grid4D{ extent };
	Grid4D next_grid4D{ extent };
	int y_start4D{ -(string_extent.y / 2) };
	for (auto& str : input_data)
	{
		int x_start4D{ -(string_extent.x / 2) };
		for (auto ch : str)
		{
			grid4D.set(x_start4D, y_start4D, 0, 0,  (ch == '#') ? 1 : 0);
			++x_start4D;
		}
		++y_start4D;
	}

	//grid4D.printGrid();

	for (auto i = 0; i < rounds; ++i)
	{
		grid4D.checkNeighbourhoodPerCube(next_grid4D);
		swap(grid4D, next_grid4D);
		next_grid4D.flushGrid();
	}

	std::cout << "Number of active cubes is: " << grid4D.countActiveCubes() << std::endl;

	return 0;
}