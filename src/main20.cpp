#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <bitset>
#include <map>

static constexpr int NumTiles{ 3 };

static constexpr int TileDim{ 10 };
static constexpr int TileDim_nb{ 8 };
static constexpr int ImageSize{ TileDim_nb * NumTiles };
static constexpr int numPermTile{ 8 };
using TileRow = std::bitset<TileDim>;
using ImageRow = std::bitset<ImageSize>;
using ImageState = std::bitset< NumTiles * NumTiles>;


enum class BorderLoc : int
{
	TOP = 0,
	RIGHT = 1,
	BOTTOM = 2,
	LEFT = 3
};

struct ImageTile
{
	std::vector<TileRow> rotateImage(const std::vector<TileRow>& image)
	{
		std::vector<TileRow> new_image;
		new_image.resize(image.size());
		for (auto i = 0; i < TileDim; ++i)
		{
			for (auto j = 0; j < TileDim; ++j)
			{
				new_image[i][j] = image[j][TileDim - 1 - i];
			}
		}
		return new_image;
	}

	std::vector<TileRow> rotateImageLeft(const std::vector<TileRow>& image)
	{
		std::vector<TileRow> new_image;
		new_image.resize(image.size());
		for (auto i = 0; i < TileDim; ++i)
		{
			for (auto j = 0; j < TileDim; ++j)
			{
				new_image[i][j] = image[TileDim - 1 - j][i];
			}
		}
		return new_image;
	}

	std::vector<TileRow> flipImageHorizontal(const std::vector<TileRow>& image)
	{
		std::vector<TileRow> new_image;
		new_image.resize(image.size());
		for (auto i = 0; i < TileDim; ++i)
		{
			new_image[i] = image[TileDim - 1 - i];
		}
		return new_image;
	}

	std::vector<TileRow> flipImageVertical(const std::vector<TileRow>& image)
	{
		std::vector<TileRow> new_image;
		new_image.resize(image.size());
		for (auto i = 0; i < TileDim; ++i)
		{
			for (auto j = 0; j < TileDim; ++j)
			{
				new_image[i][j] = image[i][TileDim - 1 - j];
			}
		}
		return new_image;
	}

	std::vector<TileRow> getBorder(const std::vector<TileRow>& image)
	{
		std::vector<TileRow> active_border;
		// Top Border
		active_border.push_back(image[0]);

		// Right Border
		TileRow right_border;
		for (auto i = 0; i < TileDim; ++i)
			if (image[i].test(0))
				right_border.set(i);
		active_border.push_back(std::move(right_border));

		// Bottom Border
		active_border.push_back(image[TileDim - 1]);

		// Left Border
		TileRow left_border;
		for (auto i = 0; i < TileDim; ++i)
			if (image[i].test(TileDim - 1))
				left_border.set(i);
		active_border.push_back(std::move(left_border));
		return active_border;
	}

	void generateMetaData()
	{
		// Normal orientation
		images.push_back(data);
		borders.push_back(std::move(getBorder(data)));

		// Rotate 90Deg
		auto image_90 = rotateImage(data);
		images.push_back(image_90);
		borders.push_back(std::move(getBorder(image_90)));

		// Rotate 180Deg
		auto image_180 = rotateImage(image_90);
		images.push_back(image_180);
		borders.push_back(std::move(getBorder(image_180)));

		// Rotate 270Deg
		auto image_270 = rotateImage(image_180);
		images.push_back(image_270);
		borders.push_back(std::move(getBorder(image_270)));

		// Flip horizontal
		auto flip_hor = flipImageHorizontal(data);
		images.push_back(flip_hor);
		borders.push_back(std::move(getBorder(flip_hor)));

		// Flip vertical
		auto flip_vert = flipImageVertical(data);
		images.push_back(flip_vert);
		borders.push_back(std::move(getBorder(flip_vert)));

		// Flip +45°
		auto flip_plus_45 = rotateImageLeft(flip_hor);
		images.push_back(flip_plus_45);
		borders.push_back(std::move(getBorder(flip_plus_45)));

		// Flip -45°
		auto flip_minus_45 = rotateImage(flip_hor);
		images.push_back(flip_minus_45);
		borders.push_back(std::move(getBorder(flip_minus_45)));
	}

	void printTile(const std::vector<TileRow>& tile)
	{
		for (auto& row : tile)
		{
			std::cout << row << std::endl;
		}
	}

	// Members
	int tile_ID{ 0 };
	int active_offset{ 0 };
	std::vector<TileRow> data;
	std::vector<std::vector<TileRow>> borders; // Top -> Right -> Bottom -> Left per border
	std::vector<std::vector<TileRow>> images;
};

struct ImageGrid
{
	ImageGrid()
	{
		for (auto i = 0; i < NumTiles; ++i)
		{
			std::vector<ImageTile> tiles;
			tiles.resize(NumTiles);
			data.push_back(std::move(tiles));
		}
	}

	bool checkFit(const ImageTile& tile, int row, int col)
	{
		bool it_fits{ true };
		if (row > 0)
		{
			// Check also towards top
			auto& top_border_incoming = tile.borders[tile.active_offset][static_cast<int>(BorderLoc::TOP)];
			auto& bottom_border_present = data[row - 1][col].borders[data[row - 1][col].active_offset][static_cast<int>(BorderLoc::BOTTOM)];
			it_fits = (top_border_incoming == bottom_border_present);
		}
		if (it_fits && col > 0)
		{
			// Check to the left
			auto& left_border_incoming = tile.borders[tile.active_offset][static_cast<int>(BorderLoc::LEFT)];
			auto& right_border_present = data[row][col - 1].borders[data[row][col - 1].active_offset][static_cast<int>(BorderLoc::RIGHT)];
			it_fits = (left_border_incoming == right_border_present);
		}
		return it_fits;
	}

	void receiveInput(ImageTile&& tile)
	{
		int row = size / NumTiles;
		int col = size % NumTiles;
		data[row][col] = tile;
		++size;
	}

	bool tryReceiveInput(ImageTile&& tile)
	{
		int row = size / NumTiles;
		int col = size % NumTiles;
		if (checkFit(tile, row, col))
		{
			data[row][col] = tile;
			++size;
			return true;
		}
		return false;
	}

	bool resetLastAction()
	{
		--size;
		int row = size / NumTiles;
		int col = size % NumTiles;
		data[row][col] = ImageTile();
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

	void printGridGraphically()
	{
		for (auto row = 0; row < NumTiles; ++row)
		{
			for (auto i = 0; i < TileDim; ++i)
			{
				for (auto col = 0; col < NumTiles; ++col)
				{
					auto num = row * NumTiles + col;
					if (num < size)
					{
						std::cout << data[row][col].images[data[row][col].active_offset][i] << " ";
					}
				}
				if(size)
					std::cout << std::endl;
			}
			if (size)
				std::cout << "------------------------------------------\n";
		}
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

	bool placeNextTile(ImageGrid layout, int nesting_depth=0)
	{
		if (layout.size == (NumTiles * NumTiles))
		{
			fitting_grid = layout;
			return true;
		}
		for (auto row = 0; row < NumTiles; ++row)
		{
			for (auto col = 0; col < NumTiles; ++col)
			{
				int pos = row * NumTiles + col;
				if (!layout.state.test(pos))
				{
					// This tile has not been placed yet
					for (auto perm = 0; perm < numPermTile; ++perm)
					{
						auto tile = grid.data[row][col];
						tile.active_offset = perm;
						if (layout.tryReceiveInput(std::move(tile)))
						{
							// Set state
							layout.state.set(pos);

							// Try place next tile
							if (placeNextTile(layout, nesting_depth + 1))
								return true;

							// Did not work, reset state again
							layout.state.reset(pos);
							layout.resetLastAction();
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
		fitting_grid.printGridGraphically();
		std::cout << fitting_grid.data[0][0].tile_ID << " | " << fitting_grid.data[0][NumTiles - 1].tile_ID << " | ";
		std::cout << fitting_grid.data[NumTiles - 1][0].tile_ID << " | " << fitting_grid.data[NumTiles - 1][NumTiles - 1].tile_ID << std::endl;
	}

	unsigned long long getCornerProduct()
	{
		return static_cast<unsigned long long>(fitting_grid.data[0][0].tile_ID)
			* static_cast<unsigned long long>(fitting_grid.data[0][NumTiles - 1].tile_ID)
			* static_cast<unsigned long long>(fitting_grid.data[NumTiles - 1][0].tile_ID)
			* static_cast<unsigned long long>(fitting_grid.data[NumTiles - 1][NumTiles - 1].tile_ID);
	}

	// Members
	ImageGrid grid;
	ImageGrid fitting_grid;
};

struct FittedImage
{
	FittedImage()
	{
		for (auto i = 0; i < ImageSize; ++i)
		{
			image.push_back(std::move(ImageRow{}));
		}
	}

	void constructDenseImage(const ImageGrid& grid)
	{
		for (auto row = 0; row < NumTiles; ++row)
		{
			for (auto col = 0; col < NumTiles; ++col)
			{
				auto& currentTile = grid.data[row][NumTiles - 1 - col].images[grid.data[row][NumTiles - 1 - col].active_offset];

				for (auto i = 0; i < TileDim_nb; ++i)
				{
					for (auto j = 0; j < TileDim_nb; ++j)
					{
						image[row * TileDim_nb + i][col * TileDim_nb + j] = currentTile[1 + i][1 + j];
					}
				}
			}
		}
		final_image = image;
	}

	void locateScaryMonster()
	{
		int num_monsters{ 0 };


		std::cout << "Found " << num_monsters << " scary monsters!" << std::endl;
	}

	void printImage()
	{
		std::cout << "----------------------------\n";
		for (auto& row : image)
			std::cout << row << std::endl;
		std::cout << "----------------------------\n";
	}

	std::vector<ImageRow> image;
	std::vector<ImageRow> final_image;
	static constexpr int monster_length{ 20 };
	ImageRow monster_l1{ "00000000000000000010" };
	ImageRow monster_l2{ "10000110000110000111" };
	ImageRow monster_l3{ "10010010010010010000" };
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
			tile.data.push_back(std::move(TileRow(line, 0, line.size(), '.', '#')));
		}
		image.grid.receiveInput(std::move(tile));
	}
	
	image.generateMetaData();

	// Task 1
	image.assembleImage();
	std::cout << "Task 1: Result is: " << image.getCornerProduct() << std::endl;

	// Task 2
	FittedImage fitted_image;
	fitted_image.constructDenseImage(image.fitting_grid);
	fitted_image.printImage();
	fitted_image.locateScaryMonster();

	return 0;
}