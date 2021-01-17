/******************************************************************************

    N'gine Lib for C++
    Gui (Declaraciones)
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



#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED



/*** Includes ***/
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "maze.h"
#include "robot.h"



/*** Declaracion de la clase ***/
class Gui {

    public:

        // Constructor
        Gui();

        // Destructor
        ~Gui();

        // Carga los recursos
        bool Load();

        // Crea la interficie
        void Create();

        // Actualiza los datos de la interficie
        void Update(Robot* _robot);

        // Render de los elementos de la interficie
        void RenderBack();
        void RenderFront();


    private:

        NGN_SpriteData* cursor_sprite_data;             // Datos del sprite del cursor
        NGN_Sprite* cursor_sprite;                      // Sprite del cursor

        // Path
        void DrawPath();                                // Metodo para "marcar" el camino mas optimo

        // Overlay de los pesos
        NGN_TextFont* monofonto_20;                     // Tipografia
        NGN_TextLayer* dumbbels_text_layer;             // Capa de texto
        void DumbbellsOverlay();                        // Sobreimprime el peso de cada tile

        // Gui
        NGN_TextFont* monofonto_24;                     // Tipografia
        NGN_TextLayer* gui_text_layer;                  // Capa de texto

        // Objetos vinculados
        Maze* maze;
        Robot* robot;


};



#endif // GUI_H_INCLUDED
