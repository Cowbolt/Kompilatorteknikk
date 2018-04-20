#include <vslc.h>

// Tasks
static void node_print ( node_t *root, int nesting );
static void simplify_tree ( node_t **simplified, node_t *root );
static void simplify_subtree ( node_t *nd );
static void node_finalize ( node_t *discard );
static int is_single_node ( node_t *nd);
static int is_list_node ( node_t *nd);
// Choices
static void destroy_subtree ( node_t *discard );
static void remove_single_node ( node_t *nd, node_t *nd_parent, int index );
static node_t* simplify_list( node_t *nd );
static node_t* iterate_list( node_t *nd, int depth );
static void simplify_print_statement ( node_t *nd );

// node_index_t syntax_nodes[] = {GLOBAL, ARGUMENT_LIST, PARAMETER_LIST, PRINT_ITEM, STATEMENT};

/* External interface */
void
destroy_syntax_tree ( void )
{
    destroy_subtree ( root );
}


void
simplify_syntax_tree ( void )
{
    /** We have three cases where we need to simplify.
    /   Case 1: Single nodes, i.e. nonterminal nodes with only one child. Snip, point parent to child.
    /   Case 2: Internal nodes of list structures. H(H1,(H2,(H3,T))) should be rewritten H(H1,H2,H3,T),
    /           since H can have several children.
    /   Case 3: Constant expressions, resolve arithmetic subtrees and replace topmost node with value.
    /
    /   We begin with the rootnode, calling our function recursively. We check the type of the node. If the type
    /   corresponds to a syntactical node, we call a function which resolves the node and fixes the pointers.
    **/
    // printf("Turn up!\n");
    simplify_subtree(root);


}

void
simplify_subtree (node_t *nd)
{
    // printf("Calling simplify_subtree on node with children: %i\n",nd->n_children);
    for (int i = 0; i < nd->n_children; i++)
    {
        node_t *child = nd->children[i];
        if (child == NULL)
            continue;
        // Check to see if node is single node
        while (is_single_node(child)) 
        {
            remove_single_node(child, nd, i);
            // Update child to refer to the oedipal heir.
            child = nd->children[i];
        }

        // Check to see if node is list node.
        if (is_list_node(child))
        {
            if (child->n_children)
            {
                nd->children[i] = simplify_list(child);
                child = nd->children[i];                
            }
            if (child->type == PRINT_LIST){
                simplify_print_statement(nd);
                simplify_subtree(nd);
            }

        }

        simplify_subtree(child);
    }


}

void
simplify_print_statement(node_t *nd)
{
    node_t *backup = nd->children[0];
    nd->n_children = nd->children[0]->n_children;
    nd->children = nd->children[0]->children;
    backup->children = NULL;
    backup->n_children = 0;
    node_finalize(backup);
}

int
is_single_node (node_t *nd)
{
    switch(nd->type)
    {
        case GLOBAL: case ARGUMENT_LIST: case PARAMETER_LIST: case STATEMENT: case PRINT_ITEM:
        return 1;

        case EXPRESSION: case PRINT_LIST:
        return (nd->data == NULL && nd->n_children == 1);

        default:
        return 0;
    };
}

int
is_list_node (node_t *nd)
{
    switch(nd->type)
    {
        case GLOBAL_LIST: case STATEMENT_LIST: case PRINT_LIST: case EXPRESSION_LIST:
        case VARIABLE_LIST: case DECLARATION_LIST:
        return 1;

        default:
        return 0;
    };
}



void
remove_single_node (node_t *nd, node_t *nd_parent, int index)
{
    /** We want to tie the parent of the node we pass to its child. Well, we can easily access the child node,
    /   but what about the parent? In the simplify_subtree loop, we have access to the parent node, as well
    /   as the index of the child, so we can just pass those along.
    **/
    nd_parent->children[index] = nd->children[0];
    node_finalize(nd);
}

node_t *
simplify_list (node_t *nd)
{
    node_t *new_node = iterate_list(nd, 1);
    node_finalize(nd);
    return new_node;
    // printf("Overwriting:\n");


}

node_t*
iterate_list (node_t *nd, int depth)
{
    node_t *new_node = malloc(sizeof(*new_node));

    if (nd->n_children > 1)
    {
        new_node = iterate_list(nd->children[0],depth+1);
        new_node->children[new_node->n_children-depth] = nd->children[1];
        node_finalize(nd->children[0]); 
    }
    else
    {
        new_node->type = nd->type;
        new_node->data = NULL;
        new_node->entry = NULL;
        // should it be depth? Depth starts at 1. If it goes to 3, we want 3 children.
        new_node->n_children = depth;   
        new_node->children = malloc(new_node->n_children*sizeof(new_node));
        new_node->children[0] = nd->children[0];
    }

    
    return new_node;
}


void
print_syntax_tree ( void )
{
    node_print ( root, 0 );
}


void
node_init (node_t *nd, node_index_t type, void *data, uint64_t n_children, ...)
{
    va_list child_list;
    *nd = (node_t) {
        .type = type,
        .data = data,
        .entry = NULL,
        .n_children = n_children,
        .children = (node_t **) malloc ( n_children * sizeof(node_t *) )
    };
    va_start ( child_list, n_children );
    for ( uint64_t i=0; i<n_children; i++ )
        nd->children[i] = va_arg ( child_list, node_t * );
    va_end ( child_list );
}

/* Internal choices */

static void
node_print ( node_t *root, int nesting )
{
    if ( root != NULL )
    {
        printf ( "%*c%s", nesting, ' ', node_string[root->type] );
        if ( root->type == IDENTIFIER_DATA ||
             root->type == STRING_DATA ||
             root->type == EXPRESSION ) 
            printf ( "(%s)", (char *) root->data );
        else if ( root->type == NUMBER_DATA )
            printf ( "(%ld)", *((int64_t *)root->data) );
        putchar ( '\n' );
        for ( int64_t i=0; i<root->n_children; i++ )
            node_print ( root->children[i], nesting+1 );
    }
    else
        printf ( "%*c%p\n", nesting, ' ', root );
}


static void
node_finalize ( node_t *discard )
{
    if ( discard != NULL )
    {
        free ( discard->data );
        free ( discard->children );
        free ( discard );
    }
}


static void
destroy_subtree ( node_t *discard )
{
    if ( discard != NULL )
    {
        for ( uint64_t i=0; i<discard->n_children; i++ )
            destroy_subtree ( discard->children[i] );
        node_finalize ( discard );
    }
}
