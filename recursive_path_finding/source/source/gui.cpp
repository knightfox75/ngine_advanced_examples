/******************************************************************************

    N'gine Lib for C++
    Gui (Programa)
    Version 1.1.0-r

    Proyecto iniciado el 23 de Noviembre del 2020
    (cc) 2020 - 2021 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 1.3.0-beta o superior

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
#include "gui.h"
#include "maze.h"
#include "robot.h"



/*** Constructor de la clase ***/
Gui::Gui() {

    // Objetos vinculados
    maze = Maze::GetInstance();
    robot = NULL;

    // Cursor del raton
    cursor_sprite_data = NULL;
    cursor_sprite = NULL;

    // Tipografias
    monofonto_20 = NULL;
    monofonto_24 = NULL;
    // Capas de texto
    dumbbels_text_layer = NULL;
    gui_text_layer = NULL;

}



/*** Destructor de la clase ***/
Gui::~Gui() {

    // Objetos vinculados
    robot = NULL;

    // Cursor del raton
    delete cursor_sprite; cursor_sprite = NULL;
    delete cursor_sprite_data; cursor_sprite_data = NULL;

    // Capa de texto
    delete dumbbels_text_layer; dumbbels_text_layer = NULL;
    delete gui_text_layer; gui_text_layer = NULL;
    // Tipografia
    delete monofonto_20; monofonto_20 = NULL;
    delete monofonto_24; monofonto_24 = NULL;

}



/*** Carga los recursos ***/
bool Gui::Load() {

    // Imagen del cursor
    cursor_sprite_data = ngn->load->Sprite("data/sprites/cursor.spr");
    if (!cursor_sprite_data) return false;

    // Tipografias
    monofonto_20 = ngn->load->TrueTypeFont("data/fonts/monofonto.ttf", 20, true, 0xFFFFFF, 1, 0x000000);
    if (!monofonto_20) return false;
    monofonto_24 = ngn->load->TrueTypeFont("data/fonts/monofonto.ttf", 24);
    if (!monofonto_24) return false;

    // Carga correcta
    return true;

}



/*** Crea la interfaz ***/
void Gui::Create() {

    // Crea el cursor del raton
    cursor_sprite = new NGN_Sprite(cursor_sprite_data);

    // Capa de texto (Pesos)
    dumbbels_text_layer = new NGN_TextLayer(monofonto_20, NULL, 48, 56, 1184, 608);
    dumbbels_text_layer->CanvasColor(0xFFFFFF00);
    dumbbels_text_layer->Padding(0);
    dumbbels_text_layer->Cls();
    dumbbels_text_layer->visible = false;

    // Capa de texto (GUI)
    gui_text_layer = new NGN_TextLayer(monofonto_24, NULL, 8, 8, 600, 32);
    gui_text_layer->Cls();
    gui_text_layer->Print("[F1] SHOW DUMBBELLS     [F2] START/STOP ROBOT");

}



/*** Actualiza la interfaz ***/
void Gui::Update(Robot* _robot) {

    // Guarda los vinculos
    robot = _robot;

    // Cursor del raton
    cursor_sprite->Position(ngn->input->mouse.x, ngn->input->mouse.y);

    // Actualiza el path segun el robot
    DrawPath();

    // Muestra la red de pesos si se solicita
    DumbbellsOverlay();

}



/*** Render de los elementos del fondo ***/
void Gui::RenderBack() {

    // Capa de texto con los pesos del robot
    ngn->render->TextLayer(dumbbels_text_layer);

}



/*** Render de los elementos del primer plano ***/
void Gui::RenderFront() {

    // Cursor del raton
    ngn->render->Sprite(cursor_sprite);
    // Texto de la GUI
    ngn->render->TextLayer(gui_text_layer);

}



/*** Actualiza el dibujo del path ***/
void Gui::DrawPath() {

    // Actualiza la informacion en pantalla
    for (uint8_t y = 0; y < maze->height; y ++) {
        for (uint8_t x = 0; x < maze->width; x ++) {
            if (maze->shape[y][x] == maze->id_floor) {
                if (robot) {
                    switch (robot->maze_path[y][x]) {
                        case maze->id_start:
                            maze->maze_sprites[y][x]->frame = maze->token_floor_red;
                            break;
                        case maze->id_end:
                            maze->maze_sprites[y][x]->frame = maze->token_floor_green;
                            break;
                        case maze->id_target:
                            maze->maze_sprites[y][x]->frame = maze->token_floor_cyan;
                            break;
                        default:
                            maze->maze_sprites[y][x]->frame = maze->token_floor_blue;
                            break;
                    }
                } else {
                    maze->maze_sprites[y][x]->frame = maze->token_floor_blue;
                }
            }
        }
    }

}



/*** OVERLAY - Imprime la red de pesos ***/
void Gui::DumbbellsOverlay() {

    // F1 activar/desactivar
    if (ngn->input->key_F1->down) dumbbels_text_layer->visible = !dumbbels_text_layer->visible;

    if (!dumbbels_text_layer->visible) return;

    dumbbels_text_layer->Cls();

    for (uint32_t y = 0; y < maze->height; y ++) {
        for (uint32_t x = 0; x < maze->width; x ++) {
            dumbbels_text_layer->Locate(((x << 5) + 5), ((y << 5) + 2));
            if (robot) {
                if (robot->maze_dumbbells[y][x] == maze->id_target)
                    dumbbels_text_layer->Print("[]");
                else if (robot->maze_dumbbells[y][x] >= 0) {
                    dumbbels_text_layer->Print(ngn->toolbox->Int2String(robot->maze_dumbbells[y][x], 2, "0"));
                }
            } else {
                dumbbels_text_layer->Print(ngn->toolbox->Int2String(0, 2, "0"));
            }
        }
    }

}
