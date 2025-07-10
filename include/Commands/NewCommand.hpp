#include <iostream>

class NewCommand : public Command {
public:
	NewCommand();
	~NewCommand();
	
	int execute(int argc, char* argv[]) override {
		if (argc < 3) {
			std::cerr << "Error: 'new' requires the project name.\n";
			return 1;
		}

		std::string projectName = argv[2];
		bool initGit = (arc > 3 && std::string(argv[3]) == "--init-git");

		// TODO: Logic to create a new project...
		return 0;
	}
};