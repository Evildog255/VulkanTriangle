#include "Window.h"

int main() {
    try {
        std::unique_ptr<Runtime> app = std::make_unique<Runtime>(800, 600);
    }
    catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
    return 0;
}