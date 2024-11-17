#include "Monitor.h"
#include <thread>
#include <chrono>
#include <cstdlib>
using namespace std;

/**
 * g++ main.cpp Monitor.cpp -o simulapc
 * ./simulapc -p 5 -c 5 -s 10 -t 5
 */
void productor(Monitor &monitor, int id) { /* Función que simula las hebras productas */
    for (int i = 0; i < 20; ++i) {
        monitor.agregar(rand() % 100, id); /* Agregar un item aleatorio al buffer */
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void consumidor(Monitor &monitor, int id, int timeout) { /* Función que simula las hebras consumidoras */
    while(true) {
        int item;
        if(!monitor.sacar(item, id, timeout)) { /* Si no se pudo sacar el item */
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main(int argc, char **argv) {
    // Verificación del número de argumentos
    if (argc != 9) {
        cerr << "Uso incorrecto: ./simulapc -p <num_productores> -c <num_consumidores> -s <tam_inicial> -t <tiempo_espera>" << endl;
        return 1;
    }
    /* Obtener los argumentos de la línea de comandos */
    int num_productores = atoi(argv[2]);
    int num_consumidores = atoi(argv[4]);
    int tam_inicial = atoi(argv[6]);
    int tiempo_espera = atoi(argv[8]);

    /* Crear el monitor e iniciar hebras productoras y consumidoras */
    Monitor monitor(tam_inicial);
    vector<thread> productores, consumidores;

    for(int i = 0; i < num_productores; ++i) { /* Crear las hebras productoras */
        productores.push_back(thread(productor, ref(monitor), i));
    }

    for(int i = 0; i < num_consumidores; ++i) { /* Crear las hebras consumidoras */
        consumidores.push_back(thread(consumidor, ref(monitor), i, tiempo_espera));
    }

    for(auto &p : productores) p.join(); /* Esperar a que todas las hebras productoras terminen */
    for(auto &c : consumidores) c.join(); /* Esperar a que todas las hebras consumidoras terminen */

    return 0;
}
