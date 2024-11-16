#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

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
            if (marcos[i] == 1)
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
        std::cout << "Tamaño del marco: " << marcos.size() << std::endl;
        std::cout << "Tamaño del marco actual: ";
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
        marcos.resize(numeroMarco);
        for (int i = 0; i < numeroMarco; ++i)
        {
            marcos[i] = -1;
        }
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
};

void fifo()
{
}

void lru()
{
}

void optimal()
{
}

int main(int argc, char *argv[])
{
}
