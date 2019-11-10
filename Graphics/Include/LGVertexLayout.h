#ifndef LG_VERTEX_LAYOUT_H_
#define LG_VERTEX_LAYOUT_H_

#pragma once

#include "LGObject.h"

namespace Lightning3D {
    class VertexLayout : public Object {
    public:
        virtual ~VertexLayout() override = 0;
    };
}

#endif