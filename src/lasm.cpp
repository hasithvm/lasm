#include "lasm.hpp"	

	
	
int main(int argc, char **argv)
	{

	/* opt::options_description desc("Options"); 
    	desc.add_options() 
      ("help", "Prints this message") 
      ("version,v", "Show version and copyright information") 
      ("like", "this"); 
 
    opt::variables_map vm; 
    try 
    { 
      opt::store(opt::parse_command_line(argc, argv, desc),  
                vm); // can throw 
 
      /** --help option 
       
      if ( vm.count("help")  ) 
      { 
        std::cout << "Basic Command Line Parameter App" << std::endl 
                  << desc << std::endl; 
        return 0; 
      } 
 
      opt::notify(vm); // throws on error, so do after help in case 
                      // there are any problems 
    } 
    catch(opt::error& e) 
    { 
      std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
      std::cerr << desc << std::endl; 
      return -1; 
    } 
 
    // application code here // 
 

*/

	cout << "Libra-8086 Emulator -- Assembler\n" << "Built on " << __DATE__  << " : " << __TIME__  << endl;
	setExpressionList(&list);




		
	  streambuf *psbuf, *backup;
		ofstream toFile;
		toFile.open("log.txt");
		backup = clog.rdbuf();
		psbuf = toFile.rdbuf();
		clog.rdbuf(psbuf);

		list.reserve(80);
		FILE *myfile = fopen(argv[1], "r");
		if (myfile)
			strOutputFile = string(argv[2]);
		yyin = myfile;
	
	if (!myfile) {
		cout << "No inputfile specified\nSwitching to interactive mode...." << endl;
	yyin = stdin;
	strOutputFile = "a.obj";
		cout << "============\nInteractive Mode"<< endl;
	}
		
		yyparse();







	preprocess(list);

for (int i = 0; i< list.size();i++)
{
	list[i]->repr(0);
}
clog << "assembly started!" << endl;
	p86Assembler asmgen;
	
	int errs;
	errs = asmgen.parse(list);
	if (errs > 0)
		cerr << errs << " errors encountered during assembly!" << endl;
	else{
		writeFile(asmgen.getSegments(), strOutputFile, asmgen.getStartingAddress());
		cout << "Output file " << strOutputFile << " created" << endl;
	}

for (int i = 0; i< list.size();i++)
{
	delete list[i];
}

		clog.rdbuf(backup);  
		toFile.close();
		return 0;
	}
