#ifndef LG_GRAPHICS_H_
#define LG_GRAPHICS_H_

#pragma once

#include "LGClear.h"
#include "LGTopology.h"
#include "LGUsage.h"

#include <memory>

namespace Lightning3D {
    class DepthStencil;
    class IndexBuffer;
    class PipelineState;
    class RenderTarget;
    class Texture1D;
    class Texture2D;
    class Texture3D;
    class UniformBuffer;
    class VertexBuffer;

    struct PipelineStateDescription;

    using Handle = void *;

    class Graphics {
    public:
        static std::unique_ptr<Graphics> Create(Handle winHandle, uint32_t width, uint32_t height, bool windowed);

        virtual ~Graphics() = default;

        virtual VertexBuffer* CreateVertexBuffer(uint32_t size, uint32_t count, Usage usage, void* initialData) = 0;
        virtual IndexBuffer*  CreateIndexBuffer(uint32_t size, uint32_t count, Usage usage, void* initialData) = 0;
        virtual UniformBuffer* CreateUniformBuffer(uint32_t size, Usage usage, void* initialData) = 0;

        virtual Texture1D* CreateTexture1D(uint32_t width, Usage usage) = 0;
        virtual Texture2D* CreateTexture2D(uint32_t width, uint32_t height, Usage usage) = 0;
        virtual Texture3D* CreateTexture3D(uint32_t width, uint32_t height, uint32_t depth, Usage usage) = 0;
        
        virtual DepthStencil* CreateDepthStencil(Texture2D* texture) = 0;
        virtual RenderTarget* CreateRenderTarget(Texture2D* texture) = 0;

        virtual PipelineState *CreatePipelineState(PipelineStateDescription &&description) = 0;

        virtual void ClearDepthStencil(DepthStencil *stencil) = 0;
        virtual void ClearRenderTarget(RenderTarget *target) = 0;

        virtual void Draw(Topology topology, uint32_t startVertex, uint32_t vertexCount) = 0;
        virtual void Draw(Topology topology, uint32_t baseVertex, uint32_t startIndex, uint32_t indexCount) = 0;

        virtual void Present() = 0;
    };
}

#endif