/******************************************************************************

    Ejemplo de una aventura conversacional: Personajes actores

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2021 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 1.5.0-beta o superior

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



#ifndef CAST_H_INCLUDED
#define CAST_H_INCLUDED



/*** Includes ***/
#include <vector>
// Includes de la libreria
#include <ngn.h>
// Includes del proyecto
#include "actor.h"



/*** Declaracion de la clase ***/
class Cast {

    public:

        // Constructor de la clase
        Cast();

        // Destructor de la clase
        ~Cast();

        // Carga los recursos de la clase
        bool Load();

        // Crea a los actores
        void Create();

        // Objetos de los actores
        std::vector<Actor*> actors;


    private:

        // Imagenes de los retratos
        NGN_SpriteData* setsuna_img;
        NGN_SpriteData* natsumi_img;


};



#endif // CAST_H_INCLUDED
