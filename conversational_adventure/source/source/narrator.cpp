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



/*** Includes ***/
// c++
#include <cstdio>
#include <iostream>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "narrator.h"
#include "settings.h"



/*** Constructor de la clase ***/
Narrator::Narrator() {

    // Objetos de codigo
    textbox = new TextBox();
    cast = new Cast();
    sr = new ScriptReader();

    // Propiedades
    st = next_st = st_idle;
    script.clear();
    show_textbox = false;
    script_line = 0;
    actor = NULL;
    eof = false;

}



/*** Destructor de la clase ***/
Narrator::~Narrator() {

    // Borra los datos adicionales
    script.clear();
    actor = NULL;

    // Objetos de codigo
    delete sr; sr = NULL;
    delete cast; cast = NULL;
    delete textbox; textbox = NULL;

}


/*** Carga de los archivos necesarios ***/
bool Narrator::Load() {

    // Recursos de los objetos de codigo
    if (!textbox->Load()) return false;                 // Caja de texto
    if (!cast->Load()) return false;                    // Actores

    // Archivos cargados con exito
    return true;

}



/*** Crea los elementos del narrador ***/
void Narrator::Create() {

    // Crea los elementos de la escena
    textbox->Create();                  // Caja de texto
    cast->Create();                     // Actores

}



/*** Logica del narrador ***/
uint8_t Narrator::Update() {

    // Reproduce un guion
    PlayScript();

    // Resultado de la ejecucion
    if (st == st_exit) {
        return FLAG_EXIT;
    } else {
        return FLAG_ZERO;
    }

}



/*** Render de los elementos graficos ***/
void Narrator::Render() {

    // Elementos de la escena
    if (show_textbox) textbox->Render();                  // Caja de texto

}



/*** Reproduce un guion ***/
void Narrator::PlayScript() {

    // Maquina de estados
    switch (st) {
        case st_idle:
            StIdle();
            break;
        case st_get_script:
            StGetScript();
            break;
        case st_parse_line:
            StParseLine();
            break;
        case st_play_line:
            StPlayLine();
            break;
    }

    // Cambio de estado
    st = next_st;

}



/*** En espera ***/
void Narrator::StIdle() {

    if (ngn->input->key_SPACE->down) {
        show_textbox = true;
        next_st = st_get_script;
    } else {
        show_textbox = false;
    }

}



/*** Carga unb script ***/
void Narrator::StGetScript() {

    // Prepara el dialogo
    script_line = 0;
    script.clear();
    actor = NULL;
    eof = false;

    // Lee el archivo
    if (sr->Read("data/scripts/test_script.txt", script)) {
        next_st = st_parse_line;
    } else {
        next_st = st_exit;
    }

}



/*** Obten una linea del script ***/
void Narrator::StParseLine() {

    bool repeat = false;
    std::string text_lines = "";
    Actor* _actor = NULL;
    bool actor_found = false;

    do {

        // Por defecto, no repitas
        repeat = false;

        // Busca si la linea, identifica a algun actor
        bool is_actor = false;
        for (uint32_t i = 0; i < cast->actors.size(); i ++) {
            if (cast->actors[i]->tag == script[script_line]) {
                _actor = cast->actors[i];
                is_actor = true;
                break;
            }
        }
        // Si es la primera linea y no es un actor, selecciona el primero de la lista por defecto
        if (script_line == 0) {
            if (!_actor) {
                _actor = actor = cast->actors[0];
                actor_found = true;
                std::cout << actor->tag << std::endl;
            }
        }

        // Si la linea es un actor
        if (is_actor) {
            // Si el actor encontrado es distinto del actual
            if (actor != _actor) {
                // Si no habias encontrado un actor anteriormente?
                if (!actor_found) {
                    // Actualiza el actor
                    actor = _actor;
                    actor_found = true;
                    // Marca para repetir
                    script_line ++;
                    repeat = true;
                }
            } else {
                // Marca para repetir
                script_line ++;
                repeat = true;
            }
        } else {
            // Si no es un actor, registra la linea
            text_lines += script[script_line];
            text_lines += "\n";
            script_line ++;
            repeat = true;
        }

        // Verifica si no es la ultima linea
        if (script_line >= script.size()) eof = true;

    } while (repeat && !eof);

    // Asigna el dialogo
    textbox->SetDialog(text_lines, TEXT_SPEED, actor);

    // Reproduce la linea
    next_st = st_play_line;

}



/*** Reproduce la linea actual ***/
void Narrator::StPlayLine() {

    // Reproduce la linea de dialogo
    if (textbox->Update() & FLAG_EXIT) {
        if (ngn->input->key_SPACE->down) {
            // Si no es el fin del archivo
            if (!eof) {
                // Siguiente linea de dialogo
                next_st = st_parse_line;
            } else {
                // Fin del archivo
                next_st = st_idle;
            }
        }
    }

}

