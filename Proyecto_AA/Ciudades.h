#pragma once

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
    ifstream archivo("Ciudades.csv");
    if (!archivo){
        cout << "No se pudo abrir el archivo Ciudades.csv" << endl;
        return;
    }

    string linea;
    getline(archivo, linea); // Saltar encabezado del CSV
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

