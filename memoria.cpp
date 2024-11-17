#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <getopt.h>
#include <queue>
#include <algorithm>

struct espacioMarco
{
    int numeroMarco;
    int espacioUsado;
    bool lleno;
    std::vector<int> marcos;

    // Retornar el numero de indice de un marco no usado si hay alguno, si no retorna -1
    int indiceDisponible()
    {
        if (lleno)
        {
            return -1;
        }
        for (int i = 0; i < numeroMarco; ++i)
        {
            if (marcos[i] == -1)
            {
                return i;
            }
        }
        return -1;
    }

    // Asignar un nuevo npv a marco
    void map(int npv, int indice, bool nuevoEspacioUtilizado)
    {
        marcos[indice] = npv;
        if (nuevoEspacioUtilizado)
        {
            espacioUsado++;
            if (espacioUsado == numeroMarco)
            {
                lleno = true;
            }
        }
    }

    void printfs()
    {
        std::cout << "\nTamaño del marco: " << marcos.size() << std::endl;
        std::cout << "Estado actual del marco: ";
        for (int i = 0; i < espacioUsado; ++i)
        {
            std::cout << marcos[i] << " ";
        }
        std::cout << std::endl;
    }

    espacioMarco(int n)
    {
        this->numeroMarco = n;
        this->lleno = false;
        this->espacioUsado = 0;
        marcos.resize(numeroMarco, -1);
    }
};

struct entradaTablaPagina
{
    int valido;
    int ref;
    int numeroPaginaVirtual;
    int marcoMapeado;

    entradaTablaPagina(int v, int r, int npv, int mf)
    {
        this->valido = v;
        this->ref = r;
        this->numeroPaginaVirtual = npv;
        this->marcoMapeado = mf;
    }
};

struct tablaDePaginaHash
{
    int tamañoTabla;
    std::vector<std::list<entradaTablaPagina>> tabla;

    tablaDePaginaHash(int tamaño)
    {
        tamañoTabla = tamaño;
        tabla.resize(tamaño);
    }

    // Función hash: devuelve el índice basado en el número de página virtual.
    int funcionHash(int numeroPaginaVirtual)
    {
        return numeroPaginaVirtual % tamañoTabla;
    }

    // Insertar una entrada en la tabla hash
    void insertar(int numeroPaginaVirtual, int mp)
    {
        int indice = funcionHash(numeroPaginaVirtual);
        tabla[indice].emplace_back(1, 0, numeroPaginaVirtual, mp); // Crear el objeto directamente en la lista
    }

    // Buscar una entrada por número de página virtual
    entradaTablaPagina *buscar(int numeroPaginaVirtual)
    {
        int indice = funcionHash(numeroPaginaVirtual);
        for (auto &entrada : tabla[indice])
        {
            if (entrada.numeroPaginaVirtual == numeroPaginaVirtual)
            {
                return &entrada;
            }
        }
        return nullptr;
    }

    void printTabla()
    {
        for (int i = 0; i < tamañoTabla; ++i)
        {
            std::cout << "Indice " << i << ": ";
            for (auto &entrada : tabla[i])
            {
                std::cout << "[NPV: " << entrada.numeroPaginaVirtual << ", MF: " << entrada.marcoMapeado << "] ";
            }
            std::cout << std::endl;
        }
    }

    void modificarBitValido(int numeroPaginaVirtual, int nuevoEstado)
    {
        int indice = funcionHash(numeroPaginaVirtual);
        for (auto &entrada : tabla[indice])
        {
            if (entrada.numeroPaginaVirtual == numeroPaginaVirtual)
            {
                entrada.valido = nuevoEstado;
                return;
            }
        }

        std::cerr << "Error: No se encontró la página virtual " << numeroPaginaVirtual << " para modificar el bit válido." << std::endl;
    }
};

void fifo(tablaDePaginaHash &tabla, espacioMarco &marco, const std::vector<int> &referencias)
{
    int fallosPagina = 0; // Contador de fallos de página

    for (size_t i = 0; i < referencias.size(); ++i)
    {
        int ref = referencias[i];

        // Verificar si la página ya está en los marcos
        bool paginaEnMemoria = false;
        for (int j = 0; j < marco.numeroMarco; ++j)
        {
            if (marco.marcos[j] == ref)
            {
                paginaEnMemoria = true;
                break;
            }
        }

        if (paginaEnMemoria)
        {
            // La página ya está en memoria, no hay fallo
            std::cout << "Acceso: " << ref << " | Hit | Estado de marcos: ";
            marco.printfs();
            continue;
        }

        // Fallo de página
        fallosPagina++;
        std::cout << "Acceso: " << ref << " | Miss | ";

        if (marco.lleno)
        {
            // Si los marcos están llenos, eliminar el marco más antiguo
            int paginaARemover = marco.marcos[0];

            // Desplazar todos los marcos hacia la izquierda
            for (int j = 0; j < marco.numeroMarco - 1; ++j)
            {
                marco.marcos[j] = marco.marcos[j + 1];
            }

            // Colocar la nueva página en el último marco
            marco.marcos[marco.numeroMarco - 1] = ref;

            // Actualizar la tabla y marcar la página removida como inválida
            auto entradaARemover = tabla.buscar(paginaARemover);
            if (entradaARemover != nullptr)
            {
                tabla.modificarBitValido(paginaARemover, 0);
            }
            else
            {
                std::cerr << "Advertencia: No se encontró la página virtual " << paginaARemover
                          << " para modificar el bit válido." << std::endl;
            }

            std::cout << "Evict: " << paginaARemover << " | ";
        }
        else
        {
            // Si hay espacio disponible, asignar la nueva página
            int indiceDisponible = marco.indiceDisponible();
            marco.map(ref, indiceDisponible, true);
        }

        // Añadir la nueva página a la tabla
        tabla.insertar(ref, marco.numeroMarco - 1);

        std::cout << "Estado de marcos: \n";
        marco.printfs();
    }

    std::cout << "Fallos de página (FIFO): " << fallosPagina << std::endl;
}
void lru(tablaDePaginaHash &tabla, espacioMarco &marco, const std::vector<int> &referencias)
{
    std::list<int> listaLRU; // Lista para rastrear el uso reciente de páginas
    int fallosPagina = 0;

    for (int ref : referencias)
    {
        auto it = std::find(listaLRU.begin(), listaLRU.end(), ref);

        if (it == listaLRU.end() || tabla.buscar(ref)->valido == 0)
        {
            // Fallo de página
            fallosPagina++;
            std::cout << "Acceso: " << ref << " | Miss\n";

            // Si el marco está lleno, expulsar la página menos recientemente usada
            if (marco.lleno)
            {
                if (!listaLRU.empty())
                {
                    int paginaARemover = listaLRU.back();
                    listaLRU.pop_back();

                    // Actualizar la tabla y liberar el marco
                    tabla.modificarBitValido(paginaARemover, 0);
                    for (int i = 0; i < marco.numeroMarco; ++i)
                    {
                        if (marco.marcos[i] == paginaARemover)
                        {
                            marco.marcos[i] = -1; // Liberar el marco
                            marco.espacioUsado--; // Reducir el espacio usado
                            marco.lleno = false;  // Actualizar estado
                            break;
                        }
                    }

                    std::cout << "Evict: " << paginaARemover << "\n";
                }
                else
                {
                    std::cerr << "Error: Lista LRU vacía al intentar expulsar una página." << std::endl;
                }
            }

            // Insertar la nueva página en el marco
            int indiceDisponible = marco.indiceDisponible();
            if (indiceDisponible == -1)
            {
                std::cerr << "Error: No hay marcos disponibles para la página " << ref << std::endl;
                continue;
            }
            marco.map(ref, indiceDisponible, true); // Asignar página al marco
            tabla.insertar(ref, indiceDisponible);  // Actualizar la tabla hash
        }
        else
        {
            // Si ya está en la lista, moverla al frente
            listaLRU.erase(it);
        }

        listaLRU.push_front(ref); // Agregar o mover la página al frente

        // Imprimir el estado actual de los marcos
        std::cout << "Estado de marcos: ";
        marco.printfs();
    }

    std::cout << "Fallos de página (LRU): " << fallosPagina << std::endl;
}

void lruReloj(tablaDePaginaHash &tabla, espacioMarco &marco, const std::vector<int> &referencias)
{
    std::vector<int> bitsReferencia(marco.numeroMarco, 0); // Bits de referencia para cada marco
    int punteroReloj = 0;                                  // Puntero del reloj
    int fallosPagina = 0;

    auto printEstado = [&]()
    {
        std::cout << "Estado actual de marcos y bits de referencia:\n";
        for (int i = 0; i < marco.numeroMarco; ++i)
        {
            std::cout << "Marco[" << i << "]: " << marco.marcos[i]
                      << " (R=" << bitsReferencia[i] << ")"
                      << (i == punteroReloj ? " <- Puntero" : "") << "\n";
        }
    };

    for (int ref : referencias)
    {
        // Verificar si la página ya está en memoria
        bool paginaEnMemoria = false;
        int marcoIndex = -1;

        for (int i = 0; i < marco.numeroMarco; ++i)
        {
            if (marco.marcos[i] == ref)
            {
                paginaEnMemoria = true;
                marcoIndex = i;
                break;
            }
        }

        if (paginaEnMemoria)
        {
            // Si la página ya está en memoria, actualizar el bit de referencia
            bitsReferencia[marcoIndex] = 1;
            std::cout << "Acceso: " << ref << " | Hit\n";
            printEstado();
            continue;
        }

        // Fallo de página
        fallosPagina++;
        std::cout << "Acceso: " << ref << " | Miss\n";

        if (marco.lleno)
        {
            while (true)
            {
                // Verificar el bit de referencia del marco apuntado por el puntero
                if (bitsReferencia[punteroReloj] == 0)
                {
                    // Encontramos un marco con bit de referencia en 0
                    int paginaARemover = marco.marcos[punteroReloj];
                    std::cout << "Evict: " << paginaARemover << "\n";

                    // Actualizar tabla y liberar marco
                    tabla.modificarBitValido(paginaARemover, 0);
                    marco.marcos[punteroReloj] = ref;                      // Reemplazar página
                    tabla.insertar(ref, punteroReloj);                     // Actualizar tabla
                    bitsReferencia[punteroReloj] = 1;                      // Encender bit de referencia
                    punteroReloj = (punteroReloj + 1) % marco.numeroMarco; // Avanzar el puntero
                    break;
                }
                else
                {
                    // Solo apagar el bit si ya hemos completado una vuelta
                    punteroReloj = (punteroReloj + 1) % marco.numeroMarco;

                    // Si hemos recorrido todos los marcos y no encontramos un bit en 0
                    if (punteroReloj == 0)
                    {
                        std::cout << "Todos los bits estaban en 1, reiniciando bits.\n";
                        for (int &bit : bitsReferencia)
                        {
                            bit = 0;
                        }
                    }
                }
            }
        }
        else
        {
            // Insertar página en un marco disponible
            int indiceDisponible = marco.indiceDisponible();
            if (indiceDisponible != -1)
            {
                marco.map(ref, indiceDisponible, true);
                tabla.insertar(ref, indiceDisponible);
                bitsReferencia[indiceDisponible] = 1;
            }
            else
            {
                std::cerr << "Error: No hay marcos disponibles para la página " << ref << "\n";
            }
        }

        printEstado();
    }

    std::cout << "Fallos de página (LRU Reloj): " << fallosPagina << "\n";
}

void OPTIMO(tablaDePaginaHash &tabla, espacioMarco &marco, const std::vector<int> &referencias)
{
    int fallosPagina = 0;

    for (size_t i = 0; i < referencias.size(); ++i)
    {
        int ref = referencias[i];

        // Verificar si la referencia ya está en memoria y es válida
        auto entrada = tabla.buscar(ref);
        if (entrada != nullptr && entrada->valido == 1)
        {
            // La página ya está en memoria; no es un fallo de página
            std::cout << "Acceso: " << ref << " | Hit\n";
            continue;
        }

        // Fallo de página
        fallosPagina++;
        std::cout << "Acceso: " << ref << " | Miss\n";

        // Si el marco está lleno, buscar la página óptima para reemplazar
        if (marco.lleno)
        {
            int paginaARemover = -1;
            int maxDistancia = -1;

            for (int marcoActual : marco.marcos)
            {
                if (marcoActual == -1)
                    continue; // Ignorar marcos no usados

                auto it = std::find(referencias.begin() + i + 1, referencias.end(), marcoActual);
                int distancia = (it == referencias.end()) ? referencias.size() : (it - referencias.begin());

                if (distancia > maxDistancia)
                {
                    maxDistancia = distancia;
                    paginaARemover = marcoActual;
                }
            }

            // Reemplazar la página seleccionada
            if (paginaARemover != -1)
            {
                std::cout << "Evict: " << paginaARemover << "\n";
                tabla.modificarBitValido(paginaARemover, 0); // Marcar página como inválida
                for (int j = 0; j < marco.numeroMarco; ++j)
                {
                    if (marco.marcos[j] == paginaARemover)
                    {
                        marco.marcos[j] = -1; // Liberar el marco
                        marco.espacioUsado--;
                        marco.lleno = false; // Actualizar el estado de lleno
                        break;
                    }
                }
            }
            else
            {
                std::cerr << "Error: No se encontró una página para reemplazar." << std::endl;
                continue;
            }
        }

        // Insertar la nueva página en el marco disponible
        int indiceDisponible = marco.indiceDisponible();
        if (indiceDisponible != -1)
        {
            marco.map(ref, indiceDisponible, true); // Asignar la nueva página al marco
            tabla.insertar(ref, indiceDisponible);  // Actualizar la tabla hash
        }
        else
        {
            std::cerr << "Error: No hay marcos disponibles para la página " << ref << std::endl;
        }

        // Imprimir el estado actual de los marcos
        std::cout << "Estado de marcos: ";
        marco.printfs();
    }

    std::cout << "Fallos de página (Óptimo): " << fallosPagina << std::endl;
}
void procesarArchivo(const std::string &archivo, std::vector<int> &referencias)
{
    std::ifstream archivoEntrada(archivo);
    if (!archivoEntrada.is_open())
    {
        std::cerr << "Error: No se pudo abrir el archivo " << archivo << std::endl;
        exit(EXIT_FAILURE);
    }

    int referencia;
    while (archivoEntrada >> referencia)
    {
        referencias.push_back(referencia);
    }
    archivoEntrada.close();
}

int main(int argc, char *argv[])
{
    int numeroMarcos = 0;           // Número de marcos iniciales.
    std::string algoritmo;          // Algoritmo de reemplazo.
    std::string archivoReferencias; // Archivo con referencias.
    std::vector<int> referencias;   // Referencias a la memoria.

    // Definir las opciones.
    int opt;
    while ((opt = getopt(argc, argv, "m:a:f:")) != -1)
    {
        switch (opt)
        {
        case 'm':
            numeroMarcos = std::stoi(optarg);
            break;
        case 'a':
            algoritmo = optarg;
            break;
        case 'f':
            archivoReferencias = optarg;
            break;
        default:
            std::cerr << "Uso: ./simulador -m <marcos> -a <algoritmo> -f <archivo>" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Validar los parámetros.
    if (numeroMarcos <= 0 || algoritmo.empty() || archivoReferencias.empty())
    {
        std::cerr << "Error: Parámetros incompletos o incorrectos." << std::endl;
        std::cerr << "Uso: ./simulador -m <marcos> -a <algoritmo> -f <archivo>" << std::endl;
        return EXIT_FAILURE;
    }

    // Procesar archivo de referencias.
    procesarArchivo(archivoReferencias, referencias);

    // Mostrar parámetros leídos.
    std::cout << "Número de marcos: " << numeroMarcos << std::endl;
    std::cout << "Algoritmo seleccionado: " << algoritmo << std::endl;
    std::cout << "Referencias: ";
    for (int ref : referencias)
    {
        std::cout << ref << " ";
    }
    std::cout << std::endl;

    tablaDePaginaHash tabla(numeroMarcos);
    espacioMarco marcos(numeroMarcos);

    // Llamar al algoritmo seleccionado.
    if (algoritmo == "FIFO")
    {
        fifo(tabla, marcos, referencias);
    }
    else if (algoritmo == "LRU")
    {
        lru(tabla, marcos, referencias);
    }
    else if (algoritmo == "OPTIMO")
    {
        OPTIMO(tabla, marcos, referencias);
    }
    else if (algoritmo == "LRUR")
    {
        lruReloj(tabla, marcos, referencias);
        std::cout << "Estado final de los marcos:" << std::endl;
        marcos.printfs();

        std::cout << "Estado final de la tabla de páginas:" << std::endl;
        tabla.printTabla();
    }
    else
    {
        std::cerr << "Error: Algoritmo no reconocido. Use FIFO, LRU, LRUR o OPTIMO." << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
