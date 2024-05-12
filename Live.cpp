#include <iostream>
#include <string>
#include <fstream>
//#include <cstdlib>

//глобальные переменные   // МОЖНО ЛИ РЕШИТЬ БЕЗ НИХ?   // и насколько это будет оптимальней? 
//int alive_cells = 0;
//int gen = 1;
//bool game_over = true;

//Создаем массив
char** create_arr(int row, int col) {
    char** arr = new char* [row];
    for (int i = 0; i < row; ++i) {
        arr[i] = new char[col];
    }
    return arr;
}

//Заполняем массив
void fill_arr(char** arr, int row, int col) {
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j){
            arr[i][j] = '-';

        }

    }
}

//Подсчет живых клеток
int count_alive_cells(char** arr, int row, int col) {
    int alive_cells=0;
    
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            if (arr[i][j] == '*') {
                alive_cells++;
                //alive_cels2++;
            }
        }
    }
    return alive_cells;
}

//Выводим массив на консоль
void print_arr(char** arr, int row, int col, int& gen2, int alive_cells){
    for (int i = 0; i < row; ++i){
        for (int j = 0; j < col; ++j) {
            std::cout << arr[i][j] << " ";
        }
        std::cout << std::endl;
    }
  std::cout << "Generaion: " << gen2 << ". Alive cells: " << alive_cells <</* ". Check game over "<< game_over<<*/std::endl;
}

// Уравниваем массивы после рождения нового поколения
void equal_array(char** old, char** next, int row, int col){
    for (int i = 0; i < row; ++i)  {
        for (int j = 0; j < col; ++j) {
            old[i][j] = next[i][j];
        }

    }
}

int born_or_die(char** old, char** next, /*int row, int col,*/ int i, int j, int arround)
{
      if (old[i][j] == '*' && (arround < 2 || arround > 3)) {
        next[i][j] = '-';

        }
      else if (old[i][j] == '-' && arround == 3) {
          next[i][j] = '*';

        } 
      else  {
          next[i][j] = old[i][j];

        }  
  return  0;
}

//Создаем новое поколение
void create_next_generation(char** old, char** next, int row, int col, int& gen) {
  int arround = 0; 
  gen++;  
  for (int i = 0; i < row; ++i){
        for (int j = 0; j < col; ++j) {



                    for(int near_i = i - 1; near_i <= i + 1; ++near_i) {
                        for(int near_j = j - 1; near_j <= j + 1; ++near_j) {
                            if (near_i >= 0 && near_i < row && near_j >= 0 && near_j < col) {
                                if (near_i != i || near_j != j)  {
                                    if (old[near_i][near_j] == '*') {
                                        arround++;
                                    }
                                }
                            }
                        }
                    }
                   /* if (old[i][j] == '*' && (arround < 2 || arround > 3)) {
                        next[i][j] = '-';

                    }  else if (old[i][j] == '-' && arround == 3) {
                        next[i][j] = '*';

                    } else  {
                        next[i][j] = old[i][j];

                    }  
                    arround = 0;*/
                    arround = born_or_die(old, next, i, j, arround);

                }
            }
}



//Проверка на окончание игры
bool check_end_game(char** old, char** next, int row, int col) {
    bool game_over = false;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j){
            if (old[i][j] != next[i][j]) {
                game_over = true;
                return game_over;  
              //break;
            } 
        }
    }
    return game_over;
}

//Освобождаем память
void delete_arr(char** arr, int row, int col){
    for (int i = 0; i < row; ++i) {
        delete[] arr[i];
    }
    delete[] arr;
}


int main(){ 
  int alive_cells = 0;
  int row;
  int col;
  static int gen2 = 1;
  bool game_over = true;

      // Считали файл и создали массив
  std::ifstream file("game.txt");// Открыли файл для чтения input.txt или game.txt для проверки разных условий завершения игры
  std::string live;
    if (!file.is_open()) {
        std::cout << "Ошибка открытия файла!";
        return 1;
    }
  file >> live;
  row = std::stoi(live);
  file >> live;
  col = std::stoi(live);

    //Создали 2 поля и заполнили их
  char** old = create_arr(row, col);
  char** next = create_arr(row, col);
  fill_arr(old, row, col);
  fill_arr(next, row, col);

    //считываем из файла и преобразоваем в живые клетки до конца читаемого файла
  int x = 0, y = 0;
  while (!file.eof()) {
        file >> x >> y;
        old[x][y] = '*';
    }
    //закрыли файл
  file.close();

  alive_cells = count_alive_cells(old, row, col);
  print_arr(old, row, col, gen2, alive_cells);
  //

  // создали цикл для игры пока не наступит одно из 2-ух условий окончания игры
  while(!(alive_cells == 0 || !game_over) ) {   //|| !game_over
  std::cout << std::endl;
  create_next_generation(old, next, row, col, gen2);
  alive_cells = count_alive_cells(next,  row,  col);
  game_over = check_end_game(old, next, row, col);
  print_arr(next, row, col, gen2, alive_cells);
  equal_array(old, next, row, col);
  }

  //
  if (alive_cells == 0){
    std::cout <<("All cells are dead. Game over");

  }
    else{
    printf("The world has stagnated. Game over");
  }

  delete_arr(old, row, col);
  delete_arr(next, row, col);


return 0;

}