#include "Application.hpp"

#include <iostream>
#include <stdexcept>
#include <cstdlib>

int main(){

    try {
        vkt::Application app;
        app.run();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}