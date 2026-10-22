#pragma once

class NodoEspecialidad
{
public:

    NodoEspecialidad(int nuevoIdEspecialidad, string nuevoNombre, string nuevaDescripcion)
    {
        IdEspecialidad = nuevoIdEspecialidad;
        Nombre = nuevoNombre;
        Descripcion = nuevaDescripcion;
        siguiente = NULL;
        anterior = NULL;
    }

private:
    int IdEspecialidad;
    string Nombre;
    string Descripcion;
    NodoEspecialidad *siguiente;
    NodoEspecialidad *anterior;

    friend class ListaEspecialidades;
};

typedef NodoEspecialidad *pnodoEspecialidad;


//╔══════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
//║                                  LISTA DOBLE CIRCULAR ESPECIALIDADES                                         ║
//╚══════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

class ListaEspecialidades
{
public:
    ListaEspecialidades(){
        primero = NULL;
    }
    ~ListaEspecialidades();
    bool ListaVacia(){
        return primero == NULL;
    }
    bool Existe(int IdEspecialidad);
    bool Insertar(int IdEspecialidad, string Nombre, string Descripcion);
    bool Buscar(int IdEspecialidad);
    bool Eliminar(int IdEspecialidad);
    bool Modificar(int IdEspecialidad, string Nombre, string Descripcion);
    void Mostrar();
    void ReportePDF(GeneradorPDF &pdf);

private:
    pnodoEspecialidad primero;
};

ListaEspecialidades::~ListaEspecialidades()
{
    if (ListaVacia()){
        return;
    }

    pnodoEspecialidad aux = primero->siguiente;

    while (aux != primero){
        pnodoEspecialidad temp = aux;
        aux = aux->siguiente;
        delete temp;
    }

    delete primero;
    primero = NULL;
}

//╔═══════════════════════════════════════════════════════════════╗
//║                         VALIDACION                            ║
//╚═══════════════════════════════════════════════════════════════╝
bool ListaEspecialidades::Existe(int IdEspecialidad)
{
    if (ListaVacia()){
        return false;
    }
    pnodoEspecialidad aux = primero;
    do{
        if (aux->IdEspecialidad == IdEspecialidad){
            return true;
        }
        aux = aux->siguiente;
    }while (aux != primero);
    return false;
}


//╔═══════════════════════════════════════════════════════════════╗
//║                          INSERTAR                             ║
//╚═══════════════════════════════════════════════════════════════╝
bool ListaEspecialidades::Insertar(int IdEspecialidad, string Nombre, string Descripcion)
{
    if (Existe(IdEspecialidad)){
        return false;
    }

    pnodoEspecialidad nuevo = new NodoEspecialidad(
        IdEspecialidad,
        Nombre,
        Descripcion
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

//╔═══════════════════════════════════════════════════════════════╗
//║                           BUSCAR                              ║
//╚═══════════════════════════════════════════════════════════════╝
bool ListaEspecialidades::Buscar(int IdEspecialidad)
{
    if (ListaVacia()){
        return false;
    }

    pnodoEspecialidad aux = primero;

    do{
        if (aux->IdEspecialidad == IdEspecialidad){
            cout << "IdEspecialidad: " << aux->IdEspecialidad << endl;
            cout << "Nombre: " << aux->Nombre << endl;
            cout << "Descripcion: " << aux->Descripcion << endl;

            return true;
        }

        aux = aux->siguiente;

    }while (aux != primero);

    return false;
}

//╔═══════════════════════════════════════════════════════════════════╗
//║                              ELIMINAR                             ║
//╚═══════════════════════════════════════════════════════════════════╝
bool ListaEspecialidades::Eliminar(int IdEspecialidad)
{
    if (ListaVacia()){
        return false;
    }
    pnodoEspecialidad aux=primero;
    do{
        if (aux->IdEspecialidad==IdEspecialidad){
            if (aux==primero&&aux->siguiente==primero){
                delete aux;
                primero = NULL;
            }
            else{
                aux->anterior->siguiente=aux->siguiente;
                aux->siguiente->anterior=aux->anterior;
                if (aux==primero){
                    primero=aux->siguiente;
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
//║                              REPORTE                            ║
//╚═════════════════════════════════════════════════════════════════╝
void ListaEspecialidades::ReportePDF(GeneradorPDF &pdf)
{
    if (ListaVacia()){
        pdf.Linea("(No hay especialidades registradas)");
        return;
    }
    pnodoEspecialidad aux = primero;
    do{
        pdf.Linea(Rellenar(to_string(aux->IdEspecialidad), 8) + Rellenar(aux->Nombre, 25)
                    + Rellenar(aux->Descripcion, 85));
        aux = aux->siguiente;
    } while (aux != primero);
}

void ReporteEspecialidades(ListaEspecialidades &Especialidades)
{
    GeneradorPDF pdf("Reporte_Especialidades.pdf", "REPORTE DE ESPECIALIDADES",
                    Rellenar("ID", 8) + Rellenar("NOMBRE", 25) + Rellenar("DESCRIPCION", 85));
    if (!pdf.Valido()){
        cout << "No se pudo crear el documento PDF." << endl;
        return;
    }
    Especialidades.ReportePDF(pdf);
    if (pdf.Guardar()){
        cout << "Reporte generado: Reporte_Especialidades.pdf" << endl;
    }
    else{
        cout << "No se pudo guardar el archivo PDF." << endl;
    }
}



//╔═════════════════════════════════════════════════════════════════╗
//║                             Modificar                           ║
//╚═════════════════════════════════════════════════════════════════╝
bool ListaEspecialidades::Modificar(int IdEspecialidad, string Nombre, string Descripcion)
{
    if (ListaVacia()){
        return false;
    }

    pnodoEspecialidad aux = primero;

    do{
        if (aux->IdEspecialidad == IdEspecialidad){
            aux->Nombre = Nombre;
            aux->Descripcion = Descripcion;
            return true;
        }

        aux = aux->siguiente;

    }while (aux != primero);

    return false;
}


//╔═══════════════════════════════════════════════════════════════╗
//║                           MOSTRAR                             ║
//╚═══════════════════════════════════════════════════════════════╝

void ListaEspecialidades::Mostrar()
{
    if (ListaVacia()){
        cout << "Lista vacia." << endl;
        return;
    }
    pnodoEspecialidad aux = primero;

    do{
        cout << "IdEspecialidad: " << aux->IdEspecialidad
                << " | Nombre: " << aux->Nombre
                << " | Descripcion: " << aux->Descripcion
                << endl;

        aux = aux->siguiente;

    }while (aux != primero);
}

//╔══════════════════════════════════════════════════════════════════════════════╗
//║                           CARGAR ESPECIALIDADES                              ║
//╚══════════════════════════════════════════════════════════════════════════════╝

void CargarEspecialidades(ListaEspecialidades &Especialidades)
{
    ifstream archivo("Especialidades.txt");

    if (!archivo){
        cout << "No se pudo abrir el archivo Especialidades.txt" << endl;
        return;
    }

    string IdEspecialidadTexto;
    string Nombre;
    string Descripcion;

    while (getline(archivo, IdEspecialidadTexto, ';') &&
            getline(archivo, Nombre, ';') &&
            getline(archivo, Descripcion))
    {
        int IdEspecialidad = stoi(IdEspecialidadTexto);

        if (Especialidades.Existe(IdEspecialidad)){
            continue;
        }

        Especialidades.Insertar( IdEspecialidad, Nombre, Descripcion);
    }

    archivo.close();

    cout << "Especialidades cargadas correctamente." << endl;
}
















//╔═══════════════════════════════════════════════════════════════════════════════════════════════╗
//║                                          NODO MEDICO                                          ║
//╚═══════════════════════════════════════════════════════════════════════════════════════════════╝

