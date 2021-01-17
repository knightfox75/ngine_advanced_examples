/******************************************************************************

    N'gine Lib for C++
    Ejemplo de pathfinding recursivo (Declaraciones)
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



#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED



/*** Includes ***/
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "settings.h"
#include "maze.h"
#include "recursive_pathfinding.h"
#include "robot.h"
#include "gui.h"



/*** Declaracion de la clase ***/
class Program {

    public:

        // Constructor
        Program();

        // Destructor
        ~Program();

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

        // Objetos de codigo
        Maze* maze;                             // Singleton del laberinto
        RecursivePathfinding* pathfinding;      // Pathfinding recursivo
        Robot* robot;                           // Robot
        Gui* gui;                               // Graphic User Interface (Interfaz grafica de usuario)

        // Seleccion con el cursor
        Vector2I32 MouseOver();                         // Metodo para calcular el tile donde esta el cursor

};


#endif // PROGRAM_H_INCLUDED
