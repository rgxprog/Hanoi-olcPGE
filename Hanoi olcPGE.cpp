/*
* 
* Hanoi olcPGE
* 
* Solución a Torres de Hanoi de forma animada
* El propósito es mostrar el movimiento de los discos entre las torres
* 
* Utilizo olcPixelGameEngine como motor gráfico
* https://github.com/OneLoneCoder/olcPixelGameEngine
* 
* RGuerreroX RGXDev 2021
*/

//-----------------------------------------------

#define OLC_PGE_APPLICATION

#include "olcPixelGameEngine.h"
#include "Hanoi.h"
#include <iostream>

//-----------------------------------------------

class HanoiOlcPge : public olc::PixelGameEngine
{
private:
    int width = 400;
    int height = 300;
    Hanoi hanoi;

public:
    HanoiOlcPge()
    {
        sAppName = "Torres de Hanoi";
        hanoi.Init(width, height, 4);
    }

public:
    bool OnUserCreate() override
    {
        return true;
    }

    //-------------------------------------------

    bool OnUserUpdate(float fElapsedTime) override
    {
        // Limpia pantalla para redibujar
        Clear(olc::BLACK);

        // Dibuja estado actual de las torres
        hanoi.Draw(this);

        // Siguiente paso en la solución
        if (GetMouse(0).bPressed)
        {
            hanoi.SolucionSiguiente();
        }

        return true;
    }

public:
    // Getters
    int GetWidth() { return width; }
    int GetHeight() { return height; }
};

//-----------------------------------------------

int main()
{
    HanoiOlcPge h;

    if (h.Construct(h.GetWidth(), h.GetHeight(), 2, 2))
    {
        h.Start();
    }

    return 0;
}

//-----------------------------------------------