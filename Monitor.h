
#ifndef MONITOR_H
#define MONITOR_H

#include <iostream>
#include <vector>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <fstream>

class Monitor {
private:
    std::vector<int> buffer; /* Buffer que almacena los elementos */
    int in, out, contador; /* Variables de control (meter, sacar, incremento o decremento del stock) */
    int capacidad;
    std::mutex mtx; /* Mutex para controlar el acceso a las variables */
    std::condition_variable not_full, not_empty; /* Variables de condición para controlar la sincronización */
    std::ofstream logfile; /* Archivo de salida para escribir los resultados */

public:
    Monitor(int size_inicial);
    ~Monitor();
    void agregar(int item, int id_productor);
    bool sacar(int &item, int id_consumidor, int timeout);
    void resize_buffer(bool expand);
};

#endif //MONITOR_H
