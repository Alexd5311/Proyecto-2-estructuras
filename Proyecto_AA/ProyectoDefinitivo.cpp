#include <iostream>
#include <fstream>
#include "Comun_PDF.h"
#include "Paises.h"
#include "Ciudades.h"
#include "Pacientes.h"
#include "Especialidades.h"
#include "Medicos.h"
#include "Citas.h"
#include "Habitaciones.h"
#include "Hospitalizaciones.h"
#include "Tratamiento.h"
#include "Diagnostico.h"
#include "Operaciones.h"




int main()
{
    ArbolABBPaises Paises;
    ArbolABBCiudades Ciudades;
    ArbolBPacientes Pacientes;
    ArbolAAEspecialidades Especialidades;
    ArbolBMedicos Medicos;
    ArbolAVLCitas Citas;
    ArbolAVLHabitaciones Habitaciones;
    ArbolRNHospitalizaciones Hospitalizaciones;
	ArbolAADiagnostico Diagnosticos;
	ArbolRNTratamientos Tratamientos;
    

    CargarPaises(Paises);
    CargarCiudades(Ciudades, Paises);
    CargarPacientes(Pacientes, Ciudades);
    CargarEspecialidades(Especialidades);
    CargarMedicos(Medicos, Especialidades);
    CargarCitas(Citas, Pacientes, Medicos, Especialidades);
    CargarHabitaciones(Habitaciones);
    CargarHospitalizaciones(Hospitalizaciones,Pacientes,Habitaciones,Medicos);
	CargarDiagnosticos(Diagnosticos);
	CargarTratamientos(Tratamientos);

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
                    while (opcionInsertar != 11)
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
						cout << "|  9. INSERTAR DIAGNOSTICO                                                                 |" << endl;
                        cout << "|  10. INSERTAR TRATAMIENTO                                                                |" << endl;
                        cout << "|  11. REGRESAR                                                                             |" << endl;
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
                            case 9:InsertarDiagnostico(Diagnosticos);break;
                            case 10:InsertarTratamiento(Tratamientos);break;
                            case 11:cout << "Regresando al menu principal..." << endl;break;
                            default:cout << "Opcion invalida." << endl;break;
                        }
                    }
                    break;
                }

            //Menu Buscar
            case 2:
                {
                    int opcionBuscar = 0;
                    while (opcionBuscar != 11)
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
                        cout << "|  9. BUSCAR DIAGNOSTICO                                                                   |" << endl;
                        cout << "|  10. BUSCAR TRATAMIENTO                                                                  |" << endl;
                        cout << "|  11. REGRESAR                                                                             |" << endl;
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
                            case 9: BuscarDiagnostico(Diagnosticos); break;
							case 10: BuscarTratamiento(Tratamientos); break;
                            case 11: cout << "Regresando al menu principal..." << endl; break;
                            default: cout << "Opcion invalida." << endl; break;
                        }
                    }
                    break;
                }

            //MENU ELIMINAR
            case 3:
                {
                    int opcionEliminar = 0;
                    while (opcionEliminar != 11)
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
                        cout << "|  9. ELIMINAR DIAGNOSTICO                                                                 |" << endl;
                        cout << "|  10. ELIMINAR TRATAMIENTO                                                                |" << endl;
                        cout << "|  11. REGRESAR                                                                             |" << endl;
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
							case 9: EliminarDiagnostico(Diagnosticos); break;
                            case 10: EliminarTratamiento(Tratamientos); break;
                            case 11: cout << "Regresando al menu principal..." << endl; break;
                            default: cout << "Opcion invalida." << endl; break;
                        }
                    }
                    break;
                }

            //MENU MODIFICAR
            case 4:
                {
                    int opcionModificar = 0;
                    while (opcionModificar != 11)
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
                        cout << "|  9. MODIFICAR DIAGNOSTICO                                                                |" << endl;
                        cout << "|  10. MODIFICAR TRATAMIENTO                                                               |" << endl;
                        cout << "|  11. REGRESAR                                                                             |" << endl;
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
                            case 9: ModificarDiagnostico(Diagnosticos); break;
							case 10: ModificarTratamiento(Tratamientos); break;
                            case 11: cout << "Regresando al menu principal..." << endl; break;
                            default: cout << "Opcion invalida." << endl; break;
                        }
                    }
                    break;
                }
                

            //MENU MOSTRAR
            case 5:
                {
                    int opcionMostrar = 0;
                    while (opcionMostrar != 11)
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
						cout << "|  9. MOSTRAR DIAGNOSTICOS                                                                 |" << endl;
						cout << "|  10. MOSTRAR TRATAMIENTOS                                                                |" << endl;
                        cout << "|  11. REGRESAR                                                                            |" << endl;
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
                            case 8: Hospitalizaciones.MostrarHospitalizaciones(); break;
							case 9: Diagnosticos.Mostrar(); break;
							case 10: Tratamientos.Mostrar(); break;
                            case 11: cout << "Regresando al menu principal..." << endl; break;
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
                        case 1: ReportePaises(Paises); break;D
                        case 2: ReporteCiudades(Ciudades, Paises); break;
                        case 3: ReporteEspecialidades(Especialidades); break;
                        case 4: ReporteHabitacionesDisponibles(Habitaciones); break;
                        case 5: ReporteHabitacionesOcupadas(Habitaciones); break;
                        case 6: cout << "Reporte Hospitalizaciones" << endl; break;
                        case 7: cout << "Reporte Hospitalizaciones de un Medico" << endl; break;
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
