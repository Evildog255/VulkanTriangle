#include "Window.h"

int main() {
    std::unique_ptr<Runtime> app = std::make_unique<Runtime>();
    try {
        app->run();
    }
    catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
    return 0;
}