//Estudiantes: Aaron Ortiz Blanco, Alejandro Sandoval
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <hpdf.h>



using namespace std;

static void HPDF_STDCALL ManejadorErrorPDF(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data)
{
    cout << "Error de libharu: codigo " << (unsigned int)error_no << ", detalle " << (int)detail_no << endl;
}

string Rellenar(string texto, int ancho)
{
    if ((int)texto.length() > ancho - 1){
        texto = texto.substr(0, ancho - 1);
    }
    while ((int)texto.length() < ancho){
        texto += " ";
    }
    return texto;
}

class GeneradorPDF
{
public:
    GeneradorPDF(string nuevoArchivo, string nuevoTitulo, string nuevasColumnas)
    {
        archivo = nuevoArchivo;
        titulo = nuevoTitulo;
        columnas = nuevasColumnas;
        numeroPagina = 0;
        y = 0;
        doc = HPDF_New(ManejadorErrorPDF, NULL);
        if (doc != NULL){
            fuente = HPDF_GetFont(doc, "Courier", "WinAnsiEncoding");
            fuenteNegrita = HPDF_GetFont(doc, "Courier-Bold", "WinAnsiEncoding");
            NuevaPagina();
        }
    }

    ~GeneradorPDF()
    {
        if (doc != NULL){
            HPDF_Free(doc);
        }
    }

    bool Valido()
    {
        return doc != NULL;
    }

    void Linea(string texto)
    {
        if (y < 60){
            NuevaPagina();
        }
        HPDF_Page_BeginText(pagina);
        HPDF_Page_SetFontAndSize(pagina, fuente, 10);
        HPDF_Page_TextOut(pagina, 50, y, texto.c_str());
        HPDF_Page_EndText(pagina);
        y -= 14;
    }

    bool Guardar()
    {
        return HPDF_SaveToFile(doc, archivo.c_str()) == HPDF_OK;
    }

private:
    void NuevaPagina()
    {
        pagina = HPDF_AddPage(doc);
        HPDF_Page_SetSize(pagina, HPDF_PAGE_SIZE_A4, HPDF_PAGE_LANDSCAPE);
        numeroPagina++;
        float alto = HPDF_Page_GetHeight(pagina);
        float ancho = HPDF_Page_GetWidth(pagina);
        y = alto - 50;

        HPDF_Page_BeginText(pagina);
        HPDF_Page_SetFontAndSize(pagina, fuenteNegrita, 14);
        HPDF_Page_TextOut(pagina, 50, y, titulo.c_str());
        HPDF_Page_EndText(pagina);
        y -= 24;

        HPDF_Page_BeginText(pagina);
        HPDF_Page_SetFontAndSize(pagina, fuenteNegrita, 10);
        HPDF_Page_TextOut(pagina, 50, y, columnas.c_str());
        HPDF_Page_EndText(pagina);
        y -= 6;
        HPDF_Page_MoveTo(pagina, 50, y);
        HPDF_Page_LineTo(pagina, ancho - 50, y);
        HPDF_Page_Stroke(pagina);
        y -= 16;

        string pie = "Pagina " + to_string(numeroPagina);
        HPDF_Page_BeginText(pagina);
        HPDF_Page_SetFontAndSize(pagina, fuente, 9);
        HPDF_Page_TextOut(pagina, ancho - 120, 30, pie.c_str());
        HPDF_Page_EndText(pagina);
    }

    HPDF_Doc doc;
    HPDF_Page pagina;
    HPDF_Font fuente;
    HPDF_Font fuenteNegrita;
    string archivo;
    string titulo;
    string columnas;
    float y;
    int numeroPagina;
};


//╔═══════════════════════════════════════════════════════════════════════════════════════════════╗
//║                                           NODO PAIS                                           ║
//╚═══════════════════════════════════════════════════════════════════════════════════════════════╝
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
    ifstream archivo("Paises.txt");
    if (!archivo){
        cout << "No se pudo abrir el archivo Paises.txt" << endl;
        return;
    }
    string linea;
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















//╔═══════════════════════════════════════════════════════════════════════════════════════════════╗
//║                                          NODO CIUDAD                                          ║
//╚═══════════════════════════════════════════════════════════════════════════════════════════════╝
class NodoCiudad
{
public:

    NodoCiudad(int nuevoCodPais, int nuevoCodCiudad, string nuevoNombre)
    {
        CodPais = nuevoCodPais;
        CodCiudad = nuevoCodCiudad;
        Nombre = nuevoNombre;
        siguiente = NULL;
    }

private:
    int CodPais;
    int CodCiudad;
    string Nombre;
    NodoCiudad *siguiente;
    friend class ListaCiudades;
};

typedef NodoCiudad *pnodoCiudad;


//╔═════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
//║                                           LISTA CIRCULAR CIUDADES                                           ║
//╚═════════════════════════════════════════════════════════════════════════════════════════════════════════════╝

class ListaCiudades
{
public:
    ListaCiudades();
    ~ListaCiudades();
    bool ListaVacia();
    bool Existe(int CodPais, int CodCiudad);
    bool Insertar(int CodPais, int CodCiudad, string Nombre);
    bool Buscar(int CodPais, int CodCiudad);
    bool Eliminar(int CodPais, int CodCiudad);
    bool EliminarPorPais(int CodPais);
    bool Modificar(int CodPais, int CodCiudad, string Nombre);
    void Mostrar();
    int ReportePDF(GeneradorPDF &pdf, int CodPais);
private:
    pnodoCiudad primero;
};

ListaCiudades::ListaCiudades(){
    primero = NULL;
}

bool ListaCiudades::ListaVacia(){
    return primero == NULL;
}


//╔═══════════════════════════════════════════════════════════════╗
//║                         VALIDACION                            ║
//╚═══════════════════════════════════════════════════════════════╝
bool ListaCiudades::Existe(int CodPais, int CodCiudad)
{
    if (ListaVacia()){
        return false;
    }
    pnodoCiudad aux = primero;
    do{
        if (aux->CodPais == CodPais && aux->CodCiudad == CodCiudad){
            return true;
        }
        aux = aux->siguiente;
    } while (aux != primero);
    return false;
}


//╔═══════════════════════════════════════════════════════════════╗
//║                          INSERTAR                             ║
//╚═══════════════════════════════════════════════════════════════╝

bool ListaCiudades::Insertar(int CodPais, int CodCiudad, string Nombre)
{
    if (Existe(CodPais, CodCiudad)){
        return false;
    }
    if (ListaVacia()){
        pnodoCiudad nuevo = new NodoCiudad(CodPais, CodCiudad, Nombre);
        primero = nuevo;
        nuevo->siguiente = primero;
    }
    else{
        pnodoCiudad nuevo = new NodoCiudad(CodPais, CodCiudad, Nombre);
        pnodoCiudad aux = primero;
        while (aux->siguiente != primero)
        {
            aux = aux->siguiente;
        }
        nuevo->siguiente = primero;
        aux->siguiente = nuevo;
    }
    return true;
}


//╔═════════════════════════════════════════════════════════════════╗
//║                             BUSCAR                              ║
//╚═════════════════════════════════════════════════════════════════╝
bool ListaCiudades::Buscar(int CodPais, int CodCiudad)
{
    if (ListaVacia())
    {
        return false;
    }
    pnodoCiudad aux = primero;
    do{
        if (aux->CodPais == CodPais && aux->CodCiudad == CodCiudad){
            cout << "CodPais: " << aux->CodPais << endl;
            cout << "CodCiudad: " << aux->CodCiudad << endl;
            cout << "Nombre: " << aux->Nombre << endl;
            return true;
        }
        aux = aux->siguiente;
    } while (aux != primero);
    return false;
}

//╔═══════════════════════════════════════════════════════════════════╗
//║                              ELIMINAR                              ║
//╚═══════════════════════════════════════════════════════════════════╝

bool ListaCiudades::Eliminar(int CodPais, int CodCiudad)
{
    if (ListaVacia()){
        return false;
    }
    if (primero->CodPais==CodPais && primero->CodCiudad==CodCiudad){
        if (primero->siguiente==primero){
            delete primero;
            primero=NULL;
            return true;
        }
        else{
            pnodoCiudad aux=primero;
            while (aux->siguiente!=primero){
                aux=aux->siguiente;
            }
            pnodoCiudad temp=primero;
            primero=primero->siguiente;
            aux->siguiente=primero;
            delete temp;
            return true;
        }
    }
    pnodoCiudad aux = primero;
    while (aux->siguiente != primero &&!(aux->siguiente->CodPais==CodPais && aux->siguiente->CodCiudad==CodCiudad)){
        aux = aux->siguiente;
    }
    if (aux->siguiente==primero){
        return false;
    }
    pnodoCiudad temp=aux->siguiente;
    aux->siguiente= aux->siguiente->siguiente;
    delete temp;
    return true;
}

bool ListaCiudades::EliminarPorPais(int CodPais)
{
    if (ListaVacia()){
        return false;
    }
    bool eliminado = false;
    pnodoCiudad aux = primero;
    do{
        if (aux->CodPais == CodPais){
            pnodoCiudad temp = aux;
            aux = aux->siguiente;
            Eliminar(temp->CodPais, temp->CodCiudad);
            eliminado = true;
        }
        else{
            aux = aux->siguiente;
        }
    } while (aux != primero);
    return eliminado;
}


//╔═════════════════════════════════════════════════════════════════╗
//║                             Modificar                           ║
//╚═════════════════════════════════════════════════════════════════╝
bool ListaCiudades::Modificar(int CodPais, int CodCiudad, string Nombre)
{
    if (ListaVacia()){
        return false;
    }
    pnodoCiudad aux = primero;
    do{
        if (aux->CodPais == CodPais && aux->CodCiudad == CodCiudad){
            aux->Nombre = Nombre;
            return true;
        }
        aux = aux->siguiente;
    } while (aux != primero);
    return false;
}


//╔═════════════════════════════════════════════════════════════════╗
//║                              REPORTE                            ║
//╚═════════════════════════════════════════════════════════════════╝
int ListaCiudades::ReportePDF(GeneradorPDF &pdf, int CodPais)
{
    int cantidad = 0;
    if (ListaVacia()){
        return cantidad;
    }
    pnodoCiudad aux = primero;
    do{
        if (aux->CodPais == CodPais){
            pdf.Linea(Rellenar(to_string(aux->CodCiudad), 10) + Rellenar(aux->Nombre, 40));
            cantidad++;
        }
        aux = aux->siguiente;
    } while (aux != primero);
    return cantidad;
}

void ReporteCiudades(ListaCiudades &Ciudades, ListaPaises &Paises)
{
    int CodPais;
    cout << " " << endl;
    cout << "Ingrese el CodPais: ";
    cin >> CodPais;

    if (!Paises.Existe(CodPais)){
        cout << "El CodPais no existe." << endl;
        return;
    }

    GeneradorPDF pdf("Reporte_Ciudades.pdf", "REPORTE DE CIUDADES - PAIS " + to_string(CodPais),
                    Rellenar("CODCIUDAD", 10) + Rellenar("NOMBRE", 40));
    if (!pdf.Valido()){
        cout << "No se pudo crear el documento PDF." << endl;
        return;
    }
    if (Ciudades.ReportePDF(pdf, CodPais) == 0){
        pdf.Linea("(Este pais no tiene ciudades registradas)");
    }
    if (pdf.Guardar()){
        cout << "Reporte generado: Reporte_Ciudades.pdf" << endl;
    }
    else{
        cout << "No se pudo guardar el archivo PDF." << endl;
    }
}


//╔═══════════════════════════════════════════════════════════════════╗
//║                              MOSTRAR                              ║
//╚═══════════════════════════════════════════════════════════════════╝
void ListaCiudades::Mostrar()
{
    if (ListaVacia()){
        cout << "La lista de ciudades esta vacia." << endl;
        return;
    }
    pnodoCiudad aux = primero;
    do{
        cout << "CodPais: " << aux->CodPais<< " | CodCiudad: " << aux->CodCiudad<< " | Nombre: " << aux->Nombre << endl;
        aux = aux->siguiente;
    } while (aux != primero);
}



ListaCiudades::~ListaCiudades()
{
    if (ListaVacia()){
        return;
    }
    pnodoCiudad aux = primero;
    while (aux != NULL){
        pnodoCiudad aux1 = primero;
        pnodoCiudad tempo = primero;
        while (aux1->siguiente != primero){
            aux1 = aux1->siguiente;
        }
        primero = primero->siguiente;
        aux1->siguiente = primero;
        delete tempo;
        aux = primero;
        if (primero == aux1){
            delete primero;
            primero = NULL;
            aux = NULL;
        }
    }
}


//╔═════════════════════════════════════════════════════════════════════╗
//║                            CARGAR CIUDADES                          ║
//╚═════════════════════════════════════════════════════════════════════╝

void CargarCiudades(ListaCiudades &Ciudades, ListaPaises &Paises){
    ifstream archivo("Ciudades.txt");
    if (!archivo){
        cout << "No se pudo abrir el archivo Ciudades.txt" << endl;
        return;
    }

    string linea;
    string codPaisTexto;
    string codCiudadTexto;
    string Nombre;
    while (getline(archivo, linea)){
        if (linea != ""){
            size_t separador1 = linea.find(';');
            if (separador1 != string::npos)
            {
                size_t separador2 = linea.find(';', separador1 + 1);
                if (separador2 != string::npos)
                {
                    codPaisTexto = linea.substr(0, separador1);
                    codCiudadTexto = linea.substr(
                        separador1 + 1,
                        separador2 - separador1 - 1
                    );
                    Nombre = linea.substr(separador2 + 1);
                    int CodPais = stoi(codPaisTexto);
                    int CodCiudad = stoi(codCiudadTexto);
                    if (!Paises.Existe(CodPais))
                    {
                        continue;
                    }
                    Ciudades.Insertar(CodPais, CodCiudad, Nombre);
                }
            }
        }
    }
    archivo.close();
    cout << "Ciudades cargadas correctamente." << endl;
}









//╔═══════════════════════════════════════════════════════════════════════════════════════════════╗
//║                                         NODO PACIENTE                                         ║
//╚═══════════════════════════════════════════════════════════════════════════════════════════════╝

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
    ifstream archivo("Medicos.txt");
    if (!archivo){
        cout << "No se pudo abrir el archivo Medicos.txt" << endl;
        return;
    }

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
























//╔═══════════════════════════════════════════════════════════════════════════════════════════════╗
//║                                         NODO CITA                                             ║
//╚═══════════════════════════════════════════════════════════════════════════════════════════════╝

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
    ifstream archivo("Citas.txt");
    if (!archivo){
        cout << "No se pudo abrir el archivo Citas.txt" << endl;
        return;
    }

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






















//╔═════════════════════════════════════════════════════════════════════════════╗
//║                         NODO HABITACION                                     ║
//╚═════════════════════════════════════════════════════════════════════════════╝

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

void CargarHospitalizaciones(ListaHospitalizaciones &Hospitalizaciones, TablaHashingPacientes &Pacientes, 
    ListaHabitaciones &Habitaciones, TablaHashingMedicos &Medicos)
{
    ifstream archivo("Hospitalizaciones.txt");

    if (!archivo){
        cout << "No se pudo abrir el archivo Hospitalizaciones.txt" << endl;
        return;
    }

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


















    //╔════════════════════════════════════════════════════════════════════════════════════════════════════╗
    //║                                               CRUD's                                               ║
    //╚════════════════════════════════════════════════════════════════════════════════════════════════════╝

    //╔════════════════════════════════════════════════════════════════════════╗
    //║                                INSERTAR                                ║
    //╚════════════════════════════════════════════════════════════════════════╝

    //Pais
void InsertarPais(ListaPaises &Paises)
{
    int CodPais;
    string Nombre;
    cout << " " << endl;
    cout << "-------------" << endl;
    cout << "INSERTAR PAIS" << endl;
    cout << "-------------" << endl;
    cout << "Ingrese el CodPais: ";
    cin >> CodPais;
    cin.ignore();
    cout << "Ingrese el nombre del pais: ";
    getline(cin, Nombre);
    if (Paises.Insertar(CodPais, Nombre)){
        cout << "Pais insertado correctamente." << endl;
    }
    else{
        cout << "El CodPais ya existe. No se puede insertar." << endl;
    }
}



//Ciudad
void InsertarCiudad(ListaCiudades &Ciudades, ListaPaises &Paises)
{
    int CodPais;
    int CodCiudad;
    string Nombre;
    cout << "INSERTAR CIUDAD" << endl;
    cout << "---------------" << endl;
    cout << "Ingrese el CodPais: ";
    cin >> CodPais;
    if (!Paises.Existe(CodPais))
    {
        cout << "El CodPais no existe. No se puede insertar la ciudad."<< endl;
        return;
    }
    cout << "Ingrese el CodCiudad: ";
    cin >> CodCiudad;
    cin.ignore();
    cout << "Ingrese el nombre de la ciudad: ";
    getline(cin, Nombre);
    if (Ciudades.Insertar(CodPais, CodCiudad, Nombre)){
        cout << "Ciudad insertada correctamente." << endl;
    }
    else{
        cout << "La ciudad ya existe en ese pais."<< endl;
    }
}

//Pacientes
void InsertarPaciente(TablaHashingPacientes &Pacientes, ListaCiudades &Ciudades)
{
    int IdPaciente;
    string Nombre;
    string FechaNacimiento;
    string Telefono;
    int CodCiudad;
    int CodPais;
    string Correo;
    cout << " " << endl;
    cout << "---------------" << endl;
    cout << "INSERTAR PACIENTE" << endl;
    cout << "-----------------" << endl;
    cout << "Ingrese el IdPaciente: ";
    cin >> IdPaciente;

    if (Pacientes.Existe(IdPaciente)){
        cout << "El IdPaciente ya existe. " << "No se puede insertar." << endl;
        return;
    }
    cin.ignore();
    cout << "Ingrese el nombre: ";
    getline(cin, Nombre);
    cout << "Ingrese la fecha de nacimiento (AAAA-MM-DD): ";
    getline(cin, FechaNacimiento);
    cout << "Ingrese el telefono: ";
    getline(cin, Telefono);
    cout << "Ingrese el CodPais: ";
    cin >> CodPais;
    cout << "Ingrese el CodCiudad: ";
    cin >> CodCiudad;
    if (!Ciudades.Existe(CodPais, CodCiudad))
    {
        cout << "La ciudad no existe en el pais indicado." << endl;
        return;
    }

    cin.ignore();

    cout << "Ingrese el correo: ";
    getline(cin, Correo);
    if (Pacientes.Insertar( IdPaciente, Nombre, FechaNacimiento, Telefono, CodCiudad, CodPais, Correo)){
        cout << "Paciente insertado correctamente." << endl;
    }
}



//Especialidad
void InsertarEspecialidad(ListaEspecialidades &Especialidades)
{
    int IdEspecialidad;
    string Nombre;
    string Descripcion;
    cout << " " << endl;
    cout << "----------------------" << endl;
    cout << "INSERTAR ESPECIALIDAD" << endl;
    cout << "----------------------" << endl;
    cout << "Ingrese el IdEspecialidad: ";
    cin >> IdEspecialidad;
    if (Especialidades.Existe(IdEspecialidad)){
        cout << "El IdEspecialidad ya existe. No se puede insertar." << endl;
        return;
    }
    cin.ignore();
    cout << "Ingrese el nombre: ";
    getline(cin, Nombre);
    cout << "Ingrese la descripcion: ";
    getline(cin, Descripcion);
    if (Especialidades.Insertar(IdEspecialidad, Nombre, Descripcion)){
        cout << "Especialidad insertada correctamente." << endl;
    }
    else{
        cout << "No se pudo insertar la especialidad." << endl;
    }
}

//Medicos
void InsertarMedico(TablaHashingMedicos &Medicos, ListaEspecialidades &Especialidades)
{
    int IdMedico;
    string Nombre;
    string Telefono;
    string Correo;
    int IdEspecialidad;
    cout << " " << endl;
    cout << "----------------" << endl;
    cout << "INSERTAR MEDICO" << endl;
    cout << "----------------" << endl;
    cout << "Ingrese el IdMedico: ";
    cin >> IdMedico;
    if (Medicos.Existe(IdMedico)){
        cout << "El IdMedico ya existe. No se puede insertar." << endl;
        return;
    }
    cout << "Ingrese el IdEspecialidad: ";
    cin >> IdEspecialidad;
    if (!Especialidades.Existe(IdEspecialidad)){
        cout << "El IdEspecialidad no existe. No se puede insertar el medico." << endl;
        return;
    }
    cin.ignore();
    cout << "Ingrese el nombre: ";
    getline(cin, Nombre);
    cout << "Ingrese el telefono: ";
    getline(cin, Telefono);
    cout << "Ingrese el correo: ";
    getline(cin, Correo);
    if (Medicos.Insertar(
        IdMedico,
        Nombre,
        Telefono,
        Correo,
        IdEspecialidad))
    {
        cout << "Medico insertado correctamente." << endl;
    }
    else{
        cout << "No se pudo insertar el medico." << endl;
    }
}

//cITAS
void InsertarCita(ListaCitas &Citas, TablaHashingPacientes &Pacientes,
    TablaHashingMedicos &Medicos, ListaEspecialidades &Especialidades)
{
    int IdCita;
    int IdPaciente;
    int IdMedico;
    int IdEspecialidad;
    string Fecha;
    string Hora;
    string Motivo;
    string Estado;

    cout << " " << endl;
    cout << "----------------" << endl;
    cout << "INSERTAR CITA" << endl;
    cout << "----------------" << endl;
    cout << "Ingrese el IdCita: ";
    cin >> IdCita;
    if (Citas.Existe(IdCita)){
        cout << "El IdCita ya existe. No se puede insertar." << endl;
        return;
    }
    cout << "Ingrese el IdPaciente: ";
    cin >> IdPaciente;
    if (!Pacientes.Existe(IdPaciente)){
        cout << "El IdPaciente no existe. No se puede insertar la cita." << endl;
        return;
    }

    cout << "Ingrese el IdMedico: ";
    cin >> IdMedico;
    if (!Medicos.Existe(IdMedico)){
        cout << "El IdMedico no existe. No se puede insertar la cita." << endl;
        return;
    }

    cout << "Ingrese el IdEspecialidad: ";
    cin >> IdEspecialidad;
    if (!Especialidades.Existe(IdEspecialidad)){
        cout << "El IdEspecialidad no existe. No se puede insertar la cita." << endl;
        return;
    }
    cin.ignore();

    cout << "Ingrese la fecha: ";
    getline(cin, Fecha);
    cout << "Ingrese la hora: ";
    getline(cin, Hora);
    cout << "Ingrese el motivo: ";
    getline(cin, Motivo);
    cout << "Ingrese el estado: ";
    getline(cin, Estado);
    if (Estado != "0" && Estado != "1"){
        cout << "Estado de cita invalido." << endl;
        return;
    }

    if (Citas.Insertar(IdCita, IdPaciente, IdMedico, IdEspecialidad, Fecha, Hora, Motivo, Estado))
    {
        cout << "Cita insertada correctamente." << endl;
    }
    else{
        cout << "No se pudo insertar la cita." << endl;
    }
}


//Habitaciones
void InsertarHabitacion(ListaHabitaciones &Habitaciones)
{
    int IdHabitacion;
    int Numero;
    int Piso;
    int Tipo;
    int Estado;
    cout << " " << endl;
    cout << "----------------------" << endl;
    cout << "INSERTAR HABITACION" << endl;
    cout << "----------------------" << endl;
    cout << "Ingrese el IdHabitacion: ";
    cin >> IdHabitacion;
    if (Habitaciones.Existe(IdHabitacion)){
        cout << "El IdHabitacion ya existe. No se puede insertar." << endl;
        return;
    }
    cout << "Ingrese el numero: ";
    cin >> Numero;
    cout << "Ingrese el piso: ";
    cin >> Piso;
    cout << "Ingrese el tipo (1: Individual, 2: Compartida, 3: UCI): ";
    cin >> Tipo;
    if (Tipo < 1 || Tipo > 3){
        cout << "Tipo de habitacion invalido." << endl;
        return;
    }
    cout << "Ingrese el estado (0: Disponible, 1: Ocupada): ";
    cin >> Estado;
    if (Estado < 0 || Estado > 1){
        cout << "Estado de habitacion invalido." << endl;
        return;
    }
    if (Habitaciones.Insertar(
        IdHabitacion,
        Numero,
        Piso,
        Tipo,
        Estado))
    {
        cout << "Habitacion insertada correctamente." << endl;
    }
    else{
        cout << "No se pudo insertar la habitacion." << endl;
    }
}

//Hospitalizaciones
void InsertarHospitalizacion(ListaHospitalizaciones &Hospitalizaciones, TablaHashingPacientes &Pacientes, 
    ListaHabitaciones &Habitaciones, TablaHashingMedicos &Medicos)
{
    int IdHospitalizacion;
    int IdPaciente;
    int IdHabitacion;
    int IdMedico;
    string FechaIngreso;
    string FechaSalida;
    string Motivo;

    cout << " " << endl;
    cout << "-----------------------------" << endl;
    cout << "INSERTAR HOSPITALIZACION" << endl;
    cout << "-----------------------------" << endl;

    cout << "Ingrese el IdHospitalizacion: ";
    cin >> IdHospitalizacion;
    if (Hospitalizaciones.Existe(IdHospitalizacion)){
        cout << "El IdHospitalizacion ya existe. No se puede insertar." << endl;
        return;
    }

    cout << "Ingrese el IdPaciente: ";
    cin >> IdPaciente;
    if (!Pacientes.Existe(IdPaciente)){
        cout << "El IdPaciente no existe. No se puede insertar la hospitalizacion." << endl;
        return;
    }

    cout << "Ingrese el IdHabitacion: ";
    cin >> IdHabitacion;
    if (!Habitaciones.Existe(IdHabitacion)){
        cout << "El IdHabitacion no existe. No se puede insertar la hospitalizacion." << endl;
        return;
    }

    cout << "Ingrese el IdMedico: ";
    cin >> IdMedico;
    if (!Medicos.Existe(IdMedico)){
        cout << "El IdMedico no existe. No se puede insertar la hospitalizacion." << endl;
        return;
    }

    cin.ignore();
    cout << "Ingrese la fecha de ingreso: ";
    getline(cin, FechaIngreso);

    cout << "Ingrese la fecha de salida (0000 si no tiene): ";
    getline(cin, FechaSalida);

    cout << "Ingrese el motivo: ";
    getline(cin, Motivo);

    if (Hospitalizaciones.Insertar(IdHospitalizacion, IdPaciente, IdHabitacion, IdMedico, FechaIngreso, FechaSalida, Motivo))
    {
        cout << "Hospitalizacion insertada correctamente." << endl;
    }
    else{
        cout << "No se pudo insertar la hospitalizacion." << endl;
    }
}




    //╔════════════════════════════════════════════════════════════════════════╗
    //║                                 BUSCAR                                 ║
    //╚════════════════════════════════════════════════════════════════════════╝

//Paises
void BuscarPais(ListaPaises &Paises)
{
    int CodPais;
    cout << " " << endl;
    cout << "-------------" << endl;
    cout << "BUSCAR PAIS" << endl;
    cout << "-----------" << endl;
    cout << "Ingrese el CodPais: ";
    cin >> CodPais;
    if (!Paises.Buscar(CodPais)){
        cout << "El CodPais no existe." << endl;
    }
}

//Ciudades
void BuscarCiudad(ListaCiudades &Ciudades, ListaPaises &Paises)
{
    int CodPais;
    int CodCiudad;
    cout << " " << endl;
    cout << "-------------" << endl;
    cout << "BUSCAR CIUDAD" << endl;
    cout << "-------------" << endl;

    cout << "Ingrese el CodPais: ";
    cin >> CodPais;

    if (!Paises.Existe(CodPais))
    {
        cout << "El CodPais no existe." << endl;
        return;
    }
    cout << "Ingrese el CodCiudad: ";
    cin >> CodCiudad;
    if (!Ciudades.Buscar(CodPais, CodCiudad))
    {
        cout << "La ciudad no existe." << endl;
    }
}

//Pacientes
void BuscarPaciente(TablaHashingPacientes &Pacientes, ListaCiudades &Ciudades, ListaPaises &Paises)
{
    int IdPaciente;
    int CodCiudad;
    int CodPais;
    cout << " " << endl;
    cout << "---------------" << endl;
    cout << "BUSCAR PACIENTE" << endl;
    cout << "---------------" << endl;

    cout << "Ingrese el CodPais: ";
    cin >> CodPais;
    if (!Paises.Existe(CodPais))
    {
        cout << "El CodPais no existe." << endl;
        return;
    }

        cout << "Ingrese el CodCiudad: ";
    cin >> CodCiudad;
    if (!Ciudades.Buscar(CodPais, CodCiudad))
    {
        cout << "La ciudad no existe." << endl;
    }

    cout << "Ingrese el IdPaciente: ";
    cin >> IdPaciente;
    if (!Pacientes.Buscar(IdPaciente))
    {
        cout << "El IdPaciente no existe." << endl;
    }


}


//Especialidad
void BuscarEspecialidad(ListaEspecialidades &Especialidades)
{
    int IdEspecialidad;
    cout << " " << endl;
    cout << "----------------------" << endl;
    cout << "BUSCAR ESPECIALIDAD" << endl;
    cout << "----------------------" << endl;
    cout << "Ingrese el IdEspecialidad: ";
    cin >> IdEspecialidad;
    if (!Especialidades.Buscar(IdEspecialidad)){
        cout << "El IdEspecialidad no existe." << endl;
    }
}

//Medicos
void BuscarMedico(TablaHashingMedicos &Medicos, ListaEspecialidades &Especialidades)
{
    int IdMedico;
    int IdEspecialidad;
    cout << " " << endl;
    cout << "----------------" << endl;
    cout << "BUSCAR MEDICO" << endl;
    cout << "----------------" << endl;

    cout << "Ingrese el IdEspecialidad: ";
    cin >> IdEspecialidad;
    if (!Especialidades.Buscar(IdEspecialidad)){
        cout << "El IdEspecialidad no existe." << endl;
    }

    cout << "Ingrese el IdMedico: ";
    cin >> IdMedico;
    if (!Medicos.Buscar(IdMedico)){
        cout << "El IdMedico no existe." << endl;
    }
}

//Citas
void BuscarCita(ListaCitas &Citas, TablaHashingMedicos &Medicos, TablaHashingPacientes &Pacientes)
{
    int IdCita;
    int IdPaciente;
    int IdMedico;
    cout << " " << endl;
    cout << "----------------" << endl;
    cout << "BUSCAR CITA" << endl;
    cout << "----------------" << endl;

    cout << "Ingrese el IdPaciente: ";
    cin >> IdPaciente;
    if (!Pacientes.Buscar(IdPaciente))
    {
        cout << "El IdPaciente no existe." << endl;
    }

    cout << "Ingrese el IdMedico: ";
    cin >> IdMedico;
    if (!Medicos.Buscar(IdMedico)){
        cout << "El IdMedico no existe." << endl;
    }

    cout << "Ingrese el IdCita: ";
    cin >> IdCita;
    if (!Citas.Buscar(IdCita)){
        cout << "El IdCita no existe." << endl;
    }
}


//Habitacion
void BuscarHabitacion(ListaHabitaciones &Habitaciones)
{
    int IdHabitacion;
    cout << " " << endl;
    cout << "----------------------" << endl;
    cout << "BUSCAR HABITACION" << endl;
    cout << "----------------------" << endl;
    cout << "Ingrese el IdHabitacion: ";
    cin >> IdHabitacion;
    if (!Habitaciones.Buscar(IdHabitacion)){
        cout << "El IdHabitacion no existe." << endl;
    }
}


//Hospitalizaciones
void BuscarHospitalizacion(ListaHospitalizaciones &Hospitalizaciones,TablaHashingPacientes &Pacientes, ListaHabitaciones &Habitaciones)
{
    int IdHospitalizacion;
    int IdPaciente;
    int IdHabitacion;
    cout << " " << endl;
    cout << "-----------------------------" << endl;
    cout << "BUSCAR HOSPITALIZACION" << endl;
    cout << "-----------------------------" << endl;
    cout << "Ingrese el IdPaciente: ";
    cin >> IdPaciente;
    if (!Pacientes.Buscar(IdPaciente))
    {
        cout << "El IdPaciente no existe." << endl;
    }

    cout << "Ingrese el IdHabitacion: ";
    cin >> IdHabitacion;
    if (!Habitaciones.Buscar(IdHabitacion)){
        cout << "El IdHabitacion no existe." << endl;
    }

    cout << "Ingrese el IdHospitalizacion: ";
    cin >> IdHospitalizacion;
    if (!Hospitalizaciones.Buscar(IdHospitalizacion)){
        cout << "El IdHospitalizacion no existe." << endl;
    }
}


    //╔════════════════════════════════════════════════════════════════════════╗
    //║                                ELIMINAR                                ║
    //╚════════════════════════════════════════════════════════════════════════╝





void EliminarPais(ListaPaises &Paises, ListaCiudades &Ciudades, TablaHashingPacientes &Pacientes,
    ListaCitas &Citas, ListaHospitalizaciones &Hospitalizaciones)
{
    int CodPais;
    cout << " " << endl;
    cout << "-------------" << endl;
    cout << "ELIMINAR PAIS" << endl;
    cout << "-------------" << endl;
    cout << "Ingrese el CodPais: ";
    cin >> CodPais;
    if (!Paises.Existe(CodPais)){
        cout << "El CodPais no existe." << endl;
        return;
    }

    vector<int> idsPacientes = Pacientes.ObtenerIdsPorPais(CodPais);
    for (int idPaciente : idsPacientes){
        Citas.EliminarPorPaciente(idPaciente);
        Hospitalizaciones.EliminarPorPaciente(idPaciente);
    }

    Pacientes.EliminarPorPais(CodPais);
    Ciudades.EliminarPorPais(CodPais);

    if (Paises.Eliminar(CodPais)){
        cout << "Pais eliminado correctamente." << endl;
        cout << "Se eliminaron tambien ciudades, pacientes, citas y hospitalizaciones asociadas." << endl;
    }
    else{
        cout << "No se pudo eliminar el pais." << endl;
    }
}

void EliminarPaciente(TablaHashingPacientes &Pacientes, ListaCitas &Citas, ListaHospitalizaciones &Hospitalizaciones)
{
    int IdPaciente;
    cout << " " << endl;
    cout << "----------------" << endl;
    cout << "ELIMINAR PACIENTE" << endl;
    cout << "----------------" << endl;
    cout << "Ingrese el IdPaciente: ";
    cin >> IdPaciente;

    if (!Pacientes.Existe(IdPaciente)){
        cout << "El IdPaciente no existe." << endl;
        return;
    }

    int citasEliminadas = 0;
    int hospitalizacionesEliminadas = 0;

    if (Citas.EliminarPorPaciente(IdPaciente)){
        citasEliminadas = 1;
    }

    if (Hospitalizaciones.EliminarPorPaciente(IdPaciente)){
        hospitalizacionesEliminadas = 1;
    }

    if (Pacientes.Eliminar(IdPaciente)){
        cout << "Paciente eliminado correctamente." << endl;
        if (citasEliminadas > 0 || hospitalizacionesEliminadas > 0){
            cout << "Se eliminaron registros asociados: "
                 << "citas=" << citasEliminadas << ", hospitalizaciones=" << hospitalizacionesEliminadas << endl;
        }
    }
    else{
        cout << "No se pudo eliminar al paciente." << endl;
    }
}


void EliminarCiudad(ListaCiudades &Ciudades, ListaPaises &Paises, TablaHashingPacientes &Pacientes,
    ListaCitas &Citas, ListaHospitalizaciones &Hospitalizaciones)
{
    int CodPais;
    int CodCiudad;
    cout << " " << endl;
    cout << "-------------" << endl;
    cout << "ELIMINAR CIUDAD" << endl;
    cout << "-------------" << endl;

    cout << "Ingrese el CodPais: ";
    cin >> CodPais;

    if (!Paises.Existe(CodPais))
    {
        cout << "El CodPais no existe." << endl;
        return;
    }
    cout << "Ingrese el CodCiudad: ";
    cin >> CodCiudad;
    if (!Ciudades.Existe(CodPais, CodCiudad))
    {
        cout << "La ciudad no existe." << endl;
        return;
    }

    vector<int> idsPacientes = Pacientes.ObtenerIdsPorCiudad(CodPais, CodCiudad);
    for (int idPaciente : idsPacientes){
        Citas.EliminarPorPaciente(idPaciente);
        Hospitalizaciones.EliminarPorPaciente(idPaciente);
    }

    Pacientes.EliminarPorCiudad(CodPais, CodCiudad);

    if (Ciudades.Eliminar(CodPais, CodCiudad))
    {
        cout << "Ciudad eliminada correctamente." << endl;
        cout << "Se eliminaron tambien pacientes, citas y hospitalizaciones asociadas." << endl;
    }
    else
    {
        cout << "No se pudo eliminar la ciudad." << endl;
    }
}


void EliminarPaciente(TablaHashingPacientes &Pacientes)
{
    int IdPaciente;
    cout << " " << endl;
    cout << "---------------" << endl;
    cout << "ELIMINAR PACIENTE" << endl;
    cout << "---------------" << endl;
    cout << "Ingrese el IdPaciente: ";
    cin >> IdPaciente;

    if (!Pacientes.Existe(IdPaciente))
    {
        cout << "El IdPaciente no existe." << endl;
        return;
    }
    if (Pacientes.Eliminar(IdPaciente))
    {
        cout << "Paciente eliminado correctamente." << endl;
    }
    else
    {
        cout << "No se pudo eliminar el paciente." << endl;
    }
}



void EliminarMedico(TablaHashingMedicos &Medicos, ListaCitas &Citas, ListaHospitalizaciones &Hospitalizaciones)
{
    int IdMedico;
    cout << " " << endl;
    cout << "----------------" << endl;
    cout << "ELIMINAR MEDICO" << endl;
    cout << "----------------" << endl;
    cout << "Ingrese el IdMedico: ";
    cin >> IdMedico;

    if (!Medicos.Existe(IdMedico))
    {
        cout << "El IdMedico no existe." << endl;
        return;
    }

    Citas.EliminarPorMedico(IdMedico);
    Hospitalizaciones.EliminarPorMedico(IdMedico);

    if (Medicos.Eliminar(IdMedico))
    {
        cout << "Medico eliminado correctamente." << endl;
    }
    else
    {
        cout << "No se pudo eliminar el medico." << endl;
    }
}

void EliminarEspecialidad(ListaEspecialidades &Especialidades, TablaHashingMedicos &Medicos,
    ListaCitas &Citas, ListaHospitalizaciones &Hospitalizaciones)
{
    int IdEspecialidad;
    cout << " " << endl;
    cout << "----------------------" << endl;
    cout << "ELIMINAR ESPECIALIDAD" << endl;
    cout << "----------------------" << endl;
    cout << "Ingrese el IdEspecialidad: ";
    cin >> IdEspecialidad;

    if (!Especialidades.Existe(IdEspecialidad))
    {
        cout << "La especialidad no existe." << endl;
        return;
    }

    vector<int> idsMedicos = Medicos.ObtenerIdsPorEspecialidad(IdEspecialidad);
    for (int idMedico : idsMedicos)
    {
        Citas.EliminarPorMedico(idMedico);
        Hospitalizaciones.EliminarPorMedico(idMedico);
    }

    Medicos.EliminarPorEspecialidad(IdEspecialidad);

    if (Especialidades.Eliminar(IdEspecialidad))
    {
        cout << "Especialidad eliminada correctamente." << endl;
    }
    else
    {
        cout << "No se pudo eliminar la especialidad." << endl;
    }
}

void EliminarCita(ListaCitas &Citas)
{
    int IdCita;
    cout << " " << endl;
    cout << "----------------" << endl;
    cout << "ELIMINAR CITA" << endl;
    cout << "----------------" << endl;
    cout << "Ingrese el IdCita: ";
    cin >> IdCita;

    if (!Citas.Existe(IdCita))
    {
        cout << "El IdCita no existe." << endl;
        return;
    }

    if (Citas.Eliminar(IdCita))
    {
        cout << "Cita eliminada correctamente." << endl;
    }
    else
    {
        cout << "No se pudo eliminar la cita." << endl;
    }
}

void EliminarHabitacion(ListaHabitaciones &Habitaciones, ListaHospitalizaciones &Hospitalizaciones)
{
    int IdHabitacion;
    cout << " " << endl;
    cout << "----------------------" << endl;
    cout << "ELIMINAR HABITACION" << endl;
    cout << "----------------------" << endl;
    cout << "Ingrese el IdHabitacion: ";
    cin >> IdHabitacion;

    if (!Habitaciones.Existe(IdHabitacion))
    {
        cout << "La habitacion no existe." << endl;
        return;
    }

    Hospitalizaciones.EliminarPorHabitacion(IdHabitacion);

    if (Habitaciones.Eliminar(IdHabitacion))
    {
        cout << "Habitacion eliminada correctamente." << endl;
    }
    else
    {
        cout << "No se pudo eliminar la habitacion." << endl;
    }
}

void EliminarHospitalizacion(ListaHospitalizaciones &Hospitalizaciones)
{
    int IdHospitalizacion;
    cout << " " << endl;
    cout << "-----------------------------" << endl;
    cout << "ELIMINAR HOSPITALIZACION" << endl;
    cout << "-----------------------------" << endl;
    cout << "Ingrese el IdHospitalizacion: ";
    cin >> IdHospitalizacion;

    if (!Hospitalizaciones.Existe(IdHospitalizacion))
    {
        cout << "La hospitalizacion no existe." << endl;
        return;
    }

    if (Hospitalizaciones.Eliminar(IdHospitalizacion))
    {
        cout << "Hospitalizacion eliminada correctamente." << endl;
    }
    else
    {
        cout << "No se pudo eliminar la hospitalizacion." << endl;
    }
}

//╔═════════════════════════════════════════════════════════════════╗
//║                             Modificar                           ║
//╚═════════════════════════════════════════════════════════════════╝

void ModificarPais(ListaPaises &Paises)
{
    int CodPais;
    string Nombre;
    cout << " " << endl;
    cout << "-------------" << endl;
    cout << "MODIFICAR PAIS" << endl;
    cout << "-------------" << endl;
    cout << "Ingrese el CodPais: ";
    cin >> CodPais;
    if (!Paises.Existe(CodPais)){
        cout << "El CodPais no existe." << endl;
        return;
    }
    cin.ignore();
    cout << "Ingrese el nuevo nombre del pais: ";
    getline(cin, Nombre);
    if (Paises.Modificar(CodPais, Nombre)){
        cout << "Pais modificado correctamente." << endl;
    }
    else{
        cout << "No se pudo modificar el pais." << endl;
    }
}


void ModificarCiudad(ListaCiudades &Ciudades, ListaPaises &Paises)
{
    int CodPais;
    int CodCiudad;
    string Nombre;
    cout << " " << endl;
    cout << "-------------" << endl;
    cout << "MODIFICAR CIUDAD" << endl;
    cout << "-------------" << endl;
    cout << "Ingrese el CodPais: ";
    cin >> CodPais;
    if (!Paises.Existe(CodPais)){
        cout << "El CodPais no existe." << endl;
        return;
    }
    cout << "Ingrese el CodCiudad: ";
    cin >> CodCiudad;
    if (!Ciudades.Existe(CodPais, CodCiudad)){
        cout << "La ciudad no existe." << endl;
        return;
    }
    cin.ignore();
    cout << "Ingrese el nuevo nombre de la ciudad: ";
    getline(cin, Nombre);
    if (Ciudades.Modificar(CodPais, CodCiudad, Nombre)){
        cout << "Ciudad modificada correctamente." << endl;
    }
    else{
        cout << "No se pudo modificar la ciudad." << endl;
    }
}

void ModificarPaciente(TablaHashingPacientes &Pacientes, ListaCiudades &Ciudades)
{
    int IdPaciente;
    string Nombre;
    string FechaNacimiento;
    string Telefono;
    int CodCiudad;
    int CodPais;
    string Correo;

    cout << " " << endl;
    cout << "---------------" << endl;
    cout << "MODIFICAR PACIENTE" << endl;
    cout << "---------------" << endl;
    cout << "Ingrese el IdPaciente: ";
    cin >> IdPaciente;

    if (!Pacientes.Existe(IdPaciente)){
        cout << "El IdPaciente no existe." << endl;
        return;
    }

    cin.ignore();
    cout << "Ingrese el nuevo nombre: ";
    getline(cin, Nombre);
    cout << "Ingrese la nueva fecha de nacimiento (AAAA-MM-DD): ";
    getline(cin, FechaNacimiento);
    cout << "Ingrese el nuevo telefono: ";
    getline(cin, Telefono);
    cout << "Ingrese el nuevo CodPais: ";
    cin >> CodPais;
    cout << "Ingrese el nuevo CodCiudad: ";
    cin >> CodCiudad;

    if (!Ciudades.Existe(CodPais, CodCiudad)){
        cout << "La ciudad no existe en el pais indicado." << endl;
        return;
    }

    cin.ignore();
    cout << "Ingrese el nuevo correo: ";
    getline(cin, Correo);

    if (Pacientes.Modificar(IdPaciente, Nombre, FechaNacimiento, Telefono, CodCiudad, CodPais, Correo)){
        cout << "Paciente modificado correctamente." << endl;
    }
    else{
        cout << "No se pudo modificar el paciente." << endl;
    }
}

void ModificarEspecialidad(ListaEspecialidades &Especialidades)
{
    int IdEspecialidad;
    string Nombre;
    string Descripcion;

    cout << " " << endl;
    cout << "----------------------" << endl;
    cout << "MODIFICAR ESPECIALIDAD" << endl;
    cout << "----------------------" << endl;
    cout << "Ingrese el IdEspecialidad: ";
    cin >> IdEspecialidad;

    if (!Especialidades.Existe(IdEspecialidad)){
        cout << "El IdEspecialidad no existe." << endl;
        return;
    }

    cin.ignore();
    cout << "Ingrese el nuevo nombre: ";
    getline(cin, Nombre);
    cout << "Ingrese la nueva descripcion: ";
    getline(cin, Descripcion);

    if (Especialidades.Modificar(IdEspecialidad, Nombre, Descripcion)){
        cout << "Especialidad modificada correctamente." << endl;
    }
    else{
        cout << "No se pudo modificar la especialidad." << endl;
    }
}

void ModificarMedico(TablaHashingMedicos &Medicos, ListaEspecialidades &Especialidades)
{
    int IdMedico;
    string Nombre;
    string Telefono;
    string Correo;
    int IdEspecialidad;

    cout << " " << endl;
    cout << "----------------" << endl;
    cout << "MODIFICAR MEDICO" << endl;
    cout << "----------------" << endl;
    cout << "Ingrese el IdMedico: ";
    cin >> IdMedico;

    if (!Medicos.Existe(IdMedico)){
        cout << "El IdMedico no existe." << endl;
        return;
    }

    cout << "Ingrese el nuevo IdEspecialidad: ";
    cin >> IdEspecialidad;

    if (!Especialidades.Existe(IdEspecialidad)){
        cout << "El IdEspecialidad no existe." << endl;
        return;
    }

    cin.ignore();
    cout << "Ingrese el nuevo nombre: ";
    getline(cin, Nombre);
    cout << "Ingrese el nuevo telefono: ";
    getline(cin, Telefono);
    cout << "Ingrese el nuevo correo: ";
    getline(cin, Correo);

    if (Medicos.Modificar(IdMedico, Nombre, Telefono, Correo, IdEspecialidad)){
        cout << "Medico modificado correctamente." << endl;
    }
    else{
        cout << "No se pudo modificar el medico." << endl;
    }
}

void ModificarCita(ListaCitas &Citas, TablaHashingPacientes &Pacientes,
    TablaHashingMedicos &Medicos, ListaEspecialidades &Especialidades)
{
    int IdCita;
    int IdPaciente;
    int IdMedico;
    int IdEspecialidad;
    string Fecha;
    string Hora;
    string Motivo;
    string Estado;

    cout << " " << endl;
    cout << "----------------" << endl;
    cout << "MODIFICAR CITA" << endl;
    cout << "----------------" << endl;
    cout << "Ingrese el IdCita: ";
    cin >> IdCita;

    if (!Citas.Existe(IdCita)){
        cout << "El IdCita no existe." << endl;
        return;
    }

    cin.ignore();
    cout << "Ingrese el nuevo IdPaciente: ";
    cin >> IdPaciente;
    if (!Pacientes.Existe(IdPaciente)){
        cout << "El IdPaciente no existe." << endl;
        return;
    }
    cout << "Ingrese el nuevo IdMedico: ";
    cin >> IdMedico;
    if (!Medicos.Existe(IdMedico)){
        cout << "El IdMedico no existe." << endl;
        return;
    }
    cout << "Ingrese el nuevo IdEspecialidad: ";
    cin >> IdEspecialidad;
    if (!Especialidades.Existe(IdEspecialidad)){
        cout << "El IdEspecialidad no existe." << endl;
        return;
    }
    cin.ignore();
    cout << "Ingrese la nueva fecha: ";
    getline(cin, Fecha);
    cout << "Ingrese la nueva hora: ";
    getline(cin, Hora);
    cout << "Ingrese el nuevo motivo: ";
    getline(cin, Motivo);
    cout << "Ingrese el nuevo estado(0. Pendiente, 1. Confirmada): ";
    getline(cin, Estado);
    cout << "Estado ingresado: " << Estado << endl;
    if (Estado != "0" && Estado != "1"){
        cout << "Estado de cita invalido." << endl;
        return;
    }
    cout << "Estado validado: " << Estado << endl;
    if (Citas.Modificar(IdCita, IdPaciente, IdMedico, IdEspecialidad, Fecha, Hora, Motivo, Estado)){
        cout << "Cita modificada correctamente." << endl;
    }
    else{
        cout << "No se pudo modificar la cita." << endl;
    }
}


void ModificarHabitacion(ListaHabitaciones &Habitaciones)
{
    int IdHabitacion;
    int Numero;
    int Piso;
    int Tipo;
    int Estado;

    cout << " " << endl;
    cout << "----------------------" << endl;
    cout << "MODIFICAR HABITACION" << endl;
    cout << "----------------------" << endl;
    cout << "Ingrese el IdHabitacion: ";
    cin >> IdHabitacion;

    if (!Habitaciones.Existe(IdHabitacion)){
        cout << "La habitacion no existe." << endl;
        return;
    }

    cout << "Ingrese el nuevo numero: ";
    cin >> Numero;
    cout << "Ingrese el nuevo piso: ";
    cin >> Piso;
    cout << "Ingrese el nuevo tipo (1. Individual, 2. Compartida, 3. UCI): ";
    cin >> Tipo;
    if (Tipo < 1 || Tipo > 3){
        cout << "Tipo de habitacion invalido." << endl;
        return;
    }
    cout << "Ingrese el nuevo estado (0. Disponible, 1. Ocupada): ";
    cin >> Estado;
    if (Estado < 0 || Estado > 1){
        cout << "Estado de habitacion invalido." << endl;
        return;
    }
    cout << "Estado escogido: " << Estado << endl;

    if (Habitaciones.Modificar(IdHabitacion, Numero, Piso, Tipo, Estado)){
        cout << "Habitacion modificada correctamente." << endl;
    }
    else{
        cout << "No se pudo modificar la habitacion." << endl;
    }
}


void ModificarHospitalizacion(ListaHospitalizaciones &Hospitalizaciones, TablaHashingPacientes &Pacientes,
    ListaHabitaciones &Habitaciones, TablaHashingMedicos &Medicos)
{
    int IdHospitalizacion;
    int IdPaciente;
    int IdHabitacion;
    int IdMedico;
    string FechaIngreso;
    string FechaSalida;
    string Motivo;

    cout << " " << endl;
    cout << "-----------------------------" << endl;
    cout << "MODIFICAR HOSPITALIZACION" << endl;
    cout << "-----------------------------" << endl;
    cout << "Ingrese el IdHospitalizacion: ";
    cin >> IdHospitalizacion;

    if (!Hospitalizaciones.Existe(IdHospitalizacion)){
        cout << "La hospitalizacion no existe." << endl;
        return;
    }

    cout << "Ingrese el nuevo IdPaciente: ";
    cin >> IdPaciente;
    if (!Pacientes.Existe(IdPaciente)){
        cout << "El IdPaciente no existe." << endl;
        return;
    }

    cout << "Ingrese el nuevo IdHabitacion: ";
    cin >> IdHabitacion;
    if (!Habitaciones.Existe(IdHabitacion)){
        cout << "El IdHabitacion no existe." << endl;
        return;
    }

    cout << "Ingrese el nuevo IdMedico: ";
    cin >> IdMedico;
    if (!Medicos.Existe(IdMedico)){
        cout << "El IdMedico no existe." << endl;
        return;
    }

    cin.ignore();
    cout << "Ingrese la nueva fecha de ingreso: ";
    getline(cin, FechaIngreso);
    
    cout << "Ingrese la nueva fecha de salida (0000 si no tiene): ";
    getline(cin, FechaSalida);

    cout << "Ingrese el nuevo motivo: ";
    getline(cin, Motivo);
    if (Hospitalizaciones.Modificar(IdHospitalizacion, IdPaciente, IdHabitacion, IdMedico, FechaIngreso, FechaSalida, Motivo)){
        cout << "Hospitalizacion modificada correctamente." << endl;
    }
    else{
        cout << "No se pudo modificar la hospitalizacion." << endl;
    }
}















//╔════════════════════════════════════════════════════════════════════════════════════════╗
//║                                         MAIN                                           ║
//╚════════════════════════════════════════════════════════════════════════════════════════╝
int main()
{
    ListaPaises Paises;
    ListaCiudades Ciudades;
    TablaHashingPacientes Pacientes;
    ListaEspecialidades Especialidades;
    TablaHashingMedicos Medicos;
    ListaCitas Citas;
    ListaHabitaciones Habitaciones;
    ListaHospitalizaciones Hospitalizaciones;

    CargarPaises(Paises);
    CargarCiudades(Ciudades, Paises);
    CargarPacientes(Pacientes, Ciudades);
    CargarEspecialidades(Especialidades);
    CargarMedicos(Medicos, Especialidades);
    CargarCitas(Citas, Pacientes, Medicos, Especialidades);
    CargarHabitaciones(Habitaciones);
    CargarHospitalizaciones(Hospitalizaciones,Pacientes,Habitaciones,Medicos);


    int opcion = 0;

    while (opcion != 7)
    {
        cout << " " << endl;
        cout << "+------------------------------------------------------------------------------------------+" << endl;
        cout << "|                              SISTEMA HOSPITALARIO                                        |" << endl;
        cout << "+------------------------------------------------------------------------------------------+" << endl;
        cout << "|  1. INSERTAR DATOS                                                                       |" << endl;
        cout << "|  2. BUSCAR DATOS                                                                         |" << endl;
        cout << "|  3. ELIMINAR DATOS                                                                       |" << endl;
        cout << "|  4. MODIFICAR DATOS                                                                      |" << endl;
        cout << "|  5. MOSTRAR DATOS                                                                        |" << endl;
        cout << "|  6. REPORTES                                                                             |" << endl;
        cout << "|  7. SALIR                                                                                |" << endl;
        cout << "+------------------------------------------------------------------------------------------+" << endl;
        cout << "Ingrese una opcion:  ";
        cin >> opcion;

        switch (opcion)
        {
            //Menu Insertar
            case 1:
                {
                    int opcionInsertar = 0;
                    while (opcionInsertar != 9)
                    {
                        cout << " " << endl;

                        cout << "+------------------------------------------------------------------------------------------+" << endl;
                        cout << "|                                  INSERTAR DATOS                                          |" << endl;
                        cout << "+------------------------------------------------------------------------------------------+" << endl;
                        cout << "|  1. INSERTAR PAIS                                                                        |" << endl;
                        cout << "|  2. INSERTAR CIUDAD                                                                      |" << endl;
                        cout << "|  3. INSERTAR PACIENTE                                                                    |" << endl;
                        cout << "|  4. INSERTAR MEDICO                                                                      |" << endl;
                        cout << "|  5. INSERTAR ESPECIALIDAD                                                                |" << endl;
                        cout << "|  6. INSERTAR CITA                                                                        |" << endl;
                        cout << "|  7. INSERTAR HABITACION                                                                  |" << endl;
                        cout << "|  8. INSERTAR HOSPITALIZACION                                                             |" << endl;
                        cout << "|  9. REGRESAR                                                                             |" << endl;
                        cout << "+------------------------------------------------------------------------------------------+" << endl;

                        cout << "Ingrese una opcion: ";
                        cin >> opcionInsertar;

                        switch (opcionInsertar)
                        {
                            case 1:InsertarPais(Paises);break;
                            case 2:InsertarCiudad(Ciudades, Paises);break;
                            case 3:InsertarPaciente(Pacientes, Ciudades);break;
                            case 4:InsertarMedico(Medicos, Especialidades);break;
                            case 5:InsertarEspecialidad(Especialidades);break;
                            case 6:InsertarCita(Citas, Pacientes, Medicos, Especialidades);break;
                            case 7:InsertarHabitacion(Habitaciones);break;
                            case 8:InsertarHospitalizacion(Hospitalizaciones,Pacientes,Habitaciones,Medicos);break;
                            case 9:cout << "Regresando al menu principal..." << endl;break;
                            default:cout << "Opcion invalida." << endl;break;
                        }
                    }
                    break;
                }

            //Menu Buscar
            case 2:
                {
                    int opcionBuscar = 0;
                    while (opcionBuscar != 9)
                    {
                        cout << " " << endl;
                        cout << "+------------------------------------------------------------------------------------------+" << endl;
                        cout << "|                                   BUSCAR DATOS                                           |" << endl;
                        cout << "+------------------------------------------------------------------------------------------+" << endl;
                        cout << "|  1. BUSCAR PAIS                                                                          |" << endl;
                        cout << "|  2. BUSCAR CIUDAD                                                                        |" << endl;
                        cout << "|  3. BUSCAR PACIENTE                                                                      |" << endl;
                        cout << "|  4. BUSCAR MEDICO                                                                        |" << endl;
                        cout << "|  5. BUSCAR ESPECIALIDAD                                                                  |" << endl;
                        cout << "|  6. BUSCAR CITA                                                                          |" << endl;
                        cout << "|  7. BUSCAR HABITACION                                                                    |" << endl;
                        cout << "|  8. BUSCAR HOSPITALIZACION                                                               |" << endl;
                        cout << "|  9. REGRESAR                                                                             |" << endl;
                        cout << "+------------------------------------------------------------------------------------------+" << endl;
                        cout << "Ingrese una opcion: ";
                        cin >> opcionBuscar;
                        switch (opcionBuscar)
                        {
                            case 1: BuscarPais(Paises); break;
                            case 2: BuscarCiudad(Ciudades, Paises); break;
                            case 3: BuscarPaciente(Pacientes, Ciudades, Paises); break;
                            case 4: BuscarMedico(Medicos, Especialidades); break;
                            case 5: BuscarEspecialidad(Especialidades); break;
                            case 6: BuscarCita(Citas, Medicos, Pacientes); break;
                            case 7: BuscarHabitacion(Habitaciones); break;
                            case 8: BuscarHospitalizacion(Hospitalizaciones, Pacientes, Habitaciones); break;
                            case 9: cout << "Regresando al menu principal..." << endl; break;
                            default: cout << "Opcion invalida." << endl; break;
                        }
                    }
                    break;
                }

            //MENU ELIMINAR
            case 3:
                {
                    int opcionEliminar = 0;
                    while (opcionEliminar != 9)
                    {
                        cout << " " << endl;
                        cout << "+------------------------------------------------------------------------------------------+" << endl;
                        cout << "|                                  ELIMINAR DATOS                                          |" << endl;
                        cout << "+------------------------------------------------------------------------------------------+" << endl;
                        cout << "|  1. ELIMINAR PAIS                                                                        |" << endl;
                        cout << "|  2. ELIMINAR CIUDAD                                                                      |" << endl;
                        cout << "|  3. ELIMINAR PACIENTE                                                                    |" << endl;
                        cout << "|  4. ELIMINAR MEDICO                                                                      |" << endl;
                        cout << "|  5. ELIMINAR ESPECIALIDAD                                                                |" << endl;
                        cout << "|  6. ELIMINAR CITA                                                                        |" << endl;
                        cout << "|  7. ELIMINAR HABITACION                                                                  |" << endl;
                        cout << "|  8. ELIMINAR HOSPITALIZACION                                                             |" << endl;
                        cout << "|  9. REGRESAR                                                                             |" << endl;
                        cout << "+------------------------------------------------------------------------------------------+" << endl;
                        cout << "Ingrese una opcion: ";
                        cin >> opcionEliminar;

                        switch (opcionEliminar)
                        {
                            case 1: EliminarPais(Paises, Ciudades, Pacientes, Citas, Hospitalizaciones); break;
                            case 2: EliminarCiudad(Ciudades, Paises, Pacientes, Citas, Hospitalizaciones); break;
                            case 3: EliminarPaciente(Pacientes, Citas, Hospitalizaciones); break;
                            case 4: EliminarMedico(Medicos, Citas, Hospitalizaciones); break;
                            case 5: EliminarEspecialidad(Especialidades, Medicos, Citas, Hospitalizaciones); break;
                            case 6: EliminarCita(Citas); break;
                            case 7: EliminarHabitacion(Habitaciones, Hospitalizaciones); break;
                            case 8: EliminarHospitalizacion(Hospitalizaciones); break;
                            case 9: cout << "Regresando al menu principal..." << endl; break;
                            default: cout << "Opcion invalida." << endl; break;
                        }
                    }
                    break;
                }

            //MENU MODIFICAR
            case 4:
                {
                    int opcionModificar = 0;
                    while (opcionModificar != 9)
                    {
                        cout << " " << endl;
                        cout << "+------------------------------------------------------------------------------------------+" << endl;
                        cout << "|                                 MODIFICAR DATOS                                          |" << endl;
                        cout << "+------------------------------------------------------------------------------------------+" << endl;
                        cout << "|  1. MODIFICAR PAIS                                                                       |" << endl;
                        cout << "|  2. MODIFICAR CIUDAD                                                                     |" << endl;
                        cout << "|  3. MODIFICAR PACIENTE                                                                   |" << endl;
                        cout << "|  4. MODIFICAR MEDICO                                                                     |" << endl;
                        cout << "|  5. MODIFICAR ESPECIALIDAD                                                               |" << endl;
                        cout << "|  6. MODIFICAR CITA                                                                       |" << endl;
                        cout << "|  7. MODIFICAR HABITACION                                                                 |" << endl;
                        cout << "|  8. MODIFICAR HOSPITALIZACION                                                            |" << endl;
                        cout << "|  9. REGRESAR                                                                             |" << endl;
                        cout << "+------------------------------------------------------------------------------------------+" << endl;
                        cout << "Ingrese una opcion: ";
                        cin >> opcionModificar;

                        switch (opcionModificar)
                        {
                            case 1: ModificarPais(Paises); break;
                            case 2: ModificarCiudad(Ciudades, Paises); break;
                            case 3: ModificarPaciente(Pacientes, Ciudades); break;
                            case 4: ModificarMedico(Medicos, Especialidades); break;
                            case 5: ModificarEspecialidad(Especialidades); break;
                            case 6: ModificarCita(Citas, Pacientes, Medicos, Especialidades); break;
                            case 7: ModificarHabitacion(Habitaciones); break;
                            case 8: ModificarHospitalizacion(Hospitalizaciones, Pacientes, Habitaciones, Medicos); break;
                            case 9: cout << "Regresando al menu principal..." << endl; break;
                            default: cout << "Opcion invalida." << endl; break;
                        }
                    }
                    break;
                }
                

            //MENU MOSTRAR
            case 5:
                {
                    int opcionMostrar = 0;
                    while (opcionMostrar != 9)
                    {
                        cout << " " << endl;
                        cout << "+------------------------------------------------------------------------------------------+" << endl;
                        cout << "|                                  MOSTRAR DATOS                                           |" << endl;
                        cout << "+------------------------------------------------------------------------------------------+" << endl;
                        cout << "|  1. MOSTRAR PAISES                                                                       |" << endl;
                        cout << "|  2. MOSTRAR CIUDADES                                                                     |" << endl;
                        cout << "|  3. MOSTRAR PACIENTES                                                                    |" << endl;
                        cout << "|  4. MOSTRAR MEDICOS                                                                      |" << endl;
                        cout << "|  5. MOSTRAR ESPECIALIDADES                                                               |" << endl;
                        cout << "|  6. MOSTRAR CITAS                                                                        |" << endl;
                        cout << "|  7. MOSTRAR HABITACIONES                                                                 |" << endl;
                        cout << "|  8. MOSTRAR HOSPITALIZACIONES                                                            |" << endl;
                        cout << "|  9. REGRESAR                                                                             |" << endl;
                        cout << "+------------------------------------------------------------------------------------------+" << endl;
                        cout << "Ingrese una opcion: ";
                        cin >> opcionMostrar;

                        switch (opcionMostrar)
                        {
                            case 1: Paises.Mostrar(); break;
                            case 2: Ciudades.Mostrar(); break;
                            case 3: Pacientes.Mostrar(); break;
                            case 4: Medicos.Mostrar(); break;
                            case 5: Especialidades.Mostrar(); break;
                            case 6: Citas.Mostrar(); break;
                            case 7: Habitaciones.Mostrar(); break;
                            case 8: Hospitalizaciones.Mostrar(); break;
                            case 9: cout << "Regresando al menu principal..." << endl; break;
                            default: cout << "Opcion invalida." << endl; break;
                        }
                    }
                    break;
                }
                break;

            case 6:
            {
                int opcionReporte = 0;
                while (opcionReporte != 13)
                {
                    cout << " " << endl;
                    cout << "+------------------------------------------------------------------------------------------+" << endl;
                    cout << "|                                      REPORTES                                            |" << endl;
                    cout << "+------------------------------------------------------------------------------------------+" << endl;
                    cout << "|  1.  REPORTE PAISES                                                                      |" << endl;
                    cout << "|  2.  REPORTE CIUDADES                                                                    |" << endl;
                    cout << "|  3.  REPORTE ESPECIALIDADES                                                              |" << endl;
                    cout << "|  4.  REPORTE HABITACIONES DISPONIBLES                                                    |" << endl;
                    cout << "|  5.  REPORTE HABITACIONES OCUPADAS                                                       |" << endl;
                    cout << "|  6.  REPORTE HOSPITALIZACIONES                                                           |" << endl;
                    cout << "|  7.  REPORTE HOSPITALIZACIONES DE UN MEDICO                                              |" << endl;
                    cout << "|  8.  REPORTE MEDICOS                                                                     |" << endl;
                    cout << "|  9.  REPORTE MEDICOS DE UNA ESPECIALIDAD                                                 |" << endl;
                    cout << "|  10. REPORTE CITAS POR FECHA                                                             |" << endl;
                    cout << "|  11. REPORTE CITAS POR ESPECIALIDAD                                                      |" << endl;
                    cout << "|  12. REPORTE CITAS DE UN MEDICO                                                          |" << endl;
                    cout << "|  13. REGRESAR                                                                             |" << endl;
                    cout << "+------------------------------------------------------------------------------------------+" << endl;
                    cout << "Ingrese una opcion: ";
                    cin >> opcionReporte;
                    switch (opcionReporte)
                    {
                        case 1: ReportePaises(Paises); break;
                        case 2: ReporteCiudades(Ciudades, Paises); break;
                        case 3: ReporteEspecialidades(Especialidades); break;
                        case 4: ReporteHabitacionesDisponibles(Habitaciones); break;
                        case 5: ReporteHabitacionesOcupadas(Habitaciones); break;
                        case 6: ReporteHospitalizaciones(Hospitalizaciones); break;
                        case 7: ReporteHospitalizacionesPorMedico(Hospitalizaciones); break;
                        case 8: ReporteMedicos(Medicos); break;
                        case 9: ReporteMedicosPorEspecialidad(Medicos); break;
                        case 10: ReporteCitasPorFecha(Citas); break;
                        case 11: ReporteCitasPorEspecialidad(Citas); break;
                        case 12: ReporteCitasPorMedico(Citas); break;
                        case 13: cout << "Regresando al menu principal..." << endl; break;
                        default: cout << "Opcion invalida." << endl; break;
                    }
                }
                break;
            }
            
            case 7:
                cout << "Saliendo del programa..." << endl;
                break;

            default:
                cout << "Opcion invalida." << endl;
                break;
        }
    }
}
