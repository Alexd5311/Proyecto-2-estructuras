#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Comun_PDF.h"

using namespace std;

class ArbolBPacientes;
class ArbolBMedicos;
class ListaEspecialidades;

class NodoCita
{
public:
    int IdCita;
    int IdPaciente;
    int IdMedico;
    int IdEspecialidad;
    string Fecha;
    string Hora;
    string Motivo;
    string Estado;
    int FB;
    NodoCita* Hizq;
    NodoCita* Hder;

    NodoCita(int nuevoIdCita, int nuevoIdPaciente, int nuevoIdMedico,
        int nuevoIdEspecialidad, string nuevaFecha, string nuevaHora,
        string nuevoMotivo, string nuevoEstado)
    {
        IdCita = nuevoIdCita;
        IdPaciente = nuevoIdPaciente;
        IdMedico = nuevoIdMedico;
        IdEspecialidad = nuevoIdEspecialidad;
        Fecha = nuevaFecha;
        Hora = nuevaHora;
        Motivo = nuevoMotivo;
        Estado = nuevoEstado;
        FB = 0;
        Hizq = nullptr;
        Hder = nullptr;
    }
};

typedef NodoCita* pNodoCita;

class ArbolAVLCitas
{
private:
    pNodoCita raiz;

    void Equilibrar1(NodoCita*& n, bool& Hh)
    {
        NodoCita* n1;
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

    void Equilibrar2(NodoCita*& n, bool& Hh)
    {
        NodoCita* n1;
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

    void InsertarBalanceado(pNodoCita& ra, bool& Hh,
        int IdCita, int IdPaciente, int IdMedico, int IdEspecialidad,
        string Fecha, string Hora, string Motivo, string Estado)
    {
        NodoCita* n1;

        if (ra == nullptr)
        {
            ra = new NodoCita(IdCita, IdPaciente, IdMedico, IdEspecialidad, Fecha, Hora, Motivo, Estado);
            Hh = true;
        }
        else if (IdCita < ra->IdCita)
        {
            InsertarBalanceado(ra->Hizq, Hh, IdCita, IdPaciente, IdMedico, IdEspecialidad, Fecha, Hora, Motivo, Estado);
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
        else if (IdCita > ra->IdCita)
        {
            InsertarBalanceado(ra->Hder, Hh, IdCita, IdPaciente, IdMedico, IdEspecialidad, Fecha, Hora, Motivo, Estado);
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

    void Borrar(pNodoCita& nodo, bool& Hh)
    {
        pNodoCita q;
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

    void BorrarBalanceado(pNodoCita& subarbol, bool& Hh, int IdCita)
    {
        pNodoCita q;
        if (subarbol != nullptr)
        {
            if (IdCita < subarbol->IdCita)
            {
                BorrarBalanceado(subarbol->Hizq, Hh, IdCita);
                if (Hh)
                    Equilibrar1(subarbol, Hh);
            }
            else if (IdCita > subarbol->IdCita)
            {
                BorrarBalanceado(subarbol->Hder, Hh, IdCita);
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
                    pNodoCita reemplazo = q->Hizq;
                    while (reemplazo->Hder != nullptr)
                        reemplazo = reemplazo->Hder;

                    q->IdCita = reemplazo->IdCita;
                    q->IdPaciente = reemplazo->IdPaciente;
                    q->IdMedico = reemplazo->IdMedico;
                    q->IdEspecialidad = reemplazo->IdEspecialidad;
                    q->Fecha = reemplazo->Fecha;
                    q->Hora = reemplazo->Hora;
                    q->Motivo = reemplazo->Motivo;
                    q->Estado = reemplazo->Estado;

                    BorrarBalanceado(subarbol->Hizq, Hh, reemplazo->IdCita);
                    if (Hh)
                        Equilibrar1(subarbol, Hh);
                }
            }
        }
    }

    void RotacionDobleIzquierda(NodoCita*& n, NodoCita*& n1)
    {
        NodoCita* n2 = n1->Hder;
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

    void RotacionDobleDerecha(NodoCita*& n, NodoCita*& n1)
    {
        NodoCita* n2 = n1->Hizq;
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

    void RotacionSimpleDerecha(NodoCita*& n, NodoCita*& n1)
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

    void RotacionSimpleIzquierda(NodoCita*& n, NodoCita*& n1)
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

    void MostrarInorden(pNodoCita n)
    {
        if (n != nullptr)
        {
            MostrarInorden(n->Hizq);
            MostrarRegistro(n);
            MostrarInorden(n->Hder);
        }
    }

    void MostrarRegistro(pNodoCita n)
    {
        cout << "IdCita: " << n->IdCita
            << " | IdPaciente: " << n->IdPaciente
            << " | IdMedico: " << n->IdMedico
            << " | IdEspecialidad: " << n->IdEspecialidad
            << " | Fecha: " << n->Fecha
            << " | Hora: " << n->Hora
            << " | Motivo: " << n->Motivo
            << " | Estado: " << n->Estado << endl;
    }

    void LiberarMemoria(pNodoCita n)
    {
        if (n != nullptr)
        {
            LiberarMemoria(n->Hizq);
            LiberarMemoria(n->Hder);
            delete n;
        }
    }

    void BuscarIdsPorPaciente(pNodoCita n, int IdPaciente, vector<int>& ids)
    {
        if (n != nullptr)
        {
            BuscarIdsPorPaciente(n->Hizq, IdPaciente, ids);
            if (n->IdPaciente == IdPaciente)
                ids.push_back(n->IdCita);
            BuscarIdsPorPaciente(n->Hder, IdPaciente, ids);
        }
    }

    void BuscarIdsPorMedico(pNodoCita n, int IdMedico, vector<int>& ids)
    {
        if (n != nullptr)
        {
            BuscarIdsPorMedico(n->Hizq, IdMedico, ids);
            if (n->IdMedico == IdMedico)
                ids.push_back(n->IdCita);
            BuscarIdsPorMedico(n->Hder, IdMedico, ids);
        }
    }

public:
    ArbolAVLCitas() : raiz(nullptr) {}

    ~ArbolAVLCitas()
    {
        LiberarMemoria(raiz);
    }

    bool ListaVacia() const
    {
        return raiz == nullptr;
    }

    bool Existe(int IdCita) const
    {
        pNodoCita actual = raiz;
        while (actual != nullptr)
        {
            if (IdCita == actual->IdCita)
                return true;
            actual = (IdCita < actual->IdCita) ? actual->Hizq : actual->Hder;
        }
        return false;
    }

    bool Insertar(int IdCita, int IdPaciente, int IdMedico, int IdEspecialidad, string Fecha, string Hora, string Motivo, string Estado)
    {
        if (Existe(IdCita))
            return false;
        bool Hh = false;
        InsertarBalanceado(raiz, Hh, IdCita, IdPaciente, IdMedico, IdEspecialidad, Fecha, Hora, Motivo, Estado);
        return true;
    }

    bool Buscar(int IdCita) const
    {
        pNodoCita actual = raiz;
        while (actual != nullptr)
        {
            if (IdCita == actual->IdCita)
            {
                cout << "IdCita: " << actual->IdCita << endl;
                cout << "IdPaciente: " << actual->IdPaciente << endl;
                cout << "IdMedico: " << actual->IdMedico << endl;
                cout << "IdEspecialidad: " << actual->IdEspecialidad << endl;
                cout << "Fecha: " << actual->Fecha << endl;
                cout << "Hora: " << actual->Hora << endl;
                cout << "Motivo: " << actual->Motivo << endl;
                cout << "Estado: " << actual->Estado << endl;
                return true;
            }
            actual = (IdCita < actual->IdCita) ? actual->Hizq : actual->Hder;
        }
        return false;
    }

    bool Eliminar(int IdCita)
    {
        if (!Existe(IdCita))
            return false;
        bool Hh = false;
        BorrarBalanceado(raiz, Hh, IdCita);
        return true;
    }

    bool EliminarPorPaciente(int IdPaciente)
    {
        vector<int> ids;
        BuscarIdsPorPaciente(raiz, IdPaciente, ids);
        for (int id : ids)
            Eliminar(id);
        return !ids.empty();
    }

    bool EliminarPorMedico(int IdMedico)
    {
        vector<int> ids;
        BuscarIdsPorMedico(raiz, IdMedico, ids);
        for (int id : ids)
            Eliminar(id);
        return !ids.empty();
    }

    bool Modificar(int IdCita, int IdPaciente, int IdMedico, int IdEspecialidad,
        string Fecha, string Hora, string Motivo, string Estado)
    {
        pNodoCita actual = raiz;
        while (actual != nullptr)
        {
            if (IdCita == actual->IdCita)
            {
                actual->IdPaciente = IdPaciente;
                actual->IdMedico = IdMedico;
                actual->IdEspecialidad = IdEspecialidad;
                actual->Fecha = Fecha;
                actual->Hora = Hora;
                actual->Motivo = Motivo;
                actual->Estado = Estado;
                return true;
            }
            actual = (IdCita < actual->IdCita) ? actual->Hizq : actual->Hder;
        }
        return false;
    }

    void Mostrar()
    {
        if (ListaVacia())
        {
            cout << "No hay citas." << endl;
            return;
        }
        MostrarInorden(raiz);
    }






    void ReportePDFPorFecha(GeneradorPDF& pdf, const string& fecha, int& cantidad)
    {
        ReportePDFPorFechaRecursivo(pdf, raiz, fecha, cantidad);
    }

    void ReportePDFPorEspecialidad(GeneradorPDF& pdf, int IdEspecialidad, int& cantidad)
    {
        ReportePDFPorEspecialidadRecursivo(pdf, raiz, IdEspecialidad, cantidad);
    }

    void ReportePDFPorMedico(GeneradorPDF& pdf, int IdMedico, int& cantidad)
    {
        ReportePDFPorMedicoRecursivo(pdf, raiz, IdMedico, cantidad);
    }

private:
    void EscribirLineaCita(GeneradorPDF& pdf, pNodoCita n)
    {
        pdf.Linea(Rellenar(to_string(n->IdCita), 8) + Rellenar(to_string(n->IdPaciente), 12)
            + Rellenar(to_string(n->IdMedico), 10) + Rellenar(to_string(n->IdEspecialidad), 15)
            + Rellenar(n->Fecha, 12) + Rellenar(n->Hora, 8)
            + Rellenar(n->Motivo, 30) + Rellenar(n->Estado, 8));
    }

    void ReportePDFPorFechaRecursivo(GeneradorPDF& pdf, pNodoCita n, const string& fecha, int& cantidad)
    {
        if (n == nullptr) return;
        ReportePDFPorFechaRecursivo(pdf, n->Hizq, fecha, cantidad);
        if (n->Fecha == fecha) { EscribirLineaCita(pdf, n); cantidad++; }
        ReportePDFPorFechaRecursivo(pdf, n->Hder, fecha, cantidad);
    }

    void ReportePDFPorEspecialidadRecursivo(GeneradorPDF& pdf, pNodoCita n, int IdEspecialidad, int& cantidad)
    {
        if (n == nullptr) return;
        ReportePDFPorEspecialidadRecursivo(pdf, n->Hizq, IdEspecialidad, cantidad);
        if (n->IdEspecialidad == IdEspecialidad) { EscribirLineaCita(pdf, n); cantidad++; }
        ReportePDFPorEspecialidadRecursivo(pdf, n->Hder, IdEspecialidad, cantidad);
    }

    void ReportePDFPorMedicoRecursivo(GeneradorPDF& pdf, pNodoCita n, int IdMedico, int& cantidad)
    {
        if (n == nullptr) return;
        ReportePDFPorMedicoRecursivo(pdf, n->Hizq, IdMedico, cantidad);
        if (n->IdMedico == IdMedico) { EscribirLineaCita(pdf, n); cantidad++; }
        ReportePDFPorMedicoRecursivo(pdf, n->Hder, IdMedico, cantidad);
    }
};

void ReporteCitasPorFecha(ArbolAVLCitas& Citas)
{
    string fecha;
    cout << "Ingrese la fecha (YYYY-MM-DD): ";
    cin >> fecha;
    GeneradorPDF pdf("Reporte_Citas_" + fecha + ".pdf", "REPORTE DE CITAS POR FECHA: " + fecha,
        Rellenar("ID CITA", 8) + Rellenar("ID PACIENTE", 12) + Rellenar("ID MEDICO", 10)
        + Rellenar("ID ESPECIALIDAD", 15) + Rellenar("FECHA", 12) + Rellenar("HORA", 8)
        + Rellenar("MOTIVO", 30) + Rellenar("ESTADO", 8));
    if (!pdf.Valido()) { cout << "No se pudo crear el documento PDF." << endl; return; }
    int cantidad = 0;
    Citas.ReportePDFPorFecha(pdf, fecha, cantidad);
    if (cantidad == 0) pdf.Linea("(No hay citas registradas para esta fecha)");
    if (pdf.Guardar()) cout << "Reporte generado: Reporte_Citas_" << fecha << ".pdf" << endl;
    else cout << "No se pudo guardar el archivo PDF." << endl;
}

void ReporteCitasPorEspecialidad(ArbolAVLCitas& Citas)
{
    int IdEspecialidad;
    cout << "Ingrese el IdEspecialidad: ";
    cin >> IdEspecialidad;
    GeneradorPDF pdf("Reporte_Citas_Especialidad_" + to_string(IdEspecialidad) + ".pdf",
        "REPORTE DE CITAS POR ESPECIALIDAD",
        Rellenar("ID CITA", 8) + Rellenar("ID PACIENTE", 12) + Rellenar("ID MEDICO", 10)
        + Rellenar("ID ESPECIALIDAD", 15) + Rellenar("FECHA", 12) + Rellenar("HORA", 8)
        + Rellenar("MOTIVO", 30) + Rellenar("ESTADO", 8));
    if (!pdf.Valido()) { cout << "No se pudo crear el documento PDF." << endl; return; }
    int cantidad = 0;
    Citas.ReportePDFPorEspecialidad(pdf, IdEspecialidad, cantidad);
    if (cantidad == 0) pdf.Linea("(No hay citas registradas para esta especialidad)");
    if (pdf.Guardar()) cout << "Reporte generado: Reporte_Citas_Especialidad_" << IdEspecialidad << ".pdf" << endl;
    else cout << "No se pudo guardar el archivo PDF." << endl;
}

void ReporteCitasPorMedico(ArbolAVLCitas& Citas)
{
    int IdMedico;
    cout << "Ingrese el IdMedico: ";
    cin >> IdMedico;
    GeneradorPDF pdf("Reporte_Citas_Medico_" + to_string(IdMedico) + ".pdf",
        "REPORTE DE CITAS POR MEDICO",
        Rellenar("ID CITA", 8) + Rellenar("ID PACIENTE", 12) + Rellenar("ID MEDICO", 10)
        + Rellenar("ID ESPECIALIDAD", 15) + Rellenar("FECHA", 12) + Rellenar("HORA", 8)
        + Rellenar("MOTIVO", 30) + Rellenar("ESTADO", 8));
    if (!pdf.Valido()) { cout << "No se pudo crear el documento PDF." << endl; return; }
    int cantidad = 0;
    Citas.ReportePDFPorMedico(pdf, IdMedico, cantidad);
    if (cantidad == 0) pdf.Linea("(No hay citas registradas para este medico)");
    if (pdf.Guardar()) cout << "Reporte generado: Reporte_Citas_Medico_" << IdMedico << ".pdf" << endl;
    else cout << "No se pudo guardar el archivo PDF." << endl;
}



void CargarCitas(ArbolAVLCitas& Citas, ArbolBPacientes& Pacientes,
    ArbolBMedicos& Medicos, ArbolAAEspecialidades& Especialidades)
{
    ifstream archivo("Citas.csv");
    if (!archivo) { cout << "No se pudo abrir el archivo Citas.csv" << endl; return; }

    string encabezado;
    getline(archivo, encabezado); // IdCita;IdPaciente;IdMedico;IdEspecialidad;Fecha;Hora;Motivo;Estado
    string IdCitaTexto, IdPacienteTexto, IdMedicoTexto, IdEspecialidadTexto;
    string Fecha, Hora, Motivo, Estado;
    while (getline(archivo, IdCitaTexto, ';') && getline(archivo, IdPacienteTexto, ';') &&
        getline(archivo, IdMedicoTexto, ';') && getline(archivo, IdEspecialidadTexto, ';') &&
        getline(archivo, Fecha, ';') && getline(archivo, Hora, ';') &&
        getline(archivo, Motivo, ';') && getline(archivo, Estado))
    {
        try
        {
            int IdCita = stoi(IdCitaTexto);
            int IdPaciente = stoi(IdPacienteTexto);
            int IdMedico = stoi(IdMedicoTexto);
            int IdEspecialidad = stoi(IdEspecialidadTexto);
            if (!Pacientes.Existe(IdPaciente) || !Medicos.Existe(IdMedico) ||
                !Especialidades.Existe(IdEspecialidad) || Citas.Existe(IdCita))
                continue;
            if (Estado != "0" && Estado != "1") continue;
            Citas.Insertar(IdCita, IdPaciente, IdMedico, IdEspecialidad, Fecha, Hora, Motivo, Estado);
        }
        catch (...) { continue; }
    }
    archivo.close();
    cout << "Citas cargadas correctamente." << endl;
}
