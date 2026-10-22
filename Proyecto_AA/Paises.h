#pragma once

class NodoPais
{
public:
    NodoPais(int nuevoCodPais, string nuevoNombre){
        CodPais = nuevoCodPais;
        Nombre = nuevoNombre;
        siguiente = NULL;
    }

private:
    int CodPais;
    string Nombre;
    NodoPais *siguiente;
    friend class ListaPaises;
};
typedef NodoPais *pnodoPais;

//╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
//║                                             LISTA SIMPLE PAISES                                             ║
//╚═════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
class ListaPaises
{
private:
    pnodoPais primero;
public:
    ListaPaises();
    bool ListaVacia();
    bool Existe(int CodPais);
    bool Insertar(int CodPais, string Nombre);
    bool Buscar(int CodPais);
    bool Eliminar(int CodPais);
    bool Modificar(int CodPais, string Nombre);
    void Mostrar();
    void ReportePDF(GeneradorPDF &pdf);
    ~ListaPaises();
};

ListaPaises::ListaPaises(){
    primero = NULL;
}

bool ListaPaises::ListaVacia(){
    return primero == NULL;
}

//╔═══════════════════════════════════════════════════════════════╗
//║                         VALIDACION                            ║
//╚═══════════════════════════════════════════════════════════════╝
bool ListaPaises::Existe(int CodPais){
    pnodoPais aux = primero;
    while (aux != NULL){
        if (aux->CodPais == CodPais){
            return true;
        }
        aux = aux->siguiente;
    }
    return false;
}

//╔═══════════════════════════════════════════════════════════════╗
//║                          INSERTAR                             ║
//╚═══════════════════════════════════════════════════════════════╝
bool ListaPaises::Insertar(int CodPais, string Nombre){
    if (Existe(CodPais)){
        return false;
    }
    if (ListaVacia()){
        primero = new NodoPais(CodPais, Nombre);
    }
    else{
        pnodoPais aux = primero;
        while (aux->siguiente != NULL){
            aux = aux->siguiente;
        }
        aux->siguiente = new NodoPais(CodPais, Nombre);
    }
    return true;
}

//╔═════════════════════════════════════════════════════════════════╗
//║                             BUSCAR                              ║
//╚═════════════════════════════════════════════════════════════════╝
bool ListaPaises::Buscar(int CodPais){
    pnodoPais aux = primero;
    while (aux != NULL){
        if (aux->CodPais == CodPais){
            cout << "CodPais: " << aux->CodPais << endl;
            cout << "Nombre: " << aux->Nombre << endl;
            return true;
        }
        aux = aux->siguiente;
    }
    return false;
}

//╔═══════════════════════════════════════════════════════════════════╗
//║                              ELIMINAR                              ║
//╚═══════════════════════════════════════════════════════════════════╝

bool ListaPaises::Eliminar(int CodPais){
    if (ListaVacia()){
        return false;
    }
    if (primero->CodPais == CodPais){
        pnodoPais temp = primero;
        primero = primero->siguiente;
        delete temp;
        return true;
    }
    pnodoPais aux = primero;
    while (aux->siguiente != NULL && aux->siguiente->CodPais!= CodPais){
        aux = aux->siguiente;
    }
    if (aux->siguiente == NULL){
        return false;
    }
    pnodoPais temp = aux->siguiente;
    aux->siguiente = aux->siguiente->siguiente;
    delete temp;
    return true;
}

//╔═════════════════════════════════════════════════════════════════╗
//║                             Modificar                           ║
//╚═════════════════════════════════════════════════════════════════╝


bool ListaPaises::Modificar(int CodPais, string Nombre){
    pnodoPais aux = primero;
    while (aux != NULL){
        if (aux->CodPais == CodPais){
            aux->Nombre = Nombre;
            return true;
        }
        aux = aux->siguiente;
    }
    return false;
}


//╔═══════════════════════════════════════════════════════════════════╗
//║                              REPORTES                             ║
//╚═══════════════════════════════════════════════════════════════════╝
void ListaPaises::ReportePDF(GeneradorPDF &pdf)
{
    pnodoPais aux = primero;
    while (aux != NULL){
        pdf.Linea(Rellenar(to_string(aux->CodPais), 10) + Rellenar(aux->Nombre, 40));
        aux = aux->siguiente;
    }
}

void ReportePaises(ListaPaises &Paises)
{
    GeneradorPDF pdf("Reporte_Paises.pdf", "REPORTE DE PAISES",
                Rellenar("CODIGO", 10) + Rellenar("NOMBRE", 40));
    if (!pdf.Valido()){
        cout << "No se pudo crear el documento PDF." << endl;
        return;
    }
    if (Paises.ListaVacia()){
        pdf.Linea("(No hay paises registrados)");
    }
    else{
        Paises.ReportePDF(pdf);
    }
    if (pdf.Guardar()){
        cout << "Reporte generado: Reporte_Paises.pdf" << endl;
    }
    else{
        cout << "No se pudo guardar el archivo PDF." << endl;
    }
}

//╔═══════════════════════════════════════════════════════════════════╗
//║                              MOSTRAR                              ║
//╚═══════════════════════════════════════════════════════════════════╝
void ListaPaises::Mostrar(){
    pnodoPais aux = primero;
    while (aux != NULL)
    {
        cout << "CodPais: " << aux->CodPais<< " | Nombre: " << aux->Nombre << endl;
        aux = aux->siguiente;
    }
}

ListaPaises::~ListaPaises()
{
    pnodoPais aux = primero;
    while (aux != NULL)
    {
        pnodoPais temp = aux;
        aux = aux->siguiente;
        delete temp;
    }
}


//╔═════════════════════════════════════════════════════════════════════╗
//║                             CARGAR PAISES                           ║
//╚═════════════════════════════════════════════════════════════════════╝
void CargarPaises(ListaPaises &Paises){
    ifstream archivo("Paises.csv");
    if (!archivo){
        cout << "No se pudo abrir el archivo Paises.csv" << endl;
        return;
    }
    string linea;
    getline(archivo, linea); // Saltar encabezado del CSV
    string codPaisTexto;
    string Nombre;
    while (getline(archivo, linea)){
        if (linea != ""){
            size_t separador = linea.find(';');
            if (separador != string::npos)
            {
                codPaisTexto = linea.substr(0, separador);
                Nombre = linea.substr(separador + 1);
                int CodPais = stoi(codPaisTexto);
                Paises.Insertar(CodPais, Nombre);
            }
        }
    }

    archivo.close();
    cout << "Paises cargados correctamente." << endl;
}

