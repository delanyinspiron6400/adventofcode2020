#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <bitset>
#include <map>
#include <sstream>
#include <algorithm>

struct Food
{
	std::vector<std::string> allergens;
	std::vector<std::string> ingredients;
};

struct Menu
{
	void generateMappings()
	{
		for (auto i = 0; i < food_items.size(); ++i)
		{
			auto& food_item = food_items[i];
			// Ingredients
			for (auto& ingredient : food_item.ingredients)
			{
				if (ingredient_mapping.contains(ingredient))
				{
					ingredient_mapping[ingredient].push_back(i);
				}
				else
				{
					ingredient_mapping[ingredient] = std::move(std::vector<int>{ i });
				}
			}

			// Allergens
			for (auto& allergen : food_item.allergens)
			{
				if (allergen_mapping.contains(allergen))
				{
					allergen_mapping[allergen].push_back(i);
				}
				else
				{
					allergen_mapping[allergen] = std::move(std::vector<int>{ i });
				}
			}
		}
	}

	std::string findIngredientAllergenMapping()
	{
		for (auto& [allergen, al_mapping] : allergen_mapping)
		{
			for (auto& [ingredient, in_mapping] : ingredient_mapping)
			{
				bool detectedMapping{ true };
				for (auto val : al_mapping)
				{
					if (std::find(in_mapping.begin(), in_mapping.end(), val) == in_mapping.end())
					{
						detectedMapping = false;
						break;
					}
				}
				if (detectedMapping)
				{
					//std::cout << "Allergen " << allergen << " is contained in " << ingredient << std::endl;
					if (ingredient_allergen_mapping.contains(allergen))
					{
						ingredient_allergen_mapping[allergen].push_back(ingredient);
					}
					else
					{
						ingredient_allergen_mapping[allergen] = std::move(std::vector<std::string>{ ingredient });
					}
				}
			}
			//std::cout << "------------------------------\n" << std::endl;
		}
		std::vector<std::vector<std::string>> mapping_helper;
		for (auto& [allergen, ingredients] : ingredient_allergen_mapping)
		{
			mapping_helper.push_back(ingredients);
		}

		// Now generate final mapping
		for (auto runs = 0; runs < mapping_helper.size(); ++runs)
		{
			for (auto i = 0; i < mapping_helper.size(); ++i)
			{
				if (mapping_helper[i].size() == 1 && !final_ingredient_allergen_mapping.contains(i))
				{
					final_ingredient_allergen_mapping[i] = mapping_helper[i][0];
					// We found a single element, delete this from other indices
					for (auto j = 0; j < mapping_helper.size(); ++j)
					{
						if (j == i)
							continue;
						auto it = std::find(mapping_helper[j].begin(), mapping_helper[j].end(), mapping_helper[i][0]);
						if (it != mapping_helper[j].end())
						{
							mapping_helper[j].erase(it);
						}
					}
				}
			}
		}
		
		
		// Generate output string
		std::string output_str;
		int index{ 0 };
		for (auto& ing : mapping_helper)
		{
			output_str += ing[0];
			if (++index != mapping_helper.size())
			{
				output_str += std::string(",");
			}
		}
		return output_str;
	}

	int countIngredientsWithoutAllergen()
	{
		int num_ingredients{ 0 };
		// Go over all ingredients
		for (auto& [ingredient, in_mapping] : ingredient_mapping)
		{
			bool not_allergen{ true };
			// For each ingredient check all allergens, if one matches, this might have allergens
			for (auto& [allergen, al_mapping] : allergen_mapping)
			{
				bool detectedMapping{ true };
				for (auto val : al_mapping)
				{
					if (std::find(in_mapping.begin(), in_mapping.end(), val) == in_mapping.end())
					{
						// Did not find mapping
						detectedMapping = false;
						break;
					}
				}
				// If all values were in there we have a mapping and this might have allergens
				if (detectedMapping)
				{
					not_allergen = false;
					break;
				}
			}
			if (not_allergen)
				num_ingredients += in_mapping.size();
		}
		return num_ingredients;
	}

	std::vector<Food> food_items;
	std::map<std::string, std::vector<int>> ingredient_mapping;
	std::map<std::string, std::vector<int>> allergen_mapping;
	std::map<std::string, std::vector<std::string>> ingredient_allergen_mapping;
	std::map<int, std::string> final_ingredient_allergen_mapping;
};

int main()
{
	std::cout << "Twentyfirst day of Advent code!" << std::endl;
	std::fstream input("../../../data/21/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	Menu menu;
	std::string line;
	while (getline(input, line))
	{
		Food food_item;
		auto ingredients = line.substr(0, line.find("(") - 1);
		auto allergens = line.substr(line.find("(") + 10);
		allergens = allergens.substr(0, allergens.find(")"));

		std::stringstream ss(ingredients);
		while (getline(ss, line, ' '))
		{
			food_item.ingredients.push_back(std::move(line));
		}

		std::stringstream ss2(allergens);
		int index{ 0 };
		while (getline(ss2, line, ','))
		{
			if(index++)
				food_item.allergens.push_back(std::move(line.substr(1)));
			else
				food_item.allergens.push_back(std::move(line));
		}
		menu.food_items.push_back(std::move(food_item));
	}

	menu.generateMappings();

	// Task 1
	std::cout << "Task1: Ingredient occurence definitely without allergens: " << menu.countIngredientsWithoutAllergen() << std::endl;

	// Task 2
	std::cout << "Task2: Output is: " << menu.findIngredientAllergenMapping() << std::endl;

	return 0;
}