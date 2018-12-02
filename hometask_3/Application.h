#pragma once
#include <vector>
#include "FileSystemFAT16.h"

class Application {
public:
    Application() = default;
    void run();

private:
    void Help();
    void Close();
    std::vector<std::string> readCommand();
    FAT16FileSystem* fs = nullptr;
};
