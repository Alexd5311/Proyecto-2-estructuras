#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "Comun_PDF.h"

using namespace std;

class NodoHabitacion
{
public:
    int IdHabitacion;
    int Numero;
    int Piso;
    int Tipo;
    int Estado;
    int FB;
    NodoHabitacion* Hizq;
    NodoHabitacion* Hder;

    NodoHabitacion(int nuevoIdHabitacion, int nuevoNumero, int nuevoPiso,
        int nuevoTipo, int nuevoEstado)
    {
        IdHabitacion = nuevoIdHabitacion;
        Numero = nuevoNumero;
        Piso = nuevoPiso;
        Tipo = nuevoTipo;
        Estado = nuevoEstado;
        FB = 0;
        Hizq = nullptr;
        Hder = nullptr;
    }
};

typedef NodoHabitacion* pNodoHabitacion;

class ArbolAVLHabitaciones
{
private:
    pNodoHabitacion raiz;

    // ╬ Equilibra en el lado izquierdo ╬
    void Equilibrar1(NodoHabitacion*& n, bool& Hh)
    {
        NodoHabitacion* n1;
        switch (n->FB)
        {
        case -1:
            n->FB = 0;
            break;
        case 0:
            n->FB = 1;
            Hh = false;
            break;
        case 1:
            n1 = n->Hder;
            if (n1->FB >= 0)
            {
                if (n1->FB == 0)
                    Hh = false;
                RotacionSimpleDerecha(n, n1);
            }
            else
            {
                RotacionDobleDerecha(n, n1);
            }
            break;
        }
    }

    // ╬ Equilibra en el lado derecho ╬
    void Equilibrar2(NodoHabitacion*& n, bool& Hh)
    {
        NodoHabitacion* n1;
        switch (n->FB)
        {
        case 1:
            n->FB = 0;
            break;
        case 0:
            n->FB = -1;
            Hh = false;
            break;
        case -1:
            n1 = n->Hizq;
            if (n1->FB <= 0)
            {
                if (n1->FB == 0)
                    Hh = false;
                RotacionSimpleIzquierda(n, n1);
            }
            else
            {
                RotacionDobleIzquierda(n, n1);
            }
            break;
        }
    }

    // ╬ Es el metodo principal de insertar, ademas balancea si hace falta ╬ 
    void InsertarBalanceado(pNodoHabitacion& ra, bool& Hh,
        int IdHabitacion, int Numero, int Piso,
        int Tipo, int Estado)
    {
        NodoHabitacion* n1;

        if (ra == nullptr)
        {
            ra = new NodoHabitacion(IdHabitacion, Numero, Piso, Tipo, Estado);
            Hh = true;
        }
        else if (IdHabitacion < ra->IdHabitacion)
        {
            InsertarBalanceado(ra->Hizq, Hh, IdHabitacion, Numero, Piso, Tipo, Estado);
            if (Hh)
            {
                switch (ra->FB)
                {
                case 1:
                    ra->FB = 0;
                    Hh = false;
                    break;
                case 0:
                    ra->FB = -1;
                    break;
                case -1:
                    n1 = ra->Hizq;
                    if (n1->FB == -1)
                        RotacionSimpleIzquierda(ra, n1);
                    else
                        RotacionDobleIzquierda(ra, n1);
                    Hh = false;
                    break;
                }
            }
        }
        else if (IdHabitacion > ra->IdHabitacion)
        {
            InsertarBalanceado(ra->Hder, Hh, IdHabitacion, Numero, Piso, Tipo, Estado);
            if (Hh)
            {
                switch (ra->FB)
                {
                case -1:
                    ra->FB = 0;
                    Hh = false;
                    break;
                case 0:
                    ra->FB = 1;
                    break;
                case 1:
                    n1 = ra->Hder;
                    if (n1->FB == 1)
                        RotacionSimpleDerecha(ra, n1);
                    else
                        RotacionDobleDerecha(ra, n1);
                    Hh = false;
                    break;
                }
            }
        }
    }

    // ╬ Elimina el nodo mas a la derecha ╬
    void Borrar(pNodoHabitacion& nodo, bool& Hh)
    {
        pNodoHabitacion q;
        if (nodo->Hder != nullptr)
        {
            Borrar(nodo->Hder, Hh);
            if (Hh)
                Equilibrar2(nodo, Hh);
        }
        else
        {
            q = nodo;
            nodo = nodo->Hizq;
            delete q;
            Hh = true;
        }
    }

    // ╬ Metodo principal de eliminar, es por el Id ╬
    void BorrarBalanceado(pNodoHabitacion& subarbol, bool& Hh, int IdHabitacion)
    {
        pNodoHabitacion q;
        if (subarbol != nullptr)
        {
            if (IdHabitacion < subarbol->IdHabitacion)
            {
                BorrarBalanceado(subarbol->Hizq, Hh, IdHabitacion);
                if (Hh)
                    Equilibrar1(subarbol, Hh);
            }
            else if (IdHabitacion > subarbol->IdHabitacion)
            {
                BorrarBalanceado(subarbol->Hder, Hh, IdHabitacion);
                if (Hh)
                    Equilibrar2(subarbol, Hh);
            }
            else
            {
                q = subarbol;
                if (q->Hizq == nullptr)
                {
                    subarbol = q->Hder;
                    delete q;
                    Hh = true;
                }
                else if (q->Hder == nullptr)
                {
                    subarbol = q->Hizq;
                    delete q;
                    Hh = true;
                }
                else
                {
                    
                    pNodoHabitacion reemplazo = q->Hizq;
                    while (reemplazo->Hder != nullptr)
                        reemplazo = reemplazo->Hder;

                    q->IdHabitacion = reemplazo->IdHabitacion;
                    q->Numero = reemplazo->Numero;
                    q->Piso = reemplazo->Piso;
                    q->Tipo = reemplazo->Tipo;
                    q->Estado = reemplazo->Estado;

                    BorrarBalanceado(subarbol->Hizq, Hh, reemplazo->IdHabitacion);
                    if (Hh)
                        Equilibrar1(subarbol, Hh);
                }
            }
        }
    }

    // ╬ Reorganiza 3 nodos, el nombre dice como xd ╬
    void RotacionDobleIzquierda(NodoHabitacion*& n, NodoHabitacion*& n1)
    {
        NodoHabitacion* n2 = n1->Hder;
        n->Hizq = n2->Hder;
        n2->Hder = n;
        n1->Hder = n2->Hizq;
        n2->Hizq = n1;

        if (n2->FB == 1)
            n1->FB = -1;
        else
            n1->FB = 0;

        if (n2->FB == -1)
            n->FB = 1;
        else
            n->FB = 0;

        n2->FB = 0;
        n = n2;
    }

    // ╬ Reorganiza 3 nodos, el nombre dice como xd ╬
    void RotacionDobleDerecha(NodoHabitacion*& n, NodoHabitacion*& n1)
    {
        NodoHabitacion* n2 = n1->Hizq;
        n->Hder = n2->Hizq;
        n2->Hizq = n;
        n1->Hizq = n2->Hder;
        n2->Hder = n1;

        if (n2->FB == 1)
            n->FB = -1;
        else
            n->FB = 0;

        if (n2->FB == -1)
            n1->FB = 1;
        else
            n1->FB = 0;

        n2->FB = 0;
        n = n2;
    }

    // ╬ Asciende el hijo derecho ╬
    void RotacionSimpleDerecha(NodoHabitacion*& n, NodoHabitacion*& n1)
    {
        n->Hder = n1->Hizq;
        n1->Hizq = n;
        if (n1->FB == 1)
        {
            n->FB = 0;
            n1->FB = 0;
        }
        else
        {
            n->FB = 1;
            n1->FB = -1;
        }
        n = n1;
    }

    // ╬ Asciende al hijo izquierdo ╬
    void RotacionSimpleIzquierda(NodoHabitacion*& n, NodoHabitacion*& n1)
    {
        n->Hizq = n1->Hder;
        n1->Hder = n;
        if (n1->FB == -1)
        {
            n->FB = 0;
            n1->FB = 0;
        }
        else
        {
            n->FB = -1;
            n1->FB = 1;
        }
        n = n1;
    }

    void MostrarInorden(pNodoHabitacion n)
    {
        if (n != nullptr)
        {
            MostrarInorden(n->Hizq);
            MostrarRegistro(n);
            MostrarInorden(n->Hder);
        }
    }

    void MostrarPreorden(pNodoHabitacion n)
    {
        if (n != nullptr)
        {
            MostrarRegistro(n);
            MostrarPreorden(n->Hizq);
            MostrarPreorden(n->Hder);
        }
    }

    void MostrarRegistro(pNodoHabitacion n)
    {
        cout << "IdHabitacion: " << n->IdHabitacion
            << " | Numero: " << n->Numero
            << " | Piso: " << n->Piso
            << " | Tipo: " << n->Tipo
            << " | Estado: " << n->Estado << endl;
    }

    void LiberarMemoria(pNodoHabitacion n)
    {
        if (n != nullptr)
        {
            LiberarMemoria(n->Hizq);
            LiberarMemoria(n->Hder);
            delete n;
        }
    }

    int ReportePDFRecursivo(GeneradorPDF& pdf, int Estado, pNodoHabitacion n)
    {
        if (n == nullptr)
            return 0;

        int cantidad = ReportePDFRecursivo(pdf, Estado, n->Hizq);
        if (n->Estado == Estado)
        {
            pdf.Linea(Rellenar(to_string(n->IdHabitacion), 8)
                + Rellenar(to_string(n->Numero), 8)
                + Rellenar(to_string(n->Piso), 8)
                + Rellenar(to_string(n->Tipo), 8)
                + Rellenar(to_string(n->Estado), 8));
            cantidad++;
        }
        cantidad += ReportePDFRecursivo(pdf, Estado, n->Hder);
        return cantidad;
    }

public:
    ArbolAVLHabitaciones() : raiz(nullptr) {}

    ~ArbolAVLHabitaciones()
    {
        LiberarMemoria(raiz);
    }

    bool ListaVacia() const
    {
        return raiz == nullptr;
    }

    bool Existe(int IdHabitacion) const
    {
        pNodoHabitacion actual = raiz;
        while (actual != nullptr)
        {
            if (IdHabitacion == actual->IdHabitacion)
                return true;
            if (IdHabitacion < actual->IdHabitacion)
                actual = actual->Hizq;
            else
                actual = actual->Hder;
        }
        return false;
    }

    bool Insertar(int IdHabitacion, int Numero, int Piso, int Tipo, int Estado)
    {
        if (Existe(IdHabitacion))
            return false;
        bool Hh = false;
        InsertarBalanceado(raiz, Hh, IdHabitacion, Numero, Piso, Tipo, Estado);
        return true;
    }

    bool Buscar(int IdHabitacion) const
    {
        pNodoHabitacion actual = raiz;
        while (actual != nullptr)
        {
            if (IdHabitacion == actual->IdHabitacion)
            {
                cout << "IdHabitacion: " << actual->IdHabitacion << endl;
                cout << "Numero: " << actual->Numero << endl;
                cout << "Piso: " << actual->Piso << endl;
                cout << "Tipo: " << actual->Tipo << endl;
                cout << "Estado: " << actual->Estado << endl;
                return true;
            }
            actual = (IdHabitacion < actual->IdHabitacion) ? actual->Hizq : actual->Hder;
        }
        return false;
    }

    bool Eliminar(int IdHabitacion)
    {
        if (!Existe(IdHabitacion))
            return false;
        bool Hh = false;
        BorrarBalanceado(raiz, Hh, IdHabitacion);
        return true;
    }

    bool Modificar(int IdHabitacion, int Numero, int Piso, int Tipo, int Estado)
    {
        pNodoHabitacion actual = raiz;
        while (actual != nullptr)
        {
            if (IdHabitacion == actual->IdHabitacion)
            {
                actual->Numero = Numero;
                actual->Piso = Piso;
                actual->Tipo = Tipo;
                actual->Estado = Estado;
                return true;
            }
            actual = (IdHabitacion < actual->IdHabitacion) ? actual->Hizq : actual->Hder;
        }
        return false;
    }

    void Mostrar()
    {
        if (ListaVacia())
        {
            cout << "No hay habitaciones." << endl;
            return;
        }
        // Se usa inorden para listar por IdHabitacion de menor a mayor.
        MostrarInorden(raiz);
    }

    int ReportePDF(GeneradorPDF& pdf, int Estado)
    {
        return ReportePDFRecursivo(pdf, Estado, raiz);
    }
};


ArbolAVLHabitaciones;

void ReporteHabitacionesDisponibles(ArbolAVLHabitaciones& Habitaciones)
{
    GeneradorPDF pdf("Reporte_Habitaciones_Disponibles.pdf", "REPORTE DE HABITACIONES DISPONIBLES",
        Rellenar("ID", 8) + Rellenar("NUMERO", 8) + Rellenar("PISO", 8)
        + Rellenar("TIPO", 8) + Rellenar("ESTADO", 8));
    if (!pdf.Valido())
    {
        cout << "No se pudo crear el documento PDF." << endl;
        return;
    }
    if (Habitaciones.ReportePDF(pdf, 0) == 0)
        pdf.Linea("(No hay habitaciones disponibles)");

    if (pdf.Guardar())
        cout << "Reporte generado: Reporte_Habitaciones_Disponibles.pdf" << endl;
    else
        cout << "No se pudo guardar el archivo PDF." << endl;
}

void ReporteHabitacionesOcupadas(ArbolAVLHabitaciones& Habitaciones)
{
    GeneradorPDF pdf("Reporte_Habitaciones_Ocupadas.pdf", "REPORTE DE HABITACIONES OCUPADAS",
        Rellenar("ID", 8) + Rellenar("NUMERO", 8) + Rellenar("PISO", 8)
        + Rellenar("TIPO", 8) + Rellenar("ESTADO", 8));
    if (!pdf.Valido())
    {
        cout << "No se pudo crear el documento PDF." << endl;
        return;
    }
    if (Habitaciones.ReportePDF(pdf, 1) == 0)
        pdf.Linea("(No hay habitaciones ocupadas)");

    if (pdf.Guardar())
        cout << "Reporte generado: Reporte_Habitaciones_Ocupadas.pdf" << endl;
    else
        cout << "No se pudo guardar el archivo PDF." << endl;
}

void CargarHabitaciones(ArbolAVLHabitaciones& Habitaciones)
{
    ifstream archivo("Habitaciones.csv");
    if (!archivo)
    {
        cout << "No se pudo abrir el archivo Habitaciones.csv" << endl;
        return;
    }

    string encabezado;
    getline(archivo, encabezado); // IdHabitacion;Numero;Piso;Tipo;Estado

    string IdHabitacionTexto, NumeroTexto, PisoTexto, TipoTexto, EstadoTexto;
    while (getline(archivo, IdHabitacionTexto, ';') &&
        getline(archivo, NumeroTexto, ';') &&
        getline(archivo, PisoTexto, ';') &&
        getline(archivo, TipoTexto, ';') &&
        getline(archivo, EstadoTexto))
    {
        try
        {
            int IdHabitacion = stoi(IdHabitacionTexto);
            int Numero = stoi(NumeroTexto);
            // Conserva el formato anterior (por ejemplo, "Piso 2") y admite tambien "2".
            int Piso = (PisoTexto.find("Piso ") == 0) ? stoi(PisoTexto.substr(5)) : stoi(PisoTexto);
            int Tipo = stoi(TipoTexto);
            int Estado = stoi(EstadoTexto);

            if (Habitaciones.Existe(IdHabitacion))
                continue; // conservar la primera aparicion de cada ID
            if (Tipo < 1 || Tipo > 3)
                continue;
            if (Estado < 0 || Estado > 1)
                continue;

            Habitaciones.Insertar(IdHabitacion, Numero, Piso, Tipo, Estado);
        }
        catch (...)
        {
            // Omitir registros mal formados sin interrumpir la carga del archivo.
            continue;
        }
    }

    archivo.close();
    cout << "Habitaciones cargadas correctamente." << endl;
}
