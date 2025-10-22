#include <array>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <format>
#include <ios>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <thread>
#include <unordered_map>

enum class Choice { Rock, Paper, Scissors, Quit };
enum class Result { Win, Loss, Draw };

std::string to_string(const Choice choice) {
  switch (choice) {
    case Choice::Rock:
      return "Rock";
    case Choice::Paper:
      return "Paper";
    case Choice::Scissors:
      return "Scissors";
    case Choice::Quit:
      return "Quit";
    default:
      std::cerr << "Fatal Error: Impossible case triggered. to_string\n";
      std::abort();
  }
}

Choice get_player_choice() {
  std::cout << "(Rock = R, Paper = P, Scissors = S, Quit = Q)\n";

  while (true) {
    std::cout << "> ";

    char raw_choice;
    std::cin >> raw_choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (tolower(raw_choice)) {
      case 'r':
        return Choice::Rock;
      case 'p':
        return Choice::Paper;
      case 's':
        return Choice::Scissors;
      case 'q':
        return Choice::Quit;
      default:
        std::cout << "Error: Invalid input.\n";
    }
  }
}

Choice get_computer_choice() {
  static const std::array<Choice, 3> choices{Choice::Rock, Choice::Paper,
                                             Choice::Scissors};

  static std::mt19937 gen(std::random_device{}());
  static std::uniform_int_distribution<> distribution(0, choices.size() - 1);

  return choices[distribution(gen)];
}

void output_choices(const Choice player_choice, const Choice computer_choice) {
  std::this_thread::sleep_for(std::chrono::milliseconds(250));
  std::cout << std::format("\nYou chose {}!\n", to_string(player_choice));
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  std::cout << "And the computer chose...";
  std::cout.flush();
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  std::cout << std::format(" {}\n!", to_string(computer_choice));
}

Result get_result_from_round(const Choice player_choice,
                             const Choice computer_choice) {
  static std::unordered_map<Choice, std::unordered_map<Choice, Result>>
      result_map{{Choice::Rock,
                  {{Choice::Rock, Result::Draw},
                   {Choice::Paper, Result::Loss},
                   {Choice::Scissors, Result::Win}}},
                 {Choice::Paper,
                  {{Choice::Rock, Result::Win},
                   {Choice::Paper, Result::Draw},
                   {Choice::Scissors, Result::Loss}}},
                 {Choice::Scissors,
                  {{Choice::Rock, Result::Loss},
                   {Choice::Paper, Result::Win},
                   {Choice::Scissors, Result::Draw}}}};

  return result_map[player_choice][computer_choice];
}

void handle_result_from_round(const Result round_result, int& player_score,
                              int& computer_score) {
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  switch (round_result) {
    case Result::Win:
      std::cout << "You won!\n";
      ++player_score;
      break;
    case Result::Loss:
      std::cout << "You lost...\n";
      ++computer_score;
      break;
    case Result::Draw:
      std::cout << "It was a draw\n";
      break;
    default:
      std::cerr << "Fatal Error: Impossible case triggered. "
                   "handle_result_from_round\n";
      std::abort();
  }

  std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main() {
  std::cout << "\n\nWelcome to my Rock Paper Scissors program!\n\n";

  int player_score{};
  int computer_score{};

  while (true) {
    // Logic needed:
    // Ask player for move
    // Terminate if they chose quit
    // Generate computer move
    // Output choices
    // Get result
    // Handle result
    // Output score

    const Choice player_choice{get_player_choice()};
    if (player_choice == Choice::Quit) break;

    const Choice computer_choice{get_computer_choice()};

    output_choices(player_choice, computer_choice);

    const Result round_result{
        get_result_from_round(player_choice, computer_choice)};

    handle_result_from_round(round_result, player_score, computer_score);
    std::cout << std::format("Player score: {} | Computer Score: {}\n\n",
                             player_score, computer_score);
  }

  std::cout << std::format("Player score: {} | Computer Score: {}\n",
                           player_score, computer_score);
  if (player_score > computer_score) {
    std::cout << "You won the game!\n";
  } else if (player_score < computer_score) {
    std::cout << "You lost the game...\n";
  } else {
    std::cout << "The game was a draw.\n";
  }

  return 0;
}