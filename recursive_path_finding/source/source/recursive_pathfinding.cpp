/******************************************************************************

    N'gine Lib for C++
    Recursive Path finding (Programa)
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
#include "recursive_pathfinding.h"
#include "maze.h"
#include "robot.h"



/*** Constructor de la clase ***/
RecursivePathfinding::RecursivePathfinding() {

    // Acceso al laberinto a traves del singleton
    maze = Maze::GetInstance();

    // Otros enlaces
    robot = NULL;

}



/*** Destructor de la clase ***/
RecursivePathfinding::~RecursivePathfinding() {

    // Anula los enalces a otros objetos
    maze = NULL;
    robot = NULL;

}



/*** Devuelve la ruta mas optima ***/
int32_t RecursivePathfinding::GetPath(Robot* _robot, Vector2I32 destination) {

    // Guarda el puntero al objeto
    robot = _robot;
    // Prepara los arrays de busqueda
    Clear();
    // Ejecuta el pathfinding recursivo
    return Find(robot->current_tile.x, robot->current_tile.y, destination.x, destination.y);

}



/*** Ejecuta el Pathfindig ***/
int32_t RecursivePathfinding::Find(int8_t sx, int8_t sy, int8_t ex, int8_t ey) {

    // Proteccion de coordenadas
    if ((sx < 0) || (ex < 0) || (sx >= maze->width) || (ex >= maze->width)) return -1;
    if ((sy < 0) || (ey < 0) || (sy >= maze->height) || (ey >= maze->height)) return -1;

    // Si la entrada o salida es invalida
    if ((maze->shape[sy][sx] != maze->id_floor) || (maze->shape[ey][ex] != maze->id_floor)) return -1;

    // Marca la salida del laberinto
    robot->maze_dumbbells[ey][ex] = maze->id_target;

    // Genera la red de pesos
    Dumbbells(sy, sx, 1);

    // Marca la salida del laberinto
    robot->maze_dumbbells[ey][ex] = maze->id_target;

    // Segun la red de pesos creada, busca la ruta mas corta
    ShortestPath(sy, sx, 1);

    // Marca las entradas y salidas
    robot->maze_path[sy][sx] = maze->id_start;
    robot->maze_path[ey][ex] = maze->id_end;

    return 0;

}



/*** Busca la ruta a la salida - Colocacion de pesos ***/
void RecursivePathfinding::Dumbbells(int8_t py, int8_t px, int32_t cnt) {

    // Si estas fuera de rango, devuelve -1
    if ((py < 0) || (px < 0) || (py >= maze->height) || (px >= maze->width)) return;

    // Exploracion previa
    if (robot->maze_dumbbells[py][px] == maze->id_target) {                                         // Si es la salida, sal
        if (path_min_length == maze->id_invalid) {
            path_min_length = cnt;
        } else {
            if (cnt < path_min_length) path_min_length = cnt;
        }
        return;
    } else if (robot->maze_dumbbells[py][px] == maze->id_invalid) {                                 // Si esta marcada como invalida, sal
        return;
    } else if ((robot->maze_dumbbells[py][px] != 0) && (robot->maze_dumbbells[py][px] < cnt)) {     // Si ya esta visitada y pesa menos, sal
        return;
    }

    // Coloca el peso en la casilla si es inferior
    robot->maze_dumbbells[py][px] = cnt;

    // Incrementa en 1 el peso
    cnt ++;

    // Recursividad en 4 direcciones
    Dumbbells((py - 1), px, cnt);
    Dumbbells((py + 1), px, cnt);
    Dumbbells(py, (px - 1), cnt);
    Dumbbells(py, (px + 1), cnt);

}



/*** Busca la ruta a la salida - Ruta mas corta ***/
int32_t RecursivePathfinding::ShortestPath(int8_t py, int8_t px, int32_t cnt) {

    // Si estas fuera de rango, devuelve -1
    if ((py < 0) || (px < 0) || (py >= maze->height) || (px >= maze->width)) return maze->id_invalid;

    // Exploracion previa
    if (robot->maze_dumbbells[py][px] == maze->id_target) {                 // Si es la salida, sal
        return maze->id_target;
    } else if (robot->maze_dumbbells[py][px] == maze->id_invalid) {         // Si esta marcada como invalida, sal
        return maze->id_invalid;
    } else if (robot->maze_dumbbells[py][px] != cnt) {                      // Si el peso no coincide, sal
        return maze->id_invalid;
    } else if (cnt > path_min_length) {                                     // Si el peso es superior al maximo de la ruta mas corta, sal
        return maze->id_invalid;
    }

    // Incrementa en 1 el peso
    cnt ++;

    // Recursividad en 4 direcciones
    if (
        (ShortestPath((py - 1), px, cnt) == maze->id_target)
        ||
        (ShortestPath((py + 1), px, cnt) == maze->id_target)
        ||
        (ShortestPath(py, (px - 1), cnt) == maze->id_target)
        ||
        (ShortestPath(py, (px + 1), cnt) == maze->id_target)
    ) {
        robot->maze_path[py][px] = maze->id_target;
        return maze->id_target;
    }

    return maze->id_invalid;

}



/*** Limpia el array de busqueda ***/
void RecursivePathfinding::Clear() {

    // Inicializa el array de busqueda, basado en el laberinto
    for (uint8_t y = 0; y < maze->height; y ++) {
        for (uint8_t x = 0; x < maze->width; x ++) {
            // Marca los muros como visitados, si no dejala a 0
            robot->maze_dumbbells[y][x] = (maze->shape[y][x] == maze->id_wall) ? maze->id_invalid:0;
            robot->maze_path[y][x] = robot->maze_dumbbells[y][x];
        }
    }

    // Resetea el peso maximo del camino mas optimo
    path_min_length = maze->id_invalid;

}
