#include "DirectX11/LGDirectX11PipelineState.h"

using namespace Lightning3D;

DirectX11PipelineState::DirectX11PipelineState(ID3D11VertexShader *vertexShader, ID3D11PixelShader *pixelShader)
    : m_vertexShader(vertexShader)
    , m_pixelShader(pixelShader) {

}

DirectX11PipelineState::~DirectX11PipelineState() {

}