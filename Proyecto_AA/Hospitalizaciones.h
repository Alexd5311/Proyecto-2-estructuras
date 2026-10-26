#pragma once

class NodoHospitalizacion
{
public:

    int IdHospitalizacion;
    int IdPaciente;
    int IdHabitacion;
    int IdMedico;
    string FechaIngreso;
    string FechaSalida;
    string Motivo;

	bool rojo;

	NodoHospitalizacion* Padre;
    NodoHospitalizacion* Hijoizq;
	NodoHospitalizacion* Hijoder;
    NodoHospitalizacion* Tio;
    NodoHospitalizacion* Abuelo;
    NodoHospitalizacion(int NuevoIdHospitalizacion, int NuevoIdPaciente,int NuevoIdHabitacion, int NuevoIdMedico, string NuevoFechaIngreso,string NuevoFechaSalida, string NuevoMotivo)
    {
		IdHospitalizacion = NuevoIdHospitalizacion;
		IdPaciente = NuevoIdPaciente;
		IdHabitacion = NuevoIdHabitacion;
		IdMedico = NuevoIdMedico;
		FechaIngreso = NuevoFechaIngreso;
		FechaSalida = NuevoFechaSalida;
		Motivo = NuevoMotivo;

		rojo = true;
		Padre = NULL;
		Hijoizq = NULL;
		Hijoder = NULL;
		Abuelo = NULL;
		Tio = NULL;
	}


private:

    NodoHospitalizacion *siguiente;
    NodoHospitalizacion *anterior;

    friend class ArbolRNHospitalizaciones;
    friend void ReporteHospitalizacionesPorMedico(ArbolRNHospitalizaciones &Hospitalizaciones);
};

typedef NodoHospitalizacion *pnodoHospitalizacion;


//╔═════════════════════════════════════════════════════════════════════════════╗
//║                          ARBOL ROJO NEGRO HOSPITALIZACIONES                      ║
//╚═════════════════════════════════════════════════════════════════════════════╝

class ArbolRNHospitalizaciones
{

private:
    NodoHospitalizacion* primero;

    NodoHospitalizacion* BuscarMinimo(NodoHospitalizacion* nodo)
    {
        if (nodo == NULL)
            return NULL;
        while (nodo->Hijoizq != NULL)
            nodo = nodo->Hijoizq;
        return nodo;
    }

    void TransformarArbol(NodoHospitalizacion* nodoOriginal, NodoHospitalizacion* NodoReemplazo)
    {
        if (nodoOriginal->Padre == NULL)
        {
            primero = NodoReemplazo;
        }
        else if (nodoOriginal == nodoOriginal->Padre->Hijoizq)
        {
            nodoOriginal->Padre->Hijoizq = NodoReemplazo;
        }
        else
        {
            nodoOriginal->Padre->Hijoder = NodoReemplazo;
        }
        if (NodoReemplazo != NULL)
        {
            NodoReemplazo->Padre = nodoOriginal->Padre;
        }
    }


    void ActualizarArbol(NodoHospitalizacion* nodo)
    {
        if (nodo == NULL)
            return;
        if (nodo->Hijoizq != NULL)
            nodo->Hijoizq->Padre = nodo;
        if (nodo->Hijoder != NULL)
            nodo->Hijoder->Padre = nodo;
        nodo->Abuelo = NULL;
        nodo->Tio = NULL;
        if (nodo->Padre != NULL)
        {
            nodo->Abuelo = nodo->Padre->Padre;
            if (nodo->Abuelo != NULL)
            {
                if (nodo->Padre == nodo->Abuelo->Hijoizq)
                    nodo->Tio = nodo->Abuelo->Hijoder;
                else
                    nodo->Tio = nodo->Abuelo->Hijoizq;
            }
        }
        ActualizarArbol(nodo->Hijoizq);
        ActualizarArbol(nodo->Hijoder);
    }

    void BalancearEliminacion(
        NodoHospitalizacion* nodo,
        NodoHospitalizacion* padre)
    {
        while (nodo != primero && ObtenerColor(nodo) == false)
        {
            if (padre == NULL)
                break;

            if (nodo == padre->Hijoizq)
            {
                NodoHospitalizacion* hermano = padre->Hijoder;

                if (ObtenerColor(hermano))
                {
                    hermano->rojo = false;
                    padre->rojo = true;
                    RotarIzquierda(padre);
                    hermano = padre->Hijoder;
                }

                if (hermano == NULL ||
                    (!ObtenerColor(hermano->Hijoizq) &&
                        !ObtenerColor(hermano->Hijoder)))
                {
                    if (hermano != NULL)
                        hermano->rojo = true;
                    nodo = padre;
                    padre = nodo->Padre;
                }
                else
                {
                    if (!ObtenerColor(hermano->Hijoder))
                    {
                        if (hermano->Hijoizq != NULL)
                            hermano->Hijoizq->rojo = false;

                        hermano->rojo = true;
                        RotarDerecha(hermano);
                        hermano = padre->Hijoder;
                    }

                    hermano->rojo = padre->rojo;
                    padre->rojo = false;

                    if (hermano->Hijoder != NULL)
                        hermano->Hijoder->rojo = false;

                    RotarIzquierda(padre);
                    nodo = primero;
                    padre = NULL;
                }
            }
            else
            {
                NodoHospitalizacion* hermano = padre->Hijoizq;

                if (ObtenerColor(hermano))
                {
                    hermano->rojo = false;
                    padre->rojo = true;
                    RotarDerecha(padre);
                    hermano = padre->Hijoizq;
                }

                if (hermano == NULL ||
                    (!ObtenerColor(hermano->Hijoder) &&
                        !ObtenerColor(hermano->Hijoizq)))
                {
                    if (hermano != NULL)
                        hermano->rojo = true;
                    nodo = padre;
                    padre = nodo->Padre;
                }
                else
                {
                    if (!ObtenerColor(hermano->Hijoizq))
                    {
                        if (hermano->Hijoder != NULL)
                            hermano->Hijoder->rojo = false;
                        hermano->rojo = true;
                        RotarIzquierda(hermano);
                        hermano = padre->Hijoizq;
                    }

                    hermano->rojo = padre->rojo;
                    padre->rojo = false;

                    if (hermano->Hijoizq != NULL)
                        hermano->Hijoizq->rojo = false;

                    RotarDerecha(padre);
                    nodo = primero;
                    padre = NULL;
                }
            }
        }

        if (nodo != NULL)
            nodo->rojo = false;

        if (primero != NULL)
            primero->rojo = false;

        ActualizarArbol(primero);
    }

    bool EliminarNodo(NodoHospitalizacion* nodo)
    {
        if (nodo == NULL)
            return false;

		NodoHospitalizacion* objetivo = nodo;
        NodoHospitalizacion* reemplazo = objetivo;
		NodoHospitalizacion* hijo = NULL;
		NodoHospitalizacion* padreHijo = NULL;

		bool colorOriginal = reemplazo->rojo;
        if (objetivo->Hijoizq == NULL)
        {
            hijo = objetivo->Hijoder;
            TransformarArbol(objetivo, objetivo->Hijoder);
            padreHijo = objetivo->Padre;
        }
        else if (objetivo->Hijoder == NULL)
        {
            hijo = objetivo->Hijoizq;
            TransformarArbol(objetivo, objetivo->Hijoizq);
            padreHijo = objetivo->Padre;
        }
        else
        {
            reemplazo = BuscarMinimo(objetivo->Hijoder);
            colorOriginal = reemplazo->rojo;
            hijo = reemplazo->Hijoder;
            if (reemplazo->Padre == objetivo)
            {
                if (hijo != NULL)
                    hijo->Padre = reemplazo;
                padreHijo = reemplazo;
            }
            else
            {
                TransformarArbol(reemplazo, reemplazo->Hijoder);
                reemplazo->Hijoder = objetivo->Hijoder;
                if (reemplazo->Hijoder != NULL)
                    reemplazo->Hijoder->Padre = reemplazo;
                padreHijo = reemplazo->Padre;
            }
            TransformarArbol(objetivo, reemplazo);
            reemplazo->Hijoizq = objetivo->Hijoizq;
            if (reemplazo->Hijoizq != NULL)
                reemplazo->Hijoizq->Padre = reemplazo;
			reemplazo->rojo = objetivo->rojo;

            delete objetivo;
            if (colorOriginal == false)
            {
                BalancearEliminacion(hijo, padreHijo);
            }
            if (primero != NULL)
            {
                primero->rojo = false;
            }
			ActualizarArbol(reemplazo);
			return true;
        }

    }

    void BuscarPorPaciente(NodoHospitalizacion* nodo, int IdPaciente, vector<NodoHospitalizacion*>& resultados) {
        if (nodo == NULL) {
            return;
        }
        if (nodo->IdPaciente == IdPaciente) {
            resultados.push_back(nodo);
        }
        BuscarPorPaciente(nodo->Hijoizq, IdPaciente, resultados);
        BuscarPorPaciente(nodo->Hijoder, IdPaciente, resultados);
	}


    void RotarDerecha(NodoHospitalizacion* nodo) {
        NodoHospitalizacion* nuevoPadre = nodo->Hijoizq;
        nodo->Hijoizq = nuevoPadre->Hijoder;
        if (nuevoPadre->Hijoder != NULL) {
            nuevoPadre->Hijoder->Padre = nodo;
        }
        nuevoPadre->Padre = nodo->Padre;
        if (nodo->Padre == NULL) {
            primero = nuevoPadre;
        }
        else if (nodo == nodo->Padre->Hijoizq) {
            nodo->Padre->Hijoizq = nuevoPadre;
        }
        else {
            nodo->Padre->Hijoder = nuevoPadre;
        }
        nuevoPadre->Hijoder = nodo;
        nodo->Padre = nuevoPadre;
        ActualizarArbol(primero);
    }

    void RotarIzquierda(NodoHospitalizacion* nodo) {
        NodoHospitalizacion* nuevoPadre = nodo->Hijoder;
        nodo->Hijoder = nuevoPadre->Hijoizq;
        if (nuevoPadre->Hijoizq != NULL) {
            nuevoPadre->Hijoizq->Padre = nodo;
        }
        nuevoPadre->Padre = nodo->Padre;
        if (nodo->Padre == NULL) {
            primero = nuevoPadre;
        }
        else if (nodo == nodo->Padre->Hijoizq) {
            nodo->Padre->Hijoizq = nuevoPadre;
        }
        else {
            nodo->Padre->Hijoder = nuevoPadre;
        }
        nuevoPadre->Hijoizq = nodo;
        nodo->Padre = nuevoPadre;
        ActualizarArbol(primero);
    }

    void Balancear(NodoHospitalizacion* nodo) {
        if (nodo->Padre == NULL) {
            nodo->rojo = false;
            return;
        }
        if (!nodo->Padre->rojo) {
            return;
        }
        if (nodo->Tio != NULL && nodo->Tio->rojo) {
            nodo->Padre->rojo = false;
            nodo->Tio->rojo = false;
            nodo->Abuelo->rojo = true;
            Balancear(nodo->Abuelo);
        }
        else {
            if (nodo == nodo->Padre->Hijoder && nodo->Padre == nodo->Abuelo->Hijoizq) {
                RotarIzquierda(nodo->Padre);
                nodo = nodo->Hijoizq;
            }
            else if (nodo == nodo->Padre->Hijoizq && nodo->Padre == nodo->Abuelo->Hijoder) {
                RotarDerecha(nodo->Padre);
                nodo = nodo->Hijoder;
            }
            nodo->Padre->rojo = false;
            nodo->Abuelo->rojo = true;
            if (nodo == nodo->Padre->Hijoizq) {
                RotarDerecha(nodo->Abuelo);
            }
            else {
                RotarIzquierda(nodo->Abuelo);
            }
        }
        primero->rojo = false;
        ActualizarArbol(primero);
    }

    void Mostrar(NodoHospitalizacion* nodo)
    {
        if (nodo == NULL) {
            return;
        }
        Mostrar(nodo->Hijoizq);
        cout << "IdHospitalizacion: " << nodo->IdHospitalizacion << ", IdPaciente: " << nodo->IdPaciente
            << ", IdHabitacion: " << nodo->IdHabitacion << ", IdMedico: " << nodo->IdMedico
            << ", FechaIngreso: " << nodo->FechaIngreso << ", FechaSalida: " << nodo->FechaSalida
            << ", Motivo: " << nodo->Motivo << endl;
        cout << "Color: " << (nodo->rojo ? "Rojo" : "Negro") << endl;

        Mostrar(nodo->Hijoder);
    }

    bool ObtenerColor(NodoHospitalizacion* nodo)
    {
        if (nodo == NULL) {
            return false;
        }
        return nodo->rojo;
    }

    void BuscarPorHabitacion(NodoHospitalizacion* nodo, int IdHabitacion, int identificadores[], int& cantidad)
    {
        if (nodo == NULL)
            return;

        BuscarPorHabitacion(
            nodo->Hijoizq, IdHabitacion, identificadores, cantidad);

        if (nodo->IdHabitacion == IdHabitacion)
        {
            identificadores[cantidad] = nodo->IdHospitalizacion;
            cantidad++;
        }

        BuscarPorHabitacion(
            nodo->Hijoder, IdHabitacion, identificadores, cantidad);
    }

    int ContarNodos(NodoHospitalizacion* nodo)
    {
        if (nodo == NULL) {
            return 0;
        }
        return 1 + ContarNodos(nodo->Hijoizq) + ContarNodos(nodo->Hijoder);
    }

    void BuscarPorMedico(NodoHospitalizacion* nodo, int IdMedico, int identificadores[], int& cantidad)
    {
        if (nodo == NULL)
            return;

        BuscarPorMedico(nodo->Hijoizq, IdMedico, identificadores, cantidad);

        if (nodo->IdMedico == IdMedico)
        {
            identificadores[cantidad] = nodo->IdHospitalizacion;
            cantidad++;
        }

        BuscarPorMedico(nodo->Hijoder, IdMedico, identificadores, cantidad);
    }
    
    void BuscarPorPaciente(NodoHospitalizacion* nodo, int IdPaciente, int identificadores[], int& cantidad)
    {
        if (nodo == NULL)
            return;
        BuscarPorPaciente(nodo->Hijoizq, IdPaciente, identificadores, cantidad);
        if (nodo->IdPaciente == IdPaciente)
        {
            identificadores[cantidad] = nodo->IdHospitalizacion;
            cantidad++;
        }
        BuscarPorPaciente(nodo->Hijoder, IdPaciente, identificadores, cantidad);
	}
public:
    
    ArbolRNHospitalizaciones() {
		primero = NULL;
    }

    bool ListaVacia() {
        return primero == NULL;
    }


    NodoHospitalizacion* BuscarHospitalizacion(int Id) {
		NodoHospitalizacion* aux = primero;
        while (aux != NULL) {
            if (aux->IdHospitalizacion == Id) {
                return aux;
            }
            else if (Id < aux->IdHospitalizacion) {
                aux = aux->Hijoizq;
            }
            else {
                aux = aux->Hijoder;
			}
        }
		return NULL;
	}

    bool InsertarHospitalizacion(int IdHospitalizacion,int IdPaciente, int IdHabitacion, int IdMedico,string FechaIngreso, string FechaSalida, string Motivo)
    {
        if (Existe(IdHospitalizacion))
        {
            cout << "La hospitalizacion con Id "
                << IdHospitalizacion
                << " ya existe." << endl;
            return false;
        }

        if (primero == NULL)
        {
            primero = new NodoHospitalizacion(
                IdHospitalizacion, IdPaciente, IdHabitacion,
                IdMedico, FechaIngreso, FechaSalida, Motivo);
            primero->rojo = false;
        }
        else
        {
            NodoHospitalizacion* nuevoNodo = new NodoHospitalizacion(
                IdHospitalizacion, IdPaciente, IdHabitacion,
                IdMedico, FechaIngreso, FechaSalida, Motivo);
            NodoHospitalizacion* aux = primero;
            NodoHospitalizacion* padre = NULL;

            while (aux != NULL)
            {
                padre = aux;

                if (IdHospitalizacion < aux->IdHospitalizacion)
                    aux = aux->Hijoizq;
                else
                    aux = aux->Hijoder;
            }
            nuevoNodo->Padre = padre;
            if (IdHospitalizacion < padre->IdHospitalizacion)
                padre->Hijoizq = nuevoNodo;
            else
                padre->Hijoder = nuevoNodo;
            Balancear(nuevoNodo);
        }

        return true;
    }

    bool Existe(int IdHospitalizacion)
    {
        return BuscarHospitalizacion(IdHospitalizacion) != NULL;
    }

    void MostrarHospitalizaciones()
    {
        if (ListaVacia()) {
            cout << "No hay hospitalizaciones registradas." << endl;
            return;
        }
        Mostrar(primero);
	}
    
	NodoHospitalizacion* Buscar(int IdHospitalizacion) {
        return BuscarHospitalizacion(IdHospitalizacion);
    }
    bool Modificar(int IdHospitalizacion, int IdPaciente, int IdHabitacion, int IdMedico, string FechaIngreso, string FechaSalida, string Motivo) {
        NodoHospitalizacion* nodo = Buscar(IdHospitalizacion);
        if (nodo != NULL) {
            nodo->IdPaciente = IdPaciente;
            nodo->IdHabitacion = IdHabitacion;
            nodo->IdMedico = IdMedico;
            nodo->FechaIngreso = FechaIngreso;
            nodo->FechaSalida = FechaSalida;
            nodo->Motivo = Motivo;
            return true;
        }
        return false;
	}

    bool Eliminar(int IdHospitalizacion) {
        NodoHospitalizacion* nodo = Buscar(IdHospitalizacion);
        if (nodo != NULL) {
            return EliminarNodo(nodo);
        }
        return false;
	}


    int EliminarPorPaciente(int IdPaciente)
    {
        int cantidadMaxima = ContarNodos(primero);
        if (cantidadMaxima == 0)
            return 0;
        int* identificadores = new int[cantidadMaxima];
        int cantidad = 0;

        BuscarPorPaciente(primero, IdPaciente, identificadores, cantidad);

        int eliminados = 0;

        for (int i = 0; i < cantidad; i++)
        {
            if (Eliminar(identificadores[i]))
                eliminados++;
        }

        delete[] identificadores;
        return eliminados;
    }

    int EliminarPorHabitacion(int IdHabitacion) {
        int identificadores[100];
        int cantidad = 0;
        BuscarPorHabitacion(primero, IdHabitacion, identificadores, cantidad);
        int eliminados = 0;
        for (int i = 0; i < cantidad; i++) {
            if (EliminarNodo(BuscarHospitalizacion(identificadores[i]))) {
                eliminados++;
            }
        }
		delete[] identificadores;
        return eliminados;
	}

    int EliminarPorMedico(int IdMedico)
    {
        int cantidadMaxima = ContarNodos(primero);
        if (cantidadMaxima == 0)
            return 0;
        int* identificadores = new int[cantidadMaxima];
        int cantidad = 0;

        BuscarPorMedico(primero, IdMedico, identificadores, cantidad);
        int eliminados = 0;
        for (int i = 0; i < cantidad; i++)
        {
            if (Eliminar(identificadores[i]))
                eliminados++;
        }

        delete[] identificadores;
        return eliminados;
    }

};


//╔═════════════════════════════════════════════════════════════════════════════╗
//║                    CARGAR HOSPITALIZACIONES                                ║
//╚═════════════════════════════════════════════════════════════════════════════╝

void CargarHospitalizaciones(ArbolRNHospitalizaciones &Hospitalizaciones, ArbolBPacientes &Pacientes, 
    ArbolAVLHabitaciones &Habitaciones, ArbolBMedicos &Medicos)
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
        if (!Pacientes.Existe(IdPaciente)) {
            continue;
        }

        if (!Habitaciones.Existe(IdHabitacion)) {
            continue;
        }

        if (!Medicos.Existe(IdMedico)) {
            continue;
        }
        Hospitalizaciones.InsertarHospitalizacion(IdHospitalizacion, IdPaciente, IdHabitacion, IdMedico, FechaIngreso, FechaSalida, Motivo);
    }

    archivo.close();
    cout << "Hospitalizaciones cargadas correctamente." << endl;
}



