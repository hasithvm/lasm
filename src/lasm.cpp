#include "lasm.hpp"	



int main(int argc, char **argv)
{


	cout << "Libra-8086 Emulator -- Assembler\n" << "assembler built on " << __DATE__  << " : " << __TIME__  << endl;
	setExpressionList(&list);





	streambuf *psbuf, *backup;
	ofstream toFile;
	toFile.open("debug.log");
	backup = clog.rdbuf();
	psbuf = toFile.rdbuf();
	clog.rdbuf(psbuf);

	list.reserve(80);
	if (argc == 3){
		FILE *myfile = fopen(argv[1], "r");


		yyin = myfile;
		strOutputFile = string(argv[2]);



		if (!myfile) {
			cout << "Invalid inputfile specified\nSwitching to interactive mode...." << endl;
			yyin = stdin;
			strOutputFile = "a.obj";
			cout << "============\nInteractive Mode"<< endl;
		}

		yyparse();
		if (err_count())
		{
			cout << "ERROR: Syntax errors detected. lasm will now exit." << endl;
			return 1;
		}




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
			cerr << errs << " error(s) encountered during assembly!" << endl;
		else{
			VirgoWriter::writeFile(asmgen.getSegments(), strOutputFile, asmgen.getStartingAddress());
			cout << "Output file " << strOutputFile << " created" << endl;
		}

		strOutputFile += ".lst";
		if (myfile)
			ListingWriter::writeFile(asmgen.getSegments(), strOutputFile, string(argv[1]));


		for (int i = 0; i< list.size();i++)
		{
			delete list[i];
		}

		clog.rdbuf(backup);  
		toFile.close();
		return 0;


	}
	else{

		cout << "Usage: lasm [infile] [outfile]" << endl;
		clog.rdbuf(backup);
		toFile.close();
		return 0;
	}
}
