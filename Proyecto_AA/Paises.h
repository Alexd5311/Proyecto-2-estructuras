
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "Comun_PDF.h"

using namespace std;

class NodoPais {
public:
    int CodPais;
    string Nombre;
    NodoPais* izquierdo;
    NodoPais* derecho;

    NodoPais(int nuevoCodPais, string nuevoNombre) {
        CodPais = nuevoCodPais;
        Nombre = nuevoNombre;
        izquierdo = NULL;
        derecho = NULL;
    }
};

class ArbolABBPaises {
private:
    NodoPais* raiz;

    NodoPais* BuscarNodo(NodoPais* nodo, int CodPais) {
        if (nodo == NULL || nodo->CodPais == CodPais)
            return nodo;
        if (CodPais < nodo->CodPais)
            return BuscarNodo(nodo->izquierdo, CodPais);
        return BuscarNodo(nodo->derecho, CodPais);
    }

    NodoPais* InsertarNodo(NodoPais* nodo, int CodPais, string Nombre) {
        if (nodo == NULL)
            return new NodoPais(CodPais, Nombre);
        if (CodPais < nodo->CodPais)
            nodo->izquierdo = InsertarNodo(nodo->izquierdo, CodPais, Nombre);
        else if (CodPais > nodo->CodPais)
            nodo->derecho = InsertarNodo(nodo->derecho, CodPais, Nombre);
        return nodo;
    }

    NodoPais* MenorNodo(NodoPais* nodo) {
        while (nodo != NULL && nodo->izquierdo != NULL)
            nodo = nodo->izquierdo;
        return nodo;
    }

    NodoPais* EliminarNodo(NodoPais* nodo, int CodPais) {
        if (nodo == NULL)
            return NULL;
        if (CodPais < nodo->CodPais)
            nodo->izquierdo = EliminarNodo(nodo->izquierdo, CodPais);
        else if (CodPais > nodo->CodPais)
            nodo->derecho = EliminarNodo(nodo->derecho, CodPais);
        else {
            if (nodo->izquierdo == NULL) {
                NodoPais* temp = nodo->derecho;
                delete nodo;
                return temp;
            }
            if (nodo->derecho == NULL) {
                NodoPais* temp = nodo->izquierdo;
                delete nodo;
                return temp;
            }
            NodoPais* temp = MenorNodo(nodo->derecho);
            nodo->CodPais = temp->CodPais;
            nodo->Nombre = temp->Nombre;
            nodo->derecho = EliminarNodo(nodo->derecho, temp->CodPais);
        }
        return nodo;
    }

    void MostrarInorden(NodoPais* nodo) {
        if (nodo != NULL) {
            MostrarInorden(nodo->izquierdo);
            cout << "CodPais: " << nodo->CodPais
                << " | Nombre: " << nodo->Nombre << endl;
            MostrarInorden(nodo->derecho);
        }
    }

    void ReporteInorden(NodoPais* nodo, GeneradorPDF& pdf) {
        if (nodo != NULL) {
            ReporteInorden(nodo->izquierdo, pdf);
            pdf.Linea(Rellenar(to_string(nodo->CodPais), 10) +
                Rellenar(nodo->Nombre, 40));
            ReporteInorden(nodo->derecho, pdf);
        }
    }

    void Liberar(NodoPais* nodo) {
        if (nodo != NULL) {
            Liberar(nodo->izquierdo);
            Liberar(nodo->derecho);
            delete nodo;
        }
    }

public:
    ArbolABBPaises() {
        raiz = NULL;
    }

    bool ListaVacia() {
        return raiz == NULL;
    }

    bool Existe(int CodPais) {
        return BuscarNodo(raiz, CodPais) != NULL;
    }

    bool Insertar(int CodPais, string Nombre) {
        if (Existe(CodPais))
            return false;
        raiz = InsertarNodo(raiz, CodPais, Nombre);
        return true;
    }

    bool Buscar(int CodPais) {
        NodoPais* encontrado = BuscarNodo(raiz, CodPais);
        if (encontrado == NULL)
            return false;
        cout << "CodPais: " << encontrado->CodPais << endl;
        cout << "Nombre: " << encontrado->Nombre << endl;
        return true;
    }

    bool Eliminar(int CodPais) {
        if (!Existe(CodPais))
            return false;
        raiz = EliminarNodo(raiz, CodPais);
        return true;
    }

    bool Modificar(int CodPais, string Nombre) {
        NodoPais* encontrado = BuscarNodo(raiz, CodPais);
        if (encontrado == NULL)
            return false;
        encontrado->Nombre = Nombre;
        return true;
    }

    void Mostrar() {
        MostrarInorden(raiz);
    }

    void ReportePDF(GeneradorPDF& pdf) {
        ReporteInorden(raiz, pdf);
    }

    ~ArbolABBPaises() {
        Liberar(raiz);
    }
};

void ReportePaises(ArbolABBPaises& Paises) {
    GeneradorPDF pdf("Reporte_Paises.pdf", "REPORTE DE PAISES",
        Rellenar("CODIGO", 10) + Rellenar("NOMBRE", 40));

    if (!pdf.Valido()) {
        cout << "No se pudo crear el documento PDF." << endl;
        return;
    }

    if (Paises.ListaVacia())
        pdf.Linea("(No hay paises registrados)");
    else
        Paises.ReportePDF(pdf);

    if (pdf.Guardar())
        cout << "Reporte generado: Reporte_Paises.pdf" << endl;
    else
        cout << "No se pudo guardar el archivo PDF." << endl;
}

void CargarPaises(ArbolABBPaises& Paises) {
    ifstream archivo("Paises.csv");

    if (!archivo) {
        cout << "No se pudo abrir el archivo Paises.csv" << endl;
        return;
    }

    string linea;
    getline(archivo, linea);

    while (getline(archivo, linea)) {
        if (linea.empty())
            continue;

        size_t separador = linea.find(';');

        if (separador != string::npos) {
            string codPaisTexto = linea.substr(0, separador);
            string Nombre = linea.substr(separador + 1);
            int CodPais = stoi(codPaisTexto);
            Paises.Insertar(CodPais, Nombre);
        }
    }

    archivo.close();
    cout << "Paises cargados correctamente." << endl;
}
