#include <stdio.h>
#include <string.h>

int main (void) {

	char str [50] = "jmp label:";
	int numColon = 0;

	numColon = (int)(strchr (str, ':') - str);
	char ch = str [strlen (str) - 1];
	//printf ("symbol: %c\n", ch);

	//if (numColon == strlen (str) - 1)
	//	printf ("symbol1: %c\n", str [numColon]);

//	printf ("strlen - 1: %d\n", strlen (str) - 1);
//	printf ("num colon: %d\n", numColon);

	char labelBuffer [50], buffer [50];
    sscanf (str, "%s %s", labelBuffer, labelBuffer);
    strncpy (buffer, labelBuffer, strlen (labelBuffer) - 1);
    printf ("%s\n", buffer);

	return 0;
}


/*















*/