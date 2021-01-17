/******************************************************************************

    N'gine Lib for C++
    Elementos del laberinto (Programa)
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
#include "maze.h"



/*** Singleton de la clase a NULL ***/
Maze* Maze::instance = NULL;



/*** Obten la instancia ***/
Maze* Maze::GetInstance() {

    if (!instance) instance = new Maze();
    return instance;

}



/*** Elimina la instancia ***/
void Maze::RemoveInstance() {

    if (instance) {
        delete instance;
        instance = NULL;
    }

}



/*** Constructor de la clase ***/
Maze::Maze() {

    // Imagenes de los tokens
    token_sprite_data = NULL;

    // Sprites de los tokens, datos de busqueda, etc
    for (uint8_t y = 0; y < height; y ++) {
        for (uint8_t x = 0; x < width; x ++) {
            // Sprites
            maze_sprites[y][x] = NULL;
        }
    }

}



/*** Destructor de la clase ***/
Maze::~Maze() {

    // Sprites de los tokens
    for (uint8_t y = 0; y < height; y ++) {
        for (uint8_t x = 0; x < width; x ++) {
            delete maze_sprites[y][x];
            maze_sprites[y][x] = NULL;
        }
    }

    // Imagenes de los tokens
    delete token_sprite_data; token_sprite_data = NULL;

}



/*** Carga los recursos ***/
bool Maze::Load() {

    // Imagenes de los tokens
    token_sprite_data = ngn->load->Sprite("data/sprites/maze_tokens.spr");
    if (!token_sprite_data) return false;

    // Carga correcta
    return true;

}



/*** Crea el laberinto ***/
void Maze::Create() {

    // Calcula la posicion inicial
    Vector2 start_pos = {0.0f, 0.0f};
    Vector2 pos = start_pos;
    start_pos.x = ((float)SCR_WIDTH / 2.0f) - ((token_width * (float)width) / 2.0f) + (token_width / 2.0f);
    start_pos.y = ((float)SCR_HEIGHT / 2.0f) - ((token_height * (float)height) / 2.0f) + (token_height / 2.0f);

    // Define las piezas de la construccion
    const uint8_t tk_floor = token_floor_blue;
    const uint8_t tk_wall = token_id_wall_yellow;
    const uint8_t tk_start = token_floor_red;
    const uint8_t tk_end = token_floor_green;

    // Bucle de creacion
    for (uint8_t y = 0; y < height; y ++) {
        // Calculo de la posicion Y
        pos.y = (start_pos.y + ((float)y * token_height));
        for (uint8_t x = 0; x < width; x ++) {
            // Calculo de la posicion X
            pos.x = (start_pos.x + ((float)x * token_width));
            // Crea el sprite
            maze_sprites[y][x] = new NGN_Sprite(token_sprite_data, pos.x, pos.y);
            // Imagen a usar
            switch (shape[y][x]) {
                case id_floor:
                    maze_sprites[y][x]->frame = tk_floor;
                    break;
                case id_wall:
                    maze_sprites[y][x]->frame = tk_wall;
                    break;
                case id_start:
                    maze_sprites[y][x]->frame = tk_start;
                    break;
                case id_end:
                    maze_sprites[y][x]->frame = tk_end;
                    break;
            }
        }
    }

}



/*** Render de los elementos del laberito ***/
void Maze::Render() {

    for (uint8_t y = 0; y < height; y ++) {
        for (uint8_t x = 0; x < width; x ++) {
            ngn->render->Sprite(maze_sprites[y][x]);
        }
    }

}
