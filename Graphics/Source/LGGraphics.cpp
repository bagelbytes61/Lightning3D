#include "LGGraphics.h"

using namespace Lightning3D;

#ifdef LG_DIRECTX11
#include "DirectX11/LGDirectX11Graphics.h"
using GraphicsImplementation = DirectX11Graphics;
#endif

std::unique_ptr<Graphics> Graphics::Create(Handle winHandle, uint32_t width, uint32_t height, bool windowed) {
    return std::make_unique<GraphicsImplementation>(winHandle, width, height, windowed);
}

