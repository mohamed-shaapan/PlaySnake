/* IMPORT LIBRARIES */
/**********************************************/
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <iterator>
#include <cstdlib> 		/* library for system()*/
#include <unistd.h> 	/* time delay */

using namespace std;

/* GLOBAL VARIABLES */
/**********************************************/
struct point
{
   int x;
   int y;
};

#define WIDTH 32
#define HEIGHT 16
#define SPACE '.'
#define NODE '0'
#define FRUIT 'F'

enum direction {STOP, LEFT, RIGHT, UP, DOWN};
direction next_direction;

char **play_board;
list<struct point> snake_body;
bool fruit_eaten;
bool gameover;
int score;

/**********************************************/
/* BOARD FUNCTIONS */
/**********************************************/
void
initialize_board()
{
	/* allocate memeory */
	play_board = (char **)malloc(HEIGHT * sizeof(char *));
	int i;
	
    for (i=0; i<HEIGHT; i++)
    {
         play_board[i] = (char *)malloc(WIDTH * sizeof(char));
    }

    /* populate */
	int row, col;
	for(row = 0; row < HEIGHT; row++)
	{
		for(col = 0; col < WIDTH; col++)
		{
			play_board[row][col] = SPACE;
		}
	}
}

void
eat_fruit(struct point location)
{
	snake_body.push_front(location);
	play_board[location.x][location.y] = NODE;
}

void
initialize_snake()
{
	/* build some body */
	int i;
	for(i = 4; i < 10; i++)
	{
		struct point tmp;
		tmp.x = 13; tmp.y = i;
		eat_fruit(tmp);
	}
}


/**********************************************/
/* MOVE SNAKE FUNCTIONS */
/**********************************************/
void
move_head(struct point new_head)
{
	/* update playboard */
	play_board[snake_body.back().x][snake_body.back().y] = SPACE;
	play_board[new_head.x][new_head.y] = NODE;

	/* update snake location */
	struct point old_point;
	struct point new_point; new_point.x = new_head.x; new_point.y = new_head.y;
	list<struct point>::iterator it;
	for(it = snake_body.begin() ; it != snake_body.end(); it++)
	{
		old_point.x = it->x;
		old_point.y = it->y;

	    it->x = new_point.x;
	    it->y = new_point.y;

	    new_point.x = old_point.x;
	    new_point.y = old_point.y;
	}
}


void
apply_move()
{
	struct point tmp;
	tmp.x = snake_body.front().x;
	tmp.y = snake_body.front().y;

	switch (next_direction)
    {
	    case LEFT:
	        --tmp.y;
	        break;
	    case RIGHT:
	        ++tmp.y;
	        break;
	    case UP:
	        --tmp.x;
	        break;
	    case DOWN:
	        ++tmp.x;
	        break;
	    default:
	        return;
    }

    /* check game over conditions */
    if(tmp.x >= HEIGHT || tmp.x < 0 || tmp.y >= WIDTH || tmp.y < 0 || play_board[tmp.x][tmp.y]==NODE)
	{
		gameover = true;
		return;
	}
	/* check if we eat fruit then grow snake */
    if(play_board[tmp.x][tmp.y] == FRUIT)
	{
		eat_fruit(tmp);
		fruit_eaten = true;
		score +=5;
		return;
	}
	move_head(tmp);

}

/**********************************************/
/* MORE UTILITY FUNCTIONS */
/**********************************************/
void
display_board()
{	
	/* clear screen first */
	system("clear");
	/* display board */
	int row, col;
	for(row = 0; row < HEIGHT; row++)
	{
		for(col = 0; col < WIDTH; col++)
		{
			printf("%c", play_board[row][col]);
		}
		printf("\n");
	}
	printf("\n\nscore = %d\n\n", score);
}

void
read_user_input()
{
    switch (getchar())
    {
	    case 'a':
	        next_direction = LEFT;
	        break;
	    case 'd':
	        next_direction = RIGHT;
	        break;
	    case 'w':
	        next_direction = UP;
	        break;
	    case 's':
	        next_direction = DOWN;
	        break;
	    default:
	    	next_direction = STOP;
	    	break;
    }

}

void
generate_fruit()
{
	if(fruit_eaten == false){return;}
	bool valid_loc = false;
	while(valid_loc == false)
	{
		int locX = rand() % HEIGHT;
		int locY = rand() % WIDTH;
		if(play_board[locX][locY] == SPACE)
		{
			valid_loc = true;
			play_board[locX][locY] = FRUIT;
		}
	}

	fruit_eaten = false;
	
}


/**********************************************/
/* MAIN FUNCTION */
/**********************************************/
int
main()
{
	system("clear");
	/* initialize game */
	initialize_board();
	initialize_snake();
	fruit_eaten = true;
	gameover = false;
	score = 0;

	/* display initial board */
	display_board();

	/* play indefinitely */
	int i;
	while(true)
	{
		usleep(100000);
		/* place random fruit */
		generate_fruit();
		/* read user input */
		read_user_input();
		/* apply move */
		apply_move();
		if(gameover == true){break;}
		/* display new board */
		display_board();
		
	}

	printf("\nGame Over Yasta!\n");

	return 0;
}





