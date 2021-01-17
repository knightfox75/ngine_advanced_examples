/******************************************************************************

    N'gine Lib for C++
    Ejemplo de pathfinding recursivo (Programa)
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
#include <cmath>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "program.h"
#include "recursive_pathfinding.h"
#include "maze.h"
#include "robot.h"
#include "gui.h"



/*** Constructor de la clase ***/
Program::Program() {

    maze = Maze::GetInstance();                             // Singleton del laberinto

    robot = new Robot();                                    // Objeto del Robot
    pathfinding = new RecursivePathfinding();               // Pathfinding recursivo
    gui = new Gui();                                        // Objeto de la interfaz

}



/*** Destructor de la clase ***/
Program::~Program() {

    delete gui; gui = NULL;
    delete pathfinding; pathfinding = NULL;
    delete robot; robot = NULL;

    Maze::RemoveInstance();
    maze = NULL;

}



/*** Inicializa N'GINE ***/
bool Program::Awake() {

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
bool Program::Start() {

    // Carga de archivos
    if (!Load()) return false;

    // Crea los elementos del programa
    Create();

    // Procesos correctos
    return true;

}



/*** Ejecucion del programa ***/
int8_t Program::Run() {

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
bool Program::Load() {

    // Laberinto
    if (!maze->Load()) return false;
    // Robot
    if (!robot->Load()) return false;
    // GUI
    if (!gui->Load()) return false;

    // Archivos cargados con exito
    return true;

}



/*** Crea los elementos del programa ***/
void Program::Create() {

    // Crea el laberinto
    maze->Create();
    // Crea el Robot
    robot->Create();
    // Crea la interfaz
    gui->Create();

}



/*** Logica del programa ***/
void Program::Logic() {

    Vector2I32 p = MouseOver();         // Lee el nº de tile segun la posicion del raton

    pathfinding->GetPath(robot, p);     // Calcula el path para este robot
    robot->Update();

    gui->Update(robot);                 // Muestra la informacion de este robot

}



/*** Render de los elementos graficos ***/
void Program::Render() {

    // Laberinto
    maze->Render();
    // Red de pesos
    gui->RenderBack();
    // Robot
    robot->Render();
    // Cursor y textos de ayuda
    gui->RenderFront();


}



/*** Seleccion con el cursor ***/
Vector2I32 Program::MouseOver() {

    // Calcula el tile segun la posicion del puntero del raton
    Vector2I32 tile = {0, 0};
    tile.x = ((ngn->input->mouse.x - maze->position_x) / (int32_t)maze->token_width);
    tile.y = ((ngn->input->mouse.y - maze->position_y) / (int32_t)maze->token_height);

    // Devuelve el resultado
    return tile;

}
