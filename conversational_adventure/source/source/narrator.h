/******************************************************************************

    Ejemplo de una aventura conversacional: Narrador de la historia

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



#ifndef NARRATOR_H_INCLUDED
#define NARRATOR_H_INCLUDED



/*** Includes ***/
// Includes de C++
#include <vector>
#include <string>
// Includes de la libreria
#include <ngn.h>
// Includes del proyecto
#include "textbox.h"
#include "cast.h"
#include "script_reader.h"



/*** Declaracion de la clase ***/
class Narrator {

    public:

        // Constructor de la clase
        Narrator();

        // Destructor de la clase
        ~Narrator();

        // Carga los recursos de la clase
        bool Load();

        // Crea la caja de texto
        void Create();

        // Actualiza el contenido de la caja de texto
        uint8_t Update();

        // Render de la caja de texto
        void Render();


    private:

        // Configuracion
        const uint8_t TEXT_SPEED = 2;       // Velocidad del texto

        // Objetos de codigo
        TextBox* textbox;           // Caja de texto
        Cast* cast;                 // Gestor de actores del reparto
        ScriptReader* sr;           // Lector de scripts

        // Control de la narracion (maquina de estados)
        uint8_t st, next_st;
        static const uint8_t st_idle = 0;
        static const uint8_t st_get_script = 1;
        static const uint8_t st_parse_line = 2;
        static const uint8_t st_play_line = 3;
        static const uint8_t st_exit = 255;

        void PlayScript();
        void StIdle();
        void StGetScript();
        void StParseLine();
        void StPlayLine();

        bool show_textbox;                      // Muestra la caja de texto?
        uint32_t script_line;                   // Linea actual del guion
        std::vector<std::string> script;        // Guarda las lineas del archivo de guion
        Actor* actor;                           // Actor actual
        bool eof;                               // Flag de fin del archivo


};



#endif // NARRATOR_H_INCLUDED
