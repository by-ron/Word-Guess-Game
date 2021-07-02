// WordGame - CLI word guessing game. Try and guess a random 5 letter word. For each guess, points are awarded based on the number of correct characters guessed and additional points for guessing the correct position of characters. 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

bool vecContainsWord(std::vector<std::string>& vec, std::string& word)
{
	for (auto& w : vec)
	{
		if (w == word)
		{
			return true;
		}
	}
	return false;
}

std::vector<int> fill_buckets(const std::string& word)
{
	std::vector<int> buckets(26, 0);
	for (char c : word)
	{
		buckets[c - 'a']++;
	}
	return buckets;
}

int score_match(const std::string& word1, const std::string& word2)
{
	const auto buckets1 = fill_buckets(word1);
	const auto buckets2 = fill_buckets(word2);

	int score = 0;
	for (int i = 0; i < 26; i++)
	{
		score += std::min(buckets1[i], buckets2[i]);
	}

	for (int i = 0; i < 5; i++)
	{
		if (word1[i] == word2[i])
		{
			score++;
		}
	}

	return score;
}

int main()
{
	std::vector<std::string> fiveWord;
	{
		// Load in text file of five letter words
		std::ifstream fiveWordFile("sgb-words.txt");

		// Append words to vector
		for (std::string line; std::getline(fiveWordFile, line);)
		{
			if (line.empty())
			{
				continue;
			}
			fiveWord.push_back(line);
		}
	}

	// Create RNG to pick random word from vector
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> dist(0, fiveWord.size() - 1);
	std::string target = fiveWord[dist(rng)];
	while (true)
	{
		std::cout << "Guess a 5 letter word: ";
		std::string guess;
		std::getline(std::cin, guess);

		if (guess == "quit")
		{
			break;
		}

		if (guess.size() != 5)
		{
			std::cout << "Don't be a fucking moron. I said FIVE letters." << std::endl;
			continue;
		}

		if (!vecContainsWord(fiveWord, guess))
		{
			std::cout << "Wow.. that's not even a word you idiot." << std::endl;
		}

		const int score = score_match(guess, target);

		if (score == 10)
		{
			std::cout << "Holy shit.. you actuall did it!" << std::endl;
			break;
		}
		else
		{
			std::cout << "You scored " << score << " points. Still an idiot. Try again." << std::endl;
			continue;
		}
	}

	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	return 0;
}