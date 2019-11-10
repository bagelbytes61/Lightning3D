#include "DirectX11/LGDirectX11RenderTarget.h"

using namespace Lightning3D;

DirectX11RenderTarget::DirectX11RenderTarget(ID3D11RenderTargetView *resource)
    : m_resource(resource) {

}

