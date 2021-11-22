#pragma once

/*
* 
* Hanoi.h
* 
* Clases para crear torres de hanoi y su solución
*	Discos:	Representa los discos y calcula su movimiento entre torres
*	Torres:	Representa cada torre y gestiona sus discos
*	Hanoi:	Contiene las torres y realiza la solución (no recursiva)
* 
* RGuerreroX RGXDev 2021
*/

//-----------------------------------------------

#include "olcPixelGameEngine.h"

//-----------------------------------------------

class Disco
{
private:
	float posX, posY;
	int destinoX, destinoY;
	int ancho, factorAncho;
	float velocidad;
	char estado;

public:
	// Para crear un disco sin propiedades (temporal)
	Disco()
	{
		posX = posY = 0;
		destinoX = destinoY = 0;
		ancho = factorAncho = 0;
		velocidad = 0;
		estado = 'p';		// p: en posición, s: subiendo, h: movimiento horizontal, b: bajando
	}

	//-------------------------------------------

	// Para crear un disco cuyo ancho se calcula de acuerdo a su índice
	Disco(int index, int posX, int posY)
	{
		this->posX = destinoX = posX;
		this->posY = destinoY = posY;
		
		factorAncho = 15;
		ancho = factorAncho * (index + 1);
		velocidad = 250.0;
		estado = 'p';		// p: en posición, s: subiendo, h: movimiento horizontal, b: bajando
	}

public:
	// Setters
	void SetDestino(int newDestX, int newDestY)
	{
		destinoX = newDestX;
		destinoY = newDestY;

		// Se acaba de definir su destino, su estado cambia a subir para salir de torre
		estado = 's';
	}

	//----------------------------------------------

	// Getters
	int GetAncho() { return ancho; }

	//----------------------------------------------

	// Dibuja el disco centrado en posición actual
	void Draw(olc::PixelGameEngine* pge)
	{
		if (estado != 'p')
			Mover(pge->ScreenHeight(), pge->GetElapsedTime());

		int finalX = posX - ancho / 2;
		pge->DrawLine(finalX, posY, finalX + ancho, posY);
	}

	//----------------------------------------------
	
	// Se mueve de acuerdo a su estado
	void Mover(int screenHeight, float fElapsedTime)
	{
		switch (estado)
		{
		case 's':
			posY -= velocidad * fElapsedTime;
			if (posY <= screenHeight / 2.0 - 10)
			{
				posY = screenHeight / 2.0 - 10;
				estado = 'h';
			}
			break;
			
		case 'h':
			char direccion;
			direccion = posX < destinoX ? 'd' : 'i';
			posX = direccion == 'd' ? posX + velocidad * fElapsedTime : posX - velocidad * fElapsedTime;

			if (direccion == 'd' && posX >= destinoX || direccion == 'i' && posX <= destinoX)
			{
				posX = destinoX;
				estado = 'b';
			}
			break;

		case 'b':
			posY += velocidad * fElapsedTime;
			if (posY >= destinoY)
			{
				posY = destinoY;
				estado = 'p';
			}

		default:
			break;
		}
	}
};

//-----------------------------------------------
//-----------------------------------------------

class Torre
{
private:
	std::vector<Disco> discos;
	std::string nombre;
	int posX;

public:
	Torre(std::string nombre)
	{
		this->nombre = nombre;
		posX = 0;
	}

public:
	// Setters
	void SetPosX(int posX) { this->posX = posX; }

	//----------------------------------------------

	// Getters
	int GetPosX() { return posX; }
	int GetNumDiscos() { return discos.size(); }

	//----------------------------------------------

	// Inserta disco en el tope de la torre
	void InsertaDisco(Disco d)
	{
		discos.insert(discos.begin(), d);
	}

	//----------------------------------------------

	// Remueve disco de la parte superior de la torre
	void RemueveDisco()
	{
		discos.erase(discos.begin());
	}

	//----------------------------------------------

	// Retorna disco superior de la torre
	Disco TomarDisco()
	{
		return discos.size() > 0 ? discos[0] : Disco();
	}

	//----------------------------------------------

	// Dibuja la torre, desde centro de pantalla hasta borde inferior
	void Draw(olc::PixelGameEngine* pge, int screenHeight)
	{
		pge->DrawLine(posX, screenHeight / 2, posX, screenHeight);
		pge->DrawString(posX + 2, screenHeight - 8, nombre);

		// Dibuja los discos
		for (int i = 0; i < discos.size(); i++)
		{
			discos[i].Draw(pge);
		}
	}
};

//-----------------------------------------------
//-----------------------------------------------

class Hanoi
{
private:
	std::vector<Torre> torres;
	int screenWidth, screenHeight;
	int numDiscos, separacionDiscos;
	int numMovs, movActual;

public:
	Hanoi()
	{
		screenWidth = screenHeight = 0;
		numDiscos = separacionDiscos = 0;
		numMovs = movActual = 0;
	}

public:
	void Init(int screenWidth, int screenHeight, int numDiscos = 3)
	{
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;

		this->numDiscos = numDiscos < 3 || numDiscos > 6 ? 3 : numDiscos;
		separacionDiscos = 12;
		CrearTorres();

		numMovs = std::pow(2, numDiscos) - 1;
	}

private:
	// Crear las tres torres
	void CrearTorres()
	{
		torres.clear();
		torres.push_back(Torre("A (origen)"));
		torres.push_back(Torre("B (auxiliar)"));
		torres.push_back(Torre("C (destino)"));

		// Definir posición de cada torre
		for (int i = 0; i < torres.size(); i++)
		{
			torres[i].SetPosX(screenWidth / 4 * (i + 1));
		}

		// Inicia los discos en torre origen
		for (int i = numDiscos - 1; i >= 0; i--)
		{
			torres[0].InsertaDisco(Disco(i, torres[0].GetPosX(), screenHeight - separacionDiscos * (numDiscos - i)));
		}
	}

public:
	// Dibuja torres
	void Draw(olc::PixelGameEngine* pge)
	{
		for (int i = 0; i < torres.size(); i++)
		{
			torres[i].Draw(pge, screenHeight);
		}
		//std::for_each(torres.begin(), torres.end(), [this, pge](Torre t) {
		//	t.Draw(pge, screenHeight);
		//	});
	}

	//----------------------------------------------

	// Para mover discos entre dos torres
	void MoverDiscos(int torreOrigen, int torreDestino)
	{
		// Tomar discos superiores de cada torre
		Disco dOrigen = torres[torreOrigen].TomarDisco();
		Disco dDestino = torres[torreDestino].TomarDisco();

		// Torre 1 está vacía
		if (dOrigen.GetAncho() == 0)
		{
			CalculaDestinoDisco(&dDestino, torres[torreOrigen]);
			torres[torreOrigen].InsertaDisco(dDestino);
			torres[torreDestino].RemueveDisco();
		}
		// Torre 2 está vacía
		else if (dDestino.GetAncho() == 0)
		{
			CalculaDestinoDisco(&dOrigen, torres[torreDestino]);
			torres[torreDestino].InsertaDisco(dOrigen);
			torres[torreOrigen].RemueveDisco();
		}
		// Disco de torre 1 es mayor a disco de torre 2
		else if (dOrigen.GetAncho() > dDestino.GetAncho())
		{
			CalculaDestinoDisco(&dDestino, torres[torreOrigen]);
			torres[torreOrigen].InsertaDisco(dDestino);
			torres[torreDestino].RemueveDisco();
		}
		// Disco de torre 2 es mayor a disco de torre 1
		else
		{
			CalculaDestinoDisco(&dOrigen, torres[torreDestino]);
			torres[torreDestino].InsertaDisco(dOrigen);
			torres[torreOrigen].RemueveDisco();
		}
	}

	//----------------------------------------------

	// Calcula nuevo destino de disco al moverlo de torre
	void CalculaDestinoDisco(Disco* d, Torre tDestino)
	{
		d->SetDestino(tDestino.GetPosX(), screenHeight - separacionDiscos * (tDestino.GetNumDiscos() + 1));
	}

	//----------------------------------------------

	// Solución paso a paso (se avanza cada paso a solicitud)
	void SolucionSiguiente()
	{
		// Cambiar de torres si el número de discos es par
		if (movActual == 0 && numDiscos % 2 == 0) {
			Torre tTempd = torres[2]; torres.pop_back();
			Torre tTempa = torres[1]; torres.pop_back();
			torres.push_back(tTempd);
			torres.push_back(tTempa);
		}

		// Decidir si continuar de acuerdo a número de movimientos
		if (movActual >= numMovs)
			return;

		movActual++;

		if (movActual % 3 == 1)
			MoverDiscos(0, 2);		// origen <--> destino
		else if (movActual % 3 == 2)
			MoverDiscos(0, 1);		// origen <--> auxiliar
		else
			MoverDiscos(1, 2);		// auxiliar <--> destino
	}


	//----------------------------------------------
};

//-----------------------------------------------