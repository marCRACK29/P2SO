//
// Created by marcosmartinez on 18-09-24.
//

#ifndef MONITOR_H
#define MONITOR_H
#include <queue>
#include <condition_variable>
#include <mutex>
using namespace std;
const int N = 10;  // Tamaño del buffer

class Monitor_prueba{
private:
    queue<int> buffer;
    int contador;
    int in, out;
    mutex mtx;
    condition_variable not_full, not_empty;

public:
    Monitor_prueba();
    void add_entry(int x, void *args);
    void get_entry(int* x, void *args);
};



#endif //MONITOR_H
