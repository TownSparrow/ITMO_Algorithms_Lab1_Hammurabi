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

    // Проверка на наличие населения
    if (state.population <= 0) {
      cout << "Все жители погибли. Игра окончена." << endl;
      break;
    }
    
    bool isResourcesEnough = false;

    do {
      isResourcesEnough = ProcessInput(state);
    } while (!isResourcesEnough);
    
    NextRound(state);

    // Проверка состояния после раунда
    if (state.population <= 0) {
      cout << "Все жители погибли. Игра окончена." << endl;
      break;
    }
  }
  PrintFinalResults(state);
  ExitGame(state);
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
