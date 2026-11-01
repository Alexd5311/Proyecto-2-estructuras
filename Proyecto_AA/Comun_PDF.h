#pragma once

// Estudiantes: Aaron Ortiz Blanco, Alejandro Sandoval

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <hpdf.h>

using namespace std;

static void HPDF_STDCALL ManejadorErrorPDF(
    HPDF_STATUS error_no,
    HPDF_STATUS detail_no,
    void* user_data)
{
    cout << "Error de libharu: codigo "
        << (unsigned int)error_no
        << ", detalle " << (int)detail_no << endl;
}

string Rellenar(string texto, int ancho)
{
    if ((int)texto.length() > ancho - 1)
        texto = texto.substr(0, ancho - 1);

    while ((int)texto.length() < ancho)
        texto += " ";

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

        // Obtener fecha y hora de generacion
        time_t ahora = time(NULL);
        tm tiempo = {};

        // Funcion segura para Visual Studio
        if (localtime_s(&tiempo, &ahora) == 0)
        {
            char fecha[20];
            char hora[20];

            strftime(fecha, sizeof(fecha), "%d/%m/%Y", &tiempo);
            strftime(hora, sizeof(hora), "%I:%M:%S %p", &tiempo);

            fechaHora = "Fecha: " + string(fecha) +
                "    Hora: " + string(hora);
        }
        else
        {
            fechaHora = "Fecha y hora no disponibles";
        }

        doc = HPDF_New(ManejadorErrorPDF, NULL);

        if (doc != NULL)
        {
            fuente = HPDF_GetFont(doc, "Courier", "WinAnsiEncoding");
            fuenteNegrita = HPDF_GetFont(doc, "Courier-Bold", "WinAnsiEncoding");
            NuevaPagina();
        }
    }

    ~GeneradorPDF()
    {
        if (doc != NULL)
            HPDF_Free(doc);
    }

    bool Valido()
    {
        return doc != NULL;
    }

    void Linea(string texto)
    {
        if (doc == NULL || pagina == NULL)
            return;

        if (y < 60)
            NuevaPagina();

        HPDF_Page_BeginText(pagina);
        HPDF_Page_SetFontAndSize(pagina, fuente, 10);
        HPDF_Page_SetRGBFill(pagina, 0.0f, 0.0f, 0.0f);
        HPDF_Page_TextOut(pagina, 50, y, texto.c_str());
        HPDF_Page_EndText(pagina);

        y -= 14;
    }

    bool Guardar()
    {
        if (doc == NULL)
            return false;

        return HPDF_SaveToFile(doc, archivo.c_str()) == HPDF_OK;
    }

private:

    void NuevaPagina()
    {
        if (doc == NULL)
            return;

        pagina = HPDF_AddPage(doc);

        if (pagina == NULL)
            return;

        HPDF_Page_SetSize(pagina, HPDF_PAGE_SIZE_A4, HPDF_PAGE_LANDSCAPE);

        numeroPagina++;

        float alto = HPDF_Page_GetHeight(pagina);
        float ancho = HPDF_Page_GetWidth(pagina);

        y = alto - 40;

        // ENCABEZADO GENERAL
        HPDF_Page_BeginText(pagina);
        HPDF_Page_SetFontAndSize(pagina, fuenteNegrita, 16);
        HPDF_Page_SetRGBFill(pagina, 0.10f, 0.25f, 0.45f);

        string encabezado = "GESTION HOSPITALARIA";
        float anchoTexto = HPDF_Page_TextWidth(pagina, encabezado.c_str());

        HPDF_Page_TextOut(pagina, (ancho - anchoTexto) / 2, y, encabezado.c_str());
        HPDF_Page_EndText(pagina);

        y -= 28;

        // TITULO DEL REPORTE
        HPDF_Page_BeginText(pagina);
        HPDF_Page_SetFontAndSize(pagina, fuenteNegrita, 12);
        HPDF_Page_SetRGBFill(pagina, 0.0f, 0.0f, 0.0f);

        anchoTexto = HPDF_Page_TextWidth(pagina, titulo.c_str());
        HPDF_Page_TextOut(pagina, (ancho - anchoTexto) / 2, y, titulo.c_str());
        HPDF_Page_EndText(pagina);

        y -= 22;

        // FECHA Y HORA
        HPDF_Page_BeginText(pagina);
        HPDF_Page_SetFontAndSize(pagina, fuente, 9);
        HPDF_Page_SetRGBFill(pagina, 0.30f, 0.30f, 0.30f);

        anchoTexto = HPDF_Page_TextWidth(pagina, fechaHora.c_str());
        HPDF_Page_TextOut(pagina, (ancho - anchoTexto) / 2, y, fechaHora.c_str());
        HPDF_Page_EndText(pagina);

        y -= 22;

        // ENCABEZADOS DE LAS COLUMNAS
        HPDF_Page_BeginText(pagina);
        HPDF_Page_SetFontAndSize(pagina, fuenteNegrita, 10);
        HPDF_Page_SetRGBFill(pagina, 0.10f, 0.25f, 0.45f);

        HPDF_Page_TextOut(pagina, 50, y, columnas.c_str());
        HPDF_Page_EndText(pagina);

        y -= 7;

        // LINEA SEPARADORA
        HPDF_Page_SetRGBStroke(pagina, 0.10f, 0.25f, 0.45f);
        HPDF_Page_SetLineWidth(pagina, 1.2f);
        HPDF_Page_MoveTo(pagina, 50, y);
        HPDF_Page_LineTo(pagina, ancho - 50, y);
        HPDF_Page_Stroke(pagina);

        y -= 18;

        // PIE DE PAGINA
        HPDF_Page_SetRGBStroke(pagina, 0.70f, 0.70f, 0.70f);
        HPDF_Page_SetLineWidth(pagina, 0.5f);
        HPDF_Page_MoveTo(pagina, 50, 43);
        HPDF_Page_LineTo(pagina, ancho - 50, 43);
        HPDF_Page_Stroke(pagina);

        string pie = "Gestion Hospitalaria";
        string numero = "Pagina " + to_string(numeroPagina);

        HPDF_Page_BeginText(pagina);
        HPDF_Page_SetFontAndSize(pagina, fuente, 9);
        HPDF_Page_SetRGBFill(pagina, 0.35f, 0.35f, 0.35f);

        HPDF_Page_TextOut(pagina, 50, 27, pie.c_str());

        anchoTexto = HPDF_Page_TextWidth(pagina, numero.c_str());
        HPDF_Page_TextOut(pagina, ancho - 50 - anchoTexto, 27, numero.c_str());

        HPDF_Page_EndText(pagina);
    }

    // Atributos inicializados para evitar C26495
    HPDF_Doc doc = NULL;
    HPDF_Page pagina = NULL;
    HPDF_Font fuente = NULL;
    HPDF_Font fuenteNegrita = NULL;

    string archivo;
    string titulo;
    string columnas;
    string fechaHora;

    float y = 0;
    int numeroPagina = 0;
};
