#include "Monitor_prueba.h"
#include <thread>
using namespace std;

//Para compilar: g++ using_monitor.cpp Monitor_prueba.cpp -o using_monitor

void productor(Monitor_prueba& monitor, void *id) {
    for (int i = 0; i < 20; ++i) {
        monitor.add_entry(i, id);
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}

void consumidor(Monitor_prueba& monitor, void *id) {
    for (int i = 0; i < 20; ++i) {
        int x;
        monitor.get_entry(&x, id);
        this_thread::sleep_for(chrono::milliseconds(1500));
    }
}

int main() {
    Monitor_prueba monitor;

    int p1 = 0;
    int p2 = 1;
    int c1 = 0;
    int c2 = 1;
    thread pid1(productor, ref(monitor), &p1);
    thread pid2(productor, ref(monitor), &p2);
    thread cid1(consumidor, ref(monitor), &c1);
    thread cid2(consumidor, ref(monitor), &c2);

    pid1.join();
    pid2.join();
    cid1.join();
    cid2.join();

    return 0;
}
