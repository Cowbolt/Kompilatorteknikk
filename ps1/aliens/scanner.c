#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "pencil.h"


/*
 * This function is called before anything else, to initialize the
 * state machine. It is certainly possible to create implementations
 * which don't require any initialization, so just leave this blank if
 * you don't need it.
 */

// Define number of states and symbols, to initialize transition table
#define NUM_STATES 19
#define NUM_SYM 12

// Define goal states
#define GOAL_ONE 3
#define GOAL_TWO 6
#define GOAL_THREE 10
#define GOAL_FOUR 14
#define GOAL_STOP 18

/*
 * We'll use a transition table. a 2d int array, first row is the state, second row is the transition value, the value is the next state.
 * Symbols: o, t, f, s, n, e, w, h, r, u, p, <other>.
 *
 *
 */

static int transtab[NUM_STATES][NUM_SYM];
static int cur_state = 0;

int get_sym_index (char c)
{
	switch(c)
	{
		case 'o': case 'O': return 1;
		case 't': case 'T': return 2;
		case 'f': case 'F': return 3;
		case 's': case 'S': return 4;
		case 'n': case 'N': return 5;
		case 'e': case 'E': return 6;
		case 'w': case 'W': return 7;
		case 'h': case 'H': return 8;
		case 'r': case 'R': return 9;
		case 'u': case 'U': return 10;
		case 'p': case 'P': return 11;
		default: return 0;
	}
}


void init_transtab ( void )
{
	// init all states to point to initital state
	for (int x = 0; x < NUM_STATES; x++)
	{
		for (int y = 0; y < NUM_SYM; y++)
		{
			transtab[x][y] = 0;
		}
	}

	// init initial state
	transtab[0][get_sym_index('o')] = 1;
	transtab[0][get_sym_index('t')] = 4;
	transtab[0][get_sym_index('f')] = 11;
	transtab[0][get_sym_index('s')] = 15;

	// init 'one'
	transtab[1][get_sym_index('n')] = 2;
	transtab[2][get_sym_index('e')] = 3;

	// init 'two'
	transtab[4][get_sym_index('w')] = 5;
	transtab[5][get_sym_index('o')] = 6;

	// init 'three'
	transtab[4][get_sym_index('h')] = 7;
	transtab[7][get_sym_index('r')] = 8;
	transtab[8][get_sym_index('e')] = 9;
	transtab[9][get_sym_index('e')] = 10;
	
	// init 'four'
	transtab[11][get_sym_index('o')] = 12;
	transtab[12][get_sym_index('u')] = 13;
	transtab[13][get_sym_index('r')] = 14;
	
	// init 'stop'
	transtab[15][get_sym_index('t')] = 16;
	transtab[16][get_sym_index('o')] = 17;
	transtab[17][get_sym_index('p')] = 18;
}   


/*
 * Return the next token from reading the given input stream.
 * The words to be recognized are 'turn', 'draw' and 'move',
 * while the returned tokens may be TURN, DRAW, MOVE or END (as
 * enumerated in 'pencil.h').
 */
command_t
next ( FILE *input )
{
	int next_char;

		while (1)
		{
			next_char = fgetc(input);
			if (next_char == EOF)
			{
				return STOP;
			} 
			
			cur_state = transtab[cur_state][get_sym_index(next_char)];
			switch (cur_state)
			{
				case GOAL_ONE: return ONE;
				case GOAL_TWO: return TWO;
				case GOAL_THREE: return THREE;
				case GOAL_FOUR: return FOUR;
				case GOAL_STOP: return STOP;
				default: break;
			}
		}


}
