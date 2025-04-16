#include <chrono>
#include <iostream>
#include <random>

constexpr char WELCOME[] = "Welcome to the Number Guessing Game!\n";
constexpr char START_GAME[] = R"(I'm thinking of a number between 1 and 100.

Please select the difficulty level:
1. Easy (10 chances)
2. Medium (5 chances)
3. Hard (3 chances)

)";
constexpr char HIGH_SCORE_START_MSG[] = R"(
Here's your high score so far:
)";

constexpr char DIFFICULTY_PROMPT[] = "Enter your choice: ";

constexpr std::string_view DIFFICULTY_NAME[4] = {"", "Easy", "Medium", "Hard"};
constexpr int DIFFICULTY_MAX_CHANCES[4] = {0, 10, 5, 3};

constexpr char GUESS_PROMPT[] = "Enter your guess: ";


int readInt(const char* prompt, bool readDifficulty = false) {
	std::cout << prompt;
	std::string output;
	while (std::getline(std::cin, output)) {
		try {
			int value = std::stoi(output);
			if (readDifficulty && (value > 3 || value < 1)) {
				std::cout << "Your input is invalid! " << prompt;
				continue;
			}
			return value;
		} catch (const std::invalid_argument& exc) {
			std::cout << "Your input is not an integer! " << prompt;
		} catch (const std::out_of_range& exc) {
			std::cout << "Your input is out of range! " << prompt;
		}
	}
	return -1;
}

int main() {
	std::mt19937 mt(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	std::uniform_int_distribution distribution(1, 100);

	std::cout << WELCOME;

	int wins[4] {}, games[4] {}, attemptSum[4] {}, best[4] {};
	std::string input;

	while (true) {
		std::cout << START_GAME;
		
		int difficulty = readInt(DIFFICULTY_PROMPT, true);
		if (difficulty == -1) break;
		std::cout << std::endl;

		std::cout << "Great! You have selected the " << DIFFICULTY_NAME[difficulty] << " difficulty level.\n";
		std::cout << "Let's start the game!\n";

		std::cout << std::endl;

		games[difficulty]++;
		int max = DIFFICULTY_MAX_CHANCES[difficulty];
		int hidden = distribution(mt);
		int success = -1;

		for (int attempt = 1; attempt <= max; attempt++) {
			int guess = readInt(GUESS_PROMPT);
			if (guess == -1) break;
			if (hidden == guess) {
				success = attempt;
				break;
			} else if (hidden < guess) {
				std::cout << "Incorrect! The number is less than " << guess << ".\n\n" << std::flush;
			} else {
				std::cout << "Incorrect! The number is greater than " << guess << ".\n\n" << std::flush;
			}
		}
		if (success == -1) {
			std::cout << "You ran out of attempts! The number was " << hidden << ".\n\n" << std::flush;
		} else {
			wins[difficulty]++;
			attemptSum[difficulty] += success;
			if (best[difficulty] == 0 || success < best[difficulty]) 
				best[difficulty] = success;
			std::cout << "Congratulations! You guessed the correct number in " << success << " attempts.\n\n" << std::flush;
		}
		std::cout << "Do you want to play again (yes/<any>)? ";
		std::getline(std::cin, input);
		for (char& c : input) c = std::tolower(c);
		if (input != "yes") {
			break;
		}

		std::cout << HIGH_SCORE_START_MSG;
		for (int d = 1; d <= 3; d++) {
			std::cout << DIFFICULTY_NAME[d] << " - ";
			if (games[d] == 0) {
				std::cout << "Not played yet";
			} else {
				std::cout << wins[d] << " wins out of " << games[d] << " games ";
				std::cout << "(" << std::fixed << std::setprecision(3) << 1.0 * wins[d] / games[d] << "% games)";
				if (wins[d] > 0) {
					std::cout << ", averaging " << std::fixed << std::setprecision(3) << 1.0 * attemptSum[d] / wins[d] << " attempts per win";
					std::cout << ", with the best game completed in " << best[d] << " attempts";
				}
			}
			std::cout << ".\n";
		}
		std::cout << std::endl;
	}
	std::cout << "Quitting." << std::endl;
	return 0;
}