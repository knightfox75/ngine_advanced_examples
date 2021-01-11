/******************************************************************************

    N'gine Lib for C++
    Ejemplo de pathfinding recursivo (Declaraciones)
    Version 1.0.0-r

    Proyecto iniciado el 23 de Noviembre del 2020
    (cc) 2020 - 2021 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 1.2.0-beta o superior

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



#ifndef DEMO_H_INCLUDED
#define DEMO_H_INCLUDED



/*** Includes ***/
// Includes de la libreria
#include <ngn.h>



/*** Parametros de la ventana ***/
static const std::string WINDOW_TITLE = "N'gine Recursive Pathfinding Example";     // Titulo de la ventana
static const uint32_t SCR_WIDTH = 1280;                                             // Resolucion
static const uint32_t SCR_HEIGHT = 720;
static const int8_t SCR_MODE_WINDOWS = NGN_SCR_WINDOW;                              // Modo de pantalla en Windows
static const int8_t SCR_MODE_LINUX = NGN_SCR_WINDOW;                                // Modo de pantalla en Linux
static const bool SHOW_MOUSE = false;                                               // Estado del puntero del raton
static const bool BILINEAR_FILTER = false;                                          // Filtrado bi-linear
static const bool VSYNC = true;                                                     // Sincronismo vertical
static const bool FPS_COUNTER = false;                                              // Contador de frames por segundo (solo en modo debug)



/*** Declaracion de la clase ***/
class Demo {

    public:

        // Constructor
        Demo();

        // Destructor
        ~Demo();

        // Procesos iniciales
        bool Awake();

        // Al iniciar el programa
        bool Start();

        // Nucleo de ejecucion
        int8_t Run();


    private:

        // Carga de archivos necesarios
        bool Load();

        // Crea los elementos
        void Create();

        // Logica del programa
        void Logic();

        // Render de los elementos graficos
        void Render();

        // Texturas del laberinto (piezas)
        static const uint8_t total_maze_tokens = 10;        // Numero total de fotogramas del sprites de las piezas
        const uint8_t token_floor_red = 0;
        const uint8_t token_floor_green = 1;
        const uint8_t token_floor_blue = 2;
        const uint8_t token_floor_yellow = 3;
        const uint8_t token_floor_cyan = 4;
        const uint8_t token_wall_red = 5;
        const uint8_t token_wall_green = 6;
        const uint8_t token_wall_blue = 7;
        const uint8_t token_wall_yellow = 8;
        const uint8_t token_wall_cyan = 9;
        NGN_SpriteData* token_sprite_data;                  // Datos del sprites de las piezas del laberinto

        // Diseño del laberinto
        const float token_w = 32.0f, token_h = 32.0f;       // Tamaño de cada bloque
        static const uint8_t maze_w = 37, maze_h = 19;      // Tamaño en bloques del laberinto
        const float maze_pos_x = ((float)SCR_WIDTH / 2.0f) - ((token_w * maze_w) / 2.0f);       // Posicion inicial del laberinto
        const float maze_pos_y = ((float)SCR_HEIGHT / 2.0f) - ((token_h * maze_h) / 2.0f);

        static const uint8_t maze_floor = 0;                // ID del suelo
        static const uint8_t maze_wall = 1;                 // ID del muro
        static const uint8_t maze_start = 8;                // Entrada del laberinto
        static const uint8_t maze_end = 9;                  // Salida del laberinto
        static const int8_t maze_invalid = -1;              // Casilla invalida
        static const int8_t maze_target = -2;               // Destino del path

        // Mapa del laberinto
        const uint32_t maze_shape[maze_h][maze_w] = {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1},
            {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
            {1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1},
            {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1},
            {1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
            {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
            {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
            {1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
            {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1},
            {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
            {1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1},
            {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
        };

        // Metodos de creacion y render del laberinto
        NGN_Sprite* maze_sprites[maze_h][maze_w];   // Array con los sprites del decorado
        void CreateMaze();                          // Crea el laberinto
        void RenderMaze();                          // Render de los bloques del laberinto

        // Metodos y datos del pathfinding
        int32_t maze_path[maze_h][maze_w];                                  // Array con el (los) caminos validos
        int32_t maze_dumbbells[maze_h][maze_w];                             // Array con los calculos de los pesos
        int32_t PathFinding(int8_t sx, int8_t sy, int8_t ex, int8_t ey);    // Metodo para ejecutar el pathfinding
        void MazeDumbbells(int8_t py, int8_t px, int32_t cnt);              // Metodo para el calculo de los pesos
        int32_t MazePath(int8_t py, int8_t px, int32_t cnt);                // Metodo para la resolucion de caminos validos
        void ClearMazePath();                                               // Metodo para resetear el contenidos de TODOS los arrays
        void DrawMazePath();                                                // Metodos para "marcar" el camino mas optimo
        int32_t path_min_length;                                            // Guarda el peso maximo del camino mas corto

        // Seleccion con el cursor
        NGN_SpriteData* cursor_sprite_data;             // Datos del sprite del cursor
        NGN_Sprite* cursor_sprite;                      // Sprite del cursor
        Vector2I32 MouseOver();                         // Metodo para calcular el tile donde esta el cursor

        // Robot
        const int32_t robot_start_x = 1;                // Posicion de salida del robot
        const int32_t robot_start_y = 1;
        const float robot_speed = 2.0f;                 // Velocidad de desplazamiento
        const float robot_angular_speed = 5.0f;         // Velocidad de giro
        static const int8_t robot_move_stop = -1;       // Definicion de los comandos de direccion
        static const int8_t robot_move_up = 3;
        static const int8_t robot_move_down = 1;
        static const int8_t robot_move_left = 2;
        static const int8_t robot_move_right = 0;
        const int8_t robot_frame_idle = 0;              // Frame a mostrar con el robot detenido
        const int8_t robot_frame_run = 3;               // Frame a mostrar con el robot en marcha
        int8_t robot_direction, robot_next_direction;   // Direccion actual y siguiente direccion
        Vector2I32 robot_tile, robot_next_tile;         // Tile actual y de destino
        Vector2 robot_velocity;                         // Vector de velocidad
        float robot_angle, robot_target_angle;          // Angulo objetivo
        bool robot_motor_on;                            // El robot se ha de mover?
        NGN_SpriteData* robot_sprite_data;              // Datos del sprite del robot
        NGN_Sprite* robot_sprite;                       // Sprite del robot
        int8_t robot_state, robot_next_state;           // Maquina de estados del robot
        static const int8_t robot_st_idle = 0;          // Estado: En espera
        static const int8_t robot_st_set_move = 1;      // Estado: Aplica la velocidad para moverse a la siguiente celda
        static const int8_t robot_st_move = 2;          // Estado: Muevete a la siguiente celda
        static const int8_t robot_st_rotate = 3;        // Estado: Rota al robot
        void CreateRobot();                             // Crea al robot
        void MoveRobot();                               // Mueve el robot
        void RobotGetNextMove();                        // Busca el siguiente tile al que moverse

        // Overlay de los pesos
        NGN_TextFont* monofonto_20;                     // Tipografia
        NGN_TextLayer* dumbbels_text_layer;             // Capa de texto
        void DumbbellsOverlay();                        // Sobreimprime el peso de cada tile

        // Gui
        NGN_TextFont* monofonto_24;                     // Tipografia
        NGN_TextLayer* gui_text_layer;                  // Capa de texto

};


#endif // DEMO_H_INCLUDED
