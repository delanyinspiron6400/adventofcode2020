#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <bitset>
#include <map>

int main()
{
	std::cout << "Twentieth day of Advent code!" << std::endl;
	std::fstream input("../../../data/20/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	return 0;
}