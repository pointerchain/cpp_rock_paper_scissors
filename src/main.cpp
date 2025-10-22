#include <array>
#include <cctype>
#include <chrono>
#include <ios>
#include <iostream>
#include <limits>
#include <print>
#include <random>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>

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
      std::unreachable();
  }
}

void output_separator() {
  std::println("\n--------------------------------------------\n");
}

Choice get_player_choice() {
  std::println("(Rock = R, Paper = P, Scissors = S, Quit = Q)");

  while (true) {
    std::print("> ");

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
        std::println("Error: Invalid input.\n");
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
  std::println("\nYou chose {}!", to_string(player_choice));
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  std::print("And the computer chose...");
  std::cout.flush();
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  std::println(" {}!", to_string(computer_choice));
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
      std::println("You won!");
      ++player_score;
      break;
    case Result::Loss:
      std::println("You lost...");
      ++computer_score;
      break;
    case Result::Draw:
      std::println("It was a draw");
      break;
    default:
      std::unreachable();
  }

  std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main() {
  std::println("\n\nWelcome to my Rock Paper Scissors program!\n");

  std::this_thread::sleep_for(std::chrono::seconds(1));

  int player_score{};
  int computer_score{};

  while (true) {
    // Logic needed:
    // Output score
    // Ask player for move
    // Terminate if they chose quit
    // Generate computer move
    // Output choices
    // Get result
    // Handle result

    output_separator();
    std::println("Player score: {} | Computer Score: {}\n", player_score,
                 computer_score);

    const Choice player_choice{get_player_choice()};
    if (player_choice == Choice::Quit) break;

    const Choice computer_choice{get_computer_choice()};

    output_choices(player_choice, computer_choice);

    const Result round_result{
        get_result_from_round(player_choice, computer_choice)};

    handle_result_from_round(round_result, player_score, computer_score);
  }

  output_separator();
  std::println("Final Score:");
  std::println("Player score: {} | Computer Score: {}\n", player_score,
               computer_score);
  if (player_score > computer_score) {
    std::println("You won the game!\n\n");
  } else if (player_score < computer_score) {
    std::println("You lost the game...\n\n");
  } else {
    std::println("The game was a draw.\n\n");
  }

  return 0;
}