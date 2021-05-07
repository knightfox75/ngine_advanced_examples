/******************************************************************************

    Ejemplo de una aventura conversacional: Caja de texto

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



#ifndef TEXTBOX_H_INCLUDED
#define TEXTBOX_H_INCLUDED



/*** Includes ***/
// C++
#include <string>
// Includes de la libreria
#include <ngn.h>
// Includes del proyecto
#include "actor.h"



/*** Declaracion de la clase ***/
class TextBox {

    public:

        // Constructor de la clase
        TextBox();

        // Destructor de la clase
        ~TextBox();

        // Carga los recursos de la clase
        bool Load();

        // Crea la caja de texto
        void Create();

        // Actualiza el contenido de la caja de texto
        uint8_t Update();

        // Render de la caja de texto
        void Render();

        // Establece un texto a reproducir
        void SetDialog(std::string text, uint8_t speed, Actor* actor = NULL);

        // Devuelve el ancho actual de la caja
        uint8_t GetWidth();

        // Define los estados
        static const uint8_t st_idle = 0;
        static const uint8_t st_play = 1;
        static const uint8_t st_next = 2;
        static const uint8_t st_end = 3;
        static const uint8_t st_exit = 4;


    private:

        // Propiedades de la caja de texto
        const Vector2I32 text_box_position = {64, 448};     // Posicion de la caja de texto en la pantalla
        const uint32_t font_size = 32;                      // Tamaño de la fuente
        const uint32_t row_offset = 36;                     // Espacio entre lineas
        const uint32_t text_color = 0xFFFFFFFF;             // Color del texto por defecto

        // Estilos de las cajas de texto
        struct TextBoxStyle {
            uint8_t columns;        // Numero de columnas en la caja
            uint8_t rows;           // Numero de filas en la caja
            uint32_t start_x;       // Posicion X inicial del texto en la caja
            uint32_t start_y;       // Posicion Y inicial del texto en la caja
        };
        // COLUMNS, ROWS, START_X, START_Y
        const TextBoxStyle boxstyle_default = {62, 5, 64, 26};
        const TextBoxStyle boxtstyle_portrait = {52, 5, 224, 26};

        // Elementos graficos de la caja de texto
        NGN_TextureData* box_img;       // Imagen de la caja de texto
        NGN_TextFont* box_font;         // Tipografia que se usara
        NGN_TextLayer* box;             // Capa de texto

        // Cursor
        NGN_SpriteData* cursor_img;         // Imagen del cursor
        NGN_Sprite* cursor;                 // Sprite del cursor

        // Reproduce un dialogo
        void PlayDialog();
        uint8_t st, next_st;

        // Escribe el texto con el efecto de maquina de escribir
        struct {
            // Control del proceso
            uint32_t offset;        // nº de caracter a imprimir
            uint8_t row;            // nº de fila
            bool more;              // Se ha llenado la caja, pero aun queda texto?
            bool eof;               // Fin del archivo (end of file)
            bool home;              // Has de colocar el cursor en la posicion inicial
            uint8_t timer;          // Contador de espera entre pulsaciones
            uint32_t blank_count;   // Contador de espacios en blanco
            bool cls;               // Borra el contenido antes de escribir
            // Propiedades
            std::string text;       // Texto a imprimir
            TextBoxStyle style;     // Estilo que esta usando la caja
            uint8_t speed;          // Velocidad del texto
            std::string header;     // Cabecera del texto (en la primera fila)
            Actor* actor;           // Actor del dialogo (si existe);
        } dialog;
        void TypeWriter(bool skip);     // Metodo para su escritura

        // Valores predeterminados del dialogo (reset)
        void ResetDialog();

        // Control del cursor
        const Vector2I32 cursor_position = {1160, 608};     // Posicion del cursor de continuar (posicion en pantalla)
        const float cursor_speed = 0.2f;                    // Velocidad de oscilacion (en radianes)
        const float cursor_amplitude = 12.0f;               // Amplitud de la oscilacion
        const float cursor_loop = (PI * 2.0f);              // Limite angular
        float cursor_angle;                                 // Control de posicion
        void CursorOn();
        void CursorOff();
        void CursorUpdate();


        // Da formato a una linea de dialogo para que encaje en la caja de texto
        void LineFormatter(std::string text, uint32_t width, std::vector<std::string> &lines);


};



#endif // TEXTBOX_H_INCLUDED
