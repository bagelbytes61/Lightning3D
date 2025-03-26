#ifndef LIGHTNING3D_CRICLE_HH_
#define LIGHTNING3D_CIRCLE_HH_

#include "Drawable.hh"

namespace Lightning3D {
    class Circle : public Drawable {
    public:
        Circle(float radius) {

        }
    private:
        static auto GenerateVertices(float radius) -> std::vector<Vertex> {

        }

    };


} // namespace Lightning3D

#endif