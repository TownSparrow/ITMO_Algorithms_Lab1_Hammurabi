#include <iostream>
#include <locale>
#include <codecvt>
#include <cstdlib>
#include <ctime>
#include "game.h"

using namespace std;

// Начало игры
void StartGame(GameState& state) {
  if (LoadGame(state)) {
    wcout << L"Найдено сохранение! Продолжить игру? (y/n): ";
    char choice;
    cin >> choice;
    if (choice == 'n') {
      InitializeGameState(state); // Используем новую функцию для инициализации
    }
  } else {
    InitializeGameState(state); // Инициализация, если нет сохранения
  }
}

// Процесс игры непосредственно
void PlayGame(GameState& state) {
  // Основной игровой цикл
  while (state.year <= 10) {
    PrintReport(state);
    int acres_to_buy, acres_to_sell, bushels_for_food, acres_to_plant;
    if (!ProcessInput(state, acres_to_buy, acres_to_sell, bushels_for_food, acres_to_plant)) {
      continue;
    }
    NextRound(state);
    SaveGame(state);
  }
}

// main =)
int main() {
  setlocale(LC_ALL, "ru_RU.UTF-8");
  wcout.imbue(locale("ru_RU.UTF-8"));
  srand(static_cast<unsigned int>(time(0))); // Инициализация генератора случайных чисел
    
  GameState state; // Объявляем состояние игры
  StartGame(state); // Запускаем игру

  PlayGame(state); // Игровой процесс

  cout << "Игра окончена!" << endl;
  return 0;
}
