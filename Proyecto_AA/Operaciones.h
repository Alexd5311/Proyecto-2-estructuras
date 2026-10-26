#pragma once

void InsertarPais(ArbolABBPaises &Paises)
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
void InsertarCiudad(ArbolABBCiudades &Ciudades, ArbolABBPaises &Paises)
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
void InsertarPaciente(ArbolBPacientes &Pacientes, ArbolABBCiudades &Ciudades)
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
void InsertarEspecialidad(ArbolAAEspecialidades &Especialidades)
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
void InsertarMedico(ArbolBMedicos &Medicos, ArbolAAEspecialidades &Especialidades)
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

//Citas
void InsertarCita(ArbolAVLCitas &Citas, ArbolBPacientes &Pacientes, ArbolBMedicos &Medicos, ArbolAAEspecialidades &Especialidades)
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
void InsertarHabitacion(ArbolAVLHabitaciones &Habitaciones)
{
    int IdHabitacion;
    int Numero;
    int Piso;
    string Pisotexto;
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
    cin >> Pisotexto;

    if (Pisotexto == "Piso") {
        cin >> Piso;
    }
    else {
        Piso = stoi(Pisotexto);
    }

    cout << "Ingrese el tipo (1: Individual, 2: Compartida, 3: UCI): ";
    if (!(cin >> Tipo)) {
        cout << "Entrada de tipo invalida." << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        return;
    }

    cout << "Tipo ingresado: " << Tipo << endl;
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
void InsertarHospitalizacion(ListaHospitalizaciones &Hospitalizaciones, ArbolBPacientes &Pacientes, ArbolAVLHabitaciones &Habitaciones, ArbolBMedicos &Medicos)
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


void InsertarDiagnostico(ArbolAADiagnostico& Diagnosticos)
{
    string id;
    string nombre, descripcion;

    cout << "Ingrese el ID del diagnostico: ";
    cin >> id;
    cin.ignore(10000, '\n');

    if (Diagnosticos.Existe(id))
    {
        cout << "Ese ID ya existe." << endl;
        return;
    }

    cout << "Ingrese el nombre del diagnostico: ";
    getline(cin, nombre);

    cout << "Ingrese la descripcion: ";
    getline(cin, descripcion);

    if (Diagnosticos.Insertar(id, nombre, descripcion))
        cout << "Diagnostico insertado correctamente." << endl;
    else
        cout << "No se pudo insertar el diagnostico." << endl;
}


    //╔════════════════════════════════════════════════════════════════════════╗
    //║                                 BUSCAR                                 ║
    //╚════════════════════════════════════════════════════════════════════════╝

//Paises
void BuscarPais(ArbolABBPaises &Paises)
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
void BuscarCiudad(ArbolABBCiudades &Ciudades, ArbolABBPaises &Paises)
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
void BuscarPaciente(ArbolBPacientes &Pacientes, ArbolABBCiudades &Ciudades, ArbolABBPaises &Paises)
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
void BuscarEspecialidad(ArbolAAEspecialidades &Especialidades)
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
void BuscarMedico(ArbolBMedicos &Medicos, ArbolAAEspecialidades &Especialidades)
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
void BuscarCita(ArbolAVLCitas &Citas, ArbolBMedicos &Medicos, ArbolBPacientes &Pacientes)
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
void BuscarHabitacion(ArbolAVLHabitaciones &Habitaciones)
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
void BuscarHospitalizacion(ListaHospitalizaciones &Hospitalizaciones,ArbolBPacientes &Pacientes, ArbolAVLHabitaciones &Habitaciones)
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

void BuscarDiagnostico(ArbolAADiagnostico& Diagnosticos)
{
    string id;

    cout << "Ingrese el ID del diagnostico que desea buscar: ";
    cin >> id;

    if (!Diagnosticos.Buscar(id))
        cout << "No se encontro el diagnostico." << endl;
}

    //╔════════════════════════════════════════════════════════════════════════╗
    //║                                ELIMINAR                                ║
    //╚════════════════════════════════════════════════════════════════════════╝





void EliminarPais(ArbolABBPaises &Paises, ArbolABBCiudades &Ciudades, ArbolBPacientes &Pacientes,
    ArbolAVLCitas &Citas, ListaHospitalizaciones &Hospitalizaciones)
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

void EliminarPaciente(ArbolBPacientes &Pacientes, ArbolAVLCitas &Citas, ListaHospitalizaciones &Hospitalizaciones)
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


void EliminarCiudad(ArbolABBCiudades &Ciudades, ArbolABBPaises &Paises, ArbolBPacientes &Pacientes, ArbolAVLCitas &Citas, ListaHospitalizaciones &Hospitalizaciones)
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


void EliminarPaciente(ArbolBPacientes &Pacientes)
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



void EliminarMedico(ArbolBMedicos &Medicos, ArbolAVLCitas &Citas, ListaHospitalizaciones &Hospitalizaciones)
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

void EliminarEspecialidad(ArbolAAEspecialidades &Especialidades, ArbolBMedicos &Medicos, ArbolAVLCitas &Citas, ListaHospitalizaciones &Hospitalizaciones)
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

void EliminarCita(ArbolAVLCitas &Citas)
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

void EliminarHabitacion(ArbolAVLHabitaciones &Habitaciones, ListaHospitalizaciones &Hospitalizaciones)
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

void EliminarDiagnostico(ArbolAADiagnostico& Diagnosticos)
{
    string id;

    cout << "Ingrese el ID del diagnostico que desea eliminar: ";
    cin >> id;

    if (Diagnosticos.Eliminar(id))
        cout << "Diagnostico eliminado correctamente." << endl;
    else
        cout << "No se encontro el diagnostico." << endl;
}

void MostrarDiagnosticos(ArbolAADiagnostico& Diagnosticos)
{
    Diagnosticos.Mostrar();
}

void GenerarReporteDiagnosticos(ArbolAADiagnostico& Diagnosticos)
{
    ReporteDiagnosticos(Diagnosticos);
}

//╔═════════════════════════════════════════════════════════════════╗
//║                             Modificar                           ║
//╚═════════════════════════════════════════════════════════════════╝

void ModificarPais(ArbolABBPaises &Paises)
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


void ModificarCiudad(ArbolABBCiudades &Ciudades, ArbolABBPaises &Paises)
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

void ModificarPaciente(ArbolBPacientes &Pacientes, ArbolABBCiudades &Ciudades)
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

void ModificarEspecialidad(ArbolAAEspecialidades &Especialidades)
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

void ModificarMedico(ArbolBMedicos &Medicos,ArbolAAEspecialidades &Especialidades)
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

void ModificarCita(ArbolAVLCitas &Citas, ArbolBPacientes &Pacientes, ArbolBMedicos &Medicos, ArbolAAEspecialidades &Especialidades)
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


void ModificarHabitacion(ArbolAVLHabitaciones &Habitaciones)
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


void ModificarHospitalizacion(ListaHospitalizaciones& Hospitalizaciones, ArbolBPacientes& Pacientes, ArbolAVLHabitaciones& Habitaciones, ArbolBMedicos& Medicos)
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

    if (!Hospitalizaciones.Existe(IdHospitalizacion)) {
        cout << "La hospitalizacion no existe." << endl;
        return;
    }

    cout << "Ingrese el nuevo IdPaciente: ";
    cin >> IdPaciente;
    if (!Pacientes.Existe(IdPaciente)) {
        cout << "El IdPaciente no existe." << endl;
        return;
    }

    cout << "Ingrese el nuevo IdHabitacion: ";
    cin >> IdHabitacion;
    if (!Habitaciones.Existe(IdHabitacion)) {
        cout << "El IdHabitacion no existe." << endl;
        return;
    }

    cout << "Ingrese el nuevo IdMedico: ";
    cin >> IdMedico;
    if (!Medicos.Existe(IdMedico)) {
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
    if (Hospitalizaciones.Modificar(IdHospitalizacion, IdPaciente, IdHabitacion, IdMedico, FechaIngreso, FechaSalida, Motivo)) {
        cout << "Hospitalizacion modificada correctamente." << endl;
    }
    else {
        cout << "No se pudo modificar la hospitalizacion." << endl;
    }
}

void ModificarDiagnostico(ArbolAADiagnostico& Diagnosticos)
{
    string id;
    string nombre, descripcion;

    cout << "Ingrese el ID del diagnostico que desea modificar: ";
    cin >> id;
    cin.ignore(10000, '\n');

    if (!Diagnosticos.Existe(id))
    {
        cout << "No existe ese diagnostico." << endl;
        return;
    }

    cout << "Ingrese el nuevo nombre: ";
    getline(cin, nombre);

    cout << "Ingrese la nueva descripcion: ";
    getline(cin, descripcion);

    if (Diagnosticos.Modificar(id, nombre, descripcion))
        cout << "Diagnostico modificado correctamente." << endl;
    else
        cout << "No se pudo modificar el diagnostico." << endl;
}

