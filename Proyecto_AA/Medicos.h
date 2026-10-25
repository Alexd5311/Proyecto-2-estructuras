#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
using namespace std;

struct RegistroMedico {
    int IdMedico;
    string Nombre;
    string Telefono;
    string Correo;
    int IdEspecialidad;
};

const int MAX_CLAVES_MEDICOS = 4;
const int MIN_CLAVES_MEDICOS = 2;

class NodoBMedicos {
public:
    int Cuenta;
    RegistroMedico Claves[MAX_CLAVES_MEDICOS + 1];
    NodoBMedicos* Ramas[MAX_CLAVES_MEDICOS + 2];

    NodoBMedicos() {
        Cuenta = 0;
        for (int i = 0; i < MAX_CLAVES_MEDICOS + 2; i++)
            Ramas[i] = nullptr;
    }
};

class ArbolBMedicos {
private:
    NodoBMedicos* Raiz;

    void Destruir(NodoBMedicos* P) {
        if (P == nullptr) return;
        for (int i = 0; i <= P->Cuenta; i++)
            Destruir(P->Ramas[i]);
        delete P;
    }

    int Posicion(NodoBMedicos* P, int IdMedico) const {
        int i = 0;
        while (i < P->Cuenta && IdMedico > P->Claves[i].IdMedico)
            i++;
        return i;
    }

    RegistroMedico* BuscarRegistro(NodoBMedicos* P, int IdMedico) {
        if (P == nullptr) return nullptr;

        int i = Posicion(P, IdMedico);
        if (i < P->Cuenta && P->Claves[i].IdMedico == IdMedico)
            return &P->Claves[i];

        return BuscarRegistro(P->Ramas[i], IdMedico);
    }

    bool InsertarRecursivo(NodoBMedicos* P, const RegistroMedico& Registro,
        bool& Dividido, RegistroMedico& Promovido, NodoBMedicos*& RamaDerecha) {
        if (P == nullptr) {
            Dividido = true;
            Promovido = Registro;
            RamaDerecha = nullptr;
            return true;
        }

        int K = Posicion(P, Registro.IdMedico);
        if (K < P->Cuenta&& P->Claves[K].IdMedico == Registro.IdMedico) {
            Dividido = false;
            return false;
        }

        bool Resultado = InsertarRecursivo(P->Ramas[K], Registro, Dividido,
            Promovido, RamaDerecha);
        if (!Resultado) return false;
        if (!Dividido) return true;

        if (P->Cuenta < MAX_CLAVES_MEDICOS) {
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

        RegistroMedico ClavesTemp[MAX_CLAVES_MEDICOS + 1];
        NodoBMedicos* RamasTemp[MAX_CLAVES_MEDICOS + 2];

        for (int i = 0; i < K; i++)
            ClavesTemp[i] = P->Claves[i];
        ClavesTemp[K] = Promovido;
        for (int i = K; i < P->Cuenta; i++)
            ClavesTemp[i + 1] = P->Claves[i];

        for (int i = 0; i <= K; i++)
            RamasTemp[i] = P->Ramas[i];
        RamasTemp[K + 1] = RamaDerecha;
        for (int i = K + 1; i <= P->Cuenta; i++)
            RamasTemp[i + 1] = P->Ramas[i];

        const int Medio = 2;
        NodoBMedicos* NuevaRama = new NodoBMedicos();

        P->Cuenta = Medio;
        for (int i = 0; i < Medio; i++)
            P->Claves[i] = ClavesTemp[i];
        for (int i = 0; i <= Medio; i++)
            P->Ramas[i] = RamasTemp[i];
        for (int i = Medio + 1; i < MAX_CLAVES_MEDICOS + 2; i++)
            P->Ramas[i] = nullptr;

        NuevaRama->Cuenta = MAX_CLAVES_MEDICOS - Medio;
        for (int i = 0; i < NuevaRama->Cuenta; i++)
            NuevaRama->Claves[i] = ClavesTemp[Medio + 1 + i];
        for (int i = 0; i <= NuevaRama->Cuenta; i++)
            NuevaRama->Ramas[i] = RamasTemp[Medio + 1 + i];

        Promovido = ClavesTemp[Medio];
        RamaDerecha = NuevaRama;
        Dividido = true;
        return true;
    }

    void Recopilar(NodoBMedicos* P, vector<RegistroMedico>& Registros) const {
        if (P == nullptr) return;
        for (int i = 0; i < P->Cuenta; i++) {
            Recopilar(P->Ramas[i], Registros);
            Registros.push_back(P->Claves[i]);
        }
        Recopilar(P->Ramas[P->Cuenta], Registros);
    }

    void MostrarRecursivo(NodoBMedicos* P) const {
        if (P == nullptr) return;
        for (int i = 0; i < P->Cuenta; i++) {
            MostrarRecursivo(P->Ramas[i]);
            const RegistroMedico& R = P->Claves[i];
            cout << "IdMedico: " << R.IdMedico
                << " | Nombre: " << R.Nombre
                << " | Telefono: " << R.Telefono
                << " | Correo: " << R.Correo
                << " | IdEspecialidad: " << R.IdEspecialidad << endl;
        }
        MostrarRecursivo(P->Ramas[P->Cuenta]);
    }

    void QuitarClave(NodoBMedicos* P, int K) {
        for (int i = K; i < P->Cuenta - 1; i++)
            P->Claves[i] = P->Claves[i + 1];
        P->Cuenta--;
    }

    int BuscarPosicion(NodoBMedicos* P, int IdMedico, bool& Encontrado) const {
        int K = 0;
        while (K < P->Cuenta && IdMedico > P->Claves[K].IdMedico)
            K++;
        Encontrado = (K < P->Cuenta && P->Claves[K].IdMedico == IdMedico);
        return K;
    }

    void Combinar(NodoBMedicos* P, int K) {
        NodoBMedicos* Izquierdo = P->Ramas[K - 1];
        NodoBMedicos* Derecho = P->Ramas[K];
        int Pos = Izquierdo->Cuenta;

        Izquierdo->Claves[Pos] = P->Claves[K - 1];
        Pos++;
        Izquierdo->Ramas[Pos] = Derecho->Ramas[0];

        for (int i = 0; i < Derecho->Cuenta; i++) {
            Izquierdo->Claves[Pos + i] = Derecho->Claves[i];
            Izquierdo->Ramas[Pos + i + 1] = Derecho->Ramas[i + 1];
        }
        Izquierdo->Cuenta += Derecho->Cuenta + 1;

        for (int i = K - 1; i < P->Cuenta - 1; i++)
            P->Claves[i] = P->Claves[i + 1];
        for (int i = K; i < P->Cuenta; i++)
            P->Ramas[i] = P->Ramas[i + 1];

        P->Cuenta--;
        P->Ramas[P->Cuenta + 1] = nullptr;
        delete Derecho;
    }

    void Restablecer(NodoBMedicos* P, int K) {
        if (K > 0 && P->Ramas[K - 1]->Cuenta > MIN_CLAVES_MEDICOS) {
            NodoBMedicos* Hijo = P->Ramas[K];
            NodoBMedicos* Izquierdo = P->Ramas[K - 1];

            for (int i = Hijo->Cuenta; i > 0; i--)
                Hijo->Claves[i] = Hijo->Claves[i - 1];
            for (int i = Hijo->Cuenta + 1; i > 0; i--)
                Hijo->Ramas[i] = Hijo->Ramas[i - 1];

            Hijo->Claves[0] = P->Claves[K - 1];
            Hijo->Ramas[0] = Izquierdo->Ramas[Izquierdo->Cuenta];
            Hijo->Cuenta++;

            P->Claves[K - 1] = Izquierdo->Claves[Izquierdo->Cuenta - 1];
            Izquierdo->Cuenta--;
            Izquierdo->Ramas[Izquierdo->Cuenta + 1] = nullptr;
        }
        else if (K < P->Cuenta && P->Ramas[K + 1]->Cuenta > MIN_CLAVES_MEDICOS) {
            NodoBMedicos* Hijo = P->Ramas[K];
            NodoBMedicos* Derecho = P->Ramas[K + 1];

            Hijo->Claves[Hijo->Cuenta] = P->Claves[K];
            Hijo->Ramas[Hijo->Cuenta + 1] = Derecho->Ramas[0];
            Hijo->Cuenta++;

            P->Claves[K] = Derecho->Claves[0];
            for (int i = 0; i < Derecho->Cuenta - 1; i++)
                Derecho->Claves[i] = Derecho->Claves[i + 1];
            for (int i = 0; i < Derecho->Cuenta; i++)
                Derecho->Ramas[i] = Derecho->Ramas[i + 1];
            Derecho->Cuenta--;
            Derecho->Ramas[Derecho->Cuenta + 1] = nullptr;
        }
        else {
            if (K > 0)
                Combinar(P, K);
            else
                Combinar(P, K + 1);
        }
    }

    bool EliminarRecursivo(NodoBMedicos* P, int IdMedico) {
        if (P == nullptr) return false;

        bool Encontrado = false;
        int K = BuscarPosicion(P, IdMedico, Encontrado);
        bool EsHoja = (P->Ramas[0] == nullptr);

        if (Encontrado) {
            if (EsHoja) {
                QuitarClave(P, K);
            }
            else {
                NodoBMedicos* Sucesor = P->Ramas[K + 1];
                while (Sucesor->Ramas[0] != nullptr)
                    Sucesor = Sucesor->Ramas[0];

                P->Claves[K] = Sucesor->Claves[0];
                bool Eliminado = EliminarRecursivo(P->Ramas[K + 1],
                    Sucesor->Claves[0].IdMedico);
                if (Eliminado && P->Ramas[K + 1] != nullptr &&
                    P->Ramas[K + 1]->Cuenta < MIN_CLAVES_MEDICOS)
                    Restablecer(P, K + 1);
            }
            return true;
        }

        bool Eliminado = EliminarRecursivo(P->Ramas[K], IdMedico);
        if (Eliminado && P->Ramas[K] != nullptr &&
            P->Ramas[K]->Cuenta < MIN_CLAVES_MEDICOS)
            Restablecer(P, K);
        return Eliminado;
    }

public:
    ArbolBMedicos() : Raiz(nullptr) {}

    ~ArbolBMedicos() {
        Destruir(Raiz);
    }

    bool ListaVacia() const {
        return Raiz == nullptr;
    }

    bool Existe(int IdMedico) {
        return BuscarRegistro(Raiz, IdMedico) != nullptr;
    }

    bool Insertar(int IdMedico, string Nombre, string Telefono,
        string Correo, int IdEspecialidad) {
        RegistroMedico Registro = { IdMedico, Nombre, Telefono, Correo, IdEspecialidad };
        bool Dividido = false;
        RegistroMedico Promovido{};
        NodoBMedicos* RamaDerecha = nullptr;

        bool Resultado = InsertarRecursivo(Raiz, Registro, Dividido,
            Promovido, RamaDerecha);
        if (!Resultado) return false;

        if (Dividido) {
            NodoBMedicos* NuevaRaiz = new NodoBMedicos();
            NuevaRaiz->Claves[0] = Promovido;
            NuevaRaiz->Ramas[0] = Raiz;
            NuevaRaiz->Ramas[1] = RamaDerecha;
            NuevaRaiz->Cuenta = 1;
            Raiz = NuevaRaiz;
        }
        return true;
    }

    bool Buscar(int IdMedico) {
        RegistroMedico* R = BuscarRegistro(Raiz, IdMedico);
        if (R == nullptr) return false;

        cout << "IdMedico: " << R->IdMedico << endl;
        cout << "Nombre: " << R->Nombre << endl;
        cout << "Telefono: " << R->Telefono << endl;
        cout << "Correo: " << R->Correo << endl;
        cout << "IdEspecialidad: " << R->IdEspecialidad << endl;
        return true;
    }

    bool Eliminar(int IdMedico) {
        if (!EliminarRecursivo(Raiz, IdMedico))
            return false;

        if (Raiz != nullptr && Raiz->Cuenta == 0) {
            NodoBMedicos* ViejaRaiz = Raiz;
            Raiz = Raiz->Ramas[0];
            ViejaRaiz->Ramas[0] = nullptr;
            delete ViejaRaiz;
        }
        return true;
    }

    bool Modificar(int IdMedico, string Nombre, string Telefono,
        string Correo, int IdEspecialidad) {
        RegistroMedico* R = BuscarRegistro(Raiz, IdMedico);
        if (R == nullptr) return false;
        R->Nombre = Nombre;
        R->Telefono = Telefono;
        R->Correo = Correo;
        R->IdEspecialidad = IdEspecialidad;
        return true;
    }

    vector<int> ObtenerIdsPorEspecialidad(int IdEspecialidad) const {
        vector<RegistroMedico> Registros;
        vector<int> ids;
        Recopilar(Raiz, Registros);
        for (const RegistroMedico& R : Registros)
            if (R.IdEspecialidad == IdEspecialidad)
                ids.push_back(R.IdMedico);
        return ids;
    }

    bool EliminarPorEspecialidad(int IdEspecialidad) {
        vector<int> ids = ObtenerIdsPorEspecialidad(IdEspecialidad);
        for (int Id : ids)
            Eliminar(Id);
        return !ids.empty();
    }

    void Mostrar() const {
        cout << endl << "MEDICOS (ARBOL B)" << endl;
        cout << "-----------------" << endl;
        MostrarRecursivo(Raiz);
    }

    int ReportePDF(GeneradorPDF& pdf) {
        vector<RegistroMedico> Registros;
        Recopilar(Raiz, Registros);
        for (const RegistroMedico& R : Registros) {
            pdf.Linea(Rellenar(to_string(R.IdMedico), 8) + Rellenar(R.Nombre, 28)
                + Rellenar(R.Telefono, 14) + Rellenar(R.Correo, 36)
                + Rellenar(to_string(R.IdEspecialidad), 8));
        }
        return static_cast<int>(Registros.size());
    }

    int ReportePDFPorEspecialidad(GeneradorPDF& pdf, int IdEspecialidad) {
        vector<RegistroMedico> Registros;
        Recopilar(Raiz, Registros);
        int cantidad = 0;
        for (const RegistroMedico& R : Registros) {
            if (R.IdEspecialidad == IdEspecialidad) {
                pdf.Linea(Rellenar(to_string(R.IdMedico), 8) + Rellenar(R.Nombre, 28)
                    + Rellenar(R.Telefono, 14) + Rellenar(R.Correo, 36)
                    + Rellenar(to_string(R.IdEspecialidad), 8));
                cantidad++;
            }
        }
        return cantidad;
    }
};

void ReporteMedicos(ArbolBMedicos& Medicos) {
    GeneradorPDF pdf("Reporte_Medicos.pdf", "REPORTE DE MEDICOS",
        Rellenar("ID", 8) + Rellenar("NOMBRE", 28) + Rellenar("TELEFONO", 14)
        + Rellenar("CORREO", 36) + Rellenar("ESPEC.", 8));

    if (!pdf.Valido()) {
        cout << "No se pudo crear el documento PDF." << endl;
        return;
    }
    if (Medicos.ReportePDF(pdf) == 0)
        pdf.Linea("(No hay medicos registrados)");

    if (pdf.Guardar())
        cout << "Reporte generado: Reporte_Medicos.pdf" << endl;
    else
        cout << "No se pudo guardar el archivo PDF." << endl;
}

void ReporteMedicosPorEspecialidad(ArbolBMedicos& Medicos) {
    int IdEspecialidad;
    cout << "Ingrese el IdEspecialidad: ";
    cin >> IdEspecialidad;

    GeneradorPDF pdf("Reporte_Medicos_Especialidad_" + to_string(IdEspecialidad) + ".pdf",
        "REPORTE DE MEDICOS POR ESPECIALIDAD",
        Rellenar("ID", 8) + Rellenar("NOMBRE", 28) + Rellenar("TELEFONO", 14)
        + Rellenar("CORREO", 36) + Rellenar("ESPEC.", 8));

    if (!pdf.Valido()) {
        cout << "No se pudo crear el documento PDF." << endl;
        return;
    }
    if (Medicos.ReportePDFPorEspecialidad(pdf, IdEspecialidad) == 0)
        pdf.Linea("(No hay medicos registrados para esta especialidad)");

    if (pdf.Guardar())
        cout << "Reporte generado: Reporte_Medicos_Especialidad_" << IdEspecialidad << ".pdf" << endl;
    else
        cout << "No se pudo guardar el archivo PDF." << endl;
}

void CargarMedicos(ArbolBMedicos& Medicos, ListaEspecialidades& Especialidades) {
    ifstream archivo("Medicos.csv");
    if (!archivo) {
        cout << "No se pudo abrir el archivo Medicos.csv" << endl;
        return;
    }

    string encabezado;
    getline(archivo, encabezado); // Idmedico;Nombre;Telefono;Correo;Idespecialidad

    string IdMedicoTexto, Nombre, Telefono, Correo, IdEspecialidadTexto;
    while (getline(archivo, IdMedicoTexto, ';') &&
        getline(archivo, Nombre, ';') &&
        getline(archivo, Telefono, ';') &&
        getline(archivo, Correo, ';') &&
        getline(archivo, IdEspecialidadTexto)) {
        try {
            int IdMedico = stoi(IdMedicoTexto);
            int IdEspecialidad = stoi(IdEspecialidadTexto);

            if (!Especialidades.Existe(IdEspecialidad))
                continue;
            if (Medicos.Existe(IdMedico))
                continue; // Conserva la primera aparicion del ID.

            Medicos.Insertar(IdMedico, Nombre, Telefono, Correo, IdEspecialidad);
        }
        catch (const invalid_argument&) {
            continue;
        }
        catch (const out_of_range&) {
            continue;
        }
    }

    archivo.close();
    cout << "Medicos cargados correctamente." << endl;
}
