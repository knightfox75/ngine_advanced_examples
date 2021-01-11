/******************************************************************************

    N'gine Lib for C++
    Ejemplo de pathfinding recursivo (Programa)
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



/*** Includes ***/
// c++
#include <cstdio>
#include <iostream>
#include <cmath>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"



/*** Constructor de la clase ***/
Demo::Demo() {

    // Imagenes de los tokens
    token_sprite_data = NULL;

    // Sprites de los tokens, datos de busqueda, etc
    for (uint8_t y = 0; y < maze_h; y ++) {
        for (uint8_t x = 0; x < maze_w; x ++) {
            // Sprites
            maze_sprites[y][x] = NULL;
        }
    }

    // Cursor del raton
    cursor_sprite_data = NULL;
    cursor_sprite = NULL;

    // Robot
    robot_sprite_data = NULL;
    robot_sprite = NULL;

    // Tipografias
    monofonto_20 = NULL;
    monofonto_24 = NULL;
    // Capas de texto
    dumbbels_text_layer = NULL;
    gui_text_layer = NULL;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Sprites de los tokens
    for (uint8_t y = 0; y < maze_h; y ++) {
        for (uint8_t x = 0; x < maze_w; x ++) {
            delete maze_sprites[y][x];
            maze_sprites[y][x] = NULL;
        }
    }

    // Imagenes de los tokens
    delete token_sprite_data; token_sprite_data = NULL;

    // Cursor del raton
    delete cursor_sprite; cursor_sprite = NULL;
    delete cursor_sprite_data; cursor_sprite_data = NULL;

    // Robot
    delete robot_sprite; robot_sprite = NULL;
    delete robot_sprite_data; robot_sprite_data = NULL;

    // Capa de texto
    delete dumbbels_text_layer; dumbbels_text_layer = NULL;
    delete gui_text_layer; gui_text_layer = NULL;
    // Tipografia
    delete monofonto_20; monofonto_20 = NULL;
    delete monofonto_24; monofonto_24 = NULL;

}



/*** Inicializa N'GINE ***/
bool Demo::Awake() {

    // Inicializa la libreria
    if (!ngn->Init()) {
        std::cout << "Critical error, can't initialize n'gine." << std::endl;
        return false;
    }

    // Crea la ventana con la resolucion nativa
    if (!ngn->graphics->Init(WINDOW_TITLE, SCR_WIDTH, SCR_HEIGHT, NGN_SCR_WINDOW, BILINEAR_FILTER, VSYNC)) return false;
    ngn->graphics->Update();

    // visibilidad del cursor del raton
    ngn->graphics->ShowMouse(SHOW_MOUSE);

    // Contador de FPS activo?
    ngn->system->fps_counter = FPS_COUNTER;

    // Selecciona el modo grafico (ventana/full screen)
    #if defined (OS_WINDOWS)
        ngn->graphics->SetMode(SCR_MODE_WINDOWS);
    #elif defined (OS_LINUX)
        ngn->graphics->SetMode(SCR_MODE_LINUX);
    #endif
    ngn->graphics->Update();

    // Muestra la version de la libreria en la consola
    #if defined (MODE_DEBUG)
        std::cout << ngn->system->GetVersion() << std::endl;
    #endif

    // Inicializacion completada con exito
    return true;

}



/*** Al iniciar el programa ***/
bool Demo::Start() {

    // Carga de archivos
    if (!Load()) return false;

    // Crea los elementos del programa
    Create();

    // Procesos correctos
    return true;

}



/*** Ejecucion del programa ***/
int8_t Demo::Run() {

    // Control del loop
    int8_t loop = -1;

    while (loop < 0) {

        // Gestor de eventos de SDL y N'gine
        ngn->system->EventUpdate();            // Actualiza los eventos


        /***
        Bucle principal del programa
        ***/
        Logic();        // Logica del programa
        Render();       // Render de los elementos graficos


        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();
        // Actualiza el sonido
        ngn->sound->Update();

        // Control del bucle principal
        if (ngn->system->quit) {    // Si se pulsa la [X] de la ventana
            loop = 0;
        } else if (ngn->input->key_ESC->down || ngn->input->controller[0].button[10].down) {    // Si se pulsa la tecla [ESC] O se pulsa el boton XBOX
            loop = 1;
        }

    }

    // Devuelve el resultado
    return loop;

}



/*** Carga de los archivos necesarios ***/
bool Demo::Load() {

    // Imagenes de los tokens
    token_sprite_data = ngn->load->Sprite("data/sprites/maze_tokens.spr");
    if (!token_sprite_data) return false;

    // Imagen del cursor
    cursor_sprite_data = ngn->load->Sprite("data/sprites/cursor.spr");
    if (!cursor_sprite_data) return false;

    // Imagen de Robot
    robot_sprite_data = ngn->load->Sprite("data/sprites/robot.spr");
    if (!robot_sprite_data) return false;

    // Tipografias
    monofonto_20 = ngn->load->TrueTypeFont("data/fonts/monofonto.ttf", 20, true, 0xFFFFFF, 1, 0x000000);
    if (!monofonto_20) return false;
    monofonto_24 = ngn->load->TrueTypeFont("data/fonts/monofonto.ttf", 24);
    if (!monofonto_24) return false;

    // Archivos cargados con exito
    return true;

}



/*** Crea los elementos del programa ***/
void Demo::Create() {

    // Crea el laberinto
    CreateMaze();

    // Crea el cursor del raton
    cursor_sprite = new NGN_Sprite(cursor_sprite_data);

    // Crea el Robot
    CreateRobot();

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



/*** Logica del programa ***/
void Demo::Logic() {

    MoveRobot();
    Vector2I32 p = MouseOver();     // Lee el nº de tile segun la posicion del raton
    ClearMazePath();                // Limpia el array de calculos
    PathFinding(robot_tile.x, robot_tile.y, p.x, p.y);    // Ejecuta el path finding
    DrawMazePath();                 // Dibuja el laberinto actualizado
    DumbbellsOverlay();             // Overlay en pantalla de las pesas

    // Cursor del raton
    cursor_sprite->Position(ngn->input->mouse.x, ngn->input->mouse.y);

}



/*** Render de los elementos graficos ***/
void Demo::Render() {

    // Laberinto
    RenderMaze();

    // Capa de texto
    ngn->render->TextLayer(dumbbels_text_layer);

    // Robot
    ngn->render->Sprite(robot_sprite);

    // Cursor del raton
    ngn->render->Sprite(cursor_sprite);

    // Texto de la GUI
    ngn->render->TextLayer(gui_text_layer);


}



/*** Crea el laberinto ***/
void Demo::CreateMaze() {

    // Calcula la posicion inicial
    Vector2 start_pos = {0.0f, 0.0f};
    Vector2 pos = start_pos;
    start_pos.x = ((float)SCR_WIDTH / 2.0f) - ((token_w * (float)maze_w) / 2.0f) + (token_w / 2.0f);
    start_pos.y = ((float)SCR_HEIGHT / 2.0f) - ((token_h * (float)maze_h) / 2.0f) + (token_h / 2.0f);

    // Define las piezas de la construccion
    const uint8_t tk_floor = token_floor_blue;
    const uint8_t tk_wall = token_wall_yellow;
    const uint8_t tk_start = token_floor_red;
    const uint8_t tk_end = token_floor_green;

    // Bucle de creacion
    for (uint8_t y = 0; y < maze_h; y ++) {
        // Calculo de la posicion Y
        pos.y = (start_pos.y + ((float)y * token_h));
        for (uint8_t x = 0; x < maze_w; x ++) {
            // Calculo de la posicion X
            pos.x = (start_pos.x + ((float)x * token_w));
            // Crea el sprite
            maze_sprites[y][x] = new NGN_Sprite(token_sprite_data, pos.x, pos.y);
            // Imagen a usar
            switch (maze_shape[y][x]) {
                case maze_floor:
                    maze_sprites[y][x]->frame = tk_floor;
                    break;
                case maze_wall:
                    maze_sprites[y][x]->frame = tk_wall;
                    break;
                case maze_start:
                    maze_sprites[y][x]->frame = tk_start;
                    break;
                case maze_end:
                    maze_sprites[y][x]->frame = tk_end;
                    break;
            }
        }
    }

}



/*** Render de los elementos del laberito ***/
void Demo::RenderMaze() {

    for (uint8_t y = 0; y < maze_h; y ++) {
        for (uint8_t x = 0; x < maze_w; x ++) {
            ngn->render->Sprite(maze_sprites[y][x]);
        }
    }

}



/*** Pathfindig ***/
int32_t Demo::PathFinding(int8_t sx, int8_t sy, int8_t ex, int8_t ey) {

    // Proteccion de coordenadas
    if ((sx < 0) || (ex < 0) || (sx >= maze_w) || (ex >= maze_w)) return -1;
    if ((sy < 0) || (ey < 0) || (sy >= maze_h) || (ey >= maze_h)) return -1;

    // Si la entrada o salida es invalida
    if ((maze_shape[sy][sx] != maze_floor) || (maze_shape[ey][ex] != maze_floor)) return -1;

    // Marca la salida del laberinto
    maze_dumbbells[ey][ex] = maze_target;

    // Genera la red de pesos
    MazeDumbbells(sy, sx, 1);

    // Marca la salida del laberinto
    maze_dumbbells[ey][ex] = maze_target;

    // Segun la red de pesos creada, busca la ruta mas corta
    MazePath(sy, sx, 1);

    // Marca las entradas y salidas
    maze_path[sy][sx] = maze_start;
    maze_path[ey][ex] = maze_end;

    return 0;

}



/*** Busca la ruta a la salida - Colocacion de pesos ***/
void Demo::MazeDumbbells(int8_t py, int8_t px, int32_t cnt) {

    // Si estas fuera de rango, devuelve -1
    if ((py < 0) || (px < 0) || (py >= maze_h) || (px >= maze_w)) return;

    // Exploracion previa
    if (maze_dumbbells[py][px] == maze_target) {                                    // Si es la salida, sal
        if (path_min_length == maze_invalid) {
            path_min_length = cnt;
        } else {
            if (cnt < path_min_length) path_min_length = cnt;
        }
        return;
    } else if (maze_dumbbells[py][px] == maze_invalid) {                            // Si esta marcada como invalida, sal
        return;
    } else if ((maze_dumbbells[py][px] != 0) && (maze_dumbbells[py][px] < cnt)) {   // Si ya esta visitada y pesa menos, sal
        return;
    }

    // Coloca el peso en la casilla si es inferior
    maze_dumbbells[py][px] = cnt;

    // Incrementa en 1 el peso
    cnt ++;

    // Recursividad en 4 direcciones
    MazeDumbbells((py - 1), px, cnt);
    MazeDumbbells((py + 1), px, cnt);
    MazeDumbbells(py, (px - 1), cnt);
    MazeDumbbells(py, (px + 1), cnt);

}



/*** Busca la ruta a la salida - Ruta mas corta ***/
int32_t Demo::MazePath(int8_t py, int8_t px, int32_t cnt) {

    // Si estas fuera de rango, devuelve -1
    if ((py < 0) || (px < 0) || (py >= maze_h) || (px >= maze_w)) return maze_invalid;

    // Exploracion previa
    if (maze_dumbbells[py][px] == maze_target) {                // Si es la salida, sal
        return maze_target;
    } else if (maze_dumbbells[py][px] == maze_invalid) {        // Si esta marcada como invalida, sal
        return maze_invalid;
    } else if (maze_dumbbells[py][px] != cnt) {                 // Si el peso no coincide, sal
        return maze_invalid;
    } else if (cnt > path_min_length) {
        return maze_invalid;
    }

    // Incrementa en 1 el peso
    cnt ++;

    // Recursividad en 4 direcciones
    if (
        (MazePath((py - 1), px, cnt) == maze_target)
        ||
        (MazePath((py + 1), px, cnt) == maze_target)
        ||
        (MazePath(py, (px - 1), cnt) == maze_target)
        ||
        (MazePath(py, (px + 1), cnt) == maze_target)
    ) {
        maze_path[py][px] = maze_target;
        return maze_target;
    }

    return maze_invalid;

}



/*** Limpia el array de busqueda ***/
void Demo::ClearMazePath() {

    // Inicializa el array de busqueda, basado en el laberinto
    for (uint8_t y = 0; y < maze_h; y ++) {
        for (uint8_t x = 0; x < maze_w; x ++) {
            // Marca los muros como visitados, si no dejala a 0
            maze_dumbbells[y][x] = (maze_shape[y][x] == maze_wall) ? maze_invalid:0;
            maze_path[y][x] = maze_dumbbells[y][x];
        }
    }

    // Resetea el peso maximo del camino mas optimo
    path_min_length = maze_invalid;

}


/*** Actualiza el dibujo del path ***/
void Demo::DrawMazePath() {

    // Actualiza la informacion en pantalla
    for (uint8_t y = 0; y < maze_h; y ++) {
        for (uint8_t x = 0; x < maze_w; x ++) {
            if (maze_shape[y][x] == maze_floor) {
                switch (maze_path[y][x]) {
                    case maze_start:
                        maze_sprites[y][x]->frame = token_floor_red;
                        break;
                    case maze_end:
                        maze_sprites[y][x]->frame = token_floor_green;
                        break;
                    case maze_target:
                        maze_sprites[y][x]->frame = token_floor_cyan;
                        break;
                    default:
                        maze_sprites[y][x]->frame = token_floor_blue;
                        break;
                }
            }
        }
    }

}



/*** Seleccion con el cursor ***/
Vector2I32 Demo::MouseOver() {

    // Calcula el tile segun la posicion del puntero del raton
    Vector2I32 tile = {0, 0};
    tile.x = ((ngn->input->mouse.x - maze_pos_x) / (int32_t)token_w);
    tile.y = ((ngn->input->mouse.y - maze_pos_y) / (int32_t)token_h);

    // Devuelve el resultado
    return tile;

}



/*** Crea al robot ***/
void Demo::CreateRobot() {

    // Crea al sprite
    robot_sprite = new NGN_Sprite(robot_sprite_data);
    robot_sprite->frame = robot_frame_idle;

    // Guarda la tile inicial
    robot_tile.x = robot_start_x;
    robot_tile.y = robot_start_y;
    robot_next_tile = robot_tile;

    // Calcula la posicion inicial
    Vector2 p = {0.0f, 0.0f};
    p.x = (((float)robot_tile.x * token_w) + maze_pos_x + (token_w / 2.0f));
    p.y = (((float)robot_tile.y * token_h) + maze_pos_y + (token_h / 2.0f));
    robot_sprite->Position(p);
    robot_velocity = {0.0f, 0.0f};
    robot_angle = robot_target_angle = 0.0f;

    // Parametros del comportamiento
    robot_direction = robot_next_direction = robot_move_stop;
    robot_state = robot_next_state = robot_st_idle;
    robot_motor_on = true;

}



/*** Mueve al robot ***/
void Demo::MoveRobot() {

    // F2 para activar o no el robot
    if (ngn->input->key_F2->down) robot_motor_on = !robot_motor_on;

    // Frame de estado
    robot_sprite->frame = robot_motor_on ? robot_frame_run:robot_frame_idle;

    // Vector de movimiento
    Vector2 d = {0.0f, 0.0f};

    // Maquina de estados
    do {

        robot_state = robot_next_state;

        switch (robot_state) {

            // En espera
            case robot_st_idle:
                // Reinicia la velocidad
                robot_velocity = {0.0f, 0.0f};
                // Busca si has de moverte a otra casilla
                RobotGetNextMove();
                // Si la casilla de estino cambia, registralo y muevete
                if ((robot_tile.x != robot_next_tile.x) || (robot_tile.y != robot_next_tile.y)) {
                    if (robot_direction != robot_next_direction) {
                        robot_target_angle = ((float)robot_next_direction * 90.f);
                        if ((robot_target_angle == 0.0f) && (robot_angle == 270.0f)) robot_angle = -90.0f;
                        if ((robot_target_angle == 270.0f) && (robot_angle == 0.0f)) robot_angle = 360.0f;
                        robot_next_state = robot_st_rotate;
                    } else {
                        robot_next_state = robot_st_set_move;
                    }
                }
                break;

            // Aplica la velocidad segun la direccion de movimiento
            case robot_st_set_move:
                switch (robot_direction) {
                    case robot_move_up:
                        robot_velocity.y = -robot_speed;
                        break;
                    case robot_move_down:
                        robot_velocity.y = robot_speed;
                        break;
                    case robot_move_left:
                        robot_velocity.x = -robot_speed;
                        break;
                    case robot_move_right:
                        robot_velocity.x = robot_speed;
                        break;
                }
                robot_next_state = robot_st_move;
                break;

            // Muevete a la siguiente celda
            case robot_st_move:
                d.x = std::abs(robot_sprite->position.x - maze_sprites[robot_next_tile.y][robot_next_tile.x]->position.x);
                d.y = std::abs(robot_sprite->position.y - maze_sprites[robot_next_tile.y][robot_next_tile.x]->position.y);
                if ((d.x < robot_speed) && (d.y < robot_speed)) {
                    robot_sprite->position = maze_sprites[robot_next_tile.y][robot_next_tile.x]->position;
                    robot_tile = robot_next_tile;
                    robot_next_state = robot_st_idle;
                } else {
                    // Si el robot esta activado...
                    if (robot_motor_on) robot_sprite->Translate(robot_velocity);
                }
                break;

            // Encarate a la siguiente celda
            case robot_st_rotate:
                if (std::abs(robot_angle - robot_target_angle) < robot_angular_speed) {
                    robot_angle = robot_target_angle;
                    robot_direction = robot_next_direction;
                    robot_next_state = robot_st_set_move;
                } else {
                    // Si el motor esta activado...
                    if (robot_motor_on) {
                        if (robot_angle < robot_target_angle) {
                            robot_angle += robot_angular_speed;
                        } else{
                            robot_angle -= robot_angular_speed;
                        }
                    }
                }
                robot_sprite->rotation = robot_angle;
                break;

        }

    } while (robot_state != robot_next_state);


}



/*** Busca el siguiente tile al que moverse ***/
void Demo::RobotGetNextMove() {

    robot_next_direction = robot_move_stop;

    Vector2I32 rt;

    if (robot_tile.y > 0) {
        rt.x = robot_tile.x;
        rt.y = (robot_tile.y - 1);
        if (
            (maze_path[rt.y][rt.x] == maze_target)
            ||
            (maze_path[rt.y][rt.x] == maze_end)
        ) {
            robot_next_direction = robot_move_up;
            robot_next_tile.x = rt.x;
            robot_next_tile.y = rt.y;
        }
    }

    if (robot_tile.y < (maze_h - 1)) {
        rt.x = robot_tile.x;
        rt.y = (robot_tile.y + 1);
        if (
            (maze_path[rt.y][rt.x] == maze_target)
            ||
            (maze_path[rt.y][rt.x] == maze_end)
        ) {
            robot_next_direction = robot_move_down;
            robot_next_tile.x = rt.x;
            robot_next_tile.y = rt.y;
        }
    }

    if (robot_tile.x > 0) {
        rt.x = (robot_tile.x - 1);
        rt.y = robot_tile.y;
        if (
            (maze_path[rt.y][rt.x] == maze_target)
            ||
            (maze_path[rt.y][rt.x] == maze_end)
        ) {
            robot_next_direction = robot_move_left;
            robot_next_tile.x = rt.x;
            robot_next_tile.y = rt.y;
        }
    }

    if (robot_tile.x < (maze_w - 1)) {
        rt.x = (robot_tile.x + 1);
        rt.y = robot_tile.y;
        if (
            (maze_path[rt.y][rt.x] == maze_target)
            ||
            (maze_path[rt.y][rt.x] == maze_end)
        ) {
            robot_next_direction = robot_move_right;
            robot_next_tile.x = rt.x;
            robot_next_tile.y = rt.y;
        }
    }

}



/*** OVERLAY - Imprime la red de pesos ***/
void Demo::DumbbellsOverlay() {

    // F1 activar/desactivar
    if (ngn->input->key_F1->down) dumbbels_text_layer->visible = !dumbbels_text_layer->visible;

    if (!dumbbels_text_layer->visible) return;

    dumbbels_text_layer->Cls();

    for (uint32_t y = 0; y < maze_h; y ++) {
        for (uint32_t x = 0; x < maze_w; x ++) {
            dumbbels_text_layer->Locate(((x << 5) + 5), ((y << 5) + 2));
            if (maze_dumbbells[y][x] == maze_target)
                dumbbels_text_layer->Print("[]");
            else if (maze_dumbbells[y][x] >= 0) {
                dumbbels_text_layer->Print(ngn->toolbox->Int2String(maze_dumbbells[y][x], 2, "0"));
            }
        }
    }

}
