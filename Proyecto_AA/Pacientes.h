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
    static constexpr int MIN = 2;

    // ╬ Limpia el arbol al inicio ╬
    void Destruir(NodoBPacientes* P)
    {
        if (P == nullptr) return;
        for (int i = 0; i <= P->Cuenta; i++)
            Destruir(P->Ramas[i]);
        delete P;
    }

    // ╬ Busca mediante el IdPaciente ╬
    int Posicion(NodoBPacientes* P, int IdPaciente)
    {
        int i = 0;
        while (i < P->Cuenta && IdPaciente > P->Claves[i].IdPaciente)
            i++;
        return i;
    }

    // ╬ 
    RegistroPaciente* BuscarRegistro(NodoBPacientes* P, int IdPaciente)
    {
        if (P == nullptr) return nullptr;
        int i = Posicion(P, IdPaciente);
        if (i < P->Cuenta && P->Claves[i].IdPaciente == IdPaciente)
            return &P->Claves[i];
        return BuscarRegistro(P->Ramas[i], IdPaciente);
    }

    // ╬ Es el insertar, en el esta el Empujar, MeterHoja y DividirNodo ╬
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

        // ╚ Hay espacio para añadir
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

        //╚ No hay espacio para añadir
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

    // ╬ Guarda en un vector toda la ifnromacion del arbol para otros metodos ╬ 
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




    // ╬ Elimina una clave de una pagina hoja. ╬
    void QuitarClave(NodoBPacientes* P, int K)
    {
        for (int i = K; i < P->Cuenta - 1; i++)
            P->Claves[i] = P->Claves[i + 1];

        P->Cuenta--;
    }

    // ╬ Devuelve el indice de la primera clave mayor o igual al ID. ╬ 
    int BuscarPosicion(NodoBPacientes* P, int IdPaciente)
    {
        int K = 0;

        while (K < P->Cuenta &&
            P->Claves[K].IdPaciente < IdPaciente)
            K++;

        return K;
    }

    // ╬ Combina el hijo izquierdo, la clave y el hijo derecho. ╬
    // ╬ El hijo izquierdo conserva la pagina resultante.       ╬
    void Combinar(NodoBPacientes* P, int K)
    {
        NodoBPacientes* Izquierdo = P->Ramas[K];
        NodoBPacientes* Derecho = P->Ramas[K + 1];

        int Pos = Izquierdo->Cuenta;

        // ╚ Bajar la clave separadora al hijo izquierdo.
        Izquierdo->Claves[Pos] = P->Claves[K];
        Pos++;

        // ╚ Copiar las claves del hijo derecho.
        for (int i = 0; i < Derecho->Cuenta; i++)
            Izquierdo->Claves[Pos + i] = Derecho->Claves[i];

        // ╚ Copiar las ramas del hijo derecho.
        for (int i = 0; i <= Derecho->Cuenta; i++)
            Izquierdo->Ramas[Pos + i] = Derecho->Ramas[i];

        Izquierdo->Cuenta += Derecho->Cuenta + 1;

        // ╚ Quitar la clave separadora del padre.
        for (int i = K; i < P->Cuenta - 1; i++)
            P->Claves[i] = P->Claves[i + 1];

        // ╚ Quitar la referencia a la pagina derecha.
        for (int i = K + 1; i < P->Cuenta; i++)
            P->Ramas[i] = P->Ramas[i + 1];

        P->Ramas[P->Cuenta] = nullptr;
        P->Cuenta--;

        delete Derecho;
    }

    // ╬ Presta una clave del hermano izquierdo o derecho. ╬ 
    // ╬ Si ninguno puede prestar, combina las paginas.    ╬ 
    void Restablecer(NodoBPacientes* P, int K)
    {
        NodoBPacientes* Hijo = P->Ramas[K];

        // Intentar prestar del hermano izquierdo.
        if (K > 0 && P->Ramas[K - 1]->Cuenta > MIN)
        {
            NodoBPacientes* Izquierdo = P->Ramas[K - 1];

            // Desplazar las claves del hijo hacia la derecha.
            for (int i = Hijo->Cuenta; i > 0; i--)
                Hijo->Claves[i] = Hijo->Claves[i - 1];

            // Desplazar las ramas del hijo.
            for (int i = Hijo->Cuenta + 1; i > 0; i--)
                Hijo->Ramas[i] = Hijo->Ramas[i - 1];

            // Bajar la clave del padre.
            Hijo->Claves[0] = P->Claves[K - 1];

            // Subir la clave mayor del hermano izquierdo.
            P->Claves[K - 1] = Izquierdo->Claves[Izquierdo->Cuenta - 1];

            // Transferir la rama correspondiente.
            Hijo->Ramas[0] = Izquierdo->Ramas[Izquierdo->Cuenta];

            Izquierdo->Ramas[Izquierdo->Cuenta] = nullptr;
            Izquierdo->Cuenta--;
            Hijo->Cuenta++;
            return;
        }

        // ╚ Intentar prestar del hermano derecho.
        if (K < P->Cuenta && P->Ramas[K + 1]->Cuenta > MIN)
        {
            NodoBPacientes* Derecho = P->Ramas[K + 1];

            // Bajar la clave del padre al hijo.
            Hijo->Claves[Hijo->Cuenta] = P->Claves[K];
            Hijo->Ramas[Hijo->Cuenta + 1] = Derecho->Ramas[0];
            Hijo->Cuenta++;

            // Subir la primera clave del hermano derecho.
            P->Claves[K] = Derecho->Claves[0];

            // Desplazar las claves y ramas del hermano derecho.
            for (int i = 0; i < Derecho->Cuenta - 1; i++)
                Derecho->Claves[i] = Derecho->Claves[i + 1];

            for (int i = 0; i < Derecho->Cuenta; i++)
                Derecho->Ramas[i] = Derecho->Ramas[i + 1];

            Derecho->Ramas[Derecho->Cuenta] = nullptr;
            Derecho->Cuenta--;
            return;
        }

        // ╚ Ningun hermano puede prestar asi que llama combinar.
        if (K > 0)
            Combinar(P, K - 1);
        else
            Combinar(P, K);
    }

    // ╬ Metodo principal de eliminacion ╬
    bool EliminarRecursivo(NodoBPacientes* P, int IdPaciente)
    {
        if (P == nullptr)
            return false;

        int K = BuscarPosicion(P, IdPaciente);

        // La clave esta en esta pagina.
        if (K < P->Cuenta &&
            P->Claves[K].IdPaciente == IdPaciente)
        {
            // Caso 1: la clave esta en una hoja.
            if (P->Ramas[0] == nullptr)
            {
                QuitarClave(P, K);
                return true;
            }

            // Caso 2: la clave esta en una pagina interna.
            // Buscar el sucesor: la menor clave del hijo derecho.
            NodoBPacientes* Sucesor = P->Ramas[K + 1];

            while (Sucesor->Ramas[0] != nullptr)
                Sucesor = Sucesor->Ramas[0];

            RegistroPaciente RegistroSucesor =
                Sucesor->Claves[0];

            // Sustituir el registro completo.
            P->Claves[K] = RegistroSucesor;

            // Eliminar el sucesor de su ubicacion original.
            bool Eliminado = EliminarRecursivo(
                P->Ramas[K + 1],
                RegistroSucesor.IdPaciente
            );

            // Reparar si el hijo quedo por debajo del minimo.
            if (Eliminado && P->Ramas[K + 1]->Cuenta < MIN)
                Restablecer(P, K + 1);

            return Eliminado;
        }

        // La clave no esta en esta pagina.
        bool Eliminado = EliminarRecursivo(P->Ramas[K], IdPaciente);

        // Solo restablecer si la eliminacion tuvo exito.
        if (Eliminado &&
            P->Ramas[K] != nullptr &&
            P->Ramas[K]->Cuenta < MIN)
        {
            Restablecer(P, K);
        }

        return Eliminado;
    }


    // ╬ Metodo de printear todo ╬
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
        if (Raiz == nullptr)
            return false;

        bool Eliminado = EliminarRecursivo(Raiz, IdPaciente);

        // Si la raiz queda vacia, su unica rama pasa a ser
        // la nueva raiz.
        if (Raiz != nullptr && Raiz->Cuenta == 0)
        {
            NodoBPacientes* AntiguaRaiz = Raiz;
            Raiz = Raiz->Ramas[0];
            delete AntiguaRaiz;
        }

        return Eliminado;
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






//╔══════════════════════════════════════════════════════════════════════╗
//║                         CARGAR PACIENTES                             ║
//╚══════════════════════════════════════════════════════════════════════╝

void CargarPacientes(ArbolBPacientes& Pacientes, ArbolABBCiudades& Ciudades)
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
