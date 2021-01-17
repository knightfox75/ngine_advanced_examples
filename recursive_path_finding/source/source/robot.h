/******************************************************************************

    N'gine Lib for C++
    Robot (Declaraciones)
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



#ifndef ROBOT_H_INCLUDED
#define ROBOT_H_INCLUDED



/*** Includes ***/
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "maze.h"



/*** Declaracion de la clase ***/
class Robot {

    public:

        // Constructor
        Robot();

        // Destructor
        ~Robot();

        // Carga los recursos del robot
        bool Load();

        // Crea al robot
        void Create();

        // Actualiza el estado del robot
        void Update();

        // Render de los sprites del robot
        void Render();

        // Datos del pathfinding para este robot
        int32_t maze_path[Maze::height][Maze::width];           // Array con el (los) caminos validos
        int32_t maze_dumbbells[Maze::height][Maze::width];      // Array con los calculos de los pesos
        Vector2I32 current_tile;                                // Tile actual y de destino final



    private:

        const int32_t start_x = 1;                	// Posicion de salida del robot
        const int32_t start_y = 1;
        const float speed = 2.0f;                 	// Velocidad de desplazamiento
        const float angular_speed = 5.0f;         	// Velocidad de giro

        static const int8_t move_stop = -1;       	// Definicion de los comandos de direccion
        static const int8_t move_up = 3;
        static const int8_t move_down = 1;
        static const int8_t move_left = 2;
        static const int8_t move_right = 0;

        const int8_t frame_idle = 0;              	// Frame a mostrar con el robot detenido
        const int8_t frame_run = 3;               	// Frame a mostrar con el robot en marcha

        int8_t direction, next_direction;   		// Direccion actual y siguiente direccion
        Vector2I32 next_tile;                     	// Siguiente tile a la que moverse
        Vector2 velocity;                         	// Vector de velocidad
        float angle, target_angle;          		// Angulo objetivo
        bool motor_on;                            	// El robot se ha de mover?

        NGN_SpriteData* sprite_data;              	// Datos del sprite del robot
        NGN_Sprite* sprite;                       	// Sprite del robot

        int8_t state, next_state;           		// Maquina de estados del robot
        static const int8_t st_idle = 0;          	// Estado: En espera
        static const int8_t st_set_move = 1;      	// Estado: Aplica la velocidad para moverse a la siguiente celda
        static const int8_t st_move = 2;          	// Estado: Muevete a la siguiente celda
        static const int8_t st_rotate = 3;        	// Estado: Rota al robot

        void Move();                              	// Mueve el robot
        void GetNextMove();                        	// Busca el siguiente tile al que moverse

        // Objetos vinculados
        Maze* maze;                                 // Acceso al laberinto a traves del singleton

};



#endif // ROBOT_H_INCLUDED
