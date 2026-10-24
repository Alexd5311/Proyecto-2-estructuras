#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

//╔══════════════════════════════════════════════════════════════════════╗
//║                         REGISTRO DE PACIENTE                         ║
//╚══════════════════════════════════════════════════════════════════════╝

struct RegistroPaciente
{
    int IdPaciente;
    string Nombre;
    string FechaNacimiento;
    string Telefono;
    int CodCiudad;
    int CodPais;
    string Correo;
};

//╔══════════════════════════════════════════════════════════════════════╗
//║                       NODO DEL ARBOL B                               ║
//╚══════════════════════════════════════════════════════════════════════╝

const int MAX_CLAVES_PACIENTES = 4;

class NodoBPacientes
{
public:
    int Cuenta;
    RegistroPaciente Claves[MAX_CLAVES_PACIENTES + 1];
    NodoBPacientes* Ramas[MAX_CLAVES_PACIENTES + 2];

    NodoBPacientes()
    {
        Cuenta = 0;
        for (int i = 0; i < MAX_CLAVES_PACIENTES + 2; i++)
            Ramas[i] = nullptr;
    }
};

//╔══════════════════════════════════════════════════════════════════════╗
//║                        ARBOL B DE PACIENTES                          ║
//╚══════════════════════════════════════════════════════════════════════╝

class ArbolBPacientes
{
private:
    NodoBPacientes* Raiz;

    void Destruir(NodoBPacientes* P)
    {
        if (P == nullptr) return;
        for (int i = 0; i <= P->Cuenta; i++)
            Destruir(P->Ramas[i]);
        delete P;
    }

    int Posicion(NodoBPacientes* P, int IdPaciente)
    {
        int i = 0;
        while (i < P->Cuenta && IdPaciente > P->Claves[i].IdPaciente)
            i++;
        return i;
    }

    RegistroPaciente* BuscarRegistro(NodoBPacientes* P, int IdPaciente)
    {
        if (P == nullptr) return nullptr;
        int i = Posicion(P, IdPaciente);
        if (i < P->Cuenta && P->Claves[i].IdPaciente == IdPaciente)
            return &P->Claves[i];
        return BuscarRegistro(P->Ramas[i], IdPaciente);
    }

    // Inserta recursivamente. Si un nodo llega a cinco claves, lo divide.
    bool InsertarRecursivo(NodoBPacientes* P, const RegistroPaciente& Registro,
        bool& Dividido, RegistroPaciente& Promovido, NodoBPacientes*& RamaDerecha)
    {
        if (P == nullptr)
        {
            Dividido = true;
            Promovido = Registro;
            RamaDerecha = nullptr;
            return true;
        }

        int K = Posicion(P, Registro.IdPaciente);
        if (K < P->Cuenta && P->Claves[K].IdPaciente == Registro.IdPaciente)
        {
            Dividido = false;
            return false;
        }

        bool Resultado = InsertarRecursivo(P->Ramas[K], Registro, Dividido,
            Promovido, RamaDerecha);
        if (!Resultado) return false;
        if (!Dividido) return true;

        // Hay espacio: insertar la clave promovida en este nodo.
        if (P->Cuenta < MAX_CLAVES_PACIENTES)
        {
            for (int i = P->Cuenta; i > K; i--)
                P->Claves[i] = P->Claves[i - 1];
            for (int i = P->Cuenta + 1; i > K + 1; i--)
                P->Ramas[i] = P->Ramas[i - 1];
            P->Claves[K] = Promovido;
            P->Ramas[K + 1] = RamaDerecha;
            P->Cuenta++;
            Dividido = false;
            return true;
        }

        // Nodo lleno: se crea temporalmente un arreglo de cinco claves.
        RegistroPaciente ClavesTemp[MAX_CLAVES_PACIENTES + 1];
        NodoBPacientes* RamasTemp[MAX_CLAVES_PACIENTES + 2];

        for (int i = 0; i < K; i++) ClavesTemp[i] = P->Claves[i];
        ClavesTemp[K] = Promovido;
        for (int i = K; i < P->Cuenta; i++) ClavesTemp[i + 1] = P->Claves[i];

        for (int i = 0; i <= K; i++) RamasTemp[i] = P->Ramas[i];
        RamasTemp[K + 1] = RamaDerecha;
        for (int i = K + 1; i <= P->Cuenta; i++) RamasTemp[i + 1] = P->Ramas[i];

        const int Medio = 2;
        NodoBPacientes* NuevaRama = new NodoBPacientes();
        P->Cuenta = Medio;
        for (int i = 0; i < Medio; i++) P->Claves[i] = ClavesTemp[i];
        for (int i = 0; i <= Medio; i++) P->Ramas[i] = RamasTemp[i];
        for (int i = Medio; i < MAX_CLAVES_PACIENTES + 1; i++)
            P->Ramas[i + 1] = nullptr;

        NuevaRama->Cuenta = MAX_CLAVES_PACIENTES - Medio;
        for (int i = 0; i < NuevaRama->Cuenta; i++)
            NuevaRama->Claves[i] = ClavesTemp[Medio + 1 + i];
        for (int i = 0; i <= NuevaRama->Cuenta; i++)
            NuevaRama->Ramas[i] = RamasTemp[Medio + 1 + i];

        Promovido = ClavesTemp[Medio];
        RamaDerecha = NuevaRama;
        Dividido = true;
        return true;
    }

    void Recopilar(NodoBPacientes* P, vector<RegistroPaciente>& Registros)
    {
        if (P == nullptr) return;
        for (int i = 0; i < P->Cuenta; i++)
        {
            Recopilar(P->Ramas[i], Registros);
            Registros.push_back(P->Claves[i]);
        }
        Recopilar(P->Ramas[P->Cuenta], Registros);
    }

    void MostrarRecursivo(NodoBPacientes* P)
    {
        if (P == nullptr) return;
        for (int i = 0; i < P->Cuenta; i++)
        {
            MostrarRecursivo(P->Ramas[i]);
            const RegistroPaciente& R = P->Claves[i];
            cout << "IdPaciente: " << R.IdPaciente
                << " | Nombre: " << R.Nombre
                << " | FechaNacimiento: " << R.FechaNacimiento
                << " | Telefono: " << R.Telefono
                << " | CodCiudad: " << R.CodCiudad
                << " | CodPais: " << R.CodPais
                << " | Correo: " << R.Correo << endl;
        }
        MostrarRecursivo(P->Ramas[P->Cuenta]);
    }

public:
    ArbolBPacientes() { Raiz = nullptr; }

    ~ArbolBPacientes() { Destruir(Raiz); }

    bool ListaVacia() { return Raiz == nullptr; }

    bool Existe(int IdPaciente)
    {
        return BuscarRegistro(Raiz, IdPaciente) != nullptr;
    }

    bool Insertar(int IdPaciente, string Nombre, string FechaNacimiento,
        string Telefono, int CodCiudad, int CodPais, string Correo)
    {
        RegistroPaciente Registro = { IdPaciente, Nombre, FechaNacimiento,
            Telefono, CodCiudad, CodPais, Correo };
        bool Dividido = false;
        RegistroPaciente Promovido{};
        NodoBPacientes* RamaDerecha = nullptr;

        bool Resultado = InsertarRecursivo(Raiz, Registro, Dividido,
            Promovido, RamaDerecha);
        if (!Resultado) return false;

        if (Dividido)
        {
            NodoBPacientes* NuevaRaiz = new NodoBPacientes();
            NuevaRaiz->Claves[0] = Promovido;
            NuevaRaiz->Ramas[0] = Raiz;
            NuevaRaiz->Ramas[1] = RamaDerecha;
            NuevaRaiz->Cuenta = 1;
            Raiz = NuevaRaiz;
        }
        return true;
    }

    bool Buscar(int IdPaciente)
    {
        RegistroPaciente* R = BuscarRegistro(Raiz, IdPaciente);
        if (R == nullptr) return false;
        cout << "IdPaciente: " << R->IdPaciente << endl;
        cout << "Nombre: " << R->Nombre << endl;
        cout << "FechaNacimiento: " << R->FechaNacimiento << endl;
        cout << "Telefono: " << R->Telefono << endl;
        cout << "CodCiudad: " << R->CodCiudad << endl;
        cout << "CodPais: " << R->CodPais << endl;
        cout << "Correo: " << R->Correo << endl;
        return true;
    }

    bool Eliminar(int IdPaciente)
    {
        vector<RegistroPaciente> Registros;
        Recopilar(Raiz, Registros);
        bool Encontrado = false;
        Destruir(Raiz);
        Raiz = nullptr;
        for (const RegistroPaciente& R : Registros)
        {
            if (R.IdPaciente == IdPaciente)
            {
                Encontrado = true;
                continue;
            }
            Insertar(R.IdPaciente, R.Nombre, R.FechaNacimiento, R.Telefono,
                R.CodCiudad, R.CodPais, R.Correo);
        }
        return Encontrado;
    }

    bool Modificar(int IdPaciente, string Nombre, string FechaNacimiento,
        string Telefono, int CodCiudad, int CodPais, string Correo)
    {
        RegistroPaciente* R = BuscarRegistro(Raiz, IdPaciente);
        if (R == nullptr) return false;
        R->Nombre = Nombre;
        R->FechaNacimiento = FechaNacimiento;
        R->Telefono = Telefono;
        R->CodCiudad = CodCiudad;
        R->CodPais = CodPais;
        R->Correo = Correo;
        return true;
    }

    vector<int> ObtenerIdsPorPais(int CodPais)
    {
        vector<RegistroPaciente> Registros;
        vector<int> ids;
        Recopilar(Raiz, Registros);
        for (const RegistroPaciente& R : Registros)
            if (R.CodPais == CodPais) ids.push_back(R.IdPaciente);
        return ids;
    }

    vector<int> ObtenerIdsPorCiudad(int CodPais, int CodCiudad)
    {
        vector<RegistroPaciente> Registros;
        vector<int> ids;
        Recopilar(Raiz, Registros);
        for (const RegistroPaciente& R : Registros)
            if (R.CodPais == CodPais && R.CodCiudad == CodCiudad)
                ids.push_back(R.IdPaciente);
        return ids;
    }

    bool EliminarPorPais(int CodPais)
    {
        vector<int> ids = ObtenerIdsPorPais(CodPais);
        for (int Id : ids) Eliminar(Id);
        return !ids.empty();
    }

    bool EliminarPorCiudad(int CodPais, int CodCiudad)
    {
        vector<int> ids = ObtenerIdsPorCiudad(CodPais, CodCiudad);
        for (int Id : ids) Eliminar(Id);
        return !ids.empty();
    }

    void Mostrar()
    {
        cout << endl << "PACIENTES (ARBOL B)" << endl;
        cout << "-------------------" << endl;
        MostrarRecursivo(Raiz);
    }
};




// Alias temporal para que los módulos existentes sigan compilando mientras
// se actualizan sus nombres de tipo. La implementación ya no utiliza hashing.
using TablaHashingPacientes = ArbolBPacientes;








//╔══════════════════════════════════════════════════════════════════════╗
//║                         CARGAR PACIENTES                             ║
//╚══════════════════════════════════════════════════════════════════════╝

void CargarPacientes(ArbolBPacientes& Pacientes, ListaCiudades& Ciudades)
{
    ifstream archivo("Pacientes.csv");
    if (!archivo)
    {
        cout << "No se pudo abrir el archivo Pacientes.csv" << endl;
        return;
    }

    string encabezado;
    getline(archivo, encabezado); // Saltar encabezado del CSV

    string IdPacienteTexto, Nombre, FechaNacimiento, Telefono;
    string CodPaisTexto, CodCiudadTexto, Correo;

    while (getline(archivo, IdPacienteTexto, ';') &&
        getline(archivo, Nombre, ';') &&
        getline(archivo, FechaNacimiento, ';') &&
        getline(archivo, Telefono, ';') &&
        getline(archivo, CodPaisTexto, ';') &&
        getline(archivo, CodCiudadTexto, ';') &&
        getline(archivo, Correo))
    {
        try
        {
            int IdPaciente = stoi(IdPacienteTexto);
            int CodPais = stoi(CodPaisTexto);
            int CodCiudad = stoi(CodCiudadTexto);

            // Se conserva la primera aparición de cada IdPaciente.
            if (Pacientes.Existe(IdPaciente)) continue;
            if (!Ciudades.Existe(CodPais, CodCiudad)) continue;

            Pacientes.Insertar(IdPaciente, Nombre, FechaNacimiento, Telefono,
                CodCiudad, CodPais, Correo);
        }
        catch (const exception&)
        {
            // Si una fila tiene datos numéricos inválidos, se omite.
            continue;
        }
    }

    archivo.close();
    cout << "Pacientes cargados correctamente." << endl;
}
