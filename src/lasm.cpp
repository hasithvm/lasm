//
//
//
//

#include "lasm.hpp"	
#include "Logger.hpp"
#include <sstream>
int main(int argc, char ** argv)
{
	cout << "Lasm -- assembler for the Libra-8086 Emulator" << endl;
	cout << "Compiled on " << __DATE__  << " : " << __TIME__  << endl;

	setExpressionList(&list);
	std::string sourceFile;
	std::string outFile;

	for (int i=1; i < argc - 1; i++)
	{

		if (!strncmp(argv[i], "-d", 3)) 
		{
			Logger::Instance().SetOutput(Logger::OUTPUT_FILE);
		}
		else if (!strncmp(argv[i], "-o", 3))
		{
			if ((int)i+1 >= argc)
			{
				cout << "ERROR: output filename expected" << endl;
				return 0;
			}
			outFile = argv[++i];
		}

	}
	sourceFile = argv[argc - 1];
	if (outFile.empty())
	{
		cout << "WARNING: no output file specified, defaulting to a.obj" << endl;
		outFile = "a.obj";
	}

	if (sourceFile.empty() || outFile.empty())
	{
		cout << "Something went wrong!" << endl;
		return 1;
	}



	if (argc > 2)
	{
	list.reserve(80);
	FILE *myfile = fopen(sourceFile.c_str(), "r");


		yyin = myfile;
		if (!myfile) {

			cout << "Invalid inputfile specified\
					Switching to interactive mode...." << endl;
			
			yyin = stdin;
			outFile = "a.obj";
			cout << "========================\
					Interactive Mode"<< endl;
		}

		yyparse();
		if (err_count())
		{
			fclose(myfile);
			cout << "ERROR: Syntax errors detected. lasm will now exit." << endl;
			return 1;
		}



		stringstream ss;

		preprocess(list);
		for (unsigned int i = 0; i< list.size();i++)
		{
			ss << *(list[i]);
		}


		Logger::Instance() << ss.str();
		Logger::Instance() << "assembly started!" << endl;
		p86Assembler asmgen;

		unsigned int err_count;
		err_count = asmgen.parse(list);
		if (err_count > 0)
			cerr << err_count << " error(s) encountered during assembly!" << endl;
		else{
			VirgoWriter::writeFile(asmgen.getSegments(), outFile, asmgen.getStartingAddress());
			cout << "Output file " << outFile << " created" << endl;
		}

		outFile.append(".lst");
		if (myfile)
			ListingWriter::writeFile(asmgen.getSegments(), outFile, sourceFile);


		for (unsigned int i = 0; i< list.size();i++)
		{
			delete list[i];
		}

		return 0;
	}

	
	else{

		cout << "Usage: lasm [-d] [-o output-file] input-file" << endl;
		return 0;
	}
}
