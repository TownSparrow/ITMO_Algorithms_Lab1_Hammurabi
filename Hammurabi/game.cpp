#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include "game.h"

using namespace std;

// Некоторые константные значения
const int bushels_per_person = 20;
const int acres_per_person = 10;
const double bushels_per_acre_seed = 0.5;
const int plague_probability = 15;
const int min_bushels_per_acre = 1;
const int max_bushels_per_acre = 6;
const int min_land_price = 17;
const int max_land_price = 26;

// Динамически изменяемые значения
int acres_to_buy, acres_to_sell, bushels_for_food, acres_to_plant;

// Инициализация состояния игры
void InitializeGameState(GameState& state) {
  state.year = 1;
  state.population = 100;
  state.bushels = 2800;
  state.acres = 1000;
  state.bushels_per_acre = 0; // будет установлено в начале каждого раунда
  state.rats_ate = 0; // будет обновлено в следующем раунде
  state.new_people = 0; // будет обновлено в следующем раунде
  state.plague = false; // начальное состояние без чумы
  state.died_of_starvation = 0; // начальное количество человек, умерших от голода за все время
  state.died_of_starvation_current_year = 0; // начальное количество человек, умерших от голода за текущий год
}

// Сохранение игры
void SaveGame(GameState& state) {
  ofstream save_file("savegame.txt");
  save_file << state.year << " " << state.population << " " << state.bushels << " "
            << state.acres << " " << state.bushels_per_acre << " " << state.rats_ate << " "
            << state.new_people << " " << state.plague << endl;
  save_file.close();
}

// Загрузка игры
bool LoadGame(GameState& state) {
  ifstream save_file("savegame.txt");
  if (save_file.is_open()) {
    save_file >> state.year >> state.population >> state.bushels >> state.acres
              >> state.bushels_per_acre >> state.rats_ate >> state.new_people >> state.plague >> state.died_of_starvation >> state.died_of_starvation_current_year;
    save_file.close();
    return true;
  }
  return false;
}

// Выход из игры
void ExitGame(GameState& state) {
  cout << "Хотите сохранить игру перед выходом? (y/n): ";
  char choice;
  while (true) {
    cin >> choice;
    if (choice == 'y') {
      SaveGame(state);
      cout << "Сохранение выполнено." << endl;
      break;
    }
    else if (choice == 'n') {
      break;
    }
    else {
      cout << "Неверный ввод. Пожалуйста, введите 'y' для подтверждения или 'n' для отказа: "; 
    }
  }
  cout << "Игра завершена." << endl;
  this_thread::sleep_for(chrono::seconds(2));
  exit(0);
}

// Отчет за шаг игры
void PrintReport(GameState& state) {
  // Разделение этапов отчёта
  cout << "_____________________________________________" << endl;

  // Приветствие
  cout << "Мой повелитель, соизволь поведать тебе в году " << state.year 
       << " твоего высочайшего правления." << endl;

  // Умершие от голода
  if (state.year > 1) {
    if (state.died_of_starvation_current_year > 0) {
      cout << state.died_of_starvation_current_year << " человек умерли с голоду, и ";
    } else {
      cout << "В этом году не было смертей от голода; ";
    }
  }

  // Новые жители
  if (state.new_people > 0) {
    cout << state.new_people << " человек прибыли в наш великий город; ";
  }

  // Информация о чуме
  if (state.plague) {
    cout << "Чума уничтожила половину населения; ";
  }

  // Текущее население
  cout << "Население города сейчас составляет " << state.population << " человек;" << endl;

  // Сбор пшеницы и пшеница с акра
  if (acres_to_plant > 0) {
    cout << "Мы собрали " << state.bushels_per_acre * acres_to_plant << " бушелей пшеницы, по "
      << state.bushels_per_acre << " бушеля с акра;" << endl;
  } else {
    cout << "Мы ничего не собрали с акров, потому что посева не было... " << endl;
  }
  

  // Пшеница, уничтоженная крысами
  cout << "Крысы истребили " << state.rats_ate << " бушелей пшеницы, оставив " 
       << state.bushels << " бушелей в амбарах;" << endl;

  // Акры земли
  cout << "Город сейчас занимает " << state.acres << " акров;" << endl;

  // Цена одного акра земли в этом году
  int land_price = rand() % (max_land_price - min_land_price + 1) + min_land_price; // Генерация цены акра
  cout << "1 акр земли стоит сейчас " << land_price << " бушель." << endl;
  cout << "Что пожелаешь, повелитель?" << endl;
}



// Реализация логики следующего раунда
void NextRound(GameState& state) {
  // Изменение года
  state.year++;

  // Генерация количества пшеницы с акров земли
  state.bushels_per_acre = rand() % (max_bushels_per_acre - min_bushels_per_acre + 1) + min_bushels_per_acre;

  // Определяем, сколько акров будет обработано
  int acres_to_cultivate = min(state.acres, state.population * acres_per_person);

  // Сколько съели крысы
  state.rats_ate = rand() % static_cast<int>(0.07 * state.bushels);

  // Проверка на чуму
  state.plague = (rand() % 100) < plague_probability;
  if (state.plague) {
    state.population /= 2; // Уменьшение населения вдвое при возникновении чумы
  }

  // Обновление количества пшеницы c учетом того, был ли посев
  //state.bushels = state.bushels - state.rats_ate + (state.bushels_per_acre * acres_to_cultivate);
  if (acres_to_plant > 0) {
    state.bushels = state.bushels - state.rats_ate + (state.bushels_per_acre * acres_to_plant);
  }
  else {
    state.bushels = state.bushels - state.rats_ate;
  }

  // Проверка потребления пшеницы и подсчет умерших от голода
  int food_needed = state.population * bushels_per_person;
  int starving_people = max(state.population - (bushels_for_food / bushels_per_person), 0);

  // Обновление состояний
  if (starving_people > 0) {
    state.died_of_starvation_current_year = starving_people;
    state.died_of_starvation += starving_people;
    state.population -= starving_people;

    // Проверка условия проигрыша
    if (static_cast<double>(starving_people) / (state.population + starving_people) > 0.45) {
      cout << "Вы проиграли. Более 45% населения умерло от голода." << endl;
      exit(0);
    }
  } else {
    state.died_of_starvation_current_year = 0; // Никто не умер, если еды хватает
  }

  // Генерация новых жителей
  state.new_people = max(0, min(50, (state.died_of_starvation_current_year / 2) + (5 - state.bushels_per_acre) * state.bushels / 600 + 1));
  state.population += state.new_people; // Обновление общего числа населения
}

// Проверка входных данных
bool ProcessInput(GameState& state) {
  int land_price = rand() % (max_land_price - min_land_price + 1) + min_land_price;
  string input;
  string ask;

  // Запрос на покупку акров
  ask = "Сколько акров земли повелеваешь купить? ";
  cout << ask;
  cin >> input;
  if (!CheckInput(state, input, ask)) return false;
  acres_to_buy = stoi(input);
  if (acres_to_buy * land_price > state.bushels) {
    PrintIncorrectWayMessage(state);
    return false;
  }

  // Запрос на продажу акров
  ask = "Сколько акров земли повелеваешь продать? ";
  cout << ask;
  cin >> input;
  if (!CheckInput(state, input, ask)) return false;
  acres_to_sell = stoi(input);
  if (acres_to_sell > state.acres) {
    PrintIncorrectWayMessage(state);
    return false;
  }

  // Запрос на потребление пшеницы
  ask = "Сколько бушелей пшеницы повелеваешь съесть? ";
  cout << ask;
  cin >> input;
  if (!CheckInput(state, input, ask)) return false;
  bushels_for_food = stoi(input);
  if (bushels_for_food > state.bushels) {
    PrintIncorrectWayMessage(state);
    return false;
  }

  // Запрос на засеивание акров
  ask = "Сколько акров земли повелеваешь засеять? ";
  cout << ask;
  cin >> input;
  if (!CheckInput(state, input, ask)) return false;
  acres_to_plant = stoi(input);
  if (acres_to_plant > state.acres) {
    PrintIncorrectWayMessage(state);
    return false;
  }
  if (acres_to_plant * bushels_per_acre_seed > state.bushels) {
    PrintIncorrectWayMessage(state);
    return false;
  }

  // Обновление состояния игры
  state.acres += acres_to_buy - acres_to_sell;
  state.bushels -= (acres_to_buy * land_price) + bushels_for_food + (acres_to_plant * bushels_per_acre_seed);

  return true;
}

// Проверка ввода на команду "exit" или число ли это вообще
bool CheckInput(GameState& state, string& input, string& ask) {
  while (true) {
    if (input == "exit") {
      ExitGame(state);
      return false;
    }
    bool isDigit = true;
    for (char c : input) {
      if (!std::isdigit(c)) {
        isDigit = false;
        break;
      }
    }
    if (isDigit) {
      try {
        int value = stoi(input);
        return true;
      } catch (const out_of_range&) {
        PrintIncorrectWayMessage(state);
        cout << ask;
        cin >> input;
      }
    } else {
      cout << "Неверный ввод. Пожалуйста, введите либо число, либо команду 'exit' для завершения игры: ";
      cin >> input;
    }
  } 
}

// Сообщение о том, что экономика распределена неправильно
void PrintIncorrectWayMessage(GameState& state) {
  cout << "О, повелитель, пощади нас! У нас только " + to_string(state.population) + " человек, " + to_string(state.bushels) + " бушелей пшеницы и " + to_string(state.acres) + " акров земли!" << endl;
}

// Вывод финальных результатов
void PrintFinalResults(GameState& state) {
  cout << "_____________________________________________" << endl;
  cout << "РЕЗУЛЬТАТЫ ИГРЫ:" << endl;

  // Вычисление статистики
  double average_death_rate = static_cast<double>(state.died_of_starvation) / state.population * 100.0 / state.year;
  int land_per_person = state.acres / state.population;

  // Вывод в соответствие с условиями
  if (average_death_rate > 33.0 && land_per_person < 7) {
    cout << "Оценка: плохо" << endl;
    cout << "Из-за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас из города. Теперь вы вынуждены влачить жалкое существование в изгнании." << endl;
  } else if (average_death_rate > 10.0 && land_per_person < 9) {
    cout << "Оценка: удовлетворительно" << endl;
    cout << "Вы правили железной рукой, подобно Нерону и Ивану Грозному. Народ вздохнул с облегчением, и никто больше не желает видеть вас правителем." << endl;
  } else if (average_death_rate > 3.0 && land_per_person < 10) {
    cout << "Оценка: хорошо" << endl;
    cout << "Вы справились вполне неплохо, у вас, конечно, есть недоброжелатели, но многие хотели бы увидеть вас во главе города снова." << endl;
  } else {
    cout << "Оценка: отлично" << endl;
    cout << "Фантастика! Карл Великий, Дизраэли и Джефферсон вместе не справились бы лучше." << endl;
  }

}