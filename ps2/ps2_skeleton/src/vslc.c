#include <stdio.h>
#include <stdlib.h>
#include <vslc.h>

node_t *root;

int
main ( int argc, char **argv )
{
    yyparse();
    // int token;
    // while ((token = yylex()) != 0)
    // {
    // 	if (token <= 255)
    // 		printf("Received token %c\n", token);
    // 	else 
    // 		printf("Received token %d\n", token);
    // 	// lexeme_print(token);
    // }
    node_print ( root, 0 );
    destroy_subtree ( root );
}
