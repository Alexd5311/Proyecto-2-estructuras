
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "comun_PDF.h"
#include "Paises.h"
using namespace std;

class NodoCiudad {
public:
    int CodPais;
    int CodCiudad;
    string Nombre;
    NodoCiudad* izquierdo;
    NodoCiudad* derecho;

    NodoCiudad(int nuevoCodPais, int nuevoCodCiudad, string nuevoNombre) {
        CodPais = nuevoCodPais;
        CodCiudad = nuevoCodCiudad;
        Nombre = nuevoNombre;
        izquierdo = NULL;
        derecho = NULL;
    }
};

typedef NodoCiudad* pnodoCiudad;

class ListaCiudades {
private:
    pnodoCiudad raiz;

    bool Menor(int CodPais1, int CodCiudad1, int CodPais2, int CodCiudad2) {
        if (CodCiudad1 != CodCiudad2)
            return CodCiudad1 < CodCiudad2;
        return CodPais1 < CodPais2;
    }

    pnodoCiudad BuscarNodo(pnodoCiudad actual, int CodPais, int CodCiudad) {
        if (actual == NULL)
            return NULL;

        if (actual->CodPais == CodPais && actual->CodCiudad == CodCiudad)
            return actual;

        if (Menor(CodPais, CodCiudad, actual->CodPais, actual->CodCiudad))
            return BuscarNodo(actual->izquierdo, CodPais, CodCiudad);

        return BuscarNodo(actual->derecho, CodPais, CodCiudad);
    }

    pnodoCiudad InsertarNodo(pnodoCiudad actual, int CodPais, int CodCiudad, string Nombre, bool& insertado) {
        if (actual == NULL) {
            insertado = true;
            return new NodoCiudad(CodPais, CodCiudad, Nombre);
        }

        if (actual->CodPais == CodPais && actual->CodCiudad == CodCiudad)
            return actual;

        if (Menor(CodPais, CodCiudad, actual->CodPais, actual->CodCiudad))
            actual->izquierdo = InsertarNodo(actual->izquierdo, CodPais, CodCiudad, Nombre, insertado);
        else
            actual->derecho = InsertarNodo(actual->derecho, CodPais, CodCiudad, Nombre, insertado);

        return actual;
    }

    pnodoCiudad MenorNodo(pnodoCiudad actual) {
        while (actual != NULL && actual->izquierdo != NULL)
            actual = actual->izquierdo;
        return actual;
    }

    pnodoCiudad EliminarNodo(pnodoCiudad actual, int CodPais, int CodCiudad, bool& eliminado) {
        if (actual == NULL)
            return NULL;

        if (Menor(CodPais, CodCiudad, actual->CodPais, actual->CodCiudad)) {
            actual->izquierdo = EliminarNodo(actual->izquierdo, CodPais, CodCiudad, eliminado);
        }
        else if (Menor(actual->CodPais, actual->CodCiudad, CodPais, CodCiudad)) {
            actual->derecho = EliminarNodo(actual->derecho, CodPais, CodCiudad, eliminado);
        }
        else {
            eliminado = true;

            if (actual->izquierdo == NULL) {
                pnodoCiudad temp = actual->derecho;
                delete actual;
                return temp;
            }

            if (actual->derecho == NULL) {
                pnodoCiudad temp = actual->izquierdo;
                delete actual;
                return temp;
            }

            pnodoCiudad temp = MenorNodo(actual->derecho);
            actual->CodPais = temp->CodPais;
            actual->CodCiudad = temp->CodCiudad;
            actual->Nombre = temp->Nombre;

            bool eliminadoTemp = false;
            actual->derecho = EliminarNodo(actual->derecho, temp->CodPais, temp->CodCiudad, eliminadoTemp);
        }

        return actual;
    }

    void RecogerCiudades(pnodoCiudad actual, int CodPais, vector<int>& codigos) {
        if (actual != NULL) {
            RecogerCiudades(actual->izquierdo, CodPais, codigos);

            if (actual->CodPais == CodPais)
                codigos.push_back(actual->CodCiudad);

            RecogerCiudades(actual->derecho, CodPais, codigos);
        }
    }

    void MostrarInorden(pnodoCiudad actual) {
        if (actual != NULL) {
            MostrarInorden(actual->izquierdo);
            cout << "CodPais: " << actual->CodPais
                << " | CodCiudad: " << actual->CodCiudad
                << " | Nombre: " << actual->Nombre << endl;
            MostrarInorden(actual->derecho);
        }
    }

    int ReporteInorden(pnodoCiudad actual, GeneradorPDF& pdf, int CodPais) {
        if (actual == NULL)
            return 0;

        int cantidad = ReporteInorden(actual->izquierdo, pdf, CodPais);

        if (actual->CodPais == CodPais) {
            pdf.Linea(Rellenar(to_string(actual->CodCiudad), 10) +
                Rellenar(actual->Nombre, 40));
            cantidad++;
        }

        cantidad += ReporteInorden(actual->derecho, pdf, CodPais);
        return cantidad;
    }

    void Liberar(pnodoCiudad actual) {
        if (actual != NULL) {
            Liberar(actual->izquierdo);
            Liberar(actual->derecho);
            delete actual;
        }
    }

public:
    ListaCiudades() {
        raiz = NULL;
    }

    ~ListaCiudades() {
        Liberar(raiz);
    }

    bool ListaVacia() {
        return raiz == NULL;
    }

    bool Existe(int CodPais, int CodCiudad) {
        return BuscarNodo(raiz, CodPais, CodCiudad) != NULL;
    }

    bool Insertar(int CodPais, int CodCiudad, string Nombre) {
        bool insertado = false;
        raiz = InsertarNodo(raiz, CodPais, CodCiudad, Nombre, insertado);
        return insertado;
    }

    bool Buscar(int CodPais, int CodCiudad) {
        pnodoCiudad actual = BuscarNodo(raiz, CodPais, CodCiudad);

        if (actual == NULL)
            return false;

        cout << "CodPais: " << actual->CodPais << endl;
        cout << "CodCiudad: " << actual->CodCiudad << endl;
        cout << "Nombre: " << actual->Nombre << endl;
        return true;
    }

    bool Eliminar(int CodPais, int CodCiudad) {
        bool eliminado = false;
        raiz = EliminarNodo(raiz, CodPais, CodCiudad, eliminado);
        return eliminado;
    }

    bool EliminarPorPais(int CodPais) {
        vector<int> codigos;
        RecogerCiudades(raiz, CodPais, codigos);

        for (int codigo : codigos)
            Eliminar(CodPais, codigo);

        return !codigos.empty();
    }

    bool Modificar(int CodPais, int CodCiudad, string Nombre) {
        pnodoCiudad actual = BuscarNodo(raiz, CodPais, CodCiudad);

        if (actual == NULL)
            return false;

        actual->Nombre = Nombre;
        return true;
    }

    void Mostrar() {
        if (ListaVacia()) {
            cout << "El arbol de ciudades esta vacio." << endl;
            return;
        }

        MostrarInorden(raiz);
    }

    int ReportePDF(GeneradorPDF& pdf, int CodPais) {
        return ReporteInorden(raiz, pdf, CodPais);
    }
};

void ReporteCiudades(ListaCiudades& Ciudades, ListaPaises& Paises) {
    int CodPais;
    cout << endl;
    cout << "Ingrese el CodPais: ";
    cin >> CodPais;

    if (!Paises.Existe(CodPais)) {
        cout << "El CodPais no existe." << endl;
        return;
    }

    GeneradorPDF pdf("Reporte_Ciudades.pdf",
        "REPORTE DE CIUDADES - PAIS " + to_string(CodPais),
        Rellenar("CODCIUDAD", 10) + Rellenar("NOMBRE", 40));

    if (!pdf.Valido()) {
        cout << "No se pudo crear el documento PDF." << endl;
        return;
    }

    if (Ciudades.ReportePDF(pdf, CodPais) == 0)
        pdf.Linea("(Este pais no tiene ciudades registradas)");

    if (pdf.Guardar())
        cout << "Reporte generado: Reporte_Ciudades.pdf" << endl;
    else
        cout << "No se pudo guardar el archivo PDF." << endl;
}


void CargarCiudades(ListaCiudades &Ciudades, ListaPaises &Paises){
    ifstream archivo("Ciudades.csv");
    if (!archivo){
        cout << "No se pudo abrir el archivo Ciudades.csv" << endl;
        return;
    }

    string linea;
    getline(archivo, linea); // Saltar encabezado del CSV
    string codPaisTexto;
    string codCiudadTexto;
    string Nombre;
    while (getline(archivo, linea)){
        if (linea != ""){
            size_t separador1 = linea.find(';');
            if (separador1 != string::npos)
            {
                size_t separador2 = linea.find(';', separador1 + 1);
                if (separador2 != string::npos)
                {
                    codPaisTexto = linea.substr(0, separador1);
                    codCiudadTexto = linea.substr(
                        separador1 + 1,
                        separador2 - separador1 - 1
                    );
                    Nombre = linea.substr(separador2 + 1);
                    int CodPais = stoi(codPaisTexto);
                    int CodCiudad = stoi(codCiudadTexto);
                    if (!Paises.Existe(CodPais))
                    {
                        continue;
                    }
                    Ciudades.Insertar(CodPais, CodCiudad, Nombre);
                }
            }
        }
    }
    archivo.close();
    cout << "Ciudades cargadas correctamente." << endl;
}

