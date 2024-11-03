#include "Monitor_prueba.h"
#include <iostream>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>
using namespace std;

// Constructor
Monitor_prueba::Monitor_prueba(): contador(0), in(0), out(0) {}

//Procedimiento para agregar un item al buffer
void Monitor_prueba::add_entry(int x, void *args) {
    int *prod = (int *)args;
    int num = *prod;
    unique_lock<mutex> lock(mtx);

    // Esperar mientras el buffer esta lleno
    not_full.wait(lock, [this](){ return buffer.size() < N; });

    //Agregar el item al buffer
    buffer.push(x);
    in = (in + 1) % N;
    contador++;

    cout<< "Productor " << num << " produciendo: " << x<< " en in " << in << " Contador " << contador << endl;

    //Notificar que hay un nuevo elemento disponible
    not_empty.notify_one();
}

//Procedimiento para sacar un item del buffer
void Monitor_prueba::get_entry(int *x, void *args) {
    int *cons = (int *)args;
    int num = *cons;
    unique_lock<mutex> lock(mtx);

    //Esperar mientras el buffer esta vacío
    not_empty.wait(lock, [this](){ return buffer.size() > 0; });

    //Obtener el ítem del buffer
    *x = buffer.front();
    buffer.pop();
    out = (out + 1) % N;
    contador--;

    cout<< "Consumidor " << num << " consumiendo: " << *x<< " en out " << out << " Contador " << contador << endl;

    //Notificar que hay un espacio disponible para rellenar
    not_full.notify_one();
}

