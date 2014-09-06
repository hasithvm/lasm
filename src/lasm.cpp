//
//
//
//


#include "lasm.hpp"	

int main(int argc, char **argv)
{


	cout << "Libra-8086 Emulator -- Assembler\n" << "assembler built on " << __DATE__  << " : " << __TIME__  << endl;
	setExpressionList(&list);

	streambuf *psbuf, *backup;
	ofstream toFile;
	int argIdx = 1;
	if (argc == 4) {
		if (!strncmp(argv[argIdx], "-d", 3)) {
			toFile.open("debug.log");
		}
		argIdx++;
		argc--;
	}
	backup = clog.rdbuf();
	psbuf = toFile.rdbuf();
	clog.rdbuf(psbuf);

	list.reserve(80);
	if (argc == 3){
		FILE *myfile = fopen(argv[argIdx], "r");


		yyin = myfile;
		strOutputFile = string(argv[argIdx + 1]);



		if (!myfile) {
			cout << "Invalid inputfile specified\nSwitching to interactive mode...." << endl;
			yyin = stdin;
			strOutputFile = "a.obj";
			cout << "============\nInteractive Mode"<< endl;
		}

		yyparse();
		if (err_count())
		{
			fclose(myfile);
			cout << "ERROR: Syntax errors detected. lasm will now exit." << endl;
			return 1;
		}




		preprocess(list);

		for (unsigned int i = 0; i< list.size();i++)
		{
			list[i]->repr(0);
		}
		clog << "assembly started!" << endl;
		p86Assembler asmgen;

		unsigned int err_count;
		err_count = asmgen.parse(list);
		if (err_count > 0)
			cerr << err_count << " error(s) encountered during assembly!" << endl;
		else{
			VirgoWriter::writeFile(asmgen.getSegments(), strOutputFile, asmgen.getStartingAddress());
			cout << "Output file " << strOutputFile << " created" << endl;
		}

		strOutputFile += ".lst";
		if (myfile)
			ListingWriter::writeFile(asmgen.getSegments(), strOutputFile, string(argv[1]));


		for (unsigned int i = 0; i< list.size();i++)
		{
			delete list[i];
		}

		clog.rdbuf(backup);  
		toFile.close();
		return 0;


	}
	else{

		cout << "Usage: lasm [-d] infile outfile" << endl;
		clog.rdbuf(backup);
		toFile.close();
		return 0;
	}
}
