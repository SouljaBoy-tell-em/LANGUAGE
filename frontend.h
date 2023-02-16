//ROOLS:


Node * STARTLANG (Node ** head, int * index, Node * node) {

	Node * mainNode = LINKFUNCTIONS (head, index, node);

	return mainNode;
}


Node * LINKFUNCTIONS (Node ** head, int * index, Node * node) {

	Node * save = NULL;

	while (true) {

		if (node[ * index].type == CONDITION && node[ * index].code == OUT)
			break;

		Node * statement = constructor ();
		statement->type = STATEMENT;
		strcpy (statement->name, "statement");
		statement->left = save;

		* head = NULL;
		statement->right = LINK (head, index, node);
		save = statement;
		( * index)+=2;
	}

	return save;
}


Node * LINK (Node ** head, int * index, Node * node) {

	* head = ADDANDSUB (head, index, node);
	Node * save = * head;

	while (true) {

		if (node[ * index].type == CONDITION && node[ * index].code == END)
			break;

		printf ("\n\nFUNC LINK: %d\n\n", node[ * index].code);
		Node * statement = constructor ();
		statement->type = STATEMENT;
		strcpy (statement->name, "statement");
		statement->left = save;
		* head = NULL;
		( * index)++;

		if (node[ * index].type == CONDITION && node[ * index].code == END)
			break;

		statement->right = ADDANDSUB (head, index, node);
		save = statement;
	}

	printf ("\n\nnext element: %d\n", node[( * index) + 2].code);
	return save;
}


Node * ADDANDSUB (Node ** head, int * index, Node * node) {

	printf ("FUNCTION: %s\n", __FUNCTION__);

	if (!( * head))
		* head = MULANDDIV (head, index, node);

	while (node[ * index].type == OPERATION && node[ * index].code == '+') {

		Node * currentNode = constructor ();

		currentNode = * head;
		* head = &node[ * index];
		( * head)->left = currentNode;
		( * index)++;
		( * head)->right = MULANDDIV (&(( * head)->right), index, node);
	}

	while (node[ * index].type == OPERATION && node[ * index].code == '-') {

		Node * currentNode = constructor ();

		currentNode = * head;
		* head = &node[ * index];
		( * head)->right = currentNode;
		( * index)++;
		( * head)->left = MULANDDIV (&(( * head)->left), index, node);
	}

	return * head;
}


Node * MULANDDIV (Node ** head, int * index, Node * node) {

	printf ("FUNCTION: %s\n", __FUNCTION__);

	if (!( * head))
		( * head) = POW (head, index, node);

	while (node[ * index].type == OPERATION && node[ * index].code == '*') {

		Node * currentNode = (Node * ) malloc (sizeof (Node));
		currentNode = * head;
		* head = &node[ * index];

		if (!( * head)->left)
			( * head)->left = currentNode;

		( * index)++;

		if (!( * head)->right)
			( * head)->right = POW (&(( * head)->right), index, node);
	}

	while (node[ * index].type == OPERATION && node[ * index].code == '/') { 

											// разделил общий цикл умножения и деления на части

		Node * currentNode = (Node * ) malloc (sizeof (Node));
		currentNode = * head;
		* head = &node[ * index];

		if (!( * head)->right)
			( * head)->right = currentNode;

		( * index)++;

		if (!( * head)->left)
			( * head)->left = POW (&(( * head)->left), index, node);
	}

	return * head;
}


Node * POW (Node ** head, int * index, Node * node) {

	printf ("FUNCTION: %s\n", __FUNCTION__);

	if (!( * head))
		( * head) = BRACKETSANDNUMBERANDVARIABLEANDFUNCTIONANDCONDITION (head, index, node);

	while (node[ * index].type == OPERATION && node[ * index].code == '^') {

		Node * currentNode = (Node * ) malloc (sizeof (Node));
		currentNode = * head;
		* head = &node[ * index];

		if (!( * head)->left)
			( * head)->left = currentNode;

		( * index)++;

		if (!( * head)->right)
			( * head)->right = BRACKETSANDNUMBERANDVARIABLEANDFUNCTIONANDCONDITION (&(( * head)->right), index, node);
	}

	return * head;
}


Node * BRACKETSANDNUMBERANDVARIABLEANDFUNCTIONANDCONDITION (Node ** head, int * index, Node * node) {

	printf ("FUNCTION: %s\n", __FUNCTION__);

	if (node[ * index].type == NUMBER) {

		if (!( * head))
			* head = GETNUMBER (head, index, node);

		( * index)++;

		return * head;
	}

	else if (node[ * index].type == VARIABLE && node[( * index) + 1].type == OPERATION && node[( * index) + 1].code == '=') {

		if (!( * head))
			* head = INITVARIABLE (head, index, node);

		return * head;
	}

	else if (node[ * index].type == VARIABLE) {

		if (!( * head))
			* head = &node[ * index];

		( * index)++;

		return * head;
	}

	else if (node[ * index].type == CIRCBRACKETS && node[ * index].code == '(') {

		( * index)++;

		if (!( * head))
			* head = ADDANDSUB (head, index, node);

		if (node[ * index].type == CIRCBRACKETS && node[ * index].code == ')') {

			( * index)++;

			return * head;
		}
	}

	else if (node[ * index].type == CONDITION) {

		if (!( * head))
			* head = CONDITIONS (head, index, node);

		return * head;
	}

	else if (node[ * index].type == FUNCTION && node[( * index) + 1].type == NAMEFUNCTION) {

		if (!( * head))
			* head = BODYFUNCTIONS (head, index, node);

		return * head;
	}

	else if (node[ * index].type == FUNCTION) {

		if (!( * head))
			* head = FUNCTIONS (head, index, node);

		return * head;
	}

	else {

		printf ("ERROR\n");
		exit (EXIT_FAILURE);
	}

	return * head;
}


Node * GETNUMBER (Node ** head, int * index, Node * node) {

	printf ("FUNCTION: %s\n", __FUNCTION__);

	if (!( * head))
		( * head) = &node[ * index];

	return &node[ * index];
}



Node * INITVARIABLE (Node ** head, int * index, Node * node) {

	printf ("FUNCTION: %s\n", __FUNCTION__);

	* head = &node[( * index) + 1];
	( * head)->left = &node[ * index];
	( * index) += 2;
	( * head)->right = ADDANDSUB (&(( * head)->right), index, node);

	return * head;
}


Node * CONDITIONS (Node ** head, int * index, Node * node) {

	printf ("FUNCTION: %s\n", __FUNCTION__);

	if (node[ * index].code == IF || node[ * index].code == WHILE) {

		* head = &node[ * index];
		( * index)++;

		Node * currentNode = NULL;
		currentNode = ADDANDSUB (&currentNode, index, node);
		
		( * head)->left = &node[ * index];
		(( * head)->left)->left = currentNode;
		( * index)++;
		(( * head)->left)->right = ADDANDSUB (&((( * head)->left)->right), index, node);
		( * index)++;
		( * head)->right = LINK (&( * head)->right, index, node);
		( * index)++;
	}

	else if (node[ * index].code == CALL && node[( * index) + 1].type == NAMEFUNCTION) {

		* head = &node[ * index];
		( * index)++;
		( * head)->left = &node[ * index];
		( * index)+=2;

		if (node[ * index].type == VARIABLE)
			( * head)->right = GETPARAMS (&(( * head)->right), index, node);

		else if (node[ * index].type == CIRCBRACKETS && node[ * index].code == ')')
			( * index)++;
	}

	else if (node[ * index].code == RETURN) {

		* head = &node[ * index];
		( * index)++;
		( * head)->left = &node[ * index];
		( * index)++;

		printf ("CURRENT NODE: %d\n", node[ * index].code);
	}

	return * head;
}


Node * FUNCTIONS (Node ** head, int * index, Node * node) {

	printf ("FUNCTION: %s\n", __FUNCTION__);

	if (node[ * index].code == PRINT || node[ * index].code == SQRT) {
		
		* head = &node[ * index];
		( * index)+=2;
		( * head)->right = ADDANDSUB (&( * head)->right, index, node);
		( * index)++;
	}

	return * head;
}


Node * BODYFUNCTIONS (Node ** head, int * index, Node * node) {

	printf ("FUNCTION: %s\n", __FUNCTION__);
	printf ("NAME FUNCTION IN: %s\n", node[( * index) + 1].name);

	* head = &node[ * index];
	( * index)++;
	( * head)->left = constructor ();
	(( * head)->left)->type = FUNCTION;
	strcpy ((( * head)->left)->name, "function");
	(( * head)->left)->left = &node[ * index];
	( * index)+=2;

	if (node[ * index].type == VARIABLE) {

		(( * head)->left)->right = GETPARAMS (&((( * head)->left)->right), index, node);
		( * index)++;
		( * head)->right = LINK (&( * head)->right, index, node);
	}

	else if (node[ * index].type == CIRCBRACKETS && node[ * index].code == ')') {

		( * index)+=2;
		( * head)->right = LINK (&( * head)->right, index, node);
	}

	return * head;
}


Node * GETPARAMS (Node ** head, int * index, Node * node) {

	Node * save = NULL;

	while (true) {

		Node * param = constructor ();
		param->type = PARAM;
		strcpy (param->name, "param");
		param->left  = save;
		param->right = &node[ * index];
		( * index)+=2;
		save = param;

		if (node[ * index].type == OPERATION && node[ * index].code == ';' && node[( * index) - 1].type == CIRCBRACKETS && node[( * index) - 1].code == ')')
			break;
	}

	return save;
}