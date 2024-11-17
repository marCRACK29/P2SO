#include "Monitor.h"

using namespace std;

Monitor::Monitor(int size_inicial) : buffer(size_inicial), in(0), out(0), contador(0), capacidad(size_inicial) {
    logfile.open("log.txt");
    logfile << "Log de ejecución" << endl;
}

Monitor::~Monitor() {
    logfile.close();
}

void Monitor::resize_buffer(bool expand) {
    int nueva_capacidad;
    {
        unique_lock lock(mtx);
        if(expand)
            nueva_capacidad = 2 * capacidad;
        else
            nueva_capacidad = max(4, capacidad / 2);

        capacidad = nueva_capacidad;
        logfile << (expand ? "Ampliando" : "Reduciendo") << " el tamaño de la cola a " << capacidad << " elementos" << endl;
        /* Simplemente para mostrar en consola lo que esta en el log */
        printf("%s el tamaño de la cola a %d elementos\n", (expand ? "Ampliando" : "Reduciendo"), capacidad);
    }

    buffer.resize(nueva_capacidad);
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
    printf("Productor %d produjo: %d | Elementos en cola: %d\n", id_productor, item, contador);

    if (contador == capacidad) {
        lock.unlock();
        resize_buffer(true); /* Ampliar el buffer si está lleno */
    } else {
        not_empty.notify_all(); /* Notificar que hay un nuevo elemento disponible */
    }
}

bool Monitor::sacar(int &item, int id_consumidor, int timeout) {
    unique_lock lock(mtx);
    if(!not_empty.wait_for(lock, chrono::seconds(timeout), [this](){ return contador > 0; })) {
        logfile << "Consumidor " << id_consumidor << " no pudo consumir: timeout" << endl;
        printf("Consumidor %d no pudo consumir: timeout\n", id_consumidor);
        return false; /* timeout alcanzado */
    }

    item = buffer[out];
    out = (out + 1) % capacidad;
    contador--;

    logfile << "Consumidor " << id_consumidor << " consumió: " << item << " | Elementos en cola: " << contador << endl;
    printf("Consumidor %d consumió: %d | Elementos en cola: %d\n", id_consumidor, item, contador);

    if(contador <= capacidad / 4 && capacidad > 4) {
        lock.unlock();
        resize_buffer(false); /* Reducir el buffer a la mitad */
    } else {
        not_full.notify_all(); /* Notificar que hay un espacio disponible para rellenar */
    }
    return true;
}




