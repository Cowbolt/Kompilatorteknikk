#include <vslc.h>


void
node_print ( node_t *root, int nesting )
{
    if ( root != NULL )
    {
        /* Print the type of node indented by the nesting level */
        printf ( "%*c%s", nesting, ' ', node_string[root->type] );

        /* For identifiers, strings, expressions and numbers,
         * print the data element also
         */
        if ( root->type == IDENTIFIER_DATA ||
             root->type == STRING_DATA ||
             root->type == EXPRESSION  ||
             root->type == RELATION) 
            printf ( "(%s)", (char *) root->data );
        else if ( root->type == NUMBER_DATA )
            printf ( "(%ld)", *((int64_t *)root->data) );

        /* Make a new line, and traverse the node's children in the same manner */
        putchar ( '\n' );
        for ( int64_t i=0; i<root->n_children; i++ )
            node_print ( root->children[i], nesting+1 );
    }
    else
        printf ( "%*c%p\n", nesting, ' ', root );
}

void
lexeme_print ( int lexeme )
{
    /* Print the type of node indented by the nesting level */
    if (lexeme <= 255)
        printf("TOKEN: %s\n", lexeme);
    else 
        printf ( "TOKEN: %d\n", node_string[lexeme] );
}


/* Take the memory allocated to a node and fill it in with the given elements */
void
node_init (node_t *nd, node_index_t type, void *data, uint64_t n_children, ...)
{
    nd->type = type;
    nd->data = data;
    nd->n_children = n_children;
    node_t **children = malloc(n_children*sizeof(nd));
    memset(children, 0, sizeof(children));

    va_list valist;
    node_t *child;
    va_start(valist, n_children);
    // Set each of the child pointers
    for (int i = 0; i < n_children; i++)
    {
        children[i] = va_arg(valist, node_t *);
    }

    va_end(valist);
    nd->children = children;
}


/* Remove a node and its contents */
void
node_finalize ( node_t *discard )
{
    free(discard->children);
    free(discard->data);
    free(discard);
}


/* Recursively remove the entire tree rooted at a node */
void
destroy_subtree ( node_t *discard )
{
    printf("Entering %s\n",node_string[discard->type]);
    for (int i= 0; i < discard->n_children; i++)
    {
        destroy_subtree(discard->children[i]);
    }   
    node_finalize(discard);
}
