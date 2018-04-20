%{
#include <vslc.h>
%}
%left '|'
%left '^'
%left '&'
%left LSHIFT RSHIFT
%left '+' '-'
%left '*' '/'
%left ASSIGNMENT
%nonassoc UMINUS
%right '~'
    //%expect 1

%token FUNC PRINT RETURN CONTINUE IF THEN ELSE WHILE DO OPENBLOCK CLOSEBLOCK
%token VAR NUMBER IDENTIFIER STRING LSHIFT RSHIFT

%%

program :
    global_list
        {
            root = (node_t *) malloc ( sizeof(node_t) );
            node_init (root, PROGRAM, NULL, 1, $1);
        }
    ;

global_list :
    global
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, GLOBAL_LIST, NULL, 1, $1);
            $$ = this;
        }
    |
    global_list global
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, GLOBAL_LIST, NULL, 2, $1, $2);
            $$ = this;
        }
    ;

global :
    function
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, GLOBAL, NULL, 1, $1);
            $$ = this;
        }
    |
    declaration
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, GLOBAL, NULL, 1, $1);
            $$ = this;
        }
    ;

statement_list :
    statement
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, STATEMENT_LIST, NULL, 1, $1);
            $$ = this;
        }
    |
    statement_list statement
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, STATEMENT_LIST, NULL, 2, $1, $2);
            $$ = this;
        }
    ;

print_list :
    print_item
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, PRINT_LIST, NULL, 1, $1);
            $$ = this;
        }
    |
    print_list ',' print_item
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, PRINT_LIST, NULL, 2, $1, $3);
            $$ = this;
        }
    ;

expression_list :
    expression
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, EXPRESSION_LIST, NULL, 1, $1);
            $$ = this;
        }
    |
    expression_list ',' expression
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, EXPRESSION_LIST, NULL, 2, $1, $3);
            $$ = this;
        }
    ;

variable_list :
    identifier
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, VARIABLE_LIST, NULL, 1, $1);
            $$ = this;
        }
    |
    variable_list ',' identifier
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, VARIABLE_LIST, NULL, 2, $1, $3);
            $$ = this;
        }
    ;

argument_list :
    expression_list
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, ARGUMENT_LIST, NULL, 1, $1);
            $$ = this;
        }
    |

    ;

parameter_list :
    variable_list
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, PARAMETER_LIST, NULL, 1, $1);
            $$ = this;
        }
    |
            {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, PARAMETER_LIST, NULL, 0);
            $$ = this;
        }
    ;

declaration_list :
    declaration
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, DECLARATION_LIST, NULL, 1, $1);
            $$ = this;
        }
    |
    declaration_list declaration
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, DECLARATION_LIST, NULL, 2, $1, $2);
            $$ = this;
        }
    ;

function :
    FUNC identifier '(' parameter_list ')' statement
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, FUNCTION, NULL,
                           3, $2, $4, $6);
            $$ = this;
        }
    ;

statement :
    assignment_statement
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, STATEMENT, NULL, 1, $1);
            $$ = this;
        }
    |
    return_statement
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, STATEMENT, NULL, 1, $1);
            $$ = this;
        }
    |
    print_statement
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, STATEMENT, NULL, 1, $1);
            $$ = this;
        }
    |
    if_statement
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, STATEMENT, NULL, 1, $1);
            $$ = this;
        }
    |
    while_statement
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, STATEMENT, NULL, 1, $1);
            $$ = this;
        }
    |
    null_statement
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, STATEMENT, NULL, 1, $1);
            $$ = this;
        }
    |
    block
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, STATEMENT, NULL, 1, $1);
            $$ = this;
        }
    ;

block :
    OPENBLOCK declaration_list statement_list CLOSEBLOCK
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, BLOCK, NULL, 2, $2, $3);
            $$ = this;
        }
    |
    OPENBLOCK statement_list CLOSEBLOCK
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, BLOCK, NULL, 1, $2);
            $$ = this;
        }
    ;

assignment_statement :
    identifier ':' '=' expression
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, ASSIGNMENT_STATEMENT, NULL, 2, $1, $4);
            $$ = this;
        }
    ;

return_statement :
    RETURN expression
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, RETURN_STATEMENT, NULL, 1, $2);
            $$ = this;
        }
    ;

print_statement :
    PRINT print_list
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, PRINT_STATEMENT, NULL, 1, $2);
            $$ = this;
        }
    ;

null_statement :
    CONTINUE
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, NULL_STATEMENT, NULL, 0);
            $$ = this;
        }
    ;

if_statement :
    IF relation THEN statement
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, IF_STATEMENT, NULL, 2, $2, $4);
            $$ = this;
        }
    |
    IF relation THEN statement ELSE statement
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, IF_STATEMENT, NULL, 3, $2, $4, $6);
            $$ = this;
        }
    ;

while_statement :
    WHILE relation DO statement
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, WHILE_STATEMENT, NULL, 2, $2, $4);
            $$ = this;
        }
    ;

relation :
    expression '=' expression
        {
            node_t *this = malloc(sizeof(node_t));
            void *data = (void *)strdup("=");
            node_init(this, RELATION, data, 2, $1, $3);
            $$ = this;
        }
    |
    expression '<' expression
        {
            node_t *this = malloc(sizeof(node_t));
            void *data = (void *)strdup("<");
            node_init(this, RELATION, data, 2, $1, $3);
            $$ = this;
        }
    |
    expression '>' expression
        {
            node_t *this = malloc(sizeof(node_t));
            void *data = (void *)strdup(">");
            node_init(this, RELATION, data, 2, $1, $3);
            $$ = this;
        }
    ;

expression :
    expression '|' expression
        {
            node_t *this = malloc(sizeof(node_t));
            void *data = (void *)strdup("|");
            node_init(this, EXPRESSION, data, 2, $1, $3);
            $$ = this;
        }
    |
    expression '^' expression
        {
            node_t *this = malloc(sizeof(node_t));
            void *data = (void *)strdup("^");
            node_init(this, EXPRESSION, data, 2, $1, $3);
            $$ = this;
        }
    |
    expression '&' expression
        {
            node_t *this = malloc(sizeof(node_t));
            void *data = (void *)strdup("&");
            node_init(this, EXPRESSION, data, 2, $1, $3);
            $$ = this;
        }
    |
    expression RSHIFT expression
        {
            node_t *this = malloc(sizeof(node_t));
            void *data = (void *)strdup(">>");
            node_init(this, EXPRESSION, data, 2, $1, $3);
            $$ = this;
        }
    |
    expression LSHIFT expression
        {
            node_t *this = malloc(sizeof(node_t));
            void *data = (void *)strdup("<<");
            node_init(this, EXPRESSION, data, 2, $1, $3);
            $$ = this;
        }
    |
    expression '+' expression
        {
            node_t *this = malloc(sizeof(node_t));
            void *data = (void *)strdup("+");
            node_init(this, EXPRESSION, data, 2, $1, $3);
            $$ = this;
        }
    |
    expression '-' expression
        {
            node_t *this = malloc(sizeof(node_t));
            void *data = (void *)strdup("-");
            node_init(this, EXPRESSION, data, 2, $1, $3);
            $$ = this;
        }
    |
    expression '*' expression
        {
            node_t *this = malloc(sizeof(node_t));
            void *data = (void *)strdup("*");
            node_init(this, EXPRESSION, data, 2, $1, $3);
            $$ = this;
        }
    |
    expression '/' expression
        {
            node_t *this = malloc(sizeof(node_t));
            void *data = (void *)strdup("/");
            node_init(this, EXPRESSION, data, 2, $1, $3);
            $$ = this;
        }
    |
    '-' expression %prec UMINUS
        {
            node_t *this = malloc(sizeof(node_t));
            void *data = (void *)strdup("-");
            node_init(this, EXPRESSION, data, 1, $2);
            $$ = this;
        }
    |
    '~' expression
        {
            node_t *this = malloc(sizeof(node_t));
            void *data = (void *)strdup("~");
            node_init(this, EXPRESSION, data, 1, $2);
            $$ = this;
        }
    |
    '(' expression ')'
        {
            $$ = $2;
        }
    |
    number
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, EXPRESSION, NULL, 1, $1);
            $$ = this;
        }
    |
    identifier
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, EXPRESSION, NULL, 1, $1);
            $$ = this;
        }
    |
    identifier '(' argument_list ')'
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, EXPRESSION, NULL, 2, $1, $3);
            $$ = this;
        }
    ;

declaration :
    VAR variable_list
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, DECLARATION, NULL, 1, $2);
            $$ = this;
        }
    ;

print_item :
    expression
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, PRINT_ITEM, NULL, 1, $1);
            $$ = this;
        }
    |
    string
        {
            node_t *this = malloc(sizeof(node_t));
            node_init(this, PRINT_ITEM, NULL, 1, $1);
            $$ = this;
        }
    ;

identifier :
    IDENTIFIER
        {
            node_t *this = malloc(sizeof(node_t));
            void *data = (void *)strdup(yytext);
            node_init(this, IDENTIFIER_DATA, data, 0);
            $$ = this;
        }
    ;

number :
    NUMBER
        {
            node_t *this = malloc(sizeof(node_t));
            int64_t *data = malloc(sizeof(int64_t));
            *data = strtol(yytext, NULL, 10);
            node_init(this, NUMBER_DATA, (void*) data, 0);
            $$ = this;
        }
    ;

string :
    STRING
        {
            node_t *this = malloc(sizeof(node_t));
            // Copy the string without the quotes
            void* data = (void*)strdup(yytext);
            node_init(this, STRING_DATA, data, 0);
            $$ = this;
        }
    ;

%%

int
yyerror ( const char *error )
{
    fprintf ( stderr, "%s on line %d\n", error, yylineno );
    exit ( EXIT_FAILURE );
}