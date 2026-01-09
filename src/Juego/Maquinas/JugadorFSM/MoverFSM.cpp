#include "MoverFSM.hpp"
#include "IdleFSM.hpp"

namespace IVJ
{
    
    MoverFSM::MoverFSM(bool flip_sprite, int dir)
        :FSM{}, flip{flip_sprite}, direccion{dir}
    {
        nombre="MoverFSM";
        std::cout<<nombre<<"\n";
    }

    FSM* MoverFSM::onInputs(const CE::IControl& control)
    {
        if(!control.arr && !control.abj && !control.der && !control.izq)
        {
            // Pasar la dirección y flip actual al estado Idle
            return new IdleFSM(flip, direccion);
        }

        // Actualizar dirección y flip según el input
        if(control.der)
        {
            flip = false;
            direccion = 0;
        }
        else if(control.izq)
        {
            flip = true;
            direccion = 0;
        }
        else if(control.arr)
        {
            direccion = 1;
        }
        else if(control.abj)
        {
            direccion = 2;
        }

        return nullptr;
    }

    void MoverFSM::onEntrar(const Entidad& obj)
    {
        auto c_sprite = obj.getComponente<CE::ISprite>();
        sprite = &c_sprite->m_sprite;
        s_w = c_sprite->width;
        s_h = c_sprite->height;

        // Configurar frames según la dirección
        if(direccion == 0) // Movimiento horizontal (izq/der)
        {
            ani_frames[0]= {10.f, 152.f};
            ani_frames[1]= {49.f, 152.f};
            ani_frames[2]= {93.f, 152.f};
            ani_frames[3]= {136.f, 152.f};
            ani_frames[4]= {184.f, 152.f};
            ani_frames[5]= {227.f, 152.f};
            ani_frames[6]= {271.f, 152.f};
            ani_frames[7]= {314.f, 152.f};

        }
        else if(direccion == 1) // Movimiento hacia arriba
        {
            ani_frames[0]= {14.f, 105.f};   // Usa los frames que necesites para arriba
            ani_frames[1]= {57.f, 105.f};
            ani_frames[2]= {101.f, 105.f};
            ani_frames[3]= {146.f, 105.f};
            ani_frames[4]= {189.f, 105.f};
            ani_frames[5]= {233.f, 105.f};
            ani_frames[6]= {278.f, 105.f};
            ani_frames[7]= {322.f, 105.f};
        }
        else if(direccion == 2) // Movimiento hacia abajo
        {
            ani_frames[0]= {13.f, 6.f};
            ani_frames[1]= {57.f, 6.f};
            ani_frames[2]= {100.f, 6.f};
            ani_frames[3]= {144.f, 6.f};
            ani_frames[4]= {188.f, 6.f};
            ani_frames[5]= {232.f, 6.f};
            ani_frames[6]= {276.f, 6.f};
            ani_frames[7]= {321.f, 6.f};
        }

        max_tiempo = 0.1f;
        tiempo = max_tiempo;
        id_actual = 0;

        // Solo aplicar flip en movimiento horizontal
        if(direccion == 0)
            flipSprite(obj);
    }

    void MoverFSM::flipSprite(const Entidad& obj)
    {
        auto c_sprite = obj.getComponente<CE::ISprite>();
        if(flip)
            sprite->setScale({-c_sprite->escala, c_sprite->escala});
        else
            sprite->setScale({c_sprite->escala, c_sprite->escala});
    }

    void MoverFSM::onSalir(const Entidad& obj)
    {
    }

    void MoverFSM::onUpdate(const Entidad& obj, float dt)
    {
        tiempo = tiempo - 1*dt;

        // Solo aplicar flip en movimiento horizontal
        if(direccion == 0)
            flipSprite(obj);

        if(tiempo <= 0)
        {
            sprite->setTextureRect(
                sf::IntRect{
                    {
                        (int)ani_frames[id_actual%8].x,
                        (int)ani_frames[id_actual%8].y
                    },
                    {
                        s_w,
                        s_h
                    }
                });
            tiempo = max_tiempo;
            id_actual++;
        }
    }
}