#pragma once
#include "../FSM.hpp"

namespace IVJ
{
    class IdleFSM : public FSM
    {
    public:
        explicit IdleFSM(bool flip_sprite = false, int dir = 0);
        ~IdleFSM()override {};
        FSM* onInputs(const CE::IControl& control)override;
        void onEntrar(const Entidad& obj) override;
        void onSalir(const Entidad& obj) override;
        void onUpdate(const Entidad& obj,float dt) override;
    private:
        void flipSprite(const Entidad& obj);

        bool flip;
        int direccion; // 0=horizontal, 1=arriba, 2=abajo

        CE::Vector2D ani_frames[4]{};
        float tiempo{};
        int id_actual{};
        float max_tiempo{};
        //sprite ref
        sf::Sprite * sprite{nullptr};
        int s_w{};
        int s_h{};
    };
}