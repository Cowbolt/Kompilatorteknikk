#include <vslc.h>
#define BUCKETNO 3
#define STRINGINC 4

static int fun_seq = 0;
static int local_seq;
// Externally visible, for the generator
extern tlhash_t *global_names;
extern char **string_list;
extern size_t n_string_list, stringc;

/* External interface */

void
create_symbol_table ( void )
{
  find_globals();
  size_t n_globals = tlhash_size ( global_names );
  symbol_t *global_list[n_globals];
  // Takes values from global_names and places them in global list
  tlhash_values ( global_names, (void **)&global_list );
  
  // Call bind_names on every global that is of type SYM_FUNCTION
  for ( size_t i=0; i<n_globals; i++ )
      if ( global_list[i]->type == SYM_FUNCTION )
          bind_names ( global_list[i], global_list[i]->node );
  
}


void
print_symbol_table ( void )
{
       print_symbols ();
       print_bindings(root);
}


void
destroy_symbol_table ( void )
{
      destroy_symtab(global_names);
}

static void
print_globals ( void )
{
    size_t n_globals = tlhash_size(global_names);
    symbol_t *global_list[n_globals];
    tlhash_values ( global_names, (void **)&global_list );
    for ( size_t g=0; g<n_globals; g++ )
        fprintf ( stderr, "global: %s\n", global_list[g]->name );
}


void
find_globals ( void )
{
  // Iterate over all globals (grandchildren of root node, handle either as functions or global variables)
  global_names = malloc(sizeof(tlhash_t*));
  tlhash_init(global_names, BUCKETNO); 
 
  node_t *global_list = root->children[0];

  for (int i = 0; i < global_list->n_children; i++)
  {
    if (global_list->children[i]->type == DECLARATION)
      // Pass variable list (child of declaration)
      treat_global_variables(global_list->children[i]->children[0]);
    else
      treat_function(global_list->children[i]);
  }
}

void
treat_global_variables ( node_t *node )
{
  for (int i = 0; i < node->n_children; i++)
  {
    symbol_t *s = create_symbol(node->children[i]->data, SYM_GLOBAL_VAR, NULL, 0, 0, NULL);
    tlhash_insert(global_names, node->children[i]->data, sizeof(void*), s);
  }
}

symbol_t *
create_symbol ( char *name, symtype_t type, node_t *node, size_t seq,
  size_t nparms, tlhash_t *locals )
{
  symbol_t *s = malloc(sizeof(symbol_t));
  s->name = name;
  s->type = type;
  s->node = node;
  s->seq = seq;
  s->nparms = nparms;
  s->locals = locals;
  return s;
}


void
treat_variable ( tlhash_t *hash_table, node_t *node )
{

}

/** We want:
*   name (identifier data)
*   enumeration
*   root node (function node)
*   sequencing number
*   parameter count
*   hash table of local names (fill it with parameter names)
*/
void
treat_function ( node_t *node )
{
  // Function name is data of first child (identifier data)
  // Parameter count is n_children of second child (variable list)

  // Allocate memory and instantiate parameter hash table
  tlhash_t *param_names = malloc(sizeof(tlhash_t));
  tlhash_init(param_names, BUCKETNO);

  // If the function has an existing variable list
  if (node->children[1] != NULL)
  {
    node_t *variable_list = node->children[1];
    int par_seq = 0;

    // Create a symbol for each param and insert it into param hashtable
    for (int i = 0; i < variable_list->n_children; i++)
    {
      symbol_t *param = create_symbol(variable_list->children[i]->data, SYM_PARAMETER, NULL, par_seq,
      0, NULL);
      par_seq++;
      // Param takes name after variable, so we can look it up.
      tlhash_insert(param_names, variable_list->children[i]->data, sizeof(void*), param);
    }


    symbol_t *s = create_symbol(node->children[0]->data, SYM_FUNCTION, node, fun_seq,
      node->children[1]->n_children, param_names);
    fun_seq++;
    tlhash_insert(global_names, node->children[0]->data, sizeof(void*), s);
  }
  else
  {
    symbol_t *s = create_symbol(node->children[0]->data, SYM_FUNCTION, node, fun_seq,
      0, param_names);
    fun_seq++;
    tlhash_insert(global_names, node->children[0]->data, sizeof(void*), s);
  }
}

// Takes function symbol_t and node
void
bind_names ( symbol_t *function, node_t *root )
{
  local_seq = 0;

  string_list = malloc(n_string_list * sizeof(char*));

  stack* scope_stack = create_stack();
  stack_push(scope_stack, function->locals);

  node_t* statement = root->children[2];
  bind_child(scope_stack, root->children[2], root);
}

void
bind_child ( stack* scope_stack, node_t *node, node_t *root )
{
  switch(node->type)
  {
    case BLOCK:
    {
      tlhash_t *local_scope = malloc(sizeof(tlhash_t*));
      tlhash_init(local_scope, BUCKETNO);
      stack_push(scope_stack, local_scope);
      for (int i = 0; i < node->n_children; i++)
      {
        bind_child(scope_stack, node->children[i], root);
      }
      stack_pop(scope_stack);
    }
    break;

    case VARIABLE_LIST:
    {
      handle_locals(scope_stack, node, root);

    }
    break;

    case IDENTIFIER_DATA:
      node->entry = get_symbol(scope_stack, node->data);
    break;

    case STRING_DATA:
      handle_string(node);

    default:
      for (int i = 0; i < node->n_children; i++)
      {
        bind_child(scope_stack, node->children[i], root);
      }
  }
}

void
handle_locals ( stack* scope_stack, node_t* node, node_t* root )
{

  for (int i = 0; i < node->n_children; i++)
  {
    // We need some temp void** to use lookup. We don't care about the value.
    void **temp = malloc(sizeof(void**));

    // Is the variable already declared in the innermost scope?
    if (!tlhash_lookup(stack_top(scope_stack), node->children[i]->data, sizeof(void *), temp))
    {
      fprintf(stderr, "Error: Variable exists in scope\n");
      exit(1);
    }
    else
    {

      symbol_t *s = create_symbol(node->children[i]->data, SYM_LOCAL_VAR, root, local_seq,
      0, NULL);
      local_seq++;
      tlhash_t* func_hash = stack_get_elem(scope_stack, scope_stack->size-1);

      // We define a name for the local variable that's unique in the function hash.
      // Comprised of variable name and local seq number.
      char* buffer = malloc(sizeof(char*));
      sprintf(buffer, "%d", local_seq);
      strcat(buffer, "#");
      strcat(buffer, (char*)node->children[i]->data);
      tlhash_insert(func_hash, buffer, sizeof(void*), s);
      tlhash_t* local_hash = stack_top(scope_stack);
      tlhash_insert(local_hash, (char*)node->children[i]->data, sizeof(void*), s);
    }
    free(temp);
  }
}


symbol_t*
get_symbol ( stack* scope_stack, char* data )
{
  for (int i = 0; i < scope_stack->size; i++)
  {
    tlhash_t *temp_hash = stack_get_elem(scope_stack, i);
    void **temp_symbol = malloc(sizeof(symbol_t**));
    int lookup = tlhash_lookup(temp_hash, data, sizeof(data), temp_symbol);
    // If lookup succeeded (returned 0) return value of temp
    if (!lookup)
      return *temp_symbol;
    lookup = tlhash_lookup(global_names, data, sizeof(data), temp_symbol);
    if (!lookup)
      return *temp_symbol;
  }
  fprintf(stderr, "Error: variable not declared\n");
  exit(1);
}

void
handle_string ( node_t *node )
{
  if (n_string_list == stringc)
  {
    n_string_list += STRINGINC;
    char** temp_list = realloc(string_list, n_string_list * sizeof(char*));
    if (temp_list == NULL)
    {
      fprintf(stderr, "Realloc failed, exiting...\n");
      exit(1);
    }
    else
    {
      string_list = temp_list;
      free(temp_list);
    }
  }

  string_list[stringc] = malloc(sizeof(char*));
  string_list[stringc] = node->data;

  
  size_t *string_index = malloc(sizeof(size_t*));
  *string_index = stringc;

  node->data = string_index;
  stringc++;
}



void
destroy_symtab ( tlhash_t* table )
{
  void** table_values = malloc(tlhash_size(table) * sizeof(tlhash_t*));
  tlhash_values(table, table_values);
  for (int i = 0; i < tlhash_size(table); i++)
  {
    if (((symbol_t*)table_values[i])->locals != NULL)
    {
      destroy_symtab(((symbol_t*)table_values[i])->locals);
    }
    free(table_values[i]);
  }
  tlhash_finalize(table);
}


static void
print_symbols ( void )
{
    printf ( "String table:\n" );
    for ( size_t s=0; s<stringc; s++ )
        printf  ( "%zu: %s\n", s, string_list[s] );
    printf ( "-- \n" );

    printf ( "Globals:\n" );
    size_t n_globals = tlhash_size(global_names);
    symbol_t *global_list[n_globals];
    tlhash_values ( global_names, (void **)&global_list );
    for ( size_t g=0; g<n_globals; g++ )
    {
        switch ( global_list[g]->type )
        {
            case SYM_FUNCTION:
                printf (
                    "%s: function %zu:\n",
                    global_list[g]->name, global_list[g]->seq
                );
                if ( global_list[g]->locals != NULL )
                {
                    size_t localsize = tlhash_size( global_list[g]->locals );
                    printf (
                        "\t%zu local variables, %zu are parameters:\n",
                        localsize, global_list[g]->nparms
                    );
                    symbol_t *locals[localsize];
                    tlhash_values(global_list[g]->locals, (void **)locals );
                    for ( size_t i=0; i<localsize; i++ )
                    {
                        printf ( "\t%s: ", locals[i]->name );
                        switch ( locals[i]->type )
                        {
                            case SYM_PARAMETER:
                                printf ( "parameter %zu\n", locals[i]->seq );
                                break;
                            case SYM_LOCAL_VAR:
                                printf ( "local var %zu\n", locals[i]->seq );
                                break;
                        }
                    }
                }
                break;
            case SYM_GLOBAL_VAR:
                printf ( "%s: global variable\n", global_list[g]->name );
                break;
        }
    }
    printf ( "-- \n" );
}


void
print_bindings ( node_t *root )
{
    if ( root == NULL )
        return;
    else if ( root->entry != NULL )
    {
        switch ( root->entry->type )
        {
            case SYM_GLOBAL_VAR: 
                printf ( "Linked global var '%s'\n", root->entry->name );
                break;
            case SYM_FUNCTION:
                printf ( "Linked function %zu ('%s')\n",
                    root->entry->seq, root->entry->name
                );
                break; 
            case SYM_PARAMETER:
                printf ( "Linked parameter %zu ('%s')\n",
                    root->entry->seq, root->entry->name
                );
                break;
            case SYM_LOCAL_VAR:
                printf ( "Linked local var %zu ('%s')\n",
                    root->entry->seq, root->entry->name
                );
                break;
        }
    } else if ( root->type == STRING_DATA ) {
        size_t string_index = *((size_t *)root->data);
        if ( string_index < stringc )
            printf ( "Linked string %zu\n", *((size_t *)root->data) );
        else
            printf ( "(Not an indexed string, root data is: %zu)\n",*((size_t *)root->data ) );
    }
    for ( size_t c=0; c<root->n_children; c++ )
        print_bindings ( root->children[c] );
}
