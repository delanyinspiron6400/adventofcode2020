#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <bitset>
#include <map>
#include <sstream>
#include <algorithm>
#include <deque>

using Player = std::deque<int>;


struct Game
{
	void playGame()
	{
		while (true)
		{
			auto val_player01 = player_01.front();
			player_01.pop_front();
			auto val_player02 = player_02.front();
			player_02.pop_front();

			if (val_player01 > val_player02)
			{
				// Player 1 wins
				player_01.push_back(val_player01);
				player_01.push_back(val_player02);
			}
			else
			{
				// Player 2 wins
				player_02.push_back(val_player02);
				player_02.push_back(val_player01);
			}

			if (player_01.empty() || player_02.empty())
				break;
		}
	}

	std::string genState(const Player& player)
	{
		std::stringstream result;
		std::copy(player.begin(), player.end(), std::ostream_iterator<int>(result));
		return result.str();
	}

	void printPlayerDeck(const Player& player, std::string&& prefix)
	{
		std::cout << prefix << "'s deck: ";
		for (auto val : player)
			std::cout << val << ", ";
		std::cout << std::endl;
	}

	// Returns if player 01 won
	bool recursiveRound(Player p_01, Player p_02, int game_id=1)
	{
		if(Print)
			std::cout << "=== Game " << game_id << " ===" << std::endl;
		std::vector<std::string> p01_state;
		std::vector<std::string> p02_state;
		
		int iter{ 1 };
		while (true)
		{
			//getchar();
			if (Print) 
			{
				std::cout << "\n--Round " << iter << " (Game " << game_id << ")--" << std::endl;
				printPlayerDeck(p_01, "Player 1");
				printPlayerDeck(p_02, "Player 2");
			}
			
			// Check state
			bool state_match{ false };
			auto p01_curr_state = genState(p_01);
			for (auto& state : p01_state)
			{
				if (state.compare(p01_curr_state) == 0)
				{
					state_match = true;
					break;
				}
			}
			p01_state.push_back(p01_curr_state);
			auto p02_curr_state = genState(p_02);
			if (state_match)
			{
				state_match = false;
				for (auto& state : p02_state)
				{
					if (state.compare(p02_curr_state) == 0)
					{
						state_match = true;
						break;
					}
				}
			}
			p02_state.push_back(p02_curr_state);
			if (state_match)
			{
				// Player 1 automatically wins
				return true;
			}

			// -----------------------------
			// Play round
			bool player_01_wins{ true };
			auto val_player01 = p_01.front();
			p_01.pop_front();
			auto val_player02 = p_02.front();
			p_02.pop_front();

			if (Print)
				std::cout << "Player 1 plays: " << val_player01 << std::endl;
			if (Print)
				std::cout << "Player 2 plays: " << val_player02 << std::endl;

			// Check if need to start recursive round
			if (val_player01 <= p_01.size() && val_player02 <= p_02.size())
			{
				// Let's start a subgame
				Player new_p01;
				for (auto i = 0; i < val_player01; ++i)
					new_p01.push_back(p_01[i]);

				Player new_p02;
				for (auto i = 0; i < val_player02; ++i)
					new_p02.push_back(p_02[i]);

				if (Print)
					std::cout << "Playing a sub - game to determine the winner..." << std::endl;
				player_01_wins = recursiveRound(std::move(new_p01), std::move(new_p02), game_id + 1);
				if (Print)
					std::cout << "...anyway, back to game "<< game_id <<"." << std::endl;
				// Who gets the card
				if (player_01_wins)
				{
					if (Print)
						std::cout << "Player 1 wins round " << iter << " of game " << game_id << std::endl;
					// Player 1 wins
					p_01.push_back(val_player01);
					p_01.push_back(val_player02);
				}
				else
				{
					if (Print)
						std::cout << "Player 2 wins round " << iter << " of game " << game_id << std::endl;
					// Player 2 wins
					p_02.push_back(val_player02);
					p_02.push_back(val_player01);
				}
			}
			else
			{
				// Normal rules
				player_01_wins = (val_player01 > val_player02);
				// Who gets the card
				if (player_01_wins)
				{
					if (Print)
						std::cout << "Player 1 wins round " << iter << " of game " << game_id << std::endl;
					// Player 1 wins
					if (val_player01 > val_player02)
					{
						p_01.push_back(val_player01);
						p_01.push_back(val_player02);
					}
					else
					{
						p_01.push_back(val_player02);
						p_01.push_back(val_player01);
					}
				}
				else
				{
					if (Print)
						std::cout << "Player 2 wins round " << iter << " of game " << game_id << std::endl;
					// Player 2 wins
					if (val_player01 > val_player02)
					{
						p_02.push_back(val_player01);
						p_02.push_back(val_player02);
					}
					else
					{
						p_02.push_back(val_player02);
						p_02.push_back(val_player01);
					}
				}
			}

			if (p_01.empty() || p_02.empty())
			{
				if (Print)
					std::cout << "The winner of game " << game_id << " is player " << ((p_01.size() != 0) ? "1" : "2") << "!" << std::endl;
				if(game_id == 1)
				{
					player_01 = p_01;
					player_02 = p_02;
				}
				return (p_01.size() != 0);
			}
			++iter;
		}
	}

	void playRecursiveCombatGame()
	{
		// Start game of recursive combat
		recursiveRound(player_01, player_02);
	}

	unsigned long long gameResult()
	{
		auto& player = player_01;
		if (player.empty())
			player = player_02;

		unsigned long long ret_val{ 0 };
		int size_of_deck = player.size();
		for (int i = size_of_deck; i > 0; --i)
		{
			auto val = player.front();
			player.pop_front();
			ret_val += static_cast<unsigned long long>(val) * i;
		}
		return ret_val;
	}

	// Members
	Player player_01;
	Player player_02;
	static constexpr bool Print{ false };
};

int main()
{
	std::cout << "Twentysecond day of Advent code!" << std::endl;
	std::fstream input("../../../data/22/input.txt", std::ios_base::in);
	if (!input.is_open())
	{
		std::cout << "Could not open file!\n";
		exit(-1);
	}

	Player player01;
	std::string line;
	int line_num{ 0 };
	while (getline(input, line))
	{
		if (line.empty())
			break;
		if (line_num++ == 0)
			continue;
		player01.push_back(std::stoi(line));
	}

	Player player02;
	line_num = 0;
	while (getline(input, line))
	{
		if (line.empty())
			break;
		if (line_num++ == 0)
			continue;
		player02.push_back(std::stoi(line));
	}

	//// Task 1
	//Game game;
	//game.player_01 = player01;
	//game.player_02 = player02;
	//game.playGame();
	//std::cout << "Task1: Player score is: " << game.gameResult() << std::endl;

	// Task 2
	Game game2;
	game2.player_01 = std::move(player01);
	game2.player_02 = std::move(player02);
	game2.playRecursiveCombatGame();
	std::cout << "Task2: Player score is: " << game2.gameResult() << std::endl;
	return 0;
}