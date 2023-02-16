Node * constructor (void) {

	Node * node = (Node * ) malloc (sizeof (Node));
	node->left  = NULL;
	node->right = NULL;

	return node;
}


unsigned long FileSize (FILE * langFile) {

    struct stat buf = {};
    
    if (fstat (fileno (langFile), &buf) == 0)
        return buf.st_size;

    return 0;
}


void graphDump (Node * currentNode, FILE * graphDumpFile) {

	fprintf (graphDumpFile, "digraph G {\n");
	graphDumpDrawNode (currentNode, graphDumpFile, NULL);
	fprintf (graphDumpFile, "\n}");
}


void graphDumpDrawNode (Node * currentNode, FILE * graphDumpFile, Node * prevNode) {


	if (!currentNode)
		return;

	if (prevNode) {

		if (prevNode->left == currentNode)
			currentNode->parent = prevNode;

		if (prevNode->right == currentNode)
			currentNode->parent = prevNode;
	}


	if (currentNode->left) {

		fprintf (graphDumpFile, "node%p->node%p[color=green];\n", currentNode, currentNode->left);
		graphDumpDrawNode (currentNode->left, graphDumpFile, currentNode);
	}

	switch (currentNode->type) {

		case NUMBER:
			fprintf (graphDumpFile, "node%p[shape=record, label=\"%d\", style=filled, color=lightblue];\n", currentNode, currentNode->code);
			break; 

		case OPERATION:
			fprintf (graphDumpFile, "node%p[shape=record, label=", currentNode);

			switch (currentNode->code) {

				case ADD:
					fprintf (graphDumpFile, "\"+\", style=filled, color=lightblue];\n");
					break;

				case SUB:
					fprintf (graphDumpFile, "\"-\", style=filled, color=lightblue];\n");
					break;

				case MUL:
					fprintf (graphDumpFile, "\"*\", style=filled, color=lightblue];\n");
					break;
		
				case DIV:
					fprintf (graphDumpFile, "\"/\", style=filled, color=lightblue];\n");
					break;

				case JA:
					fprintf (graphDumpFile, "\"&gt;\", style=filled, color=lightblue];\n");
					break;			

				case JB:
					fprintf (graphDumpFile, "\"&lt;\", style=filled, color=lightblue];\n");
					break;				

				case J:
					fprintf (graphDumpFile, "\"=\", style=filled, color=lightblue];\n");
					break;

				case JE:
					fprintf (graphDumpFile, "\"==\", style=filled, color=lightblue];\n");
					break;					

				default:
					break;
			} 
			break;

		case VARIABLE:
			fprintf (graphDumpFile, "node%p[shape=record, label=\"%s\", style=filled, color=lightblue];\n", currentNode, currentNode->name);
			break;

		case STATEMENT:
			fprintf (graphDumpFile, "node%p[shape=record, label=\"%s\", style=filled, color=lightblue];\n", currentNode, currentNode->name);
			break;

		case FUNCTION:
			fprintf (graphDumpFile, "node%p[shape=record, label=\"%s\", style=filled, color=lightblue];\n", currentNode, currentNode->name);
			break;

		case NAMEFUNCTION:
			fprintf (graphDumpFile, "node%p[shape=record, label=\"%s\", style=filled, color=lightblue];\n", currentNode, currentNode->name);
			break;

		case CONDITION:
			fprintf (graphDumpFile, "node%p[shape=record, label=\"%s\", style=filled, color=lightblue];\n", currentNode, currentNode->name);
			break;

		case PARAM:
			fprintf (graphDumpFile, "node%p[shape=record, label=\"%s\", style=filled, color=lightblue];\n", currentNode, currentNode->name);
			break;

        default:
        	break;															
	}

	if (currentNode->right) {

		fprintf (graphDumpFile, "node%p->node%p[color=red];\n", currentNode, currentNode->right);
		graphDumpDrawNode (currentNode->right, graphDumpFile, currentNode);
	}
}


char * InitializeS (FILE * langFile, int sizelangFile) {

	char * textlangFile = (char * ) malloc (sizeof (char) * (sizelangFile + 1));
	CHECK_ERROR(!textlangFile, "Problem with allocating memory for textlangFile.\n");

	fread (textlangFile, sizeof (char), sizelangFile, langFile);
	textlangFile [sizelangFile] = '\n';

	return textlangFile;
}


int InitializeTree (Node * node, char * s, int sizelangFile) {

	int index = 0;
	char * start = s;

	while (s - start < sizelangFile) {

		SkippingOfSpacesSymbols (&s);

		if (isdigit ( * s)) {

			node[index].type = NUMBER;
			node[index].code =      0;

			while (true) {

				// PART-ROOT: GetN ();
				node[index].code = node[index].code * 10 + (( * s) - '0');
				
				if (!isdigit ( * (s + 1)))
					break;

				s++;
			}
		}


		else if (( * s) == '(' || ( * s) == ')') {

			node[index].type = CIRCBRACKETS;
			node[index].code =  	* s;
		}

		else if (isalpha ( * s)) {

			node[index].type    =  VARIABLE;
			node[index].name[0] =      '\0';
			node[index].code    =         0;

			while (true) {

				node[index].name[strlen (node[index].name) + 1] = '\0';
				node[index].name[strlen (node[index].name)]     =  * s;
				node[index].code                               +=  * s;

				if (!isalpha ( * (s + 1)))
					break;

				else
					s++;
			}	

			node[index].type = listMainWords (&node[index]);

			if (node[index - 1].type == FUNCTION)
				node[index].type = NAMEFUNCTION;

			else if (node[index - 1].type == CONDITION && node[index - 1].code == CALL)
				node[index].type = NAMEFUNCTION;

		}

		else {

			node[index].type = OPERATION;
			node[index].code =         0;

			if (( * s) == '=' && (( * s) + 1) == '=') {

				strcpy (node[index].name, "==");
				node[index].code = '=' + '=' + 0;
				s+=2;
			}

			while (true) {

				node[index].name[strlen (node[index].name) + 1] = '\0';
				node[index].name[strlen (node[index].name)]     =  * s;
				node[index].code                               +=  * s;

				if ( * (s + 1) != '=')
					break;

				else
					s++;
			}
		}

		index++;
		s++;
	}

	return index;
}


int listFunction (int code) {

	switch (code) {

		case SIN:
			return SIN;
			break;

		case COS:
			return COS;
			break;

		default:
			return EMPTY;
			break;

	}

	return EMPTY;
}


void listingTxtCode (Node * node, int sizelangFile) {

	int amountIndex = InitializeTree (node, s, sizelangFile);

	printf ("amounf of index: %d\n", amountIndex);

	int i = 0;
	for (i = 0; i < amountIndex; i++) {

		printf ("type %d: %d\n", i + 1, node[i].type);
		
		switch (node[i].type) {

			case NUMBER:
				printf ("number: %d\n", node[i].code);
				break;

			case OPERATION:
				printf ("operation: %c\n", node[i].code);
				break;

			case VARIABLE:
				printf ("variable: %s\n", node[i].name);
				break;

			case CIRCBRACKETS:
				printf ("cirсlebrackets: %c\n", node[i].code);
				break;

			case CONDITION:
				printf ("condition: %s\n", node[i].name);
				break;

			case FUNCTION:
				printf ("function: %s\n", node[i].name);
				break;

			case NAMEFUNCTION:
				printf ("name function: %s\n", node[i].name);
				break;

			/*case CYCLE:
				printf ("cycle: %s\n", node[i].name);
				break;
			*/

			default:
				break;
		}
	}

	printf ("\n\n\n\n\n");
}


Type listMainWords (Node * node) {

	switch (node->code) {

		case IF:
			strcpy (node->name, "if");
			return CONDITION;
			break;

		case WHILE:
			strcpy (node->name, "while");
			return CONDITION;
			break;

		case DEF:
			strcpy (node->name, "def");
			return FUNCTION;
			break;

		case START:
			strcpy (node->name, "start");
			return FUNCTION;
			break;

		case PRINT:
			strcpy (node->name, "print");
			return FUNCTION;
			break;

		case SQRT:
			strcpy (node->name, "sqrt");
			return FUNCTION;
			break;

		case RETURN:
			strcpy (node->name, "return");
			return CONDITION;
			break;

		case END:
			strcpy (node->name, "end");
			return CONDITION;
			break;

		case CALL:
			strcpy (node->name, "call");
			return CONDITION;
			break;

		case OUT:
			strcpy (node->name, "out");
			return CONDITION;
			break;

		default:
			return VARIABLE;
			break;
	}

	return EMPTY;
}


void SkippingOfSpacesSymbols (char ** s) {

	while (true) {

		if (( ** s) != ' ' && ( ** s) != '\n' && ( ** s) != '\t')
			break;

		( * s)++;
	}
}