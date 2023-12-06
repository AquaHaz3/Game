#include "GameLauncher.h"
#include <iostream>

int test();

int main()
{

    try {
        auto launcher = GameLauncher();
        launcher.Launch();
    }
    catch (std::exception error) {
        std::cout << "Fatal Error: " << error.what() << '\n';
    }

    return 0;
}


int test() {

    char* data = new char[10];
    for (int i = 0; i < 10; i++) {
        data[i] = '0' + i;
    }

    auto str = std::string(data);
    delete[] data;

    printf("%s\n", str.c_str());

    return 0;
}