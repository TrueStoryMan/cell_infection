#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //розумні дядьки зробили це, щоб змінювати кольори

struct Info {
    char Graphics;
    int immunDays;
    int infDays;
};

void Choice(struct Info* arr[]) {
    int amount, xInf, yInf;
    cout << "Введіть кількість початково-заражених клітин:\n";
    cin >> amount;
    for (int i = 1; i <= amount; i++) {
        cout << "Введіть координати клітини №" << i << endl;
        cin >> xInf >> yInf;
        arr[xInf-1][yInf-1].Graphics = 'F';
        arr[xInf-1][yInf-1].infDays = 0;
    }
}

int Random() {
    return rand() %101;
}

void Immunity(int x, int y, struct Info* arr[]) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            if (arr[i][j].Graphics == 'I' && arr[i][j].immunDays == 4) {
                arr[i][j].Graphics = 'H'; //якщо імунітет пройшов, клітина стає доступною для зараження
                arr[i][j].immunDays = 0;
            }
            if (arr[i][j].Graphics == 'F' && arr[i][j].infDays == 5) {
                arr[i][j].Graphics = 'I'; //після 5 днів, заражена клітина отримує імунітет
                arr[i][j].infDays = 0;
                arr[i][j].immunDays = 0;
            }
        }
    }
    for (int i = 0; i < x; i++) { 
        for (int j = 0; j < y; j++) {
            arr[i][j].immunDays++; //в кінці циклу додаються дні в історію імунітету
        }
    }
}

void Infection(int x, int y, struct Info* arr[]) {
    srand(time(NULL));
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            if (arr[i][j].Graphics == 'F' && arr[i][j].infDays > 0) {
                if (Random() < 50 && i - 1 >= 0) { //50% шанс зараження верхнього сусіда
                    if (arr[i - 1][j].Graphics == 'H') {
                        arr[i - 1][j].Graphics = 'F';
                        arr[i - 1][j].infDays = 0;
                    }
                }
                if (Random() < 50 && (i + 1) != x) { //50% шанс зараження нижнього сусіда
                    if (arr[i + 1][j].Graphics == 'H') {
                        arr[i + 1][j].Graphics = 'F';
                        arr[i + 1][j].infDays = 0;
                    }
                }
                if (Random() < 50 && j - 1 >= 0) { //50% шанс зараження лівого сусіда
                    if (arr[i][j - 1].Graphics == 'H') {
                        arr[i][j - 1].Graphics = 'F';
                        arr[i][j - 1].infDays = 0;
                    }
                }
                if (Random() < 50 && (j + 1) != y) { //50% шанс зараження правого сусіда
                    if (arr[i][j + 1].Graphics == 'H') {
                        arr[i][j + 1].Graphics = 'F';
                        arr[i][j + 1].infDays = 0;
                    }
                }
            }
        }
    }
    for (int i = 0; i < x; i++) { 
        for (int j = 0; j < y; j++) {
            if (arr[i][j].Graphics == 'F')
                arr[i][j].infDays++; //в кінці циклу додаються дні в історію хвороби
        }
    }
}

void Draw(int x, int y, struct Info* arr[]) { //виводить матрицю клітин
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            if (arr[i][j].Graphics == 'H') {
                SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 10)); //зелений колір
                cout << arr[i][j].Graphics << " "; 
            }
            else if (arr[i][j].Graphics == 'F') {
                SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 12)); //червоний колір
                cout << arr[i][j].Graphics << " ";
            }
            else {
                SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 9)); //
                cout << arr[i][j].Graphics << " ";
            }
        }
        cout << endl;
    }
}

bool healthCheck(int x, int y, struct Info* arr[]){
    bool allHealthy = true;
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            if (arr[i][j].Graphics == 'F' || arr[i][j].Graphics == 'I')
                allHealthy = false;
        }
    }
    return allHealthy;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int time, x, y;
    cout << "Введіть кількість ітерацій...\n";
    cin >> time;
    cout << "Введіть розмір матриці...\n";
    cin >> x >> y;
    Info** arr = new Info * [x]; //створення двовимірного динамічного масиву структур
    for (int i = 0; i < x; i++)
        arr[i] = new Info[y];
    
    
    //SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 4));
    for (int i = 0; i < x; i++) { //заповнення "здоровими клітинами"
        for (int j = 0; j < y; j++) {
            arr[i][j].Graphics = 'H';
        }
    }
    Choice(arr);
    for (int i = 1; i <= time; i++) {
        Immunity(x, y, arr);
        Infection(x, y, arr);
        SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15)); //білий колір
        cout << "Стан клітин на " << i << " ітерації:" << endl;
        Draw(x, y, arr);
        Sleep(500);
        if (healthCheck(x, y, arr)) {
            SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));
            cout << "Вже всі здорові!!!\n";
            break;
        }
        system("cls"); //очищення консолі
    }
}

