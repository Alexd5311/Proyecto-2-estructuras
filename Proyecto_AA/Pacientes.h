#pragma once

class NodoPaciente
{
public:

    NodoPaciente(int nuevoIdPaciente, string nuevoNombre, string nuevaFechaNacimiento, 
        string nuevoTelefono, int nuevoCodCiudad, int nuevoCodPais, string nuevoCorreo)
    {
        IdPaciente = nuevoIdPaciente;
        Nombre = nuevoNombre;
        FechaNacimiento = nuevaFechaNacimiento;
        Telefono = nuevoTelefono;
        CodCiudad = nuevoCodCiudad;
        CodPais = nuevoCodPais;
        Correo = nuevoCorreo;
        siguiente = NULL;
    }

private:
    int IdPaciente;
    string Nombre;
    string FechaNacimiento;
    string Telefono;
    int CodCiudad;
    int CodPais;
    string Correo;
    NodoPaciente *siguiente;
    friend class ListaPacientes;
};

typedef NodoPaciente *pnodoPaciente;


//╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
//║                                              HASHING PACIENTES                                              ║
//╚═════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

class ListaPacientes
{
public:
    ListaPacientes(){
        primero = NULL;
    }

    ~ListaPacientes(){
        pnodoPaciente aux = primero;

        while (aux != NULL){
            pnodoPaciente temp = aux;
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
    bool Existe(int IdPaciente){
        pnodoPaciente aux = primero;

        while (aux != NULL){
            if (aux->IdPaciente == IdPaciente){
                return true;
            }
            aux = aux->siguiente;
        }

        return false;
    }

//╔═══════════════════════════════════════════════════════════════╗
//║                          INSERTAR                             ║
//╚═══════════════════════════════════════════════════════════════╝
    bool Insertar(int IdPaciente, string Nombre, string FechaNacimiento, 
        string Telefono, int CodCiudad, int CodPais, string Correo){
        
        if (Existe(IdPaciente)){
            return false;
        }
        if (ListaVacia()){
            primero = new NodoPaciente( IdPaciente, Nombre, FechaNacimiento, Telefono, CodCiudad, CodPais, Correo
            );
        }
        else{
            pnodoPaciente aux = primero;
            while (aux->siguiente != NULL){
                aux = aux->siguiente;
            }
            aux->siguiente = new NodoPaciente(IdPaciente, Nombre, FechaNacimiento, Telefono, CodCiudad, CodPais, Correo
            );
        }
        return true;
    }


//╔═════════════════════════════════════════════════════════════════╗
//║                             BUSCAR                              ║
//╚═════════════════════════════════════════════════════════════════╝
    bool Buscar(int IdPaciente)
    {
        pnodoPaciente aux = primero;
        while (aux != NULL)
        {
            if (aux->IdPaciente == IdPaciente)
            {
                cout << "IdPaciente: " << aux->IdPaciente << endl;
                cout << "Nombre: " << aux->Nombre << endl;
                cout << "FechaNacimiento: " << aux->FechaNacimiento << endl;
                cout << "Telefono: " << aux->Telefono << endl;
                cout << "CodCiudad: " << aux->CodCiudad << endl;
                cout << "CodPais: " << aux->CodPais << endl;
                cout << "Correo: " << aux->Correo << endl;
                return true;
            }
            aux = aux->siguiente;
        }
        return false;
    }

//╔═══════════════════════════════════════════════════════════════════╗
//║                              MOSTRAR                              ║
//╚═══════════════════════════════════════════════════════════════════╝
    void Mostrar()
    {
        pnodoPaciente aux = primero;
        while (aux != NULL){
            cout << "IdPaciente: " << aux->IdPaciente 
            << " | Nombre: " << aux->Nombre 
            << " | FechaNacimiento: " << aux->FechaNacimiento 
            << " | Telefono: " << aux->Telefono 
            << " | CodCiudad: " << aux->CodCiudad 
            << " | CodPais: " << aux->CodPais 
            << " | Correo: " << aux->Correo 
            << endl;
            aux = aux->siguiente;
        }
    }


    //╔═══════════════════════════════════════════════════════════════════╗
    //║                              ELIMINAR                              ║
    //╚═══════════════════════════════════════════════════════════════════╝

    bool Eliminar(int IdPaciente)
    {
        if (ListaVacia()){
            return false;
        }
        if (primero->IdPaciente == IdPaciente){
            pnodoPaciente temp = primero;
            primero = primero->siguiente;
            delete temp;
            return true;
        }
        pnodoPaciente aux = primero;
        while (aux->siguiente != NULL && aux->siguiente->IdPaciente != IdPaciente){
            aux = aux->siguiente;
        }
        if (aux->siguiente == NULL){
            return false;
        }
        pnodoPaciente temp = aux->siguiente;
        aux->siguiente = aux->siguiente->siguiente;
        delete temp;
        return true;
    }

    bool EliminarPorCiudad(int CodPais, int CodCiudad)
    {
        if (ListaVacia()){
            return false;
        }
        bool eliminado = false;
        pnodoPaciente aux = primero;
        while (aux != NULL){
            if (aux->CodPais == CodPais && aux->CodCiudad == CodCiudad){
                int IdPaciente = aux->IdPaciente;
                aux = aux->siguiente;
                Eliminar(IdPaciente);
                eliminado = true;
            }
            else{
                aux = aux->siguiente;
            }
        }
        return eliminado;
    }

    bool EliminarPorPais(int CodPais)
    {
        if (ListaVacia()){
            return false;
        }
        bool eliminado = false;
        pnodoPaciente aux = primero;
        while (aux != NULL){
            if (aux->CodPais == CodPais){
                int IdPaciente = aux->IdPaciente;
                aux = aux->siguiente;
                Eliminar(IdPaciente);
                eliminado = true;
            }
            else{
                aux = aux->siguiente;
            }
        }
        return eliminado;
    }

    void ObtenerIdsPorPais(int CodPais, vector<int> &ids)
    {
        pnodoPaciente aux = primero;
        while (aux != NULL){
            if (aux->CodPais == CodPais){
                ids.push_back(aux->IdPaciente);
            }
            aux = aux->siguiente;
        }
    }

    void ObtenerIdsPorCiudad(int CodPais, int CodCiudad, vector<int> &ids)
    {
        pnodoPaciente aux = primero;
        while (aux != NULL){
            if (aux->CodPais == CodPais && aux->CodCiudad == CodCiudad){
                ids.push_back(aux->IdPaciente);
            }
            aux = aux->siguiente;
        }
    }


//╔═════════════════════════════════════════════════════════════════╗
//║                             Modificar                           ║
//╚═════════════════════════════════════════════════════════════════╝

    bool Modificar(int IdPaciente, string Nombre, string FechaNacimiento, 
        string Telefono, int CodCiudad, int CodPais, string Correo)
    {
        pnodoPaciente aux = primero;
        while (aux != NULL)
        {
            if (aux->IdPaciente == IdPaciente)
            {
                aux->Nombre = Nombre;
                aux->FechaNacimiento = FechaNacimiento;
                aux->Telefono = Telefono;
                aux->CodCiudad = CodCiudad;
                aux->CodPais = CodPais;
                aux->Correo = Correo;
                return true;
            }
            aux = aux->siguiente;
        }
        return false;
    }
private:
    pnodoPaciente primero;
};




//╔═══════════════════════════════════════════════════════════════╗
//║                            TABLA                              ║
//╚═══════════════════════════════════════════════════════════════╝

const int Tam_Tabla = 13;
class TablaHashingPacientes
{
private:
    ListaPacientes *tabla[Tam_Tabla];
public:
    TablaHashingPacientes(){
        for (int i = 0; i < Tam_Tabla; i++){
            tabla[i] = new ListaPacientes();
        }
    }

    ~TablaHashingPacientes(){
        for (int i = 0; i < Tam_Tabla; i++){
            delete tabla[i];
        }
    }

    int FuncionHash(int IdPaciente){
        return IdPaciente % Tam_Tabla;
    }

//╔═══════════════════════════════════════════════════════════════╗
//║                         VALIDACION                            ║
//╚═══════════════════════════════════════════════════════════════╝
    bool Existe(int IdPaciente){
        int pos = FuncionHash(IdPaciente);
        return tabla[pos]->Existe(IdPaciente);
    }

    bool EliminarPorCiudad(int CodPais, int CodCiudad)
    {
        bool eliminado = false;
        for (int i = 0; i < Tam_Tabla; i++){
            if (tabla[i]->EliminarPorCiudad(CodPais, CodCiudad)){
                eliminado = true;
            }
        }
        return eliminado;
    }

    bool EliminarPorPais(int CodPais)
    {
        bool eliminado = false;
        for (int i = 0; i < Tam_Tabla; i++){
            if (tabla[i]->EliminarPorPais(CodPais)){
                eliminado = true;
            }
        }
        return eliminado;
    }

    vector<int> ObtenerIdsPorPais(int CodPais)
    {
        vector<int> ids;
        for (int i = 0; i < Tam_Tabla; i++){
            tabla[i]->ObtenerIdsPorPais(CodPais, ids);
        }
        return ids;
    }

    vector<int> ObtenerIdsPorCiudad(int CodPais, int CodCiudad)
    {
        vector<int> ids;
        for (int i = 0; i < Tam_Tabla; i++){
            tabla[i]->ObtenerIdsPorCiudad(CodPais, CodCiudad, ids);
        }
        return ids;
    }

//╔═══════════════════════════════════════════════════════════════╗
//║                          INSERTAR                             ║
//╚═══════════════════════════════════════════════════════════════╝
    bool Insertar(int IdPaciente, string Nombre, string FechaNacimiento, 
        string Telefono, int CodCiudad, int CodPais, string Correo)
    {
        int pos = FuncionHash(IdPaciente);
        return tabla[pos]->Insertar( IdPaciente, Nombre, FechaNacimiento, Telefono, CodCiudad, CodPais, Correo
        );
    }

//╔═════════════════════════════════════════════════════════════════╗
//║                             BUSCAR                              ║
//╚═════════════════════════════════════════════════════════════════╝
    bool Buscar(int IdPaciente){
        int pos = FuncionHash(IdPaciente);
        return tabla[pos]->Buscar(IdPaciente);
    }



//╔═══════════════════════════════════════════════════════════════════╗
//║                              ELIMINAR                              ║
//╚═══════════════════════════════════════════════════════════════════╝

    bool Eliminar(int IdPaciente){
        int pos = FuncionHash(IdPaciente);
        return tabla[pos]->Eliminar(IdPaciente);
    }


//╔═════════════════════════════════════════════════════════════════╗
//║                             Modificar                           ║
//╚═════════════════════════════════════════════════════════════════╝
    bool Modificar(int IdPaciente, string Nombre, string FechaNacimiento, 
        string Telefono, int CodCiudad, int CodPais, string Correo)
    {
        int pos = FuncionHash(IdPaciente);
        return tabla[pos]->Modificar( IdPaciente, Nombre, FechaNacimiento, Telefono, CodCiudad, CodPais, Correo
        );
    }


//╔═══════════════════════════════════════════════════════════════════╗
//║                              MOSTRAR                              ║
//╚═══════════════════════════════════════════════════════════════════╝
    void Mostrar(){
        cout << endl;
        cout << "TABLA HASHING DE PACIENTES" << endl;
        cout << "--------------------------" << endl;
        for (int i = 0; i < Tam_Tabla; i++){
            cout << "[" << i << "]: ";
            tabla[i]->Mostrar();
            cout << endl;
        }
    }
};


//╔═════════════════════════════════════════════════════════════════════╗
//║                           CARGAR PACIENTES                          ║
//╚═════════════════════════════════════════════════════════════════════╝
void CargarPacientes(TablaHashingPacientes &Pacientes, ListaCiudades &Ciudades)
{
    ifstream archivo("Pacientes.txt");
    if (!archivo){
        cout << "No se pudo abrir el archivo Pacientes.txt" << endl;
        return;
    }

    string IdPacienteTexto;
    string Nombre;
    string FechaNacimiento;
    string Telefono;
    string CodPaisTexto;
    string CodCiudadTexto;
    string Correo;

    while (getline(archivo, IdPacienteTexto, ';') &&
            getline(archivo, Nombre, ';') &&
            getline(archivo, FechaNacimiento, ';') &&
            getline(archivo, Telefono, ';') &&
            getline(archivo, CodPaisTexto, ';') &&
            getline(archivo, CodCiudadTexto, ';') &&
            getline(archivo, Correo))
    {
        int IdPaciente = stoi(IdPacienteTexto);
        int CodPais = stoi(CodPaisTexto);
        int CodCiudad = stoi(CodCiudadTexto);

        if (!Ciudades.Existe(CodPais, CodCiudad)){
            continue;
        }

        if (Pacientes.Existe(IdPaciente)){
            continue;
        }

        Pacientes.Insertar(
            IdPaciente,
            Nombre,
            FechaNacimiento,
            Telefono,
            CodCiudad,
            CodPais,
            Correo
        );
    }

    archivo.close();

    cout << "Pacientes cargados correctamente." << endl;
}



















//╔═══════════════════════════════════════════════════════════════════════════════════════════════╗
//║                                      NODO ESPECIALIDAD                                        ║
//╚═══════════════════════════════════════════════════════════════════════════════════════════════╝

