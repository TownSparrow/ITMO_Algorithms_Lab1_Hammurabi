#include <iostream>
#include <locale>
#include <codecvt>
#include <cstdlib>
#include <ctime>
#include "game.h"

using namespace std;

// Начало игры
void StartGame(GameState& state) {
  wcout << L"Клон игры Hammurabi. Разработчик: Леонид Воробьев (TownSparrow)" << endl;

  if (LoadGame(state)) {
    char choice;
    wcout << L"Найдено сохранение! Продолжить игру (e - закрыть игру)? (y/n/e): ";
    while (true) {
      cin >> choice;
      if (choice == 'y') {
        break;
      } else if (choice == 'n') {
        InitializeGameState(state);
        break;
      } else if (choice == 'e') {
        exit(0);
        break;
      } else {
        wcout << L"Неверный ввод. Пожалуйста, введите 'y' для подтверждения, 'n' для отказа или 'e' для выхода из игры: "; 
      }
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
    bool isResourcesEnough = false;
    // Пока игрок не подберет правильное распределение ресурсов на этом этапе, будем запрашивать распределение заново
    do {
      isResourcesEnough = ProcessInput(state, acres_to_buy, acres_to_sell, bushels_for_food, acres_to_plant);
    } while (!isResourcesEnough);
    NextRound(state);
    // SaveGame(state);
  }
  PrintFinalResults(state);
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
