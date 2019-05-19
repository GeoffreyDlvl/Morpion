#include "../headers/board.h"
#include "../headers/interface.h"
#include "../headers/history.h"
#include "../headers/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <linux/limits.h>

int main(int argc, char* argv[]){
    Interface* interface;
    srand(time(NULL));

    /*If user gives more than 2 arguments*/
    if (argc>3){
        fprintf(stderr,"%s:FATAL: %d invalid nber of args (no more than two expected)\n",argv[0],argc-1);
        return EXIT_FAILURE;
    }

    Board board;
    /*If user does not give any argument*/
    if (argc == 1){
        board = initialize_rand();
    } else {
        /*If first argument is neither option -r and -h
        OR
        first argument is option -r but is not followed by a parameter*/
        if ((strcmp(argv[1], "-r") != 0 && strcmp(argv[1], "-h") != 0) ||
            ((strcmp(argv[1], "-r") == 0) && argc == 2)) {
            fprintf(stderr, "%s:FATAL: %s invalid args (none, -r FILE or -h expected) \n", argv[0], argv[1]);
            return EXIT_FAILURE;
        }
        /*If first argument is option -h*/
        if (strcmp(argv[1], "-h") == 0) {
            // random board only useful to display help
            board = initialize_rand();
            interface = init(&board);
            print_help(interface);
            return EXIT_SUCCESS;
        }
    }
    Error* err = malloc(sizeof(Error));
    *err = NO_ERR;
    /* If a file has been passed as argument */
    if(argc == 3 && (strcmp(argv[1], "-r") == 0)) {
        /* Get the file argument absolute path (i.e. resolved path) */
        char resolved_path[PATH_MAX];
        realpath(argv[2], resolved_path);

        /* Construct board if file is valid, exit otherwise */
        if (!initialize_file(&board, resolved_path, err)) {
            print_error(*err, interface);
            free(err);
            return EXIT_FAILURE;
        }
    }

    //install_dependencies();
    set_fullscreen(true);
    display_logo();
    initialize_HistoryList();
    initialize_LinesList();
    /*Declare a new unallocated pointer: it will be allocated if required and manipulated in functions*/
    enum action playerAction = PLAY_MOVE;
    set_hint(false);
    interface = init(&board);
    bool quitGame = false;
    /*Loop termination : board size is finite therefore number of playable moves if finite*/
    while(!is_game_over(&board) && !quitGame)/*(!is_game_over(&board))  erreurs de segmentation résolus jusqu'ici*/
    {
        redraw(&board,interface);
        set_hint(false);
        execute_action(&board,interface,select_action(interface), &quitGame, err);

        if(quitGame)
            break;
        if(interface->gui)
            add_line_to_board(*err, interface);

        if(*err != NO_ERR)
        {
            print_error(*err, interface);
            press_enter_to_continue();
            *err = NO_ERR;
        }
    }
    print_game_over();
    print_score(interface);
    press_enter_to_continue();
    /* Free all allocated pointers */
    free(err);
    free_history();
    remove_points(&board);
    free_board(&board);
    set_fullscreen(false);
    clear_screen();
    return EXIT_SUCCESS;
}
      
   
 
      
      
    
  
  
