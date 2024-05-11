#ifndef _pajaro_h_
#define _pajaro_h_

namespace pajaro{
    enum class direccion {
        QUIETO = 0,
        HORIZONTAL = 1,
        VERTICAL = 2
    };

    enum class Estado {
        NULO = 0,
        VIVO = 1,
        MUERTO = 2
    }

    class pajaro{//Hacen falta: direccion, vuelo, estado, posicion
        public:
        pajaro(Direccion direccion, Estado estado, int px, int py);

    }
}

#endif