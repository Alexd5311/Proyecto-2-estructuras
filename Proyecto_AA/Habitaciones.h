#pragma once

class NodoHabitacion
{
public:
    NodoHabitacion(int nuevoIdHabitacion, int nuevoNumero, int nuevoPiso, int nuevoTipo, int nuevoEstado)
    {
        IdHabitacion = nuevoIdHabitacion;
        Numero = nuevoNumero;
        Piso = nuevoPiso;
        Tipo = nuevoTipo;
        Estado = nuevoEstado;
        siguiente = NULL;
        anterior = NULL;
    }
private:
    int IdHabitacion;
    int Numero;
    int Piso;
    int Tipo;
    int Estado;
    NodoHabitacion *siguiente;
    NodoHabitacion *anterior;
    friend class ListaHabitaciones;
};

typedef NodoHabitacion *pnodoHabitacion;

//╔═══════════════════════════════════════════════════════════════════════════════════════════╗
//║                            LISTA DOBLE CIRCULARHABITACIONES                               ║
//╚═══════════════════════════════════════════════════════════════════════════════════════════╝

class ListaHabitaciones
{
public:
    ListaHabitaciones(){
        primero = NULL;
    }
    ~ListaHabitaciones();
    bool ListaVacia(){
        return primero == NULL;
    }
    bool Existe(int IdHabitacion);
    bool Insertar(int IdHabitacion, int Numero, int Piso, int Tipo, int Estado);
    bool Buscar(int IdHabitacion);
    bool Eliminar(int IdHabitacion);
    bool Modificar(int IdHabitacion, int Numero, int Piso, int Tipo, int Estado);
    void Mostrar();
    int ReportePDF(GeneradorPDF &pdf, int Estado);
private:
    pnodoHabitacion primero;
};

ListaHabitaciones::~ListaHabitaciones()
{
    if (ListaVacia()){
        return;
    }

    pnodoHabitacion aux = primero->siguiente;

    while (aux != primero)
    {
        pnodoHabitacion temp = aux;
        aux = aux->siguiente;
        delete temp;
    }

    delete primero;
    primero = NULL;
}

//╔═══════════════════════════════════════════════════════════════╗
//║                         VALIDACION                            ║
//╚═══════════════════════════════════════════════════════════════╝
bool ListaHabitaciones::Existe(int IdHabitacion)
{
    if (ListaVacia()){
        return false;
    }
    pnodoHabitacion aux = primero;
    do{
        if (aux->IdHabitacion == IdHabitacion){
            return true;
        }
        aux = aux->siguiente;
    }
    while (aux != primero);
    return false;
}

//╔═══════════════════════════════════════════════════════════════╗
//║                          INSERTAR                             ║
//╚═══════════════════════════════════════════════════════════════╝
bool ListaHabitaciones::Insertar(int IdHabitacion, int Numero, int Piso, int Tipo, int Estado)
{
    if (Existe(IdHabitacion)){
        return false;
    }
    pnodoHabitacion nuevo = new NodoHabitacion(
        IdHabitacion,
        Numero,
        Piso,
        Tipo,
        Estado
    );

    if (ListaVacia()){
        nuevo->siguiente = nuevo;
        nuevo->anterior = nuevo;
        primero = nuevo;
    }
    else{
        nuevo->siguiente = primero;
        nuevo->anterior = primero->anterior;
        primero->anterior->siguiente = nuevo;
        primero->anterior = nuevo;
    }
    return true;
}

//╔═════════════════════════════════════════════════════════════════╗
//║                             BUSCAR                              ║
//╚═════════════════════════════════════════════════════════════════╝
bool ListaHabitaciones::Buscar(int IdHabitacion)
{
    if (ListaVacia()){
        return false;
    }
    pnodoHabitacion aux = primero;
    do{
        if (aux->IdHabitacion == IdHabitacion)
        {
            cout << "IdHabitacion: " << aux->IdHabitacion << endl;
            cout << "Numero: " << aux->Numero << endl;
            cout << "Piso: " << aux->Piso << endl;
            cout << "Tipo: " << aux->Tipo << endl;
            cout << "Estado: " << aux->Estado << endl;
            return true;
        }
        aux = aux->siguiente;
    }while (aux != primero);
    return false;
}




//╔═══════════════════════════════════════════════════════════════════╗
//║                              ELIMINAR                             ║
//╚═══════════════════════════════════════════════════════════════════╝



bool ListaHabitaciones::Eliminar(int IdHabitacion)
{
    if (ListaVacia()){
        return false;
    }
    pnodoHabitacion aux = primero;
    do{
        if (aux->IdHabitacion == IdHabitacion){
            if (aux == primero && aux->siguiente == primero){
                delete aux;
                primero = NULL;
            }
            else{
                aux->anterior->siguiente = aux->siguiente;
                aux->siguiente->anterior = aux->anterior;
                if (aux == primero){
                    primero = aux->siguiente;
                }
                delete aux;
            }
            return true;
        }
        aux = aux->siguiente;
    }while (aux != primero);
    return false;
}

//╔═════════════════════════════════════════════════════════════════╗
//║                             Modificar                           ║
//╚═════════════════════════════════════════════════════════════════╝

bool ListaHabitaciones::Modificar(int IdHabitacion, int Numero, int Piso, int Tipo, int Estado)
{
    if (ListaVacia()){
        return false;
    }
    pnodoHabitacion aux = primero;
    do{
        if (aux->IdHabitacion == IdHabitacion){
            aux->Numero = Numero;
            aux->Piso = Piso;
            aux->Tipo = Tipo;
            aux->Estado = Estado;
            return true;
        }
        aux = aux->siguiente;
    }while (aux != primero);
    return false;
}



//╔═══════════════════════════════════════════════════════════════╗
//║                          REPORTES                             ║
//╚═══════════════════════════════════════════════════════════════╝

void ReporteHabitacionesDisponibles(ListaHabitaciones &Habitaciones)
{
    GeneradorPDF pdf("Reporte_Habitaciones_Disponibles.pdf", "REPORTE DE HABITACIONES DISPONIBLES",
                    Rellenar("ID", 8) + Rellenar("NUMERO", 8) + Rellenar("PISO", 8)
                    + Rellenar("TIPO", 8) + Rellenar("ESTADO", 8));
    if (!pdf.Valido()){
        cout << "No se pudo crear el documento PDF." << endl;
        return;
    }
    if (Habitaciones.ReportePDF(pdf, 0) == 0){
        pdf.Linea("(No hay habitaciones disponibles)");
    }
    if (pdf.Guardar()){
        cout << "Reporte generado: Reporte_Habitaciones_Disponibles.pdf" << endl;
    }
    else{
        cout << "No se pudo guardar el archivo PDF." << endl;
    }
}


void ReporteHabitacionesOcupadas(ListaHabitaciones &Habitaciones)
{
    GeneradorPDF pdf("Reporte_Habitaciones_Ocupadas.pdf", "REPORTE DE HABITACIONES OCUPADAS",
                    Rellenar("ID", 8) + Rellenar("NUMERO", 8) + Rellenar("PISO", 8)
                    + Rellenar("TIPO", 8) + Rellenar("ESTADO", 8));
    if (!pdf.Valido()){
        cout << "No se pudo crear el documento PDF." << endl;
        return;
    }
    if (Habitaciones.ReportePDF(pdf, 1) == 0){
        pdf.Linea("(No hay habitaciones ocupadas)");
    }
    if (pdf.Guardar()){
        cout << "Reporte generado: Reporte_Habitaciones_Ocupadas.pdf" << endl;
    }
    else{
        cout << "No se pudo guardar el archivo PDF." << endl;
    }
}










//╔═════════════════════════════════════════════════════════════════╗
//║                             MOSTRAR                             ║
//╚═════════════════════════════════════════════════════════════════╝
void ListaHabitaciones::Mostrar()
{
    if (ListaVacia()){
        cout << "No hay habitaciones." << endl;
        return;
    }
    pnodoHabitacion aux = primero;
    do{
        cout << "IdHabitacion: " << aux->IdHabitacion
                << " | Numero: " << aux->Numero
                << " | Piso: " << aux->Piso
                << " | Tipo: " << aux->Tipo
                << " | Estado: " << aux->Estado
                << endl;
        aux = aux->siguiente;
    }while (aux != primero);
}

int ListaHabitaciones::ReportePDF(GeneradorPDF &pdf, int Estado)
{
    if (ListaVacia()){
        return 0;
    }
    int cantidad = 0;
    pnodoHabitacion aux = primero;
    do{
        if (aux->Estado == Estado){
            pdf.Linea(Rellenar(to_string(aux->IdHabitacion), 8)
                        + Rellenar(to_string(aux->Numero), 8)
                        + Rellenar(to_string(aux->Piso), 8)
                        + Rellenar(to_string(aux->Tipo), 8)
                        + Rellenar(to_string(aux->Estado), 8));
            cantidad++;
        }
        aux = aux->siguiente;
    }while (aux != primero);
    return cantidad;
}

//╔═════════════════════════════════════════════════════════════════════════════╗
//║                         CARGAR HABITACIONES                                 ║
//╚═════════════════════════════════════════════════════════════════════════════╝

void CargarHabitaciones(ListaHabitaciones &Habitaciones)
{
    ifstream archivo("Habitaciones.txt");

    if (!archivo){
        cout << "No se pudo abrir el archivo Habitaciones.txt" << endl;
        return;
    }

    string IdHabitacionTexto;
    string NumeroTexto;
    string PisoTexto;
    string TipoTexto;
    string EstadoTexto;

    while (getline(archivo, IdHabitacionTexto, ';') &&
            getline(archivo, NumeroTexto, ';') &&
            getline(archivo, PisoTexto, ';') &&
            getline(archivo, TipoTexto, ';') &&
            getline(archivo, EstadoTexto))
    {
        int IdHabitacion = stoi(IdHabitacionTexto);
        int Numero = stoi(NumeroTexto);
        int Piso = stoi(PisoTexto);
        int Tipo = stoi(TipoTexto);
        int Estado = stoi(EstadoTexto);

        if (Habitaciones.Existe(IdHabitacion)){
            continue;
        }

        if (Tipo < 1 || Tipo > 3){
            continue;
        }

        if (Estado < 0 || Estado > 1){
            continue;
        }

        Habitaciones.Insertar(
            IdHabitacion,
            Numero,
            Piso,
            Tipo,
            Estado
        );
    }

    archivo.close();

    cout << "Habitaciones cargadas correctamente." << endl;
}




















//╔═════════════════════════════════════════════════════════════════════════════╗
//║                          NODO HOSPITALIZACION                               ║
//╚═════════════════════════════════════════════════════════════════════════════╝

