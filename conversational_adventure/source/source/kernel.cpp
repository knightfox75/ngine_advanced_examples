/******************************************************************************

    N'gine Project Template: Nucleo de ejecucion del juego

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



/*** Includes ***/
// c++
#include <cstdio>
#include <iostream>
// Includes de la libreria NGN
#include <ngn.h>
// Includes del proyecto
#include "kernel.h"
#include "settings.h"



/*** Constructor de la clase ***/
Kernel::Kernel() {

    // Objetos de codigo
    background = new Background();
    narrator = new Narrator();


}



/*** Destructor de la clase ***/
Kernel::~Kernel() {

    // Objetos de codigo
    delete narrator; narrator = NULL;
    delete background; background = NULL;


}



/*** Inicializa N'GINE ***/
bool Kernel::Awake() {

    // Inicializa la libreria
    if (!ngn->Init()) {
        std::cout << "Critical error, can't initialize n'gine." << std::endl;
        return false;
    }

    // Habilita el archivo de registro
    ngn->log->OpenLogFile("debug_log.txt");

    // Crea la ventana con la resolucion nativa
    if (!ngn->graphics->Init(WINDOW_TITLE, SCR_WIDTH, SCR_HEIGHT, NGN_SCR_WINDOW, BILINEAR_FILTER, VSYNC)) return false;
    ngn->graphics->Update();

    // Visibilidad del cursor del raton
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

    // Usa el archivo empaquetado de datos si es la version release
    #if defined (MODE_RELEASE)
        ngn->load->SetPackage("data.pkg", "U<m>?'jA!2LvZ,nq");
    #endif

	// Muestra la version de la libreria en la consola
	#if defined (MODE_DEBUG)
		ngn->log->Message(ngn->system->GetVersion() + " started successfully!");
	#endif

    // Inicializacion completada con exito
    return true;

}



/*** Al iniciar el programa ***/
bool Kernel::Start() {

    // Carga de archivos
    if (!Load()) return false;

    // Crea los elementos del programa
    Create();

    // Procesos correctos
    return true;

}



/*** Ejecuta el juego ***/
void Kernel::Run() {

    // Control del loop
    uint8_t loop = FLAG_ZERO;

    while (loop == FLAG_ZERO) {

        // Gestor de eventos de SDL y N'gine
        ngn->system->EventUpdate();

        // Logica del programa
        loop |= Update();
        // Render de los elementos graficos
        Render();

        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();
        // Actualiza el sonido
        ngn->sound->Update();

        // Salida del programa
        if (
            ngn->system->quit                                           // Si se pulsa la [X] de la ventana o ALT+F4
            ||
            ngn->input->key_ESC->down                                   // la tecla ESC
            ||
            ngn->input->controller[0].button[XBOX_BUTTON_XBOX].down     // o el boton [X] del gamepad de XBOX
        ) loop |= FLAG_EXIT;

    }

}


/*** Carga de los archivos necesarios ***/
bool Kernel::Load() {

    // Recursos de los objetos de codigo
    if (!background->Load()) return false;              // Decorado de fondo
    if (!narrator->Load()) return false;                // Narrador

    // Archivos cargados con exito
    return true;

}



/*** Crea los elementos del programa ***/
void Kernel::Create() {

    // Crea los elementos de la escena
    background->Create();               // Decorado de fondo
    narrator->Create();                 // Narrador

}



/*** Logica del programa ***/
uint8_t Kernel::Update() {

    uint8_t r = FLAG_ZERO;

    // Elementos de la escena
    background->Update();                           // Decorado de fondo
    if (narrator->Update() > 0) r |= FLAG_EXIT;     // Narrador

    return r;

}



/*** Render de los elementos graficos ***/
void Kernel::Render() {

    // Elementos de la escena
    background->Render();               // Decorado de fondo
    narrator->Render();                 // Narrador

}
