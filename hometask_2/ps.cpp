#include <iostream>
#include <dirent.h>
#include <fstream>
#include <vector>
#include <stdexcept>

class Process {

public:

    explicit Process(int pid) :
            Pid(pid)

    {
        std::string fPath = "/proc/" + std::to_string(pid) + "/comm";

	try{
        	std::ifstream file(fPath);
            	file >> Name;
            	file.close();
        }
        catch ( const std::ifstream::failure& error){
           throw error;
        }
    }

    void print_pid_name(){
	std::cout << Pid << "\t\t\t" << Name << std::endl; 
	}

private:
    int Pid;
    std::string Name;
};



int main() {

    std::vector<Process> processes;

    DIR* dir = opendir ("/proc");
    if (!dir) {
        std::cerr << "Cannot open /proc \n";
        exit(1);
    }

    struct dirent* ent;
    std::string pid;

    while ((ent = readdir (dir)) != nullptr) {
        pid = ent->d_name;

        try {
            int pid_int = std::stoi(pid);
            processes.emplace_back(Process(pid_int));
        }
        catch (std::invalid_argument& err)
	{}
        
    }
    closedir (dir);

    for (auto& proc : processes)
        proc.print_pid_name();

    return 0;
}
