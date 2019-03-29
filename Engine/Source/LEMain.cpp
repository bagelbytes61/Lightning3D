#include "LEEngine.h"
#include "LEGame.h"

#include "DirectX11/LGDirectX11IndexBuffer.h"

#include <stdexcept>

using namespace Lightning3D::Engine;
//using namespace Lightning3D::Graphics;



int main(int argc, char* argv[]) try {
        Engine::Run();

        return EXIT_SUCCESS;
    }
    catch (std::runtime_error e) {
        return EXIT_FAILURE;
}