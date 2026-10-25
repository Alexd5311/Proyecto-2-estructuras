#pragma once

class ListaHospitalizaciones;

class NodoHospitalizacion
{
public:

    NodoHospitalizacion(int nuevoIdHospitalizacion, int nuevoIdPaciente,
        int nuevoIdHabitacion, int nuevoIdMedico, string nuevaFechaIngreso,
        string nuevaFechaSalida, string nuevoMotivo)
    {
        IdHospitalizacion = nuevoIdHospitalizacion;
        IdPaciente = nuevoIdPaciente;
        IdHabitacion = nuevoIdHabitacion;
        IdMedico = nuevoIdMedico;
        FechaIngreso = nuevaFechaIngreso;
        FechaSalida = nuevaFechaSalida;
        Motivo = nuevoMotivo;
        siguiente = NULL;
        anterior = NULL;
    }

private:
    int IdHospitalizacion;
    int IdPaciente;
    int IdHabitacion;
    int IdMedico;
    string FechaIngreso;
    string FechaSalida;
    string Motivo;

    NodoHospitalizacion *siguiente;
    NodoHospitalizacion *anterior;

    friend class ListaHospitalizaciones;
    friend void ReporteHospitalizacionesPorMedico(ListaHospitalizaciones &Hospitalizaciones);
};

typedef NodoHospitalizacion *pnodoHospitalizacion;


//╔═════════════════════════════════════════════════════════════════════════════╗
//║                          LISTA DOBLE HOSPITALIZACIONES                      ║
//╚═════════════════════════════════════════════════════════════════════════════╝

class ListaHospitalizaciones
{
public:
    ListaHospitalizaciones(){
        primero = NULL;
    }
    ~ListaHospitalizaciones();
    bool ListaVacia(){
        return primero == NULL;
    }
    bool Existe(int IdHospitalizacion);
    bool Insertar(int IdHospitalizacion, int IdPaciente,
        int IdHabitacion, int IdMedico, string FechaIngreso,
        string FechaSalida, string Motivo);
    bool Buscar(int IdHospitalizacion);
    bool Eliminar(int IdHospitalizacion);
    bool EliminarPorPaciente(int IdPaciente);
    bool EliminarPorHabitacion(int IdHabitacion);
    bool EliminarPorMedico(int IdMedico);
    bool Modificar(int IdHospitalizacion, int IdPaciente,
        int IdHabitacion, int IdMedico, string FechaIngreso,
        string FechaSalida, string Motivo);
    void Mostrar();
    void ReportePDF(GeneradorPDF &pdf);
    friend void ReporteHospitalizacionesPorMedico(ListaHospitalizaciones &Hospitalizaciones);
private:
    pnodoHospitalizacion primero;
};


ListaHospitalizaciones::~ListaHospitalizaciones()
{
    pnodoHospitalizacion aux;
    while (primero){
        aux = primero;
        primero = primero->siguiente;
        delete aux;
    }
    primero = NULL;
}


//╔═══════════════════════════════════════════════════════════════╗
//║                         VALIDACION                            ║
//╚═══════════════════════════════════════════════════════════════╝
bool ListaHospitalizaciones::Existe(int IdHospitalizacion)
{
    pnodoHospitalizacion aux = primero;
    while (aux != NULL){
        if (aux->IdHospitalizacion == IdHospitalizacion){
            return true;
        }
        aux = aux->siguiente;
    }
    return false;
}


//╔═══════════════════════════════════════════════════════════════╗
//║                          INSERTAR                             ║
//╚═══════════════════════════════════════════════════════════════╝
bool ListaHospitalizaciones::Insertar(int IdHospitalizacion,
    int IdPaciente, int IdHabitacion, int IdMedico,
    string FechaIngreso, string FechaSalida, string Motivo)
{
    if (Existe(IdHospitalizacion)){
        return false;
    }
    pnodoHospitalizacion nuevo = new NodoHospitalizacion(
        IdHospitalizacion,
        IdPaciente,
        IdHabitacion,
        IdMedico,
        FechaIngreso,
        FechaSalida,
        Motivo
    );

    if (ListaVacia()){
        primero = nuevo;
        primero->anterior = NULL;
    }
    else
    {
        pnodoHospitalizacion aux = primero;
        while (aux->siguiente != NULL){
            aux = aux->siguiente;
        }
        aux->siguiente = nuevo;
        nuevo->anterior = aux;
    }
    return true;
}



//╔═════════════════════════════════════════════════════════════════╗
//║                             BUSCAR                              ║
//╚═════════════════════════════════════════════════════════════════╝
bool ListaHospitalizaciones::Buscar(int IdHospitalizacion)
{
    pnodoHospitalizacion aux = primero;
    while (aux != NULL){
        if (aux->IdHospitalizacion == IdHospitalizacion){
            cout << "IdHospitalizacion: " << aux->IdHospitalizacion << endl;
            cout << "IdPaciente: " << aux->IdPaciente << endl;
            cout << "IdHabitacion: " << aux->IdHabitacion << endl;
            cout << "IdMedico: " << aux->IdMedico << endl;
            cout << "FechaIngreso: " << aux->FechaIngreso << endl;
            cout << "FechaSalida: " << aux->FechaSalida << endl;
            cout << "Motivo: " << aux->Motivo << endl;
            return true;
        }
        aux = aux->siguiente;
    }
    return false;
}


//╔═══════════════════════════════════════════════════════════════════╗
//║                              ELIMINAR                              ║
//╚═══════════════════════════════════════════════════════════════════╝


bool ListaHospitalizaciones::Eliminar(int IdHospitalizacion)
{
    if (ListaVacia()){
        return false;
    }
    pnodoHospitalizacion aux = primero;
    while (aux != NULL){
        if (aux->IdHospitalizacion == IdHospitalizacion){
            if (aux == primero){
                primero = primero->siguiente;
                if (primero != NULL)
                    primero->anterior = NULL;
            }
            else{
                aux->anterior->siguiente = aux->siguiente;
                if (aux->siguiente != NULL)
                    aux->siguiente->anterior = aux->anterior;
            }
            delete aux;
            return true;
        }
        aux = aux->siguiente;
    }
    return false;
}


bool ListaHospitalizaciones::EliminarPorHabitacion(int IdHabitacion)
{
    bool eliminado = false;
    pnodoHospitalizacion aux = primero;

    while (aux != NULL)
    {
        if (aux->IdHabitacion == IdHabitacion)
        {
            pnodoHospitalizacion temp = aux;

            if (aux == primero)
            {
                primero = primero->siguiente;
                if (primero != NULL)
                    primero->anterior = NULL;
            }
            else
            {
                aux->anterior->siguiente = aux->siguiente;
                if (aux->siguiente != NULL)
                    aux->siguiente->anterior = aux->anterior;
            }
            aux = aux->siguiente;
            delete temp;
            eliminado = true;
        }
        else
        {
            aux = aux->siguiente;
        }
    }
    return eliminado;
}

bool ListaHospitalizaciones::EliminarPorMedico(int IdMedico)
{
    bool eliminado = false;
    pnodoHospitalizacion aux = primero;

    while (aux != NULL)
    {
        if (aux->IdMedico == IdMedico)
        {
            pnodoHospitalizacion temp = aux;

            if (aux == primero)
            {
                primero = primero->siguiente;
                if (primero != NULL)
                    primero->anterior = NULL;
            }
            else
            {
                aux->anterior->siguiente = aux->siguiente;
                if (aux->siguiente != NULL)
                    aux->siguiente->anterior = aux->anterior;
            }

            aux = aux->siguiente;
            delete temp;
            eliminado = true;
        }
        else
        {
            aux = aux->siguiente;
        }
    }

    return eliminado;
}




//╔═════════════════════════════════════════════════════════════════╗
//║                             Modificar                           ║
//╚═════════════════════════════════════════════════════════════════╝



bool ListaHospitalizaciones::Modificar(int IdHospitalizacion,
    int IdPaciente, int IdHabitacion, int IdMedico,
    string FechaIngreso, string FechaSalida, string Motivo)
{
    pnodoHospitalizacion aux = primero;
    while (aux != NULL)
    {
        if (aux->IdHospitalizacion == IdHospitalizacion)
        {
            aux->IdPaciente = IdPaciente;
            aux->IdHabitacion = IdHabitacion;
            aux->IdMedico = IdMedico;
            aux->FechaIngreso = FechaIngreso;
            aux->FechaSalida = FechaSalida;
            aux->Motivo = Motivo;
            return true;
        }
        aux = aux->siguiente;
    }
    return false;
}

//╔═════════════════════════════════════════════════════════════════╗
//║                             REPORTES                            ║
//╚═════════════════════════════════════════════════════════════════╝
void ListaHospitalizaciones::ReportePDF(GeneradorPDF &pdf)
{
    if (ListaVacia()){
        pdf.Linea("(No hay hospitalizaciones registradas)");
        return;
    }
    pnodoHospitalizacion aux = primero;
    while (aux != NULL){
        string salida = aux->FechaSalida;
        if (salida.find("0000") == 0){
            salida = "Sin fecha de salida";
        }
        pdf.Linea(Rellenar(to_string(aux->IdHospitalizacion), 8) + Rellenar(to_string(aux->IdPaciente), 10)
                    + Rellenar(to_string(aux->IdHabitacion), 8) + Rellenar(to_string(aux->IdMedico), 8)
                    + Rellenar(aux->FechaIngreso, 14) + Rellenar(salida, 21)
                    + Rellenar(aux->Motivo, 49));
        aux = aux->siguiente;
    }
}

void ReporteHospitalizaciones(ListaHospitalizaciones &Hospitalizaciones)
{
    GeneradorPDF pdf("Reporte_Hospitalizaciones.pdf", "REPORTE DE HOSPITALIZACIONES",
                    Rellenar("ID", 8) + Rellenar("PACIENTE", 10) + Rellenar("HAB.", 8)
                    + Rellenar("MEDICO", 8) + Rellenar("INGRESO", 14) + Rellenar("SALIDA", 21)
                    + Rellenar("MOTIVO", 49));
    if (!pdf.Valido()){
        cout << "No se pudo crear el documento PDF." << endl;
        return;
    }
    Hospitalizaciones.ReportePDF(pdf);
    if (pdf.Guardar()){
        cout << "Reporte generado: Reporte_Hospitalizaciones.pdf" << endl;
    }
    else{
        cout << "No se pudo guardar el archivo PDF." << endl;
    }
}


void ReporteHospitalizacionesPorMedico(ListaHospitalizaciones &Hospitalizaciones)
{
    int IdMedico;
    cout << "Ingrese el IdMedico: ";
    cin >> IdMedico;

    GeneradorPDF pdf("Reporte_Hospitalizaciones_Por_Medico.pdf", "REPORTE DE HOSPITALIZACIONES POR MEDICO",
                    Rellenar("ID", 8) + Rellenar("PACIENTE", 10) + Rellenar("HAB.", 8)
                    + Rellenar("MEDICO", 8) + Rellenar("INGRESO", 14) + Rellenar("SALIDA", 21)
                    + Rellenar("MOTIVO", 49));
    if (!pdf.Valido()){
        cout << "No se pudo crear el documento PDF." << endl;
        return;
    }
    pnodoHospitalizacion aux = Hospitalizaciones.primero;
    bool encontrado = false;
    while (aux != NULL){
        if (aux->IdMedico == IdMedico){
            string salida = aux->FechaSalida;
            if (salida.find("0000") == 0){
                salida = "Sin fecha de salida";
            }
            pdf.Linea(Rellenar(to_string(aux->IdHospitalizacion), 8) + Rellenar(to_string(aux->IdPaciente), 10)
                        + Rellenar(to_string(aux->IdHabitacion), 8) + Rellenar(to_string(aux->IdMedico), 8)
                        + Rellenar(aux->FechaIngreso, 14) + Rellenar(salida, 21)
                        + Rellenar(aux->Motivo, 49));
            encontrado = true;
        }
        aux = aux->siguiente;
    }
    if (!encontrado){
        pdf.Linea("(No hay hospitalizaciones registradas para este medico)");
    }
    if (pdf.Guardar()){
        cout << "Reporte generado: Reporte_Hospitalizaciones_Por_Medico.pdf" << endl;
    }
    else{
        cout << "No se pudo guardar el archivo PDF." << endl;
    }
}

//╔═════════════════════════════════════════════════════════════════╗
//║                             MOSTRAR                             ║
//╚═════════════════════════════════════════════════════════════════╝
bool ListaHospitalizaciones::EliminarPorPaciente(int IdPaciente)
{
    bool eliminado = false;
    pnodoHospitalizacion aux = primero;

    while (aux != NULL)
    {
        if (aux->IdPaciente == IdPaciente)
        {
            pnodoHospitalizacion temp = aux;

            if (aux == primero)
            {
                primero = primero->siguiente;
                if (primero != NULL)
                    primero->anterior = NULL;
            }
            else
            {
                aux->anterior->siguiente = aux->siguiente;
                if (aux->siguiente != NULL)
                    aux->siguiente->anterior = aux->anterior;
            }

            aux = aux->siguiente;
            delete temp;
            eliminado = true;
        }
        else
        {
            aux = aux->siguiente;
        }
    }

    return eliminado;
}

void ListaHospitalizaciones::Mostrar()
{
    pnodoHospitalizacion aux = primero;
    while (aux != NULL){
        cout << "IdHospitalizacion: " << aux->IdHospitalizacion
                << " | IdPaciente: " << aux->IdPaciente
                << " | IdHabitacion: " << aux->IdHabitacion
                << " | IdMedico: " << aux->IdMedico
                << " | FechaIngreso: " << aux->FechaIngreso
                << " | FechaSalida: " << aux->FechaSalida
                << " | Motivo: " << aux->Motivo
                << endl;
        aux = aux->siguiente;
    }
}


//╔═════════════════════════════════════════════════════════════════════════════╗
//║                    CARGAR HOSPITALIZACIONES                                ║
//╚═════════════════════════════════════════════════════════════════════════════╝

void CargarHospitalizaciones(ListaHospitalizaciones &Hospitalizaciones, ArbolBPacientes &Pacientes, 
    ListaHabitaciones &Habitaciones, ArbolBMedicos &Medicos)
{
    ifstream archivo("Hospitalizaciones.csv");

    if (!archivo){
        cout << "No se pudo abrir el archivo Hospitalizaciones.csv" << endl;
        return;
    }

    
    string encabezado;
    getline(archivo, encabezado); // Saltar encabezado del CSV
    string IdHospitalizacionTexto;
    string IdPacienteTexto;
    string IdHabitacionTexto;
    string IdMedicoTexto;
    string FechaIngreso;
    string FechaSalida;
    string Motivo;

    while (getline(archivo, IdHospitalizacionTexto, ';') &&
            getline(archivo, IdPacienteTexto, ';') &&
            getline(archivo, IdHabitacionTexto, ';') &&
            getline(archivo, IdMedicoTexto, ';') &&
            getline(archivo, FechaIngreso, ';') &&
            getline(archivo, FechaSalida, ';') &&
            getline(archivo, Motivo))
    {
        int IdHospitalizacion = stoi(IdHospitalizacionTexto);
        int IdPaciente = stoi(IdPacienteTexto);
        int IdHabitacion = stoi(IdHabitacionTexto);
        int IdMedico = stoi(IdMedicoTexto);
        if (!Pacientes.Existe(IdPaciente)){
            continue;
        }

        if (!Habitaciones.Existe(IdHabitacion)){
            continue;
        }

        if (!Medicos.Existe(IdMedico)){
            continue;
        }

        if (Hospitalizaciones.Existe(IdHospitalizacion)){
            continue;
        }
        Hospitalizaciones.Insertar(IdHospitalizacion, IdPaciente, IdHabitacion, IdMedico, FechaIngreso, FechaSalida, Motivo);
    }

    archivo.close();
    cout << "Hospitalizaciones cargadas correctamente." << endl;
}
