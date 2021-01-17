/******************************************************************************

    N'gine Lib for C++
    Robot (Programa)
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
#include "robot.h"
#include "maze.h"




/*** Constructor de la clase ***/
Robot::Robot() {

    // Acceso al laberinto a traves del singleton
    maze = Maze::GetInstance();

    // Robot
    sprite_data = NULL;
    sprite = NULL;

}



/*** Destructor de la clase ***/
Robot::~Robot() {

    // Robot
    delete sprite; sprite = NULL;
    delete sprite_data; sprite_data = NULL;

    // Anula los enalces a otros objetos
    maze = NULL;

}



/*** Carga los recursos del robot ***/
bool Robot::Load() {

    sprite_data = ngn->load->Sprite("data/sprites/robot.spr");
    if (!sprite_data) return false;

    return true;

}



/*** Crea al robot ***/
void Robot::Create() {

    // Crea al sprite
    sprite = new NGN_Sprite(sprite_data);
    sprite->frame = frame_idle;

    // Guarda la tile inicial
    current_tile.x = start_x;
    current_tile.y = start_y;
    next_tile = current_tile;

    // Calcula la posicion inicial
    Vector2 p = {0.0f, 0.0f};
    p.x = (((float)current_tile.x * maze->token_width) + maze->position_x + (maze->token_width / 2.0f));
    p.y = (((float)current_tile.y * maze->token_height) + maze->position_y + (maze->token_height / 2.0f));
    sprite->Position(p);
    velocity = {0.0f, 0.0f};
    angle = target_angle = 0.0f;

    // Parametros del comportamiento
    direction = next_direction = move_stop;
    state = next_state = st_idle;
    motor_on = true;

}



/*** Actualiza al robot ***/
void Robot::Update() {

    // F2 para activar o no el robot
    if (ngn->input->key_F2->down) motor_on = !motor_on;

    // Muevelo
    Move();

}



/*** Render de los sprites del robot ***/
void Robot::Render() {

    ngn->render->Sprite(sprite);

}



/*** Mueve al robot ***/
void Robot::Move() {

    // Frame de estado
    sprite->frame = motor_on ? frame_run:frame_idle;

    // Vector de movimiento
    Vector2 d = {0.0f, 0.0f};

    // Maquina de estados
    do {

        state = next_state;

        switch (state) {

            // En espera
            case st_idle:
                // Reinicia la velocidad
                velocity = {0.0f, 0.0f};
                // Busca si has de moverte a otra casilla
                GetNextMove();
                // Si la casilla de estino cambia, registralo y muevete
                if ((current_tile.x != next_tile.x) || (current_tile.y != next_tile.y)) {
                    if (direction != next_direction) {
                        target_angle = ((float)next_direction * 90.f);
                        if ((target_angle == 0.0f) && (angle == 270.0f)) angle = -90.0f;
                        if ((target_angle == 270.0f) && (angle == 0.0f)) angle = 360.0f;
                        next_state = st_rotate;
                    } else {
                        next_state = st_set_move;
                    }
                }
                break;

            // Aplica la velocidad segun la direccion de movimiento
            case st_set_move:
                switch (direction) {
                    case move_up:
                        velocity.y = -speed;
                        break;
                    case move_down:
                        velocity.y = speed;
                        break;
                    case move_left:
                        velocity.x = -speed;
                        break;
                    case move_right:
                        velocity.x = speed;
                        break;
                }
                next_state = st_move;
                break;

            // Muevete a la siguiente celda
            case st_move:
                d.x = std::abs(sprite->position.x - maze->maze_sprites[next_tile.y][next_tile.x]->position.x);
                d.y = std::abs(sprite->position.y - maze->maze_sprites[next_tile.y][next_tile.x]->position.y);
                if ((d.x < speed) && (d.y < speed)) {
                    sprite->position = maze->maze_sprites[next_tile.y][next_tile.x]->position;
                    current_tile = next_tile;
                    next_state = st_idle;
                } else {
                    // Si el robot esta activado...
                    if (motor_on) sprite->Translate(velocity);
                }
                break;

            // Encarate a la siguiente celda
            case st_rotate:
                if (std::abs(angle - target_angle) < angular_speed) {
                    angle = target_angle;
                    direction = next_direction;
                    next_state = st_set_move;
                } else {
                    // Si el motor esta activado...
                    if (motor_on) {
                        if (angle < target_angle) {
                            angle += angular_speed;
                        } else{
                            angle -= angular_speed;
                        }
                    }
                }
                sprite->rotation = angle;
                break;

        }

    } while (state != next_state);


}



/*** Busca el siguiente tile al que moverse ***/
void Robot::GetNextMove() {

    next_direction = move_stop;

    Vector2I32 rt;

    if (current_tile.y > 0) {
        rt.x = current_tile.x;
        rt.y = (current_tile.y - 1);
        if (
            (maze_path[rt.y][rt.x] == maze->id_target)
            ||
            (maze_path[rt.y][rt.x] == maze->id_end)
        ) {
            next_direction = move_up;
            next_tile.x = rt.x;
            next_tile.y = rt.y;
        }
    }

    if (current_tile.y < (maze->height - 1)) {
        rt.x = current_tile.x;
        rt.y = (current_tile.y + 1);
        if (
            (maze_path[rt.y][rt.x] == maze->id_target)
            ||
            (maze_path[rt.y][rt.x] == maze->id_end)
        ) {
            next_direction = move_down;
            next_tile.x = rt.x;
            next_tile.y = rt.y;
        }
    }

    if (current_tile.x > 0) {
        rt.x = (current_tile.x - 1);
        rt.y = current_tile.y;
        if (
            (maze_path[rt.y][rt.x] == maze->id_target)
            ||
            (maze_path[rt.y][rt.x] == maze->id_end)
        ) {
            next_direction = move_left;
            next_tile.x = rt.x;
            next_tile.y = rt.y;
        }
    }

    if (current_tile.x < (maze->width - 1)) {
        rt.x = (current_tile.x + 1);
        rt.y = current_tile.y;
        if (
            (maze_path[rt.y][rt.x] == maze->id_target)
            ||
            (maze_path[rt.y][rt.x] == maze->id_end)
        ) {
            next_direction = move_right;
            next_tile.x = rt.x;
            next_tile.y = rt.y;
        }
    }

}
