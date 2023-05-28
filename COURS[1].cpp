#include <iostream>
#include <cstdlib>
//#include <Header.h>
#include <fstream>
#include <string>
#include <sstream>

//Переместить сюда определения операционной системы для использования в разных местах
#if defined(_WIN32) || defined(WIN32) || defined(__MINGW32__) || defined(__BORLANDC__)
#define OS_WIN
//ЦВЕТА WINDOWS 
#define COLOR_RED SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED)
#define COLOR_WARNING SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN) 

#define COLOR_BLUE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE)

#define COLOR_RESET SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15)

#elif defined(linux) || defined(__CYGWIN__)
#define OS_LINUX

#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m" 
#define COLOR_BLUE "\033[34m"
#define COLOR_RESET "\033[0m"

#elif (defined(__APPLE__) || defined(__OSX__) || defined(__MACOS__)) && defined(__MACH__)//Чтобы убедиться, что мы работаем на новой версии macOS
#define OS_MAC

#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m" 
#define COLOR_BLUE "\033[34m"
#define COLOR_RESET "\033[0m"

#endif

#if defined(OS_WIN)
#include <windows.h> // использовать для windows
#endif

using namespace std;

const int gridSize = 25;
void printGrid(bool gridOne[gridSize + 1][gridSize + 1]);
void determineState(bool gridOne[gridSize + 1][gridSize + 1]);
void clearScreen(void);


int main() {
    setlocale(LC_ALL, "Russian");
    // system( "color A" );//LGT green
    //cout << COLOR_GREEN;
    clearScreen();
    bool gridOne[gridSize + 1][gridSize + 1] = {};
    int x, y, n;
    string nc;
    string start;
    string filename;
    cout << "                         ====ИГРА ЖИЗНЬ====" << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << "Правила" << endl;
    cout << "Вселенная Игры жизни представляет собой бесконечную двумерную ортогональную сетку из квадратных ячеек," << endl;
    cout << " каждый из которых находится в одном из двух возможных состояний, живом или мертвом. Каждая" << endl;
    cout << "ячейка взаимодействует со своими восемью соседями, которые являются ячейками, смежными по горизонтали, вертикали или диагонали." << endl;
    cout << "На каждом шаге во времени происходят следующие переходы:" << endl;
    cout << "1. Любая живая ячейка, имеющая менее двух живых соседей, умирает, как если бы это было вызвано недостаточной заселенностью." << endl;
    cout << "2. Любая живая ячейка с двумя или тремя живыми соседями живет до следующего поколения." << endl;
    cout << "3. Любая живая ячейка с более чем тремя живыми соседями умирает, как будто из-за перенаселения." << endl;
    cout << "4. Любая мертвая ячейка, имеющая ровно трех живых соседей, становится живой клеткой, как бы путем размножения." << endl;
    cout << endl;
    cout << "O - живая ячейка" << endl;
    cout << ". - мертвая ячейка" << endl;
    cout << endl;
    cout << "Введите количество ячеек или 'r' для чтения ячеек из файла: ";
    cin >> nc;
    cout << endl;

    if (nc == "r")
    {
        while (true)
        {

            cout << "Введите имя файла для чтения из: " << endl;
            cin >> filename;

            ifstream readfile(filename);
            if (readfile.is_open())
            {
                string fileline, xx, yy;

                while (getline(readfile, fileline))
                {
                    stringstream ss(fileline);

                    getline(ss, xx, ' ');
                    getline(ss, yy, ' ');

                    x = stoi(xx);
                    y = stoi(yy);

                    gridOne[x][y] = true;
                }
                break;
            }
            else {
                cout << "Такого файла нет, попробуйте еще раз." << endl;
            }
        }
    }
    else
    {

        for (int i = 0; i < stoi(nc); i++)
        {
            cout << stoi(nc) << "Введите координаты ячейки " << i + 1 << " : ";
            cin >> x >> y;
            gridOne[x][y] = true;
            printGrid(gridOne);
        }
    }
    cout << "Настройка сетки завершена. Запустить игру? (y / n)" << endl;
    printGrid(gridOne);
    cin >> start;
    if (start == "y" || start == "Y")
    {
        while (true)
        {
            printGrid(gridOne);
            determineState(gridOne);
            Sleep(200000);
            clearScreen();
        }
    }
    else
    {
        cout << COLOR_RESET;
        clearScreen();
        return 0;
    }
}

void clearScreen(void) {
#if defined(OS_WIN)
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = { 0, 0 };

    if (hStdOut == INVALID_HANDLE_VALUE) return;

    /* Получаем количество ячеек в текущем буфере */
    GetConsoleScreenBufferInfo(hStdOut, &csbi);
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    /* Заполните весь буфер пробелами */
    FillConsoleOutputCharacter(hStdOut, (TCHAR)' ', cellCount, homeCoords, &count);

    /* Заполните весь буфер текущими цветами и атрибутами */
    FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count);

    SetConsoleCursorPosition(hStdOut, homeCoords);

#elif defined(OS_LINUX) || defined(OS_MAC)
    cout << "\033[2J;" << "\033[1;1H"; // Очищает экран и перемещает курсор на домашнюю панель управления в системах POSIX."//
#endif

}


void printGrid(bool gridOne[gridSize + 1][gridSize + 1]) {
    for (int a = 1; a < gridSize; a++)
    {
        for (int b = 1; b < gridSize; b++)
        {
            if (gridOne[a][b] == true)
            {
                cout << " O ";
            }
            else
            {
                cout << " . ";
            }
            if (b == gridSize - 1)
            {
                cout << endl;
            }
        }
    }
}

void compareGrid(bool gridOne[gridSize + 1][gridSize + 1], bool gridTwo[gridSize + 1][gridSize + 1]) {
    for (int a = 0; a < gridSize; a++)
    {
        for (int b = 0; b < gridSize; b++)
        {
            gridTwo[a][b] = gridOne[a][b];
        }
    }
}

void determineState(bool gridOne[gridSize + 1][gridSize + 1]) {
    bool gridTwo[gridSize + 1][gridSize + 1] = {};
    compareGrid(gridOne, gridTwo);

    for (int a = 1; a < gridSize; a++)
    {
        for (int b = 1; b < gridSize; b++)
        {
            int alive = 0;
            for (int c = -1; c < 2; c++)
            {
                for (int d = -1; d < 2; d++)
                {
                    if (!(c == 0 && d == 0))
                    {
                        if (gridTwo[a + c][b + d])
                        {
                            ++alive;
                        }
                    }
                }
            }
            if (alive < 2)
            {
                gridOne[a][b] = false;
            }
            else if (alive == 3)
            {
                gridOne[a][b] = true;
            }
            else if (alive > 3)
            {
                gridOne[a][b] = false;
            }
        }
    }
}