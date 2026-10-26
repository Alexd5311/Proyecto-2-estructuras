
#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include "Comun_PDF.h"

using namespace std;

// ======================================================
// NODO DEL ARBOL AA
// ======================================================

class NodoDiagnostico
{
public:
    string IdDiagnostico;
    string Nombre;
    string Descripcion;

    NodoDiagnostico* izq;
    NodoDiagnostico* der;
    int nivel;

    NodoDiagnostico(string id, string nombre, string descripcion)
    {
        IdDiagnostico = id;
        Nombre = nombre;
        Descripcion = descripcion;

        izq = nullptr;
        der = nullptr;
        nivel = 1;
    }
};

// ======================================================
// ARBOL AA DE DIAGNOSTICOS
// ======================================================

class ArbolAADiagnostico
{
private:
    NodoDiagnostico* raiz;

    // Giro hacia la derecha: skew
    NodoDiagnostico* Giro(NodoDiagnostico* nodo)
    {
        if (nodo == nullptr || nodo->izq == nullptr)
            return nodo;

        if (nodo->izq->nivel == nodo->nivel)
        {
            NodoDiagnostico* temp = nodo->izq;
            nodo->izq = temp->der;
            temp->der = nodo;
            return temp;
        }

        return nodo;
    }

    // Giro hacia la izquierda: split
    NodoDiagnostico* Reparto(NodoDiagnostico* nodo)
    {
        if (nodo == nullptr || nodo->der == nullptr ||
            nodo->der->der == nullptr)
            return nodo;

        if (nodo->nivel == nodo->der->der->nivel)
        {
            NodoDiagnostico* temp = nodo->der;
            nodo->der = temp->izq;
            temp->izq = nodo;
            temp->nivel++;
            return temp;
        }

        return nodo;
    }

    NodoDiagnostico* BuscarRec(NodoDiagnostico* nodo, string id)
    {
        if (nodo == nullptr || nodo->IdDiagnostico == id)
            return nodo;

        if (id < nodo->IdDiagnostico)
            return BuscarRec(nodo->izq, id);

        return BuscarRec(nodo->der, id);
    }

    NodoDiagnostico* InsertarRec(
        NodoDiagnostico* nodo,
        string id,
        string nombre,
        string descripcion)
    {
        if (nodo == nullptr)
            return new NodoDiagnostico(id, nombre, descripcion);

        if (id < nodo->IdDiagnostico)
            nodo->izq = InsertarRec(
                nodo->izq, id, nombre, descripcion);
        else if (id > nodo->IdDiagnostico)
            nodo->der = InsertarRec(
                nodo->der, id, nombre, descripcion);
        else
            return nodo;

        nodo = Giro(nodo);
        nodo = Reparto(nodo);

        return nodo;
    }

    int Nivel(NodoDiagnostico* nodo)
    {
        if (nodo == nullptr)
            return 0;

        return nodo->nivel;
    }

    NodoDiagnostico* Minimo(NodoDiagnostico* nodo)
    {
        while (nodo != nullptr && nodo->izq != nullptr)
            nodo = nodo->izq;

        return nodo;
    }

    NodoDiagnostico* EliminarRec(NodoDiagnostico* nodo, string id)
    {
        if (nodo == nullptr)
            return nullptr;

        if (id < nodo->IdDiagnostico)
            nodo->izq = EliminarRec(nodo->izq, id);
        else if (id > nodo->IdDiagnostico)
            nodo->der = EliminarRec(nodo->der, id);
        else
        {
            if (nodo->izq == nullptr)
            {
                NodoDiagnostico* temp = nodo->der;
                delete nodo;
                return temp;
            }
            else if (nodo->der == nullptr)
            {
                NodoDiagnostico* temp = nodo->izq;
                delete nodo;
                return temp;
            }
            else
            {
                NodoDiagnostico* sucesor = Minimo(nodo->der);

                nodo->IdDiagnostico = sucesor->IdDiagnostico;
                nodo->Nombre = sucesor->Nombre;
                nodo->Descripcion = sucesor->Descripcion;

                nodo->der = EliminarRec(
                    nodo->der, sucesor->IdDiagnostico);
            }
        }

        int nivelEsperado =
            min(Nivel(nodo->izq), Nivel(nodo->der)) + 1;

        if (nivelEsperado < nodo->nivel)
        {
            nodo->nivel = nivelEsperado;

            if (nodo->der != nullptr &&
                nodo->der->nivel > nivelEsperado)
                nodo->der->nivel = nivelEsperado;
        }

        nodo = Giro(nodo);

        if (nodo->der != nullptr)
            nodo->der = Giro(nodo->der);

        if (nodo->der != nullptr &&
            nodo->der->der != nullptr)
            nodo->der->der = Giro(nodo->der->der);

        nodo = Reparto(nodo);

        if (nodo->der != nullptr)
            nodo->der = Reparto(nodo->der);

        return nodo;
    }

    void Destruir(NodoDiagnostico* nodo)
    {
        if (nodo == nullptr)
            return;

        Destruir(nodo->izq);
        Destruir(nodo->der);
        delete nodo;
    }

    void MostrarRec(NodoDiagnostico* nodo)
    {
        if (nodo == nullptr)
            return;

        MostrarRec(nodo->izq);

        cout << "IdDiagnostico: " << nodo->IdDiagnostico
             << " | Nombre: " << nodo->Nombre
             << " | Descripcion: " << nodo->Descripcion
             << endl;

        MostrarRec(nodo->der);
    }

    void ReportePDFRec(NodoDiagnostico* nodo, GeneradorPDF& pdf)
    {
        if (nodo == nullptr)
            return;

        ReportePDFRec(nodo->izq, pdf);

        pdf.Linea(
            Rellenar(nodo->IdDiagnostico, 12) +
            Rellenar(nodo->Nombre, 25) +
            Rellenar(nodo->Descripcion, 85)
        );

        ReportePDFRec(nodo->der, pdf);
    }

public:
    ArbolAADiagnostico()
    {
        raiz = nullptr;
    }

    ~ArbolAADiagnostico()
    {
        Destruir(raiz);
    }

    bool ListaVacia()
    {
        return raiz == nullptr;
    }

    bool Existe(string id)
    {
        return BuscarRec(raiz, id) != nullptr;
    }

    bool Buscar(string id)
    {
        NodoDiagnostico* encontrado = BuscarRec(raiz, id);

        if (encontrado == nullptr)
            return false;

        cout << "IdDiagnostico: "
             << encontrado->IdDiagnostico << endl;
        cout << "Nombre: " << encontrado->Nombre << endl;
        cout << "Descripcion: "
             << encontrado->Descripcion << endl;

        return true;
    }

    bool Insertar(string id, string nombre, string descripcion)
    {
        if (Existe(id))
            return false;

        raiz = InsertarRec(raiz, id, nombre, descripcion);
        return true;
    }

    bool Modificar(string id, string nombre, string descripcion)
    {
        NodoDiagnostico* encontrado = BuscarRec(raiz, id);

        if (encontrado == nullptr)
            return false;

        encontrado->Nombre = nombre;
        encontrado->Descripcion = descripcion;

        return true;
    }

    bool Eliminar(string id)
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
            cout << "Arbol de diagnosticos vacio." << endl;
            return;
        }

        MostrarRec(raiz);
    }

    void ReportePDF(GeneradorPDF& pdf)
    {
        if (ListaVacia())
        {
            pdf.Linea("(No hay diagnosticos registrados)");
            return;
        }

        ReportePDFRec(raiz, pdf);
    }
};

// ======================================================
// CARGAR DIAGNOSTICOS DESDE EL CSV
// ======================================================


inline void CargarDiagnosticos(ArbolAADiagnostico& Diagnosticos)
{
    ifstream archivo("Diagnostico.csv");

    if (!archivo)
    {
        cout << "No se pudo abrir Diagnostico.csv" << endl;
        return;
    }

    string encabezado;
    getline(archivo, encabezado);

    string id;
    string nombre;
    string descripcion;
    int cargados = 0;

    while (getline(archivo, id, ';') &&
        getline(archivo, nombre, ';') &&
        getline(archivo, descripcion))
    {
        if (!descripcion.empty() && descripcion.back() == '\r')
            descripcion.pop_back();

        // El ID se conserva como texto: D001, D002, etc.
        if (Diagnosticos.Insertar(id, nombre, descripcion))
            cargados++;
    }

    archivo.close();

    cout << "Diagnosticos cargados: " << cargados << endl;
}


// ======================================================
// REPORTE PDF
// ======================================================

inline void ReporteDiagnosticos(ArbolAADiagnostico& Diagnosticos)
{
    GeneradorPDF pdf(
        "Reporte_Diagnosticos.pdf",
        "REPORTE DE DIAGNOSTICOS",
        Rellenar("ID", 12) +
        Rellenar("NOMBRE", 25) +
        Rellenar("DESCRIPCION", 85)
    );

    if (!pdf.Valido())
    {
        cout << "No se pudo crear el documento PDF." << endl;
        return;
    }

    Diagnosticos.ReportePDF(pdf);

    if (pdf.Guardar())
        cout << "Reporte generado: Reporte_Diagnosticos.pdf" << endl;
    else
        cout << "No se pudo guardar el archivo PDF." << endl;
}
