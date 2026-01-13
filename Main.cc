#include "Camera.hh"
#include "D3D12Renderer.hh"
#include "Sprite.hh"
#include "SpriteFactory.hh"
#include "WindowsApp.hh"
#include "WindowsWindow.hh"
#include "Matrix.hh"
#include "Viewport.hh"
#include "Vector.hh"
#include "InputBuffer.hh"

#include "Games/Asteroids3D/Asteroids3D.hh"
#include "Games/Filler/Filler.hh"

#include <chrono>
#include <iostream>

//auto GenerateRootSignature(ID3D12Device *device) -> ComPtr<ID3D12RootSignature> {
//    HRESULT hr;
//
//    //D3D12_DESCRIPTOR_RANGE descriptorRange = { };
//    //descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
//    //descriptorRange.NumDescriptors = 2u;
//    //descriptorRange.BaseShaderRegister = 0u;
//    //descriptorRange.RegisterSpace = 0u;
//    //descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
//
//    //D3D12_ROOT_DESCRIPTOR_TABLE descriptorTable = { };
//    //descriptorTable.NumDescriptorRanges = 1u;
//    //descriptorTable.pDescriptorRanges = &descriptorRange;
//
//    D3D12_ROOT_DESCRIPTOR globalConstants;
//    globalConstants.RegisterSpace = 0u;
//    globalConstants.ShaderRegister = 0u;
//
//    D3D12_ROOT_DESCRIPTOR objectConstants;
//    objectConstants.RegisterSpace = 0u;
//    objectConstants.ShaderRegister = 1u;
//
//    D3D12_ROOT_PARAMETER rootParam0 = { };
//    rootParam0.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
//    rootParam0.Descriptor = globalConstants;
//    rootParam0.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
//
//    D3D12_ROOT_PARAMETER rootParam1 = { };
//    rootParam1.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
//    rootParam1.Descriptor = objectConstants;
//    rootParam1.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
//
//
//    D3D12_ROOT_PARAMETER rootParamPS = { };
//    rootParamPS.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
//    rootParamPS.Descriptor.RegisterSpace = 0u;
//    rootParamPS.Descriptor.ShaderRegister = 1u;
//    rootParamPS.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
//
//    D3D12_ROOT_PARAMETER rootParams[] = { rootParam0, rootParam1, rootParamPS };
//
//    D3D12_ROOT_SIGNATURE_DESC desc = { };
//    desc.NumParameters = 3u;
//    desc.pParameters = rootParams;
//    desc.NumStaticSamplers = 0u;
//    desc.pStaticSamplers = nullptr;
//    desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
//
//    Microsoft::WRL::ComPtr<ID3DBlob> signature;
//    Microsoft::WRL::ComPtr<ID3DBlob> error;
//
//    hr = D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
//    if (FAILED(hr)) {
//        throw std::runtime_error("Failed to serialize Root Signature: " + std::to_string(hr));
//    }
//
//    ComPtr<ID3D12RootSignature> rootSignature;
//
//    hr = device->CreateRootSignature(0U, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
//    if (FAILED(hr)) {
//        throw std::runtime_error("Failed to create Root Signature: " + std::to_string(hr));
//    }
//
//    return rootSignature;
//}

using namespace Lightning3D;

#include <thread>

int main(int argc, char *argv[]) {
    try {
        auto app = Lightning3D::WindowsApp();

        Lightning3D::WindowDesc window_desc = { };
        window_desc.title = "Lightning3D";
        window_desc.width = 1024;
        window_desc.height = 768;
        window_desc.fullscreen = false;

        auto window = Lightning3D::WindowsWindow(&app, window_desc);

        auto renderer = Lightning3D::D3D12Renderer(&window);

        auto game = Lightning3D::Games::Filler::Filler(renderer.GetDevice(), Vec2I(window_desc.width, window_desc.height));

        //auto game = Lightning3D::Games::Asteroids3D::Asteroids3D(renderer.GetDevice(), Vec2I(window_desc.width, window_desc.height));

        app.SetMessageHandler(&game);
            
        renderer.WaitForPreviousFrame();

        int frame_counter = 0;
        auto now = std::chrono::high_resolution_clock::now();
        auto last = now;
        while (true) {
            app.Run();
            game.Draw(renderer);
            frame_counter += 1;
            now = std::chrono::high_resolution_clock::now();
            if (now - last > std::chrono::seconds(1)) {
                std::cout << "FPS: " << frame_counter << std::endl;
                frame_counter = 0;
                last = now;
            }
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    
        return 1;
    }

    return 0;
}