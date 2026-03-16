#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
using namespace std;

bool eventWithProbability(int percent) {
    static mt19937 gen(std::random_device{}());
    static uniform_int_distribution<int> dist(1, 100);

    return dist(gen) <= percent;
}

// Функция для очистки экрана (зависит от ОС)
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void displaySimulation(const vector<int>& queueLengths, int time, int servedClients) {
    clearScreen();
    std::cout << "=== СИМУЛЯЦИЯ МАГАЗИНА (Время: " << time << " мин) ===\n\n";

    for (size_t i = 0; i < queueLengths.size(); ++i) {
        cout << "Касса " << (i + 1) << ": [";
        for (int j = 0; j < queueLengths[i]; ++j) {
            cout << "#";
        }
        for (int j = queueLengths[i]; j < 10; ++j) { // Рисуем до 10 мест
            cout << ".";
        }
        cout << "] " << queueLengths[i] << " клиентов\n";
    }
    cout << "\nОбслужено всего: " << servedClients << " клиентов\n";
}

void checkout(vector<int>& queues, int& totalServed) {
    if (eventWithProbability(40) && queues[0]>0) { queues[0] -= 1; totalServed += 1; }
    if (eventWithProbability(40) &&queues[1] > 0) { queues[1] -= 1; totalServed += 1; }
    if (eventWithProbability(40) && queues[2] > 0) { queues[2] -= 1; totalServed += 1; }
}
void queuesnew(vector<int>& queues){
    int min = 0;
    for (int i = 0; i < queues.size(); i++) {
        if (queues[i] < queues[min]) { min = i; }
    }
    queues[min] += 1;
}

int main() {
    
    vector<int> queues = { 0, 0, 0 }; // Длины очередей у 3 касс
    int currentTime = 0;
    int totalServed = 0;
    
    setlocale(LC_ALL, "Russian");
    for (int step = 0; step < 50; ++step) {
        queuesnew(queues);
        if (eventWithProbability(20)) { queuesnew(queues); }
        displaySimulation(queues, currentTime, totalServed);
        checkout(queues, totalServed);
        this_thread::sleep_for(chrono::milliseconds(700)); // Ждем 0.7 сек
        currentTime++;
    }

    return 0;
}