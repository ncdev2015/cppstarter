class ICommand {
public:	
	virtual ~ICommand();
	virtual int execute(int argc, char* argv[]) = 0;	
};