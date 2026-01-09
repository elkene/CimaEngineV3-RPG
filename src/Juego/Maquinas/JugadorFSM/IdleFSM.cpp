#include "IdleFSM.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "MoverFSM.hpp"

namespace IVJ
{
    IdleFSM::IdleFSM(bool flip_sprite, int dir)
        :FSM{}, flip{flip_sprite}, direccion{dir}
    {
        nombre = "IdleFSM";
        std::cout << nombre << "\n";
    }

    FSM* IdleFSM::onInputs(const CE::IControl& control)
    {
        if(control.arr)
            return new MoverFSM(false, 1); // Movimiento hacia arriba
        else if(control.abj)
            return new MoverFSM(false, 2); // Movimiento hacia abajo
        else if(control.der)
            return new MoverFSM(false, 0); // Movimiento horizontal derecha
        else if(control.izq)
            return new MoverFSM(true, 0);  // Movimiento horizontal izquierda

        return nullptr;
    }

    void IdleFSM::onEntrar(const Entidad& obj)
    {
        auto c_sprite = obj.getComponente<CE::ISprite>();
        sprite = &c_sprite->m_sprite;
        s_w = c_sprite->width;
        s_h = c_sprite->height;

        // Configurar frames según la dirección en que está viendo
        if(direccion == 0) // Idle horizontal (izquierda/derecha)
        {
            // Frames para idle mirando a los lados
            ani_frames[0] = {14.f, 201.f};
            ani_frames[1] = {58.f, 201.f};
            ani_frames[2] = {102.f, 201.f};
            ani_frames[3] = {146.f, 201.f};
        }
        else if(direccion == 1) // Idle mirando hacia arriba
        {
            // Frames para idle mirando arriba (ajusta según tu spritesheet)
            ani_frames[0] = {11.f, 250.f};  // Cambia estos valores
            ani_frames[1] = {55.f, 250.f};
            ani_frames[2] = {99.f, 250.f};
            ani_frames[3] = {143.f, 250.f};
        }
        else if(direccion == 2) // Idle mirando hacia abajo
        {
            // Frames para idle mirando abajo
            ani_frames[0] = {14.f, 201.f};
            ani_frames[1] = {58.f, 201.f};
            ani_frames[2] = {102.f, 201.f};
            ani_frames[3] = {146.f, 201.f};
        }

        max_tiempo = 0.2f;
        tiempo = max_tiempo;
        id_actual = 0;

        // Aplicar flip solo en dirección horizontal
        if(direccion == 0)
            flipSprite(obj);
    }

    void IdleFSM::flipSprite(const Entidad& obj)
    {
        auto c_sprite = obj.getComponente<CE::ISprite>();
        if(flip)
            sprite->setScale({-c_sprite->escala, c_sprite->escala});
        else
            sprite->setScale({c_sprite->escala, c_sprite->escala});
    }

    void IdleFSM::onSalir(const Entidad& obj){}

    void IdleFSM::onUpdate(const Entidad& obj, float dt)
    {
        tiempo = tiempo - 1*dt;

        // Solo aplicar flip en dirección horizontal
        if(direccion == 0)
            flipSprite(obj);

        if(tiempo <= 0)
        {
            sprite->setTextureRect(
                sf::IntRect{
                    {
                        (int)ani_frames[id_actual%4].x,
                        (int)ani_frames[id_actual%4].y
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