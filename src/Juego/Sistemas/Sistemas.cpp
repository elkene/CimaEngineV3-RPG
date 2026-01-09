#include "Sistemas.hpp"

namespace IVJ
{
    void SistemaControl(CE::Objeto&ente, float dt)
    {
        //no hay necesidad verificar si tiene, se asume que tiene
        //control y transformada
        auto p = ente.getTransformada();
        auto c = ente.getComponente<CE::IControl>();

        if(c->arr)
            p->velocidad.y=-200;
        if(c->abj)
            p->velocidad.y=200;
        if(c->der)
            p->velocidad.x=200;
        if(c->izq)
            p->velocidad.x=-200;
        if(c->run)
            p->velocidad.escala(1.3f);

    }

    void SistemaMover(const std::vector<std::shared_ptr<CE::Objeto>>& entes, float dt)
    {

        for(auto& ente : entes)
        {
            //todo ente tiene ITransform por lo que no requiere ferificación
            auto trans = ente->getTransformada();
            trans->posicion.suma(trans->velocidad.escala(dt));

        }

    }
    bool SistemaColAABB(CE::Objeto& A, CE::Objeto& B, bool resolucion)
    {
        if(!A.tieneComponente<CE::IBoundingBox>() || !B.tieneComponente<CE::IBoundingBox>())
            return false;
        auto bA = A.getComponente<CE::IBoundingBox>()->tam;
        auto mA = A.getComponente<CE::IBoundingBox>()->mitad;
        auto *pa = &A.getTransformada()->posicion;
        auto prevA = A.getTransformada()->pos_prev;
        auto bB = B.getComponente<CE::IBoundingBox>()->tam;
        auto mB = B.getComponente<CE::IBoundingBox>()->mitad;
        auto pb = B.getTransformada()->posicion;
        //calculos
        bool H = pa->y-mA.y < pb.y+bB.y - mB.y && pb.y-mB.y < pa->y+bA.y - mA.y;
        bool V = pa->x-mA.x < pb.x+bB.x - mB.x && pb.x-mB.x < pa->x+bA.x - mA.x;
        bool hay_colision = H && V;
        if(hay_colision && resolucion)
            *pa = prevA;
        return hay_colision;
    }

    bool SistemaColAABBMid(CE::Objeto& A, CE::Objeto& B, bool resolucion)
    {
        if(!A.tieneComponente<CE::IBoundingBox>() || !B.tieneComponente<CE::IBoundingBox>())
            return false;
        auto midA = A.getComponente<CE::IBoundingBox>()->mitad;
        auto *pa = &A.getTransformada()->posicion;
        auto prevA = A.getTransformada()->pos_prev;
        auto midB = B.getComponente<CE::IBoundingBox>()->mitad;
        auto *pb = &B.getTransformada()->posicion;
        //calculos
        float dX = std::abs(pb->x - pa->x);
        float dY = std::abs(pb->y - pa->y);
        float sumMidX = midA.x + midB.x;
        float sumMidY = midA.y + midB.y;
        bool V = sumMidX - dX > 0;
        bool H = sumMidY - dY > 0;
        bool hay_colision = V && H;
        if(resolucion && hay_colision)
            *pa = prevA;
        return hay_colision;


    }
}

