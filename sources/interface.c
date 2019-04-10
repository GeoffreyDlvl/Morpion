#include "../headers/history.h"
#include "../headers/interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>

/*@requires pboard not null
  @assigns nothing
  @ensures prints board, shows possible moves if hint set to true */
void print_board(Board* pboard, bool hint)
{
    int width = pboard->width;
    int height = pboard->height;
    int indexLines[4] = {-1,-1,-1,-1};
    int i, j;
    /* print header (y coordinates) */
    printf("  ");
    for(i = 0 ; i < width ; i++) {
        /* Header not displayed correctly if size > 99
         * Accepted because a board should not be as big.
         * A game on a board this big would be way too
         * long anyway.
         */
        if(i < 10)
            printf(" %d ", i);
        else {
            printf(" %d", i);
        }
    }
    printf("\n");
    /* print x coordinates and board */
    for(i = 0 ; i < height ; i++) {
        /* print x coordinates */
        if(i < 10)
            printf("%d ", i);
        else
            printf("%d", i);

        for(j = 0 ; j < width ; j++) {

            /* print board line */
            printf("[");
            if(pboard->points[i][j]) {
                if (Move_search(get_lines_history(),j,i,indexLines)) {
                    printf("x");
                } else {
                    printf("\u2022");
                }
            } else {
                printf(" ");
            }
            printf("]");
        }
        printf("\n");
    }

    if (hint) {
    /*list_available_moves(pboard);*/
    }
}

Coord select_move()
{
	int x,y;
	/* Ppoint p_point ;*/
	printf("Select next point coordinates [x,y] (press x then ENTER then press y then ENTER) :\n");
	scanf("%d%d",&x,&y);
	Coord coord;
	coord.x=x;
	coord.y=y;
	coord.previous=NULL;
	return coord;
}

enum action select_action()
{
    printf("Select : Play move [p] / Cancel previous move [c] / Replay cancelled move [r]\n         List valid moves [l] / Ask help [h] / Quit game [q]\n");
	char c;
	while (true) {
	    fflush(stdin);
	    scanf("%c",&c);
        switch(c) {
            case 'p':
                return PLAY_MOVE;
            break;
            case 'c':
                return CANCEL_MOVE;
            break;
            case 'r':
                return REPLAY_MOVE;
            break;
            case 'l':
                return LIST_MOVES;
            break;
            case 'h':
                return ASK_HELP;
            break;
	        case 'q':
		        return QUIT_GAME;
	        break;
	    default:
	        break;
	}
	}
}
/*
void list_available_moves(Board* pboard)
{
   TO DO 
	Coord coord = get_valid_moves(pboard);
}


	 
void ask_help(Board* pboard)
{
   TO DO 
}
*/
void print_help(void)
{
	printf("------- HELP -------\n\n");

	printf("> The goal is to add points on the board.\n");
	printf("> A point can be added to the board if it creates a line of 5 points vertically, horizontally or diagonally.\n");
	printf("> Two lines can't have more than one point in common.\n\n");
}

/*@requires pmove not null
  @assings pmove
  @ensures asks player to chose amongst possible lines and frees all those not chosen*/
void select_line(Move* pmove){
  int choice;
  Move current=*pmove;
  int i=0;
  printf("List of possible moves :\n");
  printf("1 : ");
  printf("[%d,%d]",current->x,current->y);
  current=current->previous;
  i++;

  while(!Move_isEmpty(current)){
    if (i%5==0){
      printf("\n");
      printf("%d : ",(i/5)+1);
      printf("[%d,%d]",current->x,current->y);
    } else {
	  printf("->[%d,%d]",current->x,current->y);
    }
    current=current->previous;
    i++;
  }
  printf("\nChoose your line.\n");
  do{
  	scanf("%d",&choice);
  }while(0 > choice || choice > (i/5));
  i=0;
  current=*pmove;
  for (i=0;i<(choice-1)*5;i++){
    Move_popM(&current);
  }
  Move current2=current;
  for (i=0;i<4;i++){
    current2=current2->previous;
  }
  Move tmp=current2->previous;
  current2->previous=NULL;
  pMove_free(&tmp);
  printf("You have chosen:\n");
  Move_print(current);
  *pmove=current;
}

bool confirm_quit_save(Board* pboard) {
    char quit;
    do {
        printf("Do you really want to exit the game? (y/n)\n");
        scanf(" %c", &quit);
        if (quit != 'y' && quit != 'n')
            printf("\nWrong input.\n");

    } while (quit != 'y' && quit != 'n');

    if(quit == 'y') {
        char save;
        do {
            printf("Do you want to save the current board? (y/n)\n");
            scanf(" %c", &save);
            if (save != 'y' && save != 'n')
                printf("\nWrong input.\n");

        } while (save != 'y' && save != 'n');

        if (save == 'y')
            save_board(pboard);
        return true;
    }
    else
        return false;
}


char* ask_savefile_name(){
    char* fileName = (char*)malloc(sizeof(char)*MAX_INPUT);
    printf("What name does the file should have ? :> ");
    scanf(" %s", fileName);
    return fileName;
}