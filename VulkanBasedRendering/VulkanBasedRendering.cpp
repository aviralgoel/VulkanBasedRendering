
#include "vbr_application.hpp"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {

    vbr::VbrApplication app{}; // on the stack

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}