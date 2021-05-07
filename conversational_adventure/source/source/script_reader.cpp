/******************************************************************************

    Ejemplo de una aventura conversacional: Lector de archivos de guion

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2021 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 1.5.0-beta o superior

    Requiere GCC 8.1.0 MinGW (SEH) - 64-bits
    http://downloads.sourceforge.net/project/mingw-w64/

    Requiere SDL2 (2.0.12) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SDL2_TTF (2.0.15) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.5.1) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20200306)
    (c) 2005 - 2020 by Lode Vandevenne
    http://lodev.org/lodepng/


    N'gine se distribuye bajo la licencia CREATIVE COMMONS
    "Attribution-NonCommercial 4.0 International"
    https://creativecommons.org/licenses/by-nc/4.0/

    You are free to:

        - Share
        copy and redistribute the material in any medium or format.
        - Adapt
        remix, transform, and build upon the material.

        The licensor cannot revoke these freedoms as long as you follow
        the license terms.

    Under the following terms:

        - Attribution
        You must give appropriate credit, provide a link to the license,
        and indicate if changes were made. You may do so in any reasonable
        manner, but not in any way that suggests the licensor endorses you
        or your use.

        - NonCommercial
        You may not use the material for commercial purposes.

        - No additional restrictions
        You may not apply legal terms or technological measures that
        legally restrict others from doing anything the license permits.

******************************************************************************/



/*** Includes ***/
// c++
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "script_reader.h"



/*** Constructor de la clase ***/
ScriptReader::ScriptReader() {

}



/*** Destructor de la clase ***/
ScriptReader::~ScriptReader() {

}



/*** Lee un archivo de guion ***/
bool ScriptReader::Read(std::string script_file, std::vector<std::string> &text_lines) {

    // Prepara los datos
    std::vector<uint8_t> buffer;
    buffer.clear();
    text_lines.clear();

    // Intenta cargar el archivo
    if (ngn->load->LoadFile(script_file, buffer) < 0) return false;

    // Si no hay contenido, sal
    if (buffer.size() == 0) {
        buffer.clear();
        return true;
    }

    // datos para el procesado
    std::string line = "";

    // Procesa el archivo
    for (uint32_t i = 0; i < buffer.size(); i ++) {
        // Si el caracter es final de linea
        if (buffer[i] == '\n') {
            // Almacena la linea actual
            text_lines.push_back(line);
            // Nueva linea
            line = "";
        } else if ((buffer[i] >= 0x20) && (buffer[i] != 0x7F) && (buffer[i] != 0xFF)) {
            // Si el caracter esta dentro del rango, almacenalo
            line += buffer[i];
        }
    }
    // Verifica si hay datos pendientes (ultima linea sin \n)
    if (line.size() > 0) text_lines.push_back(line);

    // Debug: Test de contenido
    //for (uint32_t i = 0; i < text_lines.size(); i ++) std::cout << text_lines[i] << std::endl;

    // Elimina el buffer de datos
    buffer.clear();

    // Carga correcta
    return true;

}
