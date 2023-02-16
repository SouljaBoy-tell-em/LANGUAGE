#include "lang_header.h"
#include "lang.h"


int main (void) {

	FILE * langFile = fopen ("langFile.txt", "r");
	CHECK_ERROR(!langFile, "Problem with opening langFile.txt\n");

	FILE * graphDumpFile = fopen ("graphDumpFile.txt", "w");
	CHECK_ERROR(!graphDumpFile, "Problem with opening graphDumpFile.txt\n");

	int sizelangFile = FileSize (langFile);
	s = (char * ) malloc (sizelangFile * sizeof (char));
	CHECK_ERROR(!s, "Problem with allocating memory for s.\n");

	s = InitializeS (langFile, sizelangFile);
	CHECK_ERROR(!s, "Problem with getting info from file. Function: InitializeS ().\n");

	Node node [sizelangFile];

	int i = 0;
	for (i = 0; i < sizelangFile; i++) {

		node[i].right = NULL;
		node[i].left  = NULL;
	}

	listingTxtCode (node, sizelangFile);

	Node * tree = NULL;
	int index   =    0;

	tree = STARTLANG (&tree, &index, node);
	graphDump (tree, graphDumpFile);


	FILE * commands = fopen ("commands.txt", "w");
	CHECK_ERROR(!commands, "Problem with opening file commands.txt\n");

	convertTree (tree, commands);
	fprintf (commands, "hlt\n");
	
	return 0;
}