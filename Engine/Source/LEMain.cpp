#include "LEEngine.h"
#include "LEGame.h"

#include <stdexcept>

using namespace Lightning3D;

int main(int argc, char* argv[]) try {
        Engine::Run();

        return EXIT_SUCCESS;
    }
    catch (std::runtime_error e) {
        return EXIT_FAILURE;
}