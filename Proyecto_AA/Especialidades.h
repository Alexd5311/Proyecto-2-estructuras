#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include "Comun_PDF.h"

using namespace std;

class NodoEspecialidad
{
public:
    int IdEspecialidad;
    string Nombre;
    string Descripcion;

    NodoEspecialidad* izq;
    NodoEspecialidad* der;
    int nivel;

    NodoEspecialidad(int id, string nombre, string descripcion)
    {
        IdEspecialidad = id;
        Nombre = nombre;
        Descripcion = descripcion;
        izq = nullptr;
        der = nullptr;
        nivel = 1;
    }
};

class ArbolAAEspecialidades
{
private:
    NodoEspecialidad* raiz;

    NodoEspecialidad* Giro(NodoEspecialidad* nodo)
    {
        if (nodo == nullptr || nodo->izq == nullptr)
            return nodo;

        if (nodo->izq->nivel == nodo->nivel)
        {
            NodoEspecialidad* temp = nodo->izq;
            nodo->izq = temp->der;
            temp->der = nodo;
            return temp;
        }

        return nodo;
    }

    NodoEspecialidad* Reparto(NodoEspecialidad* nodo)
    {
        if (nodo == nullptr || nodo->der == nullptr ||
            nodo->der->der == nullptr)
            return nodo;

        if (nodo->nivel == nodo->der->der->nivel)
        {
            NodoEspecialidad* temp = nodo->der;
            nodo->der = temp->izq;
            temp->izq = nodo;
            temp->nivel++;
            return temp;
        }

        return nodo;
    }

    NodoEspecialidad* BuscarRec(NodoEspecialidad* nodo, int id)
    {
        while (nodo != nullptr)
        {
            if (id == nodo->IdEspecialidad)
                return nodo;

            if (id < nodo->IdEspecialidad)
                nodo = nodo->izq;
            else
                nodo = nodo->der;
        }

        return nullptr;
    }

    NodoEspecialidad* InsertarRec(
        NodoEspecialidad* nodo, int id,
        string nombre, string descripcion)
    {
        if (nodo == nullptr)
            return new NodoEspecialidad(id, nombre, descripcion);

        if (id < nodo->IdEspecialidad)
            nodo->izq = InsertarRec(nodo->izq, id, nombre, descripcion);
        else if (id > nodo->IdEspecialidad)
            nodo->der = InsertarRec(nodo->der, id, nombre, descripcion);
        else
            return nodo;

        nodo = Giro(nodo);
        nodo = Reparto(nodo);

        return nodo;
    }

    void Destruir(NodoEspecialidad* nodo)
    {
        if (nodo == nullptr)
            return;

        Destruir(nodo->izq);
        Destruir(nodo->der);
        delete nodo;
    }

    void MostrarRecursivo(NodoEspecialidad* nodo)
    {
        if (nodo == nullptr)
            return;

        MostrarRecursivo(nodo->izq);

        std::cout << "IdEspecialidad: " << nodo->IdEspecialidad
            << " | Nombre: " << nodo->Nombre
            << " | Descripcion: " << nodo->Descripcion
            << std::endl;

        MostrarRecursivo(nodo->der);
    }

    void ReportePDFRec(NodoEspecialidad* nodo, GeneradorPDF& pdf)
    {
        if (nodo == nullptr)
            return;

        ReportePDFRec(nodo->izq, pdf);

        pdf.Linea(Rellenar(to_string(nodo->IdEspecialidad), 8)
            + Rellenar(nodo->Nombre, 25)
            + Rellenar(nodo->Descripcion, 85));

        ReportePDFRec(nodo->der, pdf);
    }

    NodoEspecialidad* Minimo(NodoEspecialidad* nodo)
    {
        while (nodo != nullptr && nodo->izq != nullptr)
            nodo = nodo->izq;

        return nodo;
    }

    NodoEspecialidad* EliminarRec(NodoEspecialidad* nodo, int id)
    {
        if (nodo == nullptr)
            return nullptr;

        if (id < nodo->IdEspecialidad)
            nodo->izq = EliminarRec(nodo->izq, id);
        else if (id > nodo->IdEspecialidad)
            nodo->der = EliminarRec(nodo->der, id);
        else
        {
            if (nodo->izq == nullptr || nodo->der == nullptr)
            {
                NodoEspecialidad* temp =
                    (nodo->izq != nullptr) ? nodo->izq : nodo->der;

                delete nodo;
                return temp;
            }

            NodoEspecialidad* sucesor = Minimo(nodo->der);
            nodo->IdEspecialidad = sucesor->IdEspecialidad;
            nodo->Nombre = sucesor->Nombre;
            nodo->Descripcion = sucesor->Descripcion;

            nodo->der = EliminarRec(nodo->der, sucesor->IdEspecialidad);
        }

        int nivelIzq = (nodo->izq != nullptr) ? nodo->izq->nivel : 0;
        int nivelDer = (nodo->der != nullptr) ? nodo->der->nivel : 0;
        int nivelCorrecto = min(nivelIzq, nivelDer) + 1;

        if (nivelCorrecto < nodo->nivel)
        {
            nodo->nivel = nivelCorrecto;

            if (nodo->der != nullptr &&
                nodo->der->nivel > nivelCorrecto)
                nodo->der->nivel = nivelCorrecto;
        }

        nodo = Giro(nodo);

        if (nodo->der != nullptr)
            nodo->der = Giro(nodo->der);

        if (nodo->der != nullptr && nodo->der->der != nullptr)
            nodo->der->der = Giro(nodo->der->der);

        nodo = Reparto(nodo);

        if (nodo->der != nullptr)
            nodo->der = Reparto(nodo->der);

        return nodo;
    }

public:
    ArbolAAEspecialidades()
    {
        raiz = nullptr;
    }

    ~ArbolAAEspecialidades()
    {
        Destruir(raiz);
    }

    bool ListaVacia()
    {
        return raiz == nullptr;
    }

    bool Existe(int id)
    {
        return BuscarRec(raiz, id) != nullptr;
    }

    bool Insertar(int id, string nombre, string descripcion)
    {
        if (Existe(id))
            return false;

        raiz = InsertarRec(raiz, id, nombre, descripcion);
        return true;
    }

    bool Buscar(int id)
    {
        NodoEspecialidad* aux = BuscarRec(raiz, id);

        if (aux == nullptr)
            return false;

        std::cout << "IdEspecialidad: " << aux->IdEspecialidad << std::endl;
        std::cout << "Nombre: " << aux->Nombre << std::endl;
        std::cout << "Descripcion: " << aux->Descripcion << std::endl;

        return true;
    }

    bool Modificar(int id, string nombre, string descripcion)
    {
        NodoEspecialidad* aux = BuscarRec(raiz, id);

        if (aux == nullptr)
            return false;

        aux->Nombre = nombre;
        aux->Descripcion = descripcion;
        return true;
    }

    bool Eliminar(int id)
    {
        if (!Existe(id))
            return false;

        raiz = EliminarRec(raiz, id);
        return true;
    }

    void Mostrar()
    {
        if (ListaVacia())
        {
            std::cout << "Arbol vacio." << std::endl;
            return;
        }

        MostrarRecursivo(raiz);
    }

    void ReportePDF(GeneradorPDF& pdf)
    {
        if (ListaVacia())
        {
            pdf.Linea("(No hay especialidades registradas)");
            return;
        }

        ReportePDFRec(raiz, pdf);
    }
};

inline void CargarEspecialidades(ArbolAAEspecialidades& Especialidades)
{
    ifstream archivo("Especialidades.csv");

    if (!archivo.is_open())
    {
        std::cout << "No se pudo abrir Especialidades.csv" << std::endl;
        return;
    }

    string encabezado;
    getline(archivo, encabezado);

    string idTexto, nombre, descripcion;

    while (getline(archivo, idTexto, ';') &&
        getline(archivo, nombre, ';') &&
        getline(archivo, descripcion))
    {
        try
        {
            int id = stoi(idTexto);

            if (!Especialidades.Existe(id))
                Especialidades.Insertar(id, nombre, descripcion);
        }
        catch (...)
        {
            std::cout << "Fila con ID invalido." << std::endl;
        }
    }

    std::cout << "Especialidades cargadas correctamente." << std::endl;
}

inline void ReporteEspecialidades(ArbolAAEspecialidades& Especialidades)
{
    GeneradorPDF pdf(
        "Reporte_Especialidades.pdf",
        "REPORTE DE ESPECIALIDADES",
        Rellenar("ID", 8) + Rellenar("NOMBRE", 25)
        + Rellenar("DESCRIPCION", 85));

    if (!pdf.Valido())
    {
        std::cout << "No se pudo crear el documento PDF." << std::endl;
        return;
    }

    Especialidades.ReportePDF(pdf);

    if (pdf.Guardar())
        cout << "Reporte generado: Reporte_Especialidades.pdf" << std::endl;
    else
        cout << "No se pudo guardar el archivo PDF." << std::endl;
}
