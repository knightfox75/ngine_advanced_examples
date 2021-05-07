/******************************************************************************

    Ejemplo de una aventura conversacional: Caja de texto

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
#include <cmath>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "textbox.h"
#include "settings.h"



/*** Constructor de la clase ***/
TextBox::TextBox() {

    // Punteros a objetos
    box_img = NULL;
    box_font = NULL;
    box = NULL;

    cursor_img = NULL;
    cursor = NULL;

}



/*** Destructor de la clase ***/
TextBox::~TextBox() {

    // Punteros a objetos
    delete box; box = NULL;
    delete box_font; box_font = NULL;
    delete box_img; box_img = NULL;

    delete cursor; cursor = NULL;
    delete cursor_img; cursor_img = NULL;

}



/*** Carga de los archivos necesarios ***/
bool TextBox::Load() {

    // Carga el fondo de la caja de texto
    box_img = ngn->load->Texture("data/textures/textbox.png");
    if (!box_img) return false;

    // Carga la tipografia
    box_font = ngn->load->TrueTypeFont("data/fonts/november.ttf", font_size, false);
    if (!box_font) return false;

    // Cursor de espera
    cursor_img = ngn->load->Sprite("data/sprites/cursor.spr");
    if (!cursor_img) return false;

    // Archivos cargados con exito
    return true;

}



/*** Crea los elementos de la caja de texto ***/
void TextBox::Create() {

    // Crea la capa de texto
    box = new NGN_TextLayer(box_font, box_img, text_box_position.x, text_box_position.y);

    // Cursor de espera
    cursor = new NGN_Sprite(cursor_img, cursor_position.x, cursor_position.y);

    // Valores iniciales de la caja de texto
    ResetDialog();

}



/*** Logica del programa ***/
uint8_t TextBox::Update() {

    // Reproduce el dialogo cargado, si hay alguno presente
    PlayDialog();

    // Animacion del cursor
    CursorUpdate();

    // Resultado de la ejecucion
    if (st == st_exit) {
        return FLAG_EXIT;
    } else {
        return FLAG_ZERO;
    }

}



/*** Render de los elementos graficos ***/
void TextBox::Render() {

    // Capa de texto
    ngn->render->TextLayer(box);

    // Retrato del personaje (si hay alguno)
    if (dialog.actor) {
        if (dialog.actor->sprite) ngn->render->Sprite(dialog.actor->sprite);
    }

    // Cursor de espera
    ngn->render->Sprite(cursor);

}



/*** Reproduce el dialogo cargado ***/
void TextBox::PlayDialog() {

    switch (st) {

        case st_idle:
            break;

        case st_play:
            TypeWriter(ngn->input->key_SPACE->down);
            if (dialog.eof) {
                CursorOn();
                next_st = st_end;
            } else if (dialog.more) {
                CursorOn();
                next_st = st_next;
            }
            break;

        case st_next:
            if (ngn->input->key_SPACE->down) {
                dialog.row = 0;
                dialog.more = false;
                dialog.home = true;
                dialog.timer = 0;
                dialog.cls = true;
                CursorOff();
                next_st = st_play;
            }
            break;

        case st_end:
            if (ngn->input->key_SPACE->down) {
                ResetDialog();
                box->Cls();
                next_st = st_exit;
            }
            break;

        case st_exit:
            break;

    }

    // Actualiza el estado
    st = next_st;

}



/*** Escribe un texto con el efecto maquina de escribir ***/
void TextBox::TypeWriter(bool skip) {

    do {

        // El texto esta vacio?
        if (dialog.text.size() == 0) dialog.eof = true;

        // Si es el fin de archivo o se ha llenado la caja, sal
        if (dialog.eof || dialog.more) return;

        // Has de borrar el contenido?
        if (dialog.cls) {
            box->Cls();
            dialog.cls = false;
        }

        // Autohome
        if (dialog.home) {
            box->Locate(dialog.style.start_x, dialog.style.start_y);
            dialog.home = false;
            // Hay que imprimir la cabecera?
            if (dialog.actor) {
                // Imprimela
                box->InkColor(dialog.actor->color);
                box->Print(dialog.actor->name);
                box->InkColor(text_color);
                // Siguiente fila
                dialog.row ++;
                // Si has alcanzado el limite indicalo
                if (dialog.row >= dialog.style.rows) {
                    dialog.more = true;
                } else {
                    // Siguiente linea
                    box->Locate(dialog.style.start_x, (dialog.style.start_y + (dialog.row * row_offset)));
                }
            }
        }

        // Contador de espera
        if (!skip && (dialog.blank_count == 0)) {
            dialog.timer ++;
            if (dialog.timer < dialog.speed) return;
            dialog.timer = 0;
        }

        // Segun si es un caracter valido o un salto de linea...
        std::string character = "";
        character = dialog.text.at(dialog.offset);
        if (character == "\n") {
            // Siguiente fila
            dialog.row ++;
            dialog.timer = dialog.speed;
            // Si has alcanzado el limite indicalo
            if (dialog.row >= dialog.style.rows) {
                dialog.more = true;
            } else {
                // Siguiente linea
                box->Locate(dialog.style.start_x, (dialog.style.start_y + (dialog.row * row_offset)));
            }
        } else {
            // Imprime el caracter
            box->Print(character);
            // Segun si era un espacio o no
            if (character == " ") {
                dialog.blank_count ++;
            } else {
                dialog.blank_count = 0;
            }
        }

        // Siguiente caracter
        dialog.offset ++;
        if (dialog.offset >= dialog.text.size()) dialog.eof = true;

    } while ((skip || (dialog.blank_count > 0)) && !dialog.eof && !dialog.more);

}



/*** Valores por defecto del dialogo ***/
void TextBox::ResetDialog() {

    // Control del dialogo (valores por defecto al iniciarlo)
    dialog.offset = 0;
    dialog.row = 0;
    dialog.more = false;
    dialog.eof = false;
    dialog.home = true;
    dialog.timer = 0;
    dialog.blank_count = 0;
    dialog.cls = true;
    // Propiedades del dialogo
    dialog.text = "";
    dialog.style = boxstyle_default;
    dialog.speed = 0;
    dialog.header = "";
    dialog.actor = NULL;

    // Estado del cursor
    CursorOff();

    // Maquina de estados
    st = next_st = st_idle;

}



/*** Establece un dialogo a reproducir ***/
void TextBox::SetDialog(std::string text, uint8_t speed, Actor* actor) {

    // Valores iniciales
    ResetDialog();

    // Velocidad de reproduccion
    dialog.speed = speed;

    // Estilo del dialogo
    if (actor) {
        if (actor->sprite) {
            dialog.style = boxtstyle_portrait;
            dialog.actor = actor;
            dialog.actor->sprite->Position(
                (text_box_position.x + dialog.actor->offset.x + (dialog.actor->sprite->width / 2.0f)),
                (text_box_position.y + dialog.actor->offset.y + (dialog.actor->sprite->height / 2.0f))
            );
        } else {
            dialog.style = boxstyle_default;
        }
    } else {
        dialog.style = boxstyle_default;
    }

    // Da formato al texto
    std::vector<std::string> lines;
    LineFormatter(text, dialog.style.columns, lines);
    dialog.text = "";
    for (uint32_t i = 0; i < lines.size(); i ++) {
        dialog.text += lines[i];
        dialog.text += "\n";
    }
    lines.clear();

    // Inicia el dialogo
    next_st = st_play;

}



/*** Devuelve el ancho actual seleccionado ***/
uint8_t TextBox::GetWidth() {

    return dialog.style.columns;

}



/*** Muestra el cursor ***/
void TextBox::CursorOn() {

    // Muestra el cursor
    cursor->visible = true;

    // Reinicia los parametros
    cursor_angle = 0.0f;

}



/*** Oculta el cursor ***/
void TextBox::CursorOff() {

    // Muestra el cursor
    cursor->visible = false;

    // Reinicia los parametros
    cursor_angle = 0.0f;

}



/*** Actualiza el cursor ***/
void TextBox::CursorUpdate() {

    // Si no esta visible, sal
    if (!cursor->visible) return;

    // Calcula la posicion
    Vector2 p;
    p.x = (float)cursor_position.x;
    p.y = (float)cursor_position.y + (std::sin(cursor_angle) * cursor_amplitude);

    // Aplica la posicion
    cursor->Position(p);

    // Movimiento
    cursor_angle += cursor_speed;
    if (cursor_angle > cursor_loop) cursor_angle -= cursor_loop;

}



/*** Separa en lineas el texto dado, para que encajen en la caja de texto ***/
void TextBox::LineFormatter(std::string text, uint32_t width, std::vector<std::string> &lines) {

    // Prepara el vector de salida
    lines.clear();

    // Si el ancho es 0, sal
    if (width == 0) return;

    // Prepara el analisis del texto
    uint32_t offset = 0;                    // Posicion en el texto
    std::string character = "";             // Guarda el caracter actual
    std::string current_word = "";          // Guarda la palabra actual
    std::string current_line = "";          // Guarda el contenido de la linea en curso

    // Si no hay texto valido, asume que es un salto de linea, registralo y sal
    if (text.size() == 0) {
        character = "\n";
        lines.push_back(character);
        return;
    }

    // Analisis del texto
    while (offset < text.size()) {

        // Lee el caracter actual
        character = text.at(offset);

        // Conteo de caracteres transcritos
        offset ++;

        // Filtro de caracteres de control
        if (offset == text.size()) {

            // Si el ultimo caracter no es un salto de linea ni un espacio, añadelo
            if ((character != "\n") && (character != " ")) current_word += character;
            if ((current_line.size() + current_word.size()) <= width) {
                // Y el contenido actual mas la palabra en curso no supera el limite
                current_line += current_word;       // Añade esta palabra
                lines.push_back(current_line);      // Guarda la linea actual
            } else {
                // Si no cabe
                lines.push_back(current_line);      // Guarda la linea actual
                lines.push_back(current_word);      // Guarda la palabra actual como una linea nueva
            }

        } else if (character == "\n") {

            // Si es un salto de linea
            if ((current_line.size() + current_word.size()) <= width) {
                // Y el contenido actual mas la palabra en curso no supera el limite
                current_line += current_word;       // Añade esta palabra
                lines.push_back(current_line);      // Guarda la linea actual
            } else {
                // Si no cabe
                lines.push_back(current_line);      // Guarda la linea actual
                lines.push_back(current_word);      // Guarda la palabra actual como una linea nueva
            }
            current_line = "";      // Nueva linea
            current_word = "";      // Nueva palabra

        } else if (character == " ") {

            // Si el caracter es un espacio, añade la palabra actual y ese espacio de ser posible
            if ((current_line.size() + current_word.size()) <= width) {
                // Y el contenido actual mas la palabra en curso no supera el limite
                current_line += current_word;       // Añade esta palabra
                current_word = "";                  // Nueva palabra
                // Verifica si es posible añadir el espacio detras de la palabra
                if ((current_line.size() + 1) < width) {
                    // Es posible, añade el espacio
                    current_line += character;
                } else {
                    // No es posible, guarda la linea actual y crea una de nueva
                    lines.push_back(current_line);
                    current_line = "";
                }
            } else {
                // Si no cabe
                lines.push_back(current_line);      // Guarda la linea
                current_line = "";                  // Nueva linea
            }

        } else {

            // Si el caracter no es ni espacio ni salto de linea, añadelo
            current_word += character;
            // Verifica si el contenido actual de la linea mas la nueva palabra sobrepasan el ancho
            if ((current_line.size() + current_word.size()) >= width) {
                // Si es asi, guarda la linea actual
                if (current_line.size() > 0) {
                    lines.push_back(current_line);
                } else {
                    lines.push_back(current_word);
                    current_word = "";
                }
                // Y crea una de nueva
                current_line = "";
                // Si ademas, la palabra actual supera el ancho, guardala como una linea nueva
                if (current_word.size() >= width) {
                    lines.push_back(current_word);
                    current_word = "";
                }
            }

        }

    }

}
