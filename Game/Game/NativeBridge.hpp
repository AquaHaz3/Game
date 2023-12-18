#pragma once

#include <vector>
#include <string>

#include "model/Block.h"

struct ArgsContainer {

    ArgsContainer() {
        args = std::vector<std::string>();
    }

    std::vector<std::string> args;

};

#include <string.h>

extern "C" __declspec(dllexport) void* ExecuteInGame(const char* function, ArgsContainer* args)
{
    if (strcmp(function, "AddBlock") == 0 && args->args.size() == 1) {
        int newId = Block::lastBlock;
        Block::lastBlock += 1;
        Block::addBlock((BlockID) newId, args->args[0]);
        printf("[ExecuteInGame] called 'AddBlock'\n");
        return (void*)1;
    }
    return 0;
}