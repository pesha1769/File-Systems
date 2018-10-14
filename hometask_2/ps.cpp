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
           std::cerr << "Cannot read proc Name:" << error.what() << '\n';
        }
    }

    Process() {}

    int Print_pid(){
        return this->Pid;
    }

    std::string Print_name(){
        return this->Name;
    }



private:
    int Pid;
    std::string Name;
};




class View {
public:
	View(Process &process) {
            this->process = process;
        }

        View() {}
  
        void print_pid_name() {
            std::cout << process.Print_pid() << "\t\t\t" << process.Print_name() << std::endl;
        }

    private:
        Process process;
};


int main() {

    std::vector<Process> processes;

    DIR* dir = opendir ("/proc");
    if (!dir) {
        std::cerr << "Cannot open /proc \n";
        exit(1);
    }

    struct dirent* ent;

    while ((ent = readdir (dir)) != nullptr) {
	std::string pid;
        pid = ent->d_name;

        try {
            int pid_int = std::stoi(pid);
            processes.emplace_back(pid_int);
        }
        catch (const  std::invalid_argument& err){
	    std::cerr << "Invalid argument: " << err.what() << '\n';	
        }
        
    }
    closedir (dir);

    for (auto& proc : processes)
        View(proc).print_pid_name();

    return 0;
}
