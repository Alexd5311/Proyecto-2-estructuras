#pragma once

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
