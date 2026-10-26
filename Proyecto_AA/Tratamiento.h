
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;


struct NodoTratamiento
{
    int IdTratamiento;
    string Nombre;
    string Descripcion;
    double Costo;

    bool rojo;

    NodoTratamiento* Padre;
    NodoTratamiento* Hijoizq;
    NodoTratamiento* Hijoder;

    NodoTratamiento(int id, string nombre, string descripcion, double costo)
    {
        IdTratamiento = id;
        Nombre = nombre;
        Descripcion = descripcion;
        Costo = costo;

        rojo = true;

        Padre = nullptr;
        Hijoizq = nullptr;
        Hijoder = nullptr;
    }
};



class ArbolRNTratamientos
{
private:

    NodoTratamiento* primero;



    NodoTratamiento* BuscarNodo(NodoTratamiento* nodo, int id)
    {
        if (nodo == nullptr)
            return nullptr;

        if (id == nodo->IdTratamiento)
            return nodo;

        if (id < nodo->IdTratamiento)
            return BuscarNodo(nodo->Hijoizq, id);

        return BuscarNodo(nodo->Hijoder, id);
    }



    void RotarIzquierda(NodoTratamiento* nodo)
    {
        NodoTratamiento* derecho = nodo->Hijoder;

        nodo->Hijoder = derecho->Hijoizq;

        if (derecho->Hijoizq != nullptr)
            derecho->Hijoizq->Padre = nodo;

        derecho->Padre = nodo->Padre;

        if (nodo->Padre == nullptr)
            primero = derecho;
        else if (nodo == nodo->Padre->Hijoizq)
            nodo->Padre->Hijoizq = derecho;
        else
            nodo->Padre->Hijoder = derecho;

        derecho->Hijoizq = nodo;
        nodo->Padre = derecho;
    }



    void RotarDerecha(NodoTratamiento* nodo)
    {
        NodoTratamiento* izquierdo = nodo->Hijoizq;

        nodo->Hijoizq = izquierdo->Hijoder;

        if (izquierdo->Hijoder != nullptr)
            izquierdo->Hijoder->Padre = nodo;

        izquierdo->Padre = nodo->Padre;

        if (nodo->Padre == nullptr)
            primero = izquierdo;
        else if (nodo == nodo->Padre->Hijoder)
            nodo->Padre->Hijoder = izquierdo;
        else
            nodo->Padre->Hijoizq = izquierdo;

        izquierdo->Hijoder = nodo;
        nodo->Padre = izquierdo;
    }


    void BalancearInsercion(NodoTratamiento* nodo)
    {
        while (nodo != primero &&
            nodo->Padre != nullptr &&
            nodo->Padre->rojo == true)
        {
            NodoTratamiento* padre = nodo->Padre;
            NodoTratamiento* abuelo = padre->Padre;

            if (padre == abuelo->Hijoizq)
            {
                NodoTratamiento* tio = abuelo->Hijoder;

                if (tio != nullptr && tio->rojo == true)
                {
                    padre->rojo = false;
                    tio->rojo = false;
                    abuelo->rojo = true;
                    nodo = abuelo;
                }
                else
                {
                    if (nodo == padre->Hijoder)
                    {
                        nodo = padre;
                        RotarIzquierda(nodo);
                    }

                    nodo->Padre->rojo = false;
                    nodo->Padre->Padre->rojo = true;

                    RotarDerecha(nodo->Padre->Padre);
                }
            }
            else
            {
                NodoTratamiento* tio = abuelo->Hijoizq;

                if (tio != nullptr && tio->rojo == true)
                {
                    padre->rojo = false;
                    tio->rojo = false;
                    abuelo->rojo = true;
                    nodo = abuelo;
                }
                else
                {
                    if (nodo == padre->Hijoizq)
                    {
                        nodo = padre;
                        RotarDerecha(nodo);
                    }

                    nodo->Padre->rojo = false;
                    nodo->Padre->Padre->rojo = true;

                    RotarIzquierda(nodo->Padre->Padre);
                }
            }
        }

        primero->rojo = false;
    }

    NodoTratamiento* Minimo(NodoTratamiento* nodo)
    {
        while (nodo != nullptr && nodo->Hijoizq != nullptr)
            nodo = nodo->Hijoizq;

        return nodo;
    }

    
    void Trasplantar(NodoTratamiento* viejo, NodoTratamiento* nuevo)
    {
        if (viejo->Padre == nullptr)
            primero = nuevo;
        else if (viejo == viejo->Padre->Hijoizq)
            viejo->Padre->Hijoizq = nuevo;
        else
            viejo->Padre->Hijoder = nuevo;

        if (nuevo != nullptr)
            nuevo->Padre = viejo->Padre;
    }


    void BalancearEliminacion(
        NodoTratamiento* nodo,
        NodoTratamiento* padre)
    {
        while (nodo != primero &&
            (nodo == nullptr || nodo->rojo == false))
        {
            if (padre == nullptr)
                break;

            if (nodo == padre->Hijoizq)
            {
                NodoTratamiento* hermano = padre->Hijoder;

                if (hermano == nullptr)
                {
                    nodo = padre;
                    padre = nodo->Padre;
                    continue;
                }

                if (hermano->rojo == true)
                {
                    hermano->rojo = false;
                    padre->rojo = true;
                    RotarIzquierda(padre);
                    hermano = padre->Hijoder;
                }

                bool izquierdoNegro =
                    hermano->Hijoizq == nullptr ||
                    hermano->Hijoizq->rojo == false;

                bool derechoNegro =
                    hermano->Hijoder == nullptr ||
                    hermano->Hijoder->rojo == false;

                if (izquierdoNegro && derechoNegro)
                {
                    hermano->rojo = true;
                    nodo = padre;
                    padre = nodo->Padre;
                }
                else
                {
                    if (derechoNegro)
                    {
                        if (hermano->Hijoizq != nullptr)
                            hermano->Hijoizq->rojo = false;

                        hermano->rojo = true;
                        RotarDerecha(hermano);
                        hermano = padre->Hijoder;
                    }

                    hermano->rojo = padre->rojo;
                    padre->rojo = false;

                    if (hermano->Hijoder != nullptr)
                        hermano->Hijoder->rojo = false;

                    RotarIzquierda(padre);

                    nodo = primero;
                    padre = nullptr;
                }
            }
            else
            {
                NodoTratamiento* hermano = padre->Hijoizq;

                if (hermano == nullptr)
                {
                    nodo = padre;
                    padre = nodo->Padre;
                    continue;
                }

                if (hermano->rojo == true)
                {
                    hermano->rojo = false;
                    padre->rojo = true;
                    RotarDerecha(padre);
                    hermano = padre->Hijoizq;
                }

                bool izquierdoNegro =
                    hermano->Hijoizq == nullptr ||
                    hermano->Hijoizq->rojo == false;

                bool derechoNegro =
                    hermano->Hijoder == nullptr ||
                    hermano->Hijoder->rojo == false;

                if (izquierdoNegro && derechoNegro)
                {
                    hermano->rojo = true;
                    nodo = padre;
                    padre = nodo->Padre;
                }
                else
                {
                    if (izquierdoNegro)
                    {
                        if (hermano->Hijoder != nullptr)
                            hermano->Hijoder->rojo = false;

                        hermano->rojo = true;
                        RotarIzquierda(hermano);
                        hermano = padre->Hijoizq;
                    }

                    hermano->rojo = padre->rojo;
                    padre->rojo = false;

                    if (hermano->Hijoizq != nullptr)
                        hermano->Hijoizq->rojo = false;

                    RotarDerecha(padre);

                    nodo = primero;
                    padre = nullptr;
                }
            }
        }

        if (nodo != nullptr)
            nodo->rojo = false;
    }


    void MostrarRecursivo(NodoTratamiento* nodo)
    {
        if (nodo == nullptr)
            return;

        MostrarRecursivo(nodo->Hijoizq);

        cout << "IdTratamiento: " << nodo->IdTratamiento
             << " | Nombre: " << nodo->Nombre
             << " | Descripcion: " << nodo->Descripcion
             << " | Costo: " << fixed << setprecision(2)
             << nodo->Costo << endl;

        MostrarRecursivo(nodo->Hijoder);
    }



    void Destruir(NodoTratamiento* nodo)
    {
        if (nodo == nullptr)
            return;

        Destruir(nodo->Hijoizq);
        Destruir(nodo->Hijoder);

        delete nodo;
    }

public:


    ArbolRNTratamientos()
    {
        primero = nullptr;
    }

    ~ArbolRNTratamientos()
    {
        Destruir(primero);
    }



    bool ListaVacia()
    {
        return primero == nullptr;
    }



    bool Existe(int IdTratamiento)
    {
        return BuscarNodo(primero, IdTratamiento) != nullptr;
    }



    bool Insertar(
        int IdTratamiento,
        string Nombre,
        string Descripcion,
        double Costo)
    {
        if (Existe(IdTratamiento))
            return false;

        NodoTratamiento* nuevo = new NodoTratamiento(
            IdTratamiento, Nombre, Descripcion, Costo);

        NodoTratamiento* padre = nullptr;
        NodoTratamiento* actual = primero;

        while (actual != nullptr)
        {
            padre = actual;

            if (IdTratamiento < actual->IdTratamiento)
                actual = actual->Hijoizq;
            else
                actual = actual->Hijoder;
        }

        nuevo->Padre = padre;

        if (padre == nullptr)
            primero = nuevo;
        else if (IdTratamiento < padre->IdTratamiento)
            padre->Hijoizq = nuevo;
        else
            padre->Hijoder = nuevo;

        BalancearInsercion(nuevo);

        return true;
    }



    bool Buscar(int IdTratamiento)
    {
        NodoTratamiento* nodo = BuscarNodo(primero, IdTratamiento);

        if (nodo == nullptr)
            return false;

        cout << "IdTratamiento: " << nodo->IdTratamiento << endl;
        cout << "Nombre: " << nodo->Nombre << endl;
        cout << "Descripcion: " << nodo->Descripcion << endl;
        cout << "Costo: " << fixed << setprecision(2)
             << nodo->Costo << endl;

        return true;
    }

    bool Modificar(
        int IdTratamiento,
        string Nombre,
        string Descripcion,
        double Costo)
    {
        NodoTratamiento* nodo = BuscarNodo(primero, IdTratamiento);

        if (nodo == nullptr)
            return false;

        nodo->Nombre = Nombre;
        nodo->Descripcion = Descripcion;
        nodo->Costo = Costo;

        return true;
    }
    bool Eliminar(int IdTratamiento)
    {
        NodoTratamiento* eliminado =
            BuscarNodo(primero, IdTratamiento);

        if (eliminado == nullptr)
            return false;

        NodoTratamiento* reemplazo = eliminado;
        NodoTratamiento* nodoBalancear = nullptr;
        NodoTratamiento* padreBalancear = nullptr;

        bool colorOriginal = reemplazo->rojo;

        if (eliminado->Hijoizq == nullptr)
        {
            nodoBalancear = eliminado->Hijoder;
            padreBalancear = eliminado->Padre;

            Trasplantar(eliminado, eliminado->Hijoder);

            if (nodoBalancear != nullptr)
                padreBalancear = nodoBalancear->Padre;
        }
        else if (eliminado->Hijoder == nullptr)
        {
            nodoBalancear = eliminado->Hijoizq;
            padreBalancear = eliminado->Padre;

            Trasplantar(eliminado, eliminado->Hijoizq);

            if (nodoBalancear != nullptr)
                padreBalancear = nodoBalancear->Padre;
        }
        else
        {
            reemplazo = Minimo(eliminado->Hijoder);
            colorOriginal = reemplazo->rojo;
            nodoBalancear = reemplazo->Hijoder;

            if (reemplazo->Padre == eliminado)
            {
                padreBalancear = reemplazo;

                if (nodoBalancear != nullptr)
                    nodoBalancear->Padre = reemplazo;
            }
            else
            {
                padreBalancear = reemplazo->Padre;

                Trasplantar(reemplazo, reemplazo->Hijoder);

                reemplazo->Hijoder = eliminado->Hijoder;
                reemplazo->Hijoder->Padre = reemplazo;
            }

            Trasplantar(eliminado, reemplazo);

            reemplazo->Hijoizq = eliminado->Hijoizq;
            reemplazo->Hijoizq->Padre = reemplazo;
            reemplazo->rojo = eliminado->rojo;
        }

        delete eliminado;

        if (colorOriginal == false)
            BalancearEliminacion(nodoBalancear, padreBalancear);

        return true;
    }

    void Mostrar()
    {
        if (primero == nullptr)
        {
            cout << "No hay tratamientos registrados." << endl;
            return;
        }

        cout << endl << "TRATAMIENTOS (ARBOL ROJO-NEGRO)" << endl;
        cout << "--------------------------------" << endl;

        MostrarRecursivo(primero);
    }

    void MostrarTratamientos()
    {
        Mostrar();
    }
};


inline void CargarTratamientos(ArbolRNTratamientos& Tratamientos)
{
    ifstream archivo("Tratamiento.csv");

    if (!archivo)
    {
        cout << "No se pudo abrir el archivo Tratamiento.csv" << endl;
        return;
    }

    string encabezado;
    getline(archivo, encabezado);

    string idTexto;
    string Nombre;
    string Descripcion;
    string costoTexto;

    int cargados = 0;

    while (getline(archivo, idTexto, ';') &&
           getline(archivo, Nombre, ';') &&
           getline(archivo, Descripcion, ';') &&
           getline(archivo, costoTexto))
    {
        try
        {
            int IdTratamiento = stoi(idTexto);
            double Costo = stod(costoTexto);

            if (Tratamientos.Insertar(
                IdTratamiento, Nombre, Descripcion, Costo))
            {
                cargados++;
            }
        }
        catch (...)
        {
            cout << "Se omitio una fila invalida de Tratamiento.csv."
                 << endl;
        }
    }

    archivo.close();

    cout << "Tratamientos cargados: " << cargados << endl;
}
