#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <utility>
#include <sstream>

class Passport
{
public:
	static constexpr unsigned int not_set_value{ 0xFFFFFFFF };

	bool checkIfValid() const
	{
		bool valid{ true };
		valid = (birth_year != not_set_value) ? valid : false;
		valid = (issue_year != not_set_value) ? valid : false;
		valid = (expiration_year != not_set_value) ? valid : false;
		valid = (!height.empty()) ? valid : false;
		valid = (!hair_colour.empty()) ? valid : false;
		valid = (!eye_colour.empty()) ? valid : false;
		valid = (passport_id != not_set_value) ? valid : false;
		return valid;
	}
	
	void parseKeyValuePair(std::string key, std::string value)
	{
		if (key.compare("byr") == 0)
		{
			// Birth year
			try
			{
				unsigned int val = std::stoi(value);
				birth_year = val;
			}
			catch (...) {}
			birth_year = 0;
		}
		else if (key.compare("iyr") == 0)
		{
			// Issue year
			try
			{
				unsigned int val = std::stoi(value);
				issue_year = val;
			}
			catch (...) {}
			issue_year = 0;
		}
		else if (key.compare("eyr") == 0)
		{
			// Expiration year
			try
			{
				unsigned int val = std::stoi(value);
				expiration_year = val;
			}
			catch (...) {}
			expiration_year = 0;
		}
		else if (key.compare("hgt") == 0)
		{
			// Height
			height = std::move(value);
		}
		else if (key.compare("hcl") == 0)
		{
			// Hair colour
			hair_colour = std::move(value);
		}
		else if (key.compare("ecl") == 0)
		{
			// Eye colour
			eye_colour = std::move(value);
		}
		else if (key.compare("pid") == 0)
		{
			// Password ID
			try
			{
				unsigned int val = std::stoi(value);
				passport_id = val;
			}
			catch (...) {}
			passport_id = 0;
		}
		else if (key.compare("cid") == 0)
		{
			// Country ID
			try
			{
				unsigned int val = std::stoi(value);
				country_id = val;
			}
			catch (...) {}
			country_id = 0;
		}
		else
			std::cout << "Unrecognized token" << std::endl;
	}

	void parseKeyValuePairWithValueCheck(std::string key, std::string value)
	{
		if (key.compare("byr") == 0)
		{
			// Birth year
			try
			{
				unsigned int val = std::stoi(value);
				if (val >= 1920 && val <= 2002)
					birth_year = val;
			}
			catch (...) {}
		}
		else if (key.compare("iyr") == 0)
		{
			// Issue year
			try
			{
				unsigned int val = std::stoi(value);
				if (val >= 2010 && val <= 2020)
					issue_year = val;
			}
			catch (...) {}
		}
		else if (key.compare("eyr") == 0)
		{
			// Expiration year
			try
			{
				unsigned int val = std::stoi(value);
				if (val >= 2020 && val <= 2030)
					expiration_year = val;
			}
			catch (...) {}
		}
		else if (key.compare("hgt") == 0)
		{
			// Height
			std::string unit = value.substr(value.length() - 2);
			if (unit.compare("in") == 0)
			{
				std::string number = value.substr(0, value.length() - 2);
				try
				{
					unsigned int num = std::stoi(number);
					if(num >= 59 && num <= 76)
						height = std::move(value);
				}
				catch (...) {}
			}
			else if(unit.compare("cm") == 0)
			{
				std::string number = value.substr(0, value.length() - 2);
				try
				{
					unsigned int num = std::stoi(number);
					if (num >= 150 && num <= 193)
						height = std::move(value);
				}
				catch (...) {}
			}
		}
		else if (key.compare("hcl") == 0)
		{
			// Hair colour
			if (value.at(0) == '#')
			{
				bool valid_colour{ true };
				for (int i = 1; i < 7 && value.length() == 7; ++i)
				{
					if((value.at(i) >= '0' && value.at(i) <= '9') || (value.at(i) >= 'a' && value.at(i) <= 'f'))
						hair_colour = std::move(value);
				}
			}
		}
		else if (key.compare("ecl") == 0)
		{
			// Eye colour
			if(value.compare("amb") == 0 ||
				value.compare("blu") == 0 ||
				value.compare("brn") == 0 ||
				value.compare("gry") == 0 ||
				value.compare("grn") == 0 ||
				value.compare("hzl") == 0 ||
				value.compare("oth") == 0)
				eye_colour = std::move(value);
		}
		else if (key.compare("pid") == 0)
		{
			// Password ID
			try
			{
				unsigned int val = std::stoi(value);
				if (value.length() == 9)
					passport_id = val;
			}
			catch (...) {}
		}
		else if (key.compare("cid") == 0)
		{
			// Country ID
			try
			{
				unsigned int val = std::stoi(value);
				country_id = val;
			}
			catch (...) {}
		}
		else
			std::cout << "Unrecognized token" << std::endl;
	}

private:
	// Members
	unsigned int birth_year{ not_set_value };
	unsigned int issue_year{ not_set_value };
	unsigned int expiration_year{ not_set_value };
	std::string height;
	std::string hair_colour;
	std::string eye_colour;
	unsigned int passport_id{ not_set_value };
	unsigned int country_id{ not_set_value };
};

template <typename Out>
void split(const std::string& s, char delim, Out result) 
{
	std::istringstream iss(s);
	std::string item;
	while (std::getline(iss, item, delim)) {
		*result++ = item;
	}
}

std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

int main()
{
	std::cout << "Fourth day of Advent code!" << std::endl;
	std::fstream input("../../../data/04/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	
	std::vector<std::string> input_data;
	std::string line;
	bool new_passport{ true };
	while (getline(input, line))
	{
		// Empty lines indicate a new passport starting
		if (line.empty())
		{
			new_passport = true;
			continue;
		}
		if (new_passport)
		{
			input_data.push_back(std::move(line));
			new_passport = false;
		}
		else
		{
			input_data[input_data.size() - 1] += std::string(" ") + std::move(line);
		}
	}

	// Go over input data and create passports
	std::vector<Passport> passports;
	passports.reserve(input_data.size());
	for (auto& item : input_data)
	{
		//std::cout << "----- New PassPort -----\n";
		Passport passport;
		auto passport_entries = split(item, ' ');
		for (auto& entry : passport_entries)
		{
			// Each entry is separated by ":"
			auto key = entry.substr(0, entry.find(":"));
			auto value = entry.substr(entry.find(":") + 1);
			passport.parseKeyValuePair(std::move(key), std::move(value));
		}
		passports.push_back(std::move(passport));
	}

	int num_valid_passports{ 0 };
	for (const auto& passport : passports)
	{
		if (passport.checkIfValid())
			++num_valid_passports;
	}
	std::cout << "The number of valid passports is: " << num_valid_passports << " out of " << passports.size() << std::endl;
	std::cout << "---------------------------------" << std::endl;

	// -----------------------------------------------------
	// Task 2
	std::vector<Passport> passports_2;
	passports_2.reserve(input_data.size());

	for (auto& item : input_data)
	{
		//std::cout << "----- New PassPort -----\n";
		Passport passport;
		auto passport_entries = split(item, ' ');
		for (auto& entry : passport_entries)
		{
			// Each entry is separated by ":"
			auto key = entry.substr(0, entry.find(":"));
			auto value = entry.substr(entry.find(":") + 1);
			passport.parseKeyValuePairWithValueCheck(std::move(key), std::move(value));
		}
		passports_2.push_back(std::move(passport));
	}
	int num_valid_passports_2{ 0 };
	for (const auto& passport : passports_2)
	{
		if (passport.checkIfValid())
			++num_valid_passports_2;
	}
	std::cout << "The number of valid passports with value check is: " << num_valid_passports_2 << " out of " << passports.size() << std::endl;

	return 0;
}