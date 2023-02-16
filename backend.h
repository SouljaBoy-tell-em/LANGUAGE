unsigned int addingNewLabel (Node * node, FILE * commands) {

	static int lastLabel     =         1;
	       int saveLastLabel = lastLabel;

	fprintf (commands, "start%d", lastLabel);
	lastLabel++;

	return saveLastLabel;
}


void convertTree (Node * node, FILE * commands) {

	if (!node)
		return;

	if (node->left && node->left->type != CLOSE)
		convertTree (node->left, commands);
	
	conditions (node, commands);	

	if (node->right && node->right->type != CLOSE)
		convertTree (node->right, commands);
}


void conditions (Node * node, FILE * commands) {

	if (node->type == CONDITION && node->code == IF) {

		EMBEDIF()
	}

	else if (node->type == CONDITION && node->code == WHILE) {

		EMBEDWHILE()
	}

	else if (node->type == FUNCTION && node->code == PRINT) {

		EMBEDPRINT()
	}


	else if (node->type == FUNCTION && node->code == SQRT) {

		EMBEDFUNCTIONSQRT()
	}

	else if (node->type == VARIABLE && node->parent->type == OPERATION && node->parent->code == '=') {

		EMBEDINITIALIZEVARIABLE()
	}
}


void determineJmp (Node * node, FILE * commands) {

	if      (node->code ==  JA) fprintf (commands, "ja " );
	else if (node->code ==  JB) fprintf (commands, "jb " );
	else if (node->code ==  JE) fprintf (commands, "je " );
	else if (node->code == JAE) fprintf (commands, "jae ");
	else if (node->code == JBE) fprintf (commands, "jbe ");
	else if (node->code == JAE) fprintf (commands, "jne ");
}


void rightPartOfInitializeVariable (Node * node, FILE * commands) {

	if (!node)
		return;

	if (node->left && node->left->type   != CLOSE)
		rightPartOfInitializeVariable (node->left,  commands);

	if (node->right && node->right->type != CLOSE)
		rightPartOfInitializeVariable (node->right, commands);

	if (node->type == VARIABLE && node->parent->type == OPERATION && node->parent->code != '=') {

		fprintf (commands, "push r%cx\n", node->code);
		fprintf (commands, "copy\n");
		fprintf (commands, "pop r%cx\n",  node->code);

		node->type = CLOSE;
	}

	else if (node->type == VARIABLE && node->parent->type == OPERATION && node->parent->code == '=' && node->parent->right != node) {

		rightPartOfInitializeVariable (node->parent->right, commands);
		fprintf (commands, "pop r%cx\n", node->code);

		node->type = CLOSE;
	}

	else if (node->type == VARIABLE && node->parent->type == OPERATION && node->parent->code == '=' && node->parent->right == node) {

		fprintf (commands, "push r%cx\n", node->code);
		fprintf (commands, "copy\n"                 );
		fprintf (commands, "pop r%cx\n",  node->code);

		node->type = CLOSE;
	}

	else if (node->type == VARIABLE && node->parent->type == FUNCTION) {

		fprintf (commands, "push r%cx\n", node->code);
		fprintf (commands, "copy\n"                 );
		fprintf (commands, "pop r%cx\n",  node->code);

		node->type = CLOSE;
	}

	else if (node->type == FUNCTION && (node->parent->type == OPERATION || node->parent->type == FUNCTION) && node->code == SQRT) { 
													// при добавлении функций поменять условие в том месте, где if;

		fprintf (commands, "%s\n", node->name);

		node->type = CLOSE;
	}

	else if (node->type == NUMBER) {

		fprintf (commands, "push %d\n", node->code);

		node->type = CLOSE;
	}


	else if (node->type == OPERATION) {

		switch (node->code) {

			case ADD:
				fprintf (commands, "add\n");
				break;

			case SUB:
				fprintf (commands, "sub\n");
				break;

			case MUL:
				fprintf (commands, "mul\n");
				break;

			case DIV:
				fprintf (commands, "div\n");
				break;

			case DEG:
				fprintf (commands, "deg\n");
				break;

			default:
				break;
		}

		node->type = CLOSE;
	}

	else if (node->type == CONDITION && node->code == IF) {

		EMBEDIF()
	}

	else if (node->type == CONDITION && node->code == WHILE) {

		EMBEDWHILE()
	}

	else if (node->type == FUNCTION && node->code == PRINT) {

		EMBEDPRINT()

		node->type = CLOSE;
	}
}