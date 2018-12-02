#include "Application.h"
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include "FileSystemFAT16.h"

void Application::run() {
    Help();

    do {
        auto command = readCommand();

        if (command.empty())
            continue;

        else if (command[0] == "help")
            Help();

        else if (command[0] == "exit") {
            Close();
            break;
        }
        else if ((command[0] == "open") && command.size() == 2) {
            fs = new FAT16FileSystem();

            try{
                fs->openImage(command[1]);
            }
            catch (std::runtime_error& error) {
                std::cout << error.what() << std::endl;
                Close();
            }
        }
        else if (command[0] == "ls") {
            if(fs)
                fs->listDir();
        }
        else if (command[0] == "cat" && command.size() == 2) {
            if(fs)
                fs->catFile(command[1]);
        }
    }
    while (true);
}

std::vector<std::string> Application::readCommand() {
    std::cout << ">";
    std::string command;
    std::getline(std::cin, command);
    std::istringstream iss(command);
    std::vector<std::string> commands{std::istream_iterator<std::string>{iss},
                                      std::istream_iterator<std::string>{}};
    return commands;
}

void Application::Close() {
    if (fs != nullptr)
        delete fs;
}


void Application::Help() {
    std::cout << "FileSystem reader"                       << std::endl;
    std::cout <<                                              std::endl;
    std::cout << "Usage: "                                 << std::endl;  
    std::cout << "open <fName>  - Open image of FAT16 "    << std::endl;
    std::cout << "ls            - List current directory"  << std::endl;    
    std::cout << "help          - Show help page"          << std::endl;
    std::cout << "exit          - Close program"           << std::endl;

}
