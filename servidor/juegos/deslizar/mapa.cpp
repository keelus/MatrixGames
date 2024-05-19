#include "mapa.h"

namespace grave
{
    // Constructor
    Mapa::Mapa(MatrizLED matriz,int socketUsuario)
    {
        // Código de inicialización si es necesario
        this->cargarmapas(1);
        this->mapacolor(this->mapeado, matriz);
        bool accion = true;
        printf("0");
        while (accion)
        {
            /* code */
            printf("1");
             int valor =this->personaje.interaccion(socketUsuario,this->mapeado);
             if( valor == 1){
                accion = false;
             }
        }
        this->cargarmapas(2);
        this->mapacolor(this->mapeado, matriz);
        accion = true;
        while (accion)
        {
            /* code */
             int valor =this->personaje.interaccion(socketUsuario,this->mapeado);
             if( valor == 1){
                accion = false;
             }
        }
    }

    
  void Mapa::cargarmapas(int nivel) {
        // Inicialización de los mapas
        printf("2");
        ColorLED mapa1[8][8] = {ColorLED::Negro}; 

        for (int i = 0; i < 8; i++) {
            mapa1[0][i] = ColorLED::Naranja;
            mapa1[7][i] = ColorLED::Naranja;
            mapa1[i][7] = ColorLED::Naranja;
            mapa1[i][0] = ColorLED::Naranja;
            mapa1[1][1] = ColorLED::Naranja;
            if (i <= 5) {
                mapa1[4][i] = ColorLED::Naranja;
            }
            if (i <= 3) {
                mapa1[6][i] = ColorLED::Naranja;
            }
            if (i <= 5 && i >= 2) {
                mapa1[i][5] = ColorLED::Naranja;
            }
        }
        mapa1[2][4] = ColorLED::Naranja;
        mapa1[2][3] = ColorLED::Amarillo;
        mapa1[5][1] = ColorLED::Verde;
        this->personaje =  Personaje();
        this->personaje.y = 5;
        this->personaje.x = 1;
        // Copiar el mapa1 al miembro mapeado
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                this->mapeado[i][j] = mapa1[i][j];
            }
        }
    }

    void Mapa::mapacolor(ColorLED mapeado[8][8], MatrizLED matriz)
    {
        printf("3");
        matriz.SetMatrizColor(mapeado);
    }

    // Destructor
    Mapa::~Mapa()
    {
        // Código de limpieza si es necesario
    }

} // namespace grave