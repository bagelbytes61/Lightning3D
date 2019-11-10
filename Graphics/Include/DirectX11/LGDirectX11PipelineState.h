#ifndef LG_DIRECTX11_PIPELINE_STATE_H_
#define LG_DIRECTX11_PIPELINE_STATE_H_

#pragma once

#include "LGPipelineState.h"

#include <d3d11.h>

#include <atlbase.h>

namespace Lightning3D {
    class DirectX11PipelineState : public PipelineState {
        friend class DirectX11Graphics;
    public:
        DirectX11PipelineState(ID3D11VertexShader *vertexShader, ID3D11PixelShader *pixelShader);
        virtual ~DirectX11PipelineState() override final;

    private:
        CComPtr<ID3D11VertexShader> m_vertexShader;
        CComPtr<ID3D11PixelShader> m_pixelShader;
    };
}

#endif