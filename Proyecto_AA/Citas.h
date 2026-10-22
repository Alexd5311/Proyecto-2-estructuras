#pragma once

class ListaCitas;

class NodoCita
{
public:

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
        siguiente = NULL;
        anterior = NULL;
    }

private:
    int IdCita;
    int IdPaciente;
    int IdMedico;
    int IdEspecialidad;
    string Fecha;
    string Hora;
    string Motivo;
    string Estado;
    NodoCita *siguiente;
    NodoCita *anterior;

    friend class ListaCitas;
    friend void ReporteCitasPorFecha(ListaCitas &Citas);
    friend void ReporteCitasPorEspecialidad(ListaCitas &Citas);
    friend void ReporteCitasPorMedico(ListaCitas &Citas);
};

typedef NodoCita *pnodoCita;

//╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
//║                                          LISTA DOBLE CITAS                                                  ║
//╚═════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

class ListaCitas
{
public:
    ListaCitas(){
        primero = NULL;
    }
    ~ListaCitas();
    bool ListaVacia(){
        return primero == NULL;
    }
    bool Existe(int IdCita);
    bool Insertar(int IdCita, int IdPaciente, int IdMedico, int IdEspecialidad, string Fecha, 
        string Hora, string Motivo, string Estado);
    bool Buscar(int IdCita);
    bool Eliminar(int IdCita);
    bool EliminarPorPaciente(int IdPaciente);
    bool EliminarPorMedico(int IdMedico);
    bool Modificar(int IdCita, int IdPaciente, int IdMedico, int IdEspecialidad, string Fecha, 
        string Hora, string Motivo, string Estado);
    void Mostrar();
    friend void ReporteCitasPorFecha(ListaCitas &Citas);
    friend void ReporteCitasPorEspecialidad(ListaCitas &Citas);
    friend void ReporteCitasPorMedico(ListaCitas &Citas);
private:
    pnodoCita primero;
};

ListaCitas::~ListaCitas()
{
    pnodoCita aux;

    while (primero)
    {
        aux = primero;
        primero = primero->siguiente;
        delete aux;
    }

    primero = NULL;
}

//╔═══════════════════════════════════════════════════════════════╗
//║                         VALIDACION                            ║
//╚═══════════════════════════════════════════════════════════════╝
bool ListaCitas::Existe(int IdCita)
{
    pnodoCita aux = primero;
    while (aux != NULL)
    {
        if (aux->IdCita == IdCita){
            return true;
        }
        aux = aux->siguiente;
    }
    return false;
}


//╔═══════════════════════════════════════════════════════════════╗
//║                          INSERTAR                             ║
//╚═══════════════════════════════════════════════════════════════╝
bool ListaCitas::Insertar(int IdCita, int IdPaciente, int IdMedico,
    int IdEspecialidad, string Fecha, string Hora,
    string Motivo, string Estado)
{
    if (Existe(IdCita)){
        return false;
    }
    pnodoCita nuevo = new NodoCita(IdCita, IdPaciente, IdMedico, IdEspecialidad, Fecha, Hora, Motivo, Estado);
    if (ListaVacia()){
        primero = nuevo;
        primero->anterior = NULL;
    }
    else{
        pnodoCita aux = primero;
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
bool ListaCitas::Buscar(int IdCita)
{
    pnodoCita aux = primero;
    while (aux != NULL)
    {
        if (aux->IdCita == IdCita)
        {
            cout << "IdCita: " << aux->IdCita << endl;
            cout << "IdPaciente: " << aux->IdPaciente << endl;
            cout << "IdMedico: " << aux->IdMedico << endl;
            cout << "IdEspecialidad: " << aux->IdEspecialidad << endl;
            cout << "Fecha: " << aux->Fecha << endl;
            cout << "Hora: " << aux->Hora << endl;
            cout << "Motivo: " << aux->Motivo << endl;
            cout << "Estado: " << aux->Estado << endl;
            return true;
        }
        aux = aux->siguiente;
    }
    return false;
}

//╔═══════════════════════════════════════════════════════════════════╗
//║                              ELIMINAR                              ║
//╚═══════════════════════════════════════════════════════════════════╝

bool ListaCitas::Eliminar(int IdCita)
{
    if (ListaVacia()){
        return false;
    }
    pnodoCita aux = primero;
    while (aux != NULL)
    {
        if (aux->IdCita == IdCita)
        {
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
            delete aux;
            return true;
        }
        aux = aux->siguiente;
    }
    return false;
}


bool ListaCitas::EliminarPorPaciente(int IdPaciente)
{
    bool eliminado = false;
    pnodoCita aux = primero;

    while (aux != NULL)
    {
        if (aux->IdPaciente == IdPaciente)
        {
            pnodoCita temp = aux;

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

bool ListaCitas::EliminarPorMedico(int IdMedico)
{
    bool eliminado = false;
    pnodoCita aux = primero;

    while (aux != NULL)
    {
        if (aux->IdMedico == IdMedico)
        {
            pnodoCita temp = aux;

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


bool ListaCitas::Modificar(int IdCita, int IdPaciente, int IdMedico,
    int IdEspecialidad, string Fecha, string Hora,
    string Motivo, string Estado)
{
    pnodoCita aux = primero;
    while (aux != NULL)
    {
        if (aux->IdCita == IdCita)
        {
            aux->IdPaciente = IdPaciente;
            aux->IdMedico = IdMedico;
            aux->IdEspecialidad = IdEspecialidad;
            aux->Fecha = Fecha;
            aux->Hora = Hora;
            aux->Motivo = Motivo;
            aux->Estado = Estado;
            return true;
        }
        aux = aux->siguiente;
    }
    return false;
}



//╔═══════════════════════════════════════════════════════════════╗
//║                         REPORTES                              ║
//╚═══════════════════════════════════════════════════════════════╝

void ReporteCitasPorFecha(ListaCitas &Citas)
{
    string fecha;
    cout << "Ingrese la fecha (YYYY-MM-DD): ";
    cin >> fecha;
    GeneradorPDF pdf("Reporte_Citas_" + fecha + ".pdf", "REPORTE DE CITAS POR FECHA: " + fecha,
                    Rellenar("ID CITA", 8) + Rellenar("ID PACIENTE", 12) + Rellenar("ID MEDICO", 10)
                    + Rellenar("ID ESPECIALIDAD", 15) + Rellenar("FECHA", 12) + Rellenar("HORA", 8)
                    + Rellenar("MOTIVO", 30) + Rellenar("ESTADO", 8));
    if (!pdf.Valido()){
        cout << "No se pudo crear el documento PDF." << endl;
        return;
    }

    pnodoCita aux = Citas.primero;
    bool encontrado = false;

    while (aux != NULL)
    {
        if (aux->Fecha == fecha)
        {
            pdf.Linea(Rellenar(to_string(aux->IdCita), 8) + Rellenar(to_string(aux->IdPaciente), 12)
                        + Rellenar(to_string(aux->IdMedico), 10) + Rellenar(to_string(aux->IdEspecialidad), 15)
                        + Rellenar(aux->Fecha, 12) + Rellenar(aux->Hora, 8)
                        + Rellenar(aux->Motivo, 30) + Rellenar(aux->Estado, 8));
            encontrado = true;
        }
        aux = aux->siguiente;
    }

    if (!encontrado){
        pdf.Linea("(No hay citas registradas para esta fecha)");
    }

    if (pdf.Guardar()){
        cout << "Reporte generado: Reporte_Citas_" << fecha << ".pdf" << endl;
    }
    else{
        cout << "No se pudo guardar el archivo PDF." << endl;
    }
}


void ReporteCitasPorEspecialidad(ListaCitas &Citas)
{
    int IdEspecialidad;
    cout << "Ingrese el IdEspecialidad: ";
    cin >> IdEspecialidad;
    GeneradorPDF pdf("Reporte_Citas_Especialidad_" + to_string(IdEspecialidad) + ".pdf",
                    "REPORTE DE CITAS POR ESPECIALIDAD",
                    Rellenar("ID CITA", 8) + Rellenar("ID PACIENTE", 12) + Rellenar("ID MEDICO", 10)
                    + Rellenar("ID ESPECIALIDAD", 15) + Rellenar("FECHA", 12) + Rellenar("HORA", 8)
                    + Rellenar("MOTIVO", 30) + Rellenar("ESTADO", 8));
    if (!pdf.Valido()){
        cout << "No se pudo crear el documento PDF." << endl;
        return;
    }
    pnodoCita aux = Citas.primero;
    bool encontrado = false;
    while (aux != NULL){
        if (aux->IdEspecialidad == IdEspecialidad){
            pdf.Linea(Rellenar(to_string(aux->IdCita), 8) + Rellenar(to_string(aux->IdPaciente), 12)
                        + Rellenar(to_string(aux->IdMedico), 10) + Rellenar(to_string(aux->IdEspecialidad), 15)
                        + Rellenar(aux->Fecha, 12) + Rellenar(aux->Hora, 8)
                        + Rellenar(aux->Motivo, 30) + Rellenar(aux->Estado, 8));
            encontrado = true;
        }
        aux = aux->siguiente;
    }
    if (!encontrado){
        pdf.Linea("(No hay citas registradas para esta especialidad)");
    }
    if (pdf.Guardar()){
        cout << "Reporte generado: Reporte_Citas_Especialidad_" << IdEspecialidad << ".pdf" << endl;
    }
    else{
        cout << "No se pudo guardar el archivo PDF." << endl;
    }
}

void ReporteCitasPorMedico(ListaCitas &Citas)
{
    int IdMedico;
    cout << "Ingrese el IdMedico: ";
    cin >> IdMedico;
    GeneradorPDF pdf("Reporte_Citas_Medico_" + to_string(IdMedico) + ".pdf",
                    "REPORTE DE CITAS POR MEDICO",
                    Rellenar("ID CITA", 8) + Rellenar("ID PACIENTE", 12) + Rellenar("ID MEDICO", 10)
                    + Rellenar("ID ESPECIALIDAD", 15) + Rellenar("FECHA", 12) + Rellenar("HORA", 8)
                    + Rellenar("MOTIVO", 30) + Rellenar("ESTADO", 8));
    if (!pdf.Valido()){
        cout << "No se pudo crear el documento PDF." << endl;
        return;
    }
    pnodoCita aux = Citas.primero;
    bool encontrado = false;
    while (aux != NULL){
        if (aux->IdMedico == IdMedico){
            pdf.Linea(Rellenar(to_string(aux->IdCita), 8) + Rellenar(to_string(aux->IdPaciente), 12)
                        + Rellenar(to_string(aux->IdMedico), 10) + Rellenar(to_string(aux->IdEspecialidad), 15)
                        + Rellenar(aux->Fecha, 12) + Rellenar(aux->Hora, 8)
                        + Rellenar(aux->Motivo, 30) + Rellenar(aux->Estado, 8));
            encontrado = true;
        }
        aux = aux->siguiente;
    }
    if (!encontrado){
        pdf.Linea("(No hay citas registradas para este medico)");
    }
    if (pdf.Guardar()){
        cout << "Reporte generado: Reporte_Citas_Medico_" << IdMedico << ".pdf" << endl;
    }
    else{
        cout << "No se pudo guardar el archivo PDF." << endl;
    }
}






//╔═══════════════════════════════════════════════════════════════════╗
//║                              MOSTRAR                              ║
//╚═══════════════════════════════════════════════════════════════════╝

void ListaCitas::Mostrar()
{
    pnodoCita aux = primero;
    while (aux != NULL)
    {
        cout << "IdCita: " << aux->IdCita
                << " | IdPaciente: " << aux->IdPaciente
                << " | IdMedico: " << aux->IdMedico
                << " | IdEspecialidad: " << aux->IdEspecialidad
                << " | Fecha: " << aux->Fecha
                << " | Hora: " << aux->Hora
                << " | Motivo: " << aux->Motivo
                << " | Estado: " << aux->Estado
                << endl;
        aux = aux->siguiente;
    }
}

//╔═════════════════════════════════════════════════════════════════════════════╗
//║                              CARGAR CITAS                                   ║
//╚═════════════════════════════════════════════════════════════════════════════╝

void CargarCitas(ListaCitas &Citas, TablaHashingPacientes &Pacientes,
    TablaHashingMedicos &Medicos, ListaEspecialidades &Especialidades)
{
    ifstream archivo("Citas.csv");
    if (!archivo){
        cout << "No se pudo abrir el archivo Citas.csv" << endl;
        return;
    }

    
    string encabezado;
    getline(archivo, encabezado); // Saltar encabezado del CSV
string IdCitaTexto;
    string IdPacienteTexto;
    string IdMedicoTexto;
    string IdEspecialidadTexto;
    string Fecha;
    string Hora;
    string Motivo;
    string Estado;

    while (getline(archivo, IdCitaTexto, ';') &&
            getline(archivo, IdPacienteTexto, ';') &&
            getline(archivo, IdMedicoTexto, ';') &&
            getline(archivo, IdEspecialidadTexto, ';') &&
            getline(archivo, Fecha, ';') &&
            getline(archivo, Hora, ';') &&
            getline(archivo, Motivo, ';') &&
            getline(archivo, Estado))
    {
        int IdCita = stoi(IdCitaTexto);
        int IdPaciente = stoi(IdPacienteTexto);
        int IdMedico = stoi(IdMedicoTexto);
        int IdEspecialidad = stoi(IdEspecialidadTexto);

        if (!Pacientes.Existe(IdPaciente)){
            continue;
        }

        if (!Medicos.Existe(IdMedico)){
            continue;
        }

        if (!Especialidades.Existe(IdEspecialidad)){
            continue;
        }

        if (Citas.Existe(IdCita)){
            continue;
        }

        if (Estado != "0" && Estado != "1"){
            continue;
        }

        Citas.Insertar(IdCita, IdPaciente, IdMedico, IdEspecialidad, Fecha, Hora, Motivo, Estado
        );
    }
    archivo.close();
    cout << "Citas cargadas correctamente." << endl;
}   