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
  int died_of_starvation_current_year;
};

void InitializeGameState(GameState& state);
void SaveGame(GameState& state);
bool LoadGame(GameState& state);
void ExitGame(GameState& state);
void PrintReport(GameState& state);
void NextRound(GameState& state);
bool ProcessInput(GameState& state);
bool CheckInput(GameState& state, string& input, string& ask);
void PrintIncorrectWayMessage(GameState& state);
void PrintFinalResults(GameState& state);

#endif // GAME_H
