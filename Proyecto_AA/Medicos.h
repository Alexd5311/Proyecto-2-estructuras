#pragma once

class NodoMedico
{
public:

    NodoMedico(int nuevoIdMedico, string nuevoNombre, string nuevoTelefono, 
        string nuevoCorreo, int nuevoIdEspecialidad)
    {
        IdMedico = nuevoIdMedico;
        Nombre = nuevoNombre;
        Telefono = nuevoTelefono;
        Correo = nuevoCorreo;
        IdEspecialidad = nuevoIdEspecialidad;
        siguiente = NULL;
    }

private:
    int IdMedico;
    string Nombre;
    string Telefono;
    string Correo;
    int IdEspecialidad;
    NodoMedico *siguiente;

    friend class ListaMedicos;
};

typedef NodoMedico *pnodoMedico;



//╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
//║                                               HASHING MEDICOS                                               ║
//╚═════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
class ListaMedicos
{
public:
    ListaMedicos(){
        primero = NULL;
    }
    ~ListaMedicos(){
        pnodoMedico aux = primero;
        while (aux != NULL){
            pnodoMedico temp = aux;
            aux = aux->siguiente;
            delete temp;
        }
    }

    bool ListaVacia(){
        return primero == NULL;
    }

//╔═══════════════════════════════════════════════════════════════╗
//║                         VALIDACION                            ║
//╚═══════════════════════════════════════════════════════════════╝
    bool Existe(int IdMedico){
        pnodoMedico aux = primero;

        while (aux != NULL){
            if (aux->IdMedico == IdMedico){
                return true;
            }
            aux = aux->siguiente;
        }

        return false;
    }

//╔═══════════════════════════════════════════════════════════════╗
//║                          INSERTAR                             ║
//╚═══════════════════════════════════════════════════════════════╝
    bool Insertar(int IdMedico, string Nombre, string Telefono, 
        string Correo, int IdEspecialidad){
        if (Existe(IdMedico)){
            return false;
        }
        if (ListaVacia()){
            primero = new NodoMedico(
                IdMedico, Nombre, Telefono, Correo, IdEspecialidad
            );
        }
        else{
            pnodoMedico aux = primero;
            while (aux->siguiente != NULL){
                aux = aux->siguiente;
            }
            aux->siguiente = new NodoMedico(
                IdMedico, Nombre, Telefono, Correo, IdEspecialidad
            );
        }
        return true;
    }

//╔═════════════════════════════════════════════════════════════════╗
//║                             BUSCAR                              ║
//╚═════════════════════════════════════════════════════════════════╝
    bool Buscar(int IdMedico)
    {
        pnodoMedico aux = primero;

        while (aux != NULL){
            if (aux->IdMedico == IdMedico){
                cout << "IdMedico: " << aux->IdMedico << endl;
                cout << "Nombre: " << aux->Nombre << endl;
                cout << "Telefono: " << aux->Telefono << endl;
                cout << "Correo: " << aux->Correo << endl;
                cout << "IdEspecialidad: " << aux->IdEspecialidad << endl;
                return true;
            }
            aux = aux->siguiente;
        }
        return false;
    }


//╔═══════════════════════════════════════════════════════════════════╗
//║                              ELIMINAR                              ║
//╚═══════════════════════════════════════════════════════════════════╝


    bool Eliminar(int IdMedico){
        if (ListaVacia()){
            return false;
        }
        if (primero->IdMedico==IdMedico){
            pnodoMedico temp=primero;
            primero= primero->siguiente;
            delete temp;
            return true;
        }
        pnodoMedico aux = primero;
        while (aux->siguiente!= NULL && aux->siguiente->IdMedico!= IdMedico){
            aux = aux->siguiente;
        }
        if (aux->siguiente==NULL){
            return false;
        }
        pnodoMedico temp = aux->siguiente;
        aux->siguiente = aux->siguiente->siguiente;
        delete temp;
        return true;
    }


//╔═════════════════════════════════════════════════════════════════╗
//║                             Modificar                           ║
//╚═════════════════════════════════════════════════════════════════╝


    bool Modificar(int IdMedico, string Nombre, string Telefono, 
        string Correo, int IdEspecialidad)
    {
        pnodoMedico aux = primero;
        while (aux != NULL){
            if (aux->IdMedico == IdMedico){
                aux->Nombre = Nombre;
                aux->Telefono = Telefono;
                aux->Correo = Correo;
                aux->IdEspecialidad = IdEspecialidad;
                return true;
            }
            aux = aux->siguiente;
        }
        return false;
    }



//╔═══════════════════════════════════════════════════════════════════╗
//║                              MOSTRAR                              ║
//╚═══════════════════════════════════════════════════════════════════╝
    void Mostrar(){
        pnodoMedico aux = primero;
        while (aux != NULL){
            cout << "IdMedico: " << aux->IdMedico
                << " | Nombre: " << aux->Nombre
                << " | Telefono: " << aux->Telefono
                << " | Correo: " << aux->Correo
                << " | IdEspecialidad: " << aux->IdEspecialidad
                << endl;
            aux = aux->siguiente;
        }
    }

    void ObtenerIdsPorEspecialidad(int IdEspecialidad, vector<int> &ids){
        pnodoMedico aux = primero;
        while (aux != NULL){
            if (aux->IdEspecialidad == IdEspecialidad){
                ids.push_back(aux->IdMedico);
            }
            aux = aux->siguiente;
        }
    }

    void EliminarPorEspecialidad(int IdEspecialidad){
        if (ListaVacia()){
            return;
        }
        while (primero != NULL && primero->IdEspecialidad == IdEspecialidad){
            pnodoMedico temp = primero;
            primero = primero->siguiente;
            delete temp;
        }
        if (ListaVacia()){
            return;
        }
        pnodoMedico aux = primero;
        while (aux->siguiente != NULL){
            if (aux->siguiente->IdEspecialidad == IdEspecialidad){
                pnodoMedico temp = aux->siguiente;
                aux->siguiente = aux->siguiente->siguiente;
                delete temp;
            }
            else{
                aux = aux->siguiente;
            }
        }
    }

    int ReportePDF(GeneradorPDF &pdf){
        int cantidad = 0;
        pnodoMedico aux = primero;
        while (aux != NULL){
            pdf.Linea(Rellenar(to_string(aux->IdMedico), 8) + Rellenar(aux->Nombre, 28)
                        + Rellenar(aux->Telefono, 14) + Rellenar(aux->Correo, 36)
                        + Rellenar(to_string(aux->IdEspecialidad), 8));
            cantidad++;
            aux = aux->siguiente;
        }
        return cantidad;
    }

    int ReportePDFPorEspecialidad(GeneradorPDF &pdf, int IdEspecialidad){
        int cantidad = 0;
        pnodoMedico aux = primero;
        while (aux != NULL){
            if (aux->IdEspecialidad == IdEspecialidad){
                pdf.Linea(Rellenar(to_string(aux->IdMedico), 8) + Rellenar(aux->Nombre, 28)
                            + Rellenar(aux->Telefono, 14) + Rellenar(aux->Correo, 36)
                            + Rellenar(to_string(aux->IdEspecialidad), 8));
                cantidad++;
            }
            aux = aux->siguiente;
        }
        return cantidad;
    }
private:
    pnodoMedico primero;
};


//╔═══════════════════════════════════════════════════════════════╗
//║                            TABLA                              ║
//╚═══════════════════════════════════════════════════════════════╝

class TablaHashingMedicos
{
private:
    ListaMedicos *tabla[Tam_Tabla];
public:
    TablaHashingMedicos(){
        for (int i = 0; i < Tam_Tabla; i++){
            tabla[i] = new ListaMedicos();
        }
    }

    ~TablaHashingMedicos(){
        for (int i = 0; i < Tam_Tabla; i++){
            delete tabla[i];
        }
    }

    int FuncionHash(int IdMedico){
        return IdMedico % Tam_Tabla;
    }

//╔═══════════════════════════════════════════════════════════════╗
//║                         VALIDACION                            ║
//╚═══════════════════════════════════════════════════════════════╝
    bool Existe(int IdMedico){
        int pos = FuncionHash(IdMedico);
        return tabla[pos]->Existe(IdMedico);
    }

//╔═══════════════════════════════════════════════════════════════╗
//║                          INSERTAR                             ║
//╚═══════════════════════════════════════════════════════════════╝
    bool Insertar(int IdMedico, string Nombre, string Telefono, 
        string Correo, int IdEspecialidad)
    {
        int pos = FuncionHash(IdMedico);
        return tabla[pos]->Insertar(
            IdMedico,
            Nombre,
            Telefono,
            Correo,
            IdEspecialidad
        );
    }

//╔═════════════════════════════════════════════════════════════════╗
//║                             BUSCAR                              ║
//╚═════════════════════════════════════════════════════════════════╝
    bool Buscar(int IdMedico){
        int pos = FuncionHash(IdMedico);
        return tabla[pos]->Buscar(IdMedico);
    }



//╔═══════════════════════════════════════════════════════════════════╗
//║                              ELIMINAR                              ║
//╚═══════════════════════════════════════════════════════════════════╝



    bool Eliminar(int IdMedico){
        int pos = FuncionHash(IdMedico);
        tabla[pos]->Eliminar(IdMedico);
        return true;
    }
    bool EliminarPorEspecialidad(int IdEspecialidad){
        for (int i = 0; i < Tam_Tabla; i++){
            tabla[i]->EliminarPorEspecialidad(IdEspecialidad);
        }
        return true;
    }

    vector<int> ObtenerIdsPorEspecialidad(int IdEspecialidad){
        vector<int> ids;
        for (int i = 0; i < Tam_Tabla; i++){
            tabla[i]->ObtenerIdsPorEspecialidad(IdEspecialidad, ids);
        }
        return ids;
    }

//╔═════════════════════════════════════════════════════════════════╗
//║                             Modificar                           ║
//╚═════════════════════════════════════════════════════════════════╝

    bool Modificar(int IdMedico, string Nombre, string Telefono, 
        string Correo, int IdEspecialidad)
    {
        int pos = FuncionHash(IdMedico);
        return tabla[pos]->Modificar(
            IdMedico,
            Nombre,
            Telefono,
            Correo,
            IdEspecialidad
        );
    }



//╔═══════════════════════════════════════════════════════════════════╗
//║                              MOSTRAR                              ║
//╚═══════════════════════════════════════════════════════════════════╝
    void Mostrar(){
        cout << endl;
        cout << " " << endl;
        cout << "------------------------" << endl;
        cout << "TABLA HASHING DE MEDICOS" << endl;
        cout << "------------------------" << endl;
        for (int i = 0; i < Tam_Tabla; i++){
            cout << "[" << i << "]: ";
            tabla[i]->Mostrar();
            cout << endl;
        }
    }

    int ReportePDF(GeneradorPDF &pdf){
        int total = 0;
        for (int i = 0; i < Tam_Tabla; i++){
            total += tabla[i]->ReportePDF(pdf);
        }
        return total;
    }

    int ReportePDFPorEspecialidad(GeneradorPDF &pdf, int IdEspecialidad){
        int total = 0;
        for (int i = 0; i < Tam_Tabla; i++){
            total += tabla[i]->ReportePDFPorEspecialidad(pdf, IdEspecialidad);
        }
        return total;
    }
};

//╔═══════════════════════════════════════════════════════════════╗
//║                           REPORTE                             ║
//╚═══════════════════════════════════════════════════════════════╝

void ReporteMedicos(TablaHashingMedicos &Medicos)
{
    GeneradorPDF pdf("Reporte_Medicos.pdf", "REPORTE DE MEDICOS",
                    Rellenar("ID", 8) + Rellenar("NOMBRE", 28) + Rellenar("TELEFONO", 14)
                    + Rellenar("CORREO", 36) + Rellenar("ESPEC.", 8));
    if (!pdf.Valido()){
        cout << "No se pudo crear el documento PDF." << endl;
        return;
    }
    if (Medicos.ReportePDF(pdf) == 0){
        pdf.Linea("(No hay medicos registrados)");
    }
    if (pdf.Guardar()){
        cout << "Reporte generado: Reporte_Medicos.pdf" << endl;
    }
    else{
        cout << "No se pudo guardar el archivo PDF." << endl;
    }
}

void ReporteMedicosPorEspecialidad(TablaHashingMedicos &Medicos)
{
    int IdEspecialidad;
    cout << "Ingrese el IdEspecialidad: ";
    cin >> IdEspecialidad;

    GeneradorPDF pdf("Reporte_Medicos_Especialidad_" + to_string(IdEspecialidad) + ".pdf",
                    "REPORTE DE MEDICOS POR ESPECIALIDAD",
                    Rellenar("ID", 8) + Rellenar("NOMBRE", 28) + Rellenar("TELEFONO", 14)
                    + Rellenar("CORREO", 36) + Rellenar("ESPEC.", 8));
    if (!pdf.Valido()){
        cout << "No se pudo crear el documento PDF." << endl;
        return;
    }
    if (Medicos.ReportePDFPorEspecialidad(pdf, IdEspecialidad) == 0){
        pdf.Linea("(No hay medicos registrados para esta especialidad)");
    }
    if (pdf.Guardar()){
        cout << "Reporte generado: Reporte_Medicos_Especialidad_" << IdEspecialidad << ".pdf" << endl;
    }
    else{
        cout << "No se pudo guardar el archivo PDF." << endl;
    }
}


//╔═════════════════════════════════════════════════════════════════════╗
//║                            CARGAR MEDICOS                           ║
//╚═════════════════════════════════════════════════════════════════════╝

void CargarMedicos(TablaHashingMedicos &Medicos, ListaEspecialidades &Especialidades){
    ifstream archivo("Medicos.csv");
    if (!archivo){
        cout << "No se pudo abrir el archivo Medicos.csv" << endl;
        return;
    }

    
    string encabezado;
    getline(archivo, encabezado); // Saltar encabezado del CSV
string IdMedicoTexto;
    string Nombre;
    string Telefono;
    string Correo;
    string IdEspecialidadTexto;

    while (getline(archivo, IdMedicoTexto, ';') &&
            getline(archivo, Nombre, ';') &&
            getline(archivo, Telefono, ';') &&
            getline(archivo, Correo, ';') &&
            getline(archivo, IdEspecialidadTexto))
    {
        int IdMedico = stoi(IdMedicoTexto);
        int IdEspecialidad = stoi(IdEspecialidadTexto);
        if (!Especialidades.Existe(IdEspecialidad)){
            continue;
        }

        if (Medicos.Existe(IdMedico)){
            continue;
        }
        Medicos.Insertar(
            IdMedico,
            Nombre,
            Telefono,
            Correo,
            IdEspecialidad
        );
    }

    archivo.close();
    cout << "Medicos cargados correctamente." << endl;
}