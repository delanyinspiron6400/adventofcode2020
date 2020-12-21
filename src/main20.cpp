#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <bitset>
#include <map>

static constexpr int TileDim{ 10 };
static constexpr int ImageDim{ 3 };
static constexpr int numPermTile{ 16 };
using ImageRow = std::bitset<TileDim>;
using ImageState = std::bitset< ImageDim * ImageDim>;

enum class BorderLoc : int
{
	TOP = 0,
	RIGHT = 1,
	BOTTOM = 2,
	LEFT = 3
};

enum class RotationState : int
{
	None = 0,
	Deg90 = 1,
	Deg180 = 2,
	Deg270 = 3
};

enum class FlipState : int
{
	None = 0,
	Horizontal = 1,
	Vertical = 2,
	Both = 3
};

struct ImageTile
{
	std::vector<ImageRow> rotateBorder(const std::vector<ImageRow>& border)
	{
		std::vector<ImageRow> new_border;
		new_border.push_back(border[3]);
		new_border.push_back(border[0]);
		new_border.push_back(border[1]);
		new_border.push_back(border[2]);
		return new_border;
	}

	std::vector<ImageRow> flipBorderVertical(const std::vector<ImageRow>& border)
	{
		std::vector<ImageRow> new_border;
		
		// Bottom becomes top
		new_border.push_back(border[2]);

		ImageRow right_border;
		for (auto i = 0; i < TileDim; ++i)
		{
			if (border[1].test(TileDim - 1 - i))
				right_border.set(i);
			else
				right_border.reset(i);
		}
		new_border.push_back(std::move(right_border));

		// Top becomes bottom
		new_border.push_back(border[0]);

		ImageRow left_border;
		for (auto i = 0; i < TileDim; ++i)
		{
			if (border[3].test(TileDim - 1 - i))
				left_border.set(i);
			else
				left_border.reset(i);
		}
		new_border.push_back(std::move(left_border));

		return new_border;
	}

	std::vector<ImageRow> flipBorderHorizontal(const std::vector<ImageRow>& border)
	{
		std::vector<ImageRow> new_border;

		ImageRow top_border;
		for (auto i = 0; i < TileDim; ++i)
		{
			if (border[0].test(TileDim - 1 - i))
				top_border.set(i);
			else
				top_border.reset(i);
		}
		new_border.push_back(std::move(top_border));

		// Top becomes bottom
		new_border.push_back(border[3]);

		ImageRow bottom_border;
		for (auto i = 0; i < TileDim; ++i)
		{
			if (border[2].test(TileDim - 1 - i))
				bottom_border.set(i);
			else
				bottom_border.reset(i);
		}
		new_border.push_back(std::move(bottom_border));

		// Bottom becomes top
		new_border.push_back(border[1]);

		return new_border;
	}
	
	void generateMetaData()
	{
		std::vector<ImageRow> active_border;
		// Top Border
		active_border.push_back(data[0]);

		// Right Border
		ImageRow right_border;
		for (auto i = 0; i < TileDim; ++i)
			if (data[i].test(0))
				right_border.set(i);
		active_border.push_back(std::move(right_border));

		// Bottom Border
		active_border.push_back(data[TileDim - 1]);

		// Left Border
		ImageRow left_border;
		for (auto i = 0; i < TileDim; ++i)
			if (data[i].test(TileDim - 1))
				left_border.set(i);
		active_border.push_back(std::move(left_border));

		// No rotation
		borders.push_back(active_border);
		auto tmp_flip_vert = flipBorderVertical(active_border);
		borders.push_back(tmp_flip_vert);
		borders.push_back(std::move(flipBorderHorizontal(active_border)));
		borders.push_back(std::move(flipBorderHorizontal(tmp_flip_vert)));

		// 90° rotation
		auto border_90deg = rotateBorder(active_border);
		borders.push_back(border_90deg);
		tmp_flip_vert = flipBorderVertical(border_90deg);
		borders.push_back(tmp_flip_vert);
		borders.push_back(std::move(flipBorderHorizontal(border_90deg)));
		borders.push_back(std::move(flipBorderHorizontal(tmp_flip_vert)));

		// 180° rotation
		auto border_180deg = rotateBorder(border_90deg);
		borders.push_back(border_180deg);
		tmp_flip_vert = flipBorderVertical(border_180deg);
		borders.push_back(tmp_flip_vert);
		borders.push_back(std::move(flipBorderHorizontal(border_180deg)));
		borders.push_back(std::move(flipBorderHorizontal(tmp_flip_vert)));

		// 270° rotation
		auto border_270deg = rotateBorder(border_180deg);
		borders.push_back(border_270deg);
		tmp_flip_vert = flipBorderVertical(border_270deg);
		borders.push_back(tmp_flip_vert);
		borders.push_back(std::move(flipBorderHorizontal(border_270deg)));
		borders.push_back(std::move(flipBorderHorizontal(tmp_flip_vert)));
	}

	// Members
	int tile_ID{ 0 };
	int active_border_offset{ 0 };
	RotationState rotation_state;
	FlipState flip_state;
	std::vector<ImageRow> data;
	std::vector<std::vector<ImageRow>> borders; // Top -> Right -> Bottom -> Left per border
};

struct ImageGrid
{
	ImageGrid()
	{
		for (auto i = 0; i < ImageDim; ++i)
		{
			std::vector<ImageTile> tiles;
			tiles.resize(ImageDim);
			data.push_back(std::move(tiles));
		}
	}

	bool checkFit(const ImageTile& tile, int row, int col)
	{
		bool it_fits{ true };
		if (row > 0)
		{
			// Check also towards top
			auto& top_border_incoming = tile.borders[tile.active_border_offset][static_cast<int>(BorderLoc::TOP)];
			auto& bottom_border_present = data[row - 1][col].borders[data[row - 1][col].active_border_offset][static_cast<int>(BorderLoc::BOTTOM)];
			it_fits = (top_border_incoming == bottom_border_present);
		}
		if (it_fits && col > 0)
		{
			// Check to the left
			auto& left_border_incoming = tile.borders[tile.active_border_offset][static_cast<int>(BorderLoc::LEFT)];
			auto& right_border_present = data[row][col - 1].borders[data[row][col - 1].active_border_offset][static_cast<int>(BorderLoc::RIGHT)];
			it_fits = (left_border_incoming == right_border_present);
		}
		return it_fits;
	}

	void receiveInput(ImageTile&& tile)
	{
		int row = size / ImageDim;
		int col = size % ImageDim;
		data[row][col] = tile;
		++size;
	}

	bool tryReceiveInput(ImageTile&& tile)
	{
		int row = size / ImageDim;
		int col = size % ImageDim;
		if (checkFit(tile, row, col))
		{
			data[row][col] = tile;
			++size;
			return true;
		}
		return false;
	}

	void generateMetaData()
	{
		std::cout << "Generate Grid MetaData" << std::endl;
		for (auto& row : data)
			for (auto& tile : row)
				tile.generateMetaData();
	}

	void printGrid()
	{
		std::cout << "----------------------" << std::endl;
		for (auto& row : data)
		{
			for (auto& tile : row)
				std::cout << " | " << tile.tile_ID;
			std::cout << " |" << std::endl;
		}
		std::cout << "----------------------" << std::endl;
	}

	int size{ 0 };
	std::vector<std::vector<ImageTile>> data;
	ImageState state;
};

struct Image
{
	void generateMetaData()
	{
		grid.generateMetaData();
		grid.printGrid();
	}

	bool placeNextTile(ImageGrid layout)
	{
		std::cout << "Place Tile called with " << layout.size << " elements in grid" << std::endl;
		std::cout << layout.state << std::endl;
		layout.printGrid();
		getchar();
		if (layout.size == (ImageDim * ImageDim))
		{
			fitting_grid = layout;
			return true;
		}
		for (auto row = 0; row < ImageDim; ++row)
		{
			for (auto col = 0; col < ImageDim; ++col)
			{
				int pos = row * ImageDim + col;
				if (!layout.state.test(pos))
				{
					std::cout << layout.state << " : tested at position: " << pos << std::endl;
					// This tile has not been placed yet
					for (auto perm = 0; perm < numPermTile; ++perm)
					{
						auto tile = grid.data[row][col];
						tile.active_border_offset = perm;
						if (layout.tryReceiveInput(std::move(tile)))
						{
							std::cout << "Test permutation " << perm << " for " << row << " | " << col << std::endl;
							// Set state
							layout.state.set(pos);
							std::cout << "We set position: " << pos << std::endl;

							// Try place next tile
							if (placeNextTile(layout))
								return true;

							// Did not work, reset state again
							layout.state.reset(pos);
						}
					}
				}
			}
		}
		return false;
	}

	void assembleImage()
	{
		std::cout << "Start assembling the grid" << std::endl;
		// Place each Tile first in the top-left corner
		ImageGrid assembled_grid;
		placeNextTile(assembled_grid);
		std::cout << "Assemble grid done" << std::endl;
		std::cout << fitting_grid.data[0][0].tile_ID << " | " << fitting_grid.data[0][ImageDim - 1].tile_ID << " | ";
		std::cout << fitting_grid.data[ImageDim - 1][0].tile_ID << " | " << fitting_grid.data[ImageDim - 1][ImageDim - 1].tile_ID << std::endl;
	}

	unsigned long long getCornerProduct()
	{
		return static_cast<unsigned long long>(fitting_grid.data[0][0].tile_ID)
			* static_cast<unsigned long long>(fitting_grid.data[0][ImageDim - 1].tile_ID)
			* static_cast<unsigned long long>(fitting_grid.data[ImageDim - 1][0].tile_ID)
			* static_cast<unsigned long long>(fitting_grid.data[ImageDim - 1][ImageDim - 1].tile_ID);
	}

	// Members
	ImageGrid grid;
	ImageGrid fitting_grid;
};

int main()
{
	std::cout << "Twentieth day of Advent code!" << std::endl;
	std::fstream input("../../../data/20/input2.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	Image image;
	std::string line;
	while (getline(input, line))
	{
		ImageTile tile;
		tile.tile_ID = std::stoi(line.substr(5, line.find(":")));
		while (getline(input, line))
		{
			if (line.empty())
				break;
			tile.data.push_back(std::move(ImageRow(line, 0, line.size(), '.', '#')));
		}
		image.grid.receiveInput(std::move(tile));
	}
	
	image.generateMetaData();

	// Task 1
	image.assembleImage();
	std::cout << "Task 1: Result is: " << image.getCornerProduct() << std::endl;

	return 0;
}