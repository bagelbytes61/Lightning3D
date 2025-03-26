#ifndef LIGHTING3D_SPRITE_FACTORY_HH_
#define LIGHTING3D_SPRITE_FACTORY_HH_

#include <memory>
#include <type_traits>

#include <d3d12.h>

#include <wrl/client.h>

using namespace Microsoft::WRL;

namespace Lightning3D {
    //class SpriteFactory {
    //public:
    //    SpriteFactory(ComPtr<ID3D12Device> device)
    //        : m_device(device) {

    //    }

    //    template<typename T, typename ...Args>
    //    auto Create(Args &&...args) -> std::enable_if_t<std::is_base_of_v<Sprite, T>, std::unique_ptr<T>> {
    //        return std::make_unique<T>(std::forward(args)...);
    //    }

    //private:
    //    ComPtr<ID3D12Device> m_device;
    //};
}

#endif