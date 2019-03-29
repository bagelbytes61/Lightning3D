#ifndef LG_GRAPHICS_H_
#define LG_GRAPHICS_H_

#pragma once

#include "LGClearFlag.h"

namespace Lightning3D::Graphics {

    class Graphics {
    public:
        virtual ~Graphics() = 0;
        
        //virtual void ClearRenderTarget(RenderTarget* target) = 0;

        //virtual void ClearDepthStencil()

        virtual void Present() = 0;
    };
}

#endif