#include "Monitor.h"

#include <algorithm>
using namespace std;

Monitor::Monitor(int size_inicial) : buffer(size_inicial), in(0), out(0), contador(0), capacidad(size_inicial) {
    logfile.open("log.txt");
    logfile << "Log de ejecución" << endl;
}

Monitor::~Monitor() {
    logfile.close();
}

void Monitor::resize_buffer(bool expand) {
    unique_lock lock(mtx); /* Bloquear el mutex para evitar condiciones de carrera */
    int nueva_capacidad;

    if(expand)
        nueva_capacidad = 2 * capacidad;
    else
        nueva_capacidad = capacidad / 2;

    buffer.resize(nueva_capacidad);
    capacidad = nueva_capacidad;
    logfile << (expand ? "Ampliando" : "Reduciendo") << " el tamaño de la cola a " << capacidad << " elementos" << endl;
}

void Monitor::agregar(int item, int id_productor) {
    unique_lock lock(mtx);
    not_full.wait(lock, [this](){ return contador < capacidad; });  /* Esperar mientras el buffer esta lleno */

    /* Agregar el item al buffer */
    /* Tal como aparece en las transparencias */
    buffer[in] = item;
    in = (in + 1) % capacidad;
    contador++;

    logfile << "Productor " << id_productor << " produjo: " << item << " | Elementos en cola: " << contador << endl;

    if (contador == capacidad)
        resize_buffer(true); /* Ampliar el buffer si está lleno */

    not_empty.notify_all(); /* Notificar que hay un nuevo elemento disponible */
}

bool Monitor::sacar(int &item, int id_consumidor, int timeout) {
    unique_lock lock(mtx);
    if(!not_empty.wait_for(lock, chrono::seconds(timeout), [this](){ return contador > 0; })) {
        return false; /* timeout alcanzado */
    }

    item = buffer[out];
    out = (out + 1) % capacidad;
    contador--;

    logfile << "Consumidor " << id_consumidor << " consumió: " << item << " | Elementos en cola: " << contador << endl;

    if(contador <= capacidad / 4 && capacidad > 4)
        resize_buffer(false); /* Reducir el buffer a la mitad */

    not_full.notify_all(); /* Notificar que hay un espacio disponible para rellenar */
    return true;
}




