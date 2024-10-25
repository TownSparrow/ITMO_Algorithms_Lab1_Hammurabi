#ifndef GAME_H
#define GAME_H
#include <string>

using namespace std;

struct GameState {
  int year;
  int population;
  int bushels;
  int acres;
  int bushels_per_acre;
  int rats_ate;
  int new_people;
  bool plague;
  int died_of_starvation;
};

void InitializeGameState(GameState& state);
void SaveGame(const GameState& state);
bool LoadGame(GameState& state);
void ExitGame(GameState& state);
void PrintReport(const GameState& state);
void NextRound(GameState& state);
bool ProcessInput(GameState& state, int& acres_to_buy, int& acres_to_sell, int& bushels_for_food, int& acres_to_plant);
bool CheckInput(GameState& state, string& input, string& ask);
void PrintIncorrectWayMessage(GameState& state);

#endif // GAME_H
