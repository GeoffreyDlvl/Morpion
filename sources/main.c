#include "../headers/board.h"
#include "../headers/interface.h"
#include "../headers/history.h"
#include<stdio.h>                                                               
#include<stdlib.h>                                                              
#include<unistd.h>                                                              
#include<sys/types.h>                                                           
#include<sys/stat.h>                                                            
#include<fcntl.h>                                                               
#include<string.h>                                                              
#include<errno.h>                                                               
                                                                                
int main(int argc, char* argv[]){   
  /* If user gives more than 2 arguments */                                             
  if (argc>3){
    fprintf(stderr,"%s:FATAL: %d invalid nber of args (no more than two expected)\n",argv[0],argc);
    return EXIT_FAILURE;
  }
  Board* pboard=NULL;
  /* If user does not give any argument */
  if (argc == 1){
    pboard = initialize_rand();
  }
  else{
    /* If first argument is neither option -r and -h  
    OR 
    first argument is option -r but is not followed by a parameter */
    if ((strcmp(argv[1],"-r")!=0 && strcmp(argv[1],"-h")!=0)||(strcmp(argv[1],"-r")==0 && argc==2)){
      fprintf(stderr,"%s:FATAL: %s invalid args (none, -r FILE or -h expected) \n",argv[0],argv[1]);
      return EXIT_FAILURE;
    }
    /* If first argument is option -h */
    if (strcmp(argv[1],"-h")==0){
      print_help();
      return EXIT_SUCCESS;
    }
    else{
      /* If file given as a parameter (following option -r) is invalid */
      if (!read_file(pboard,argv[2])){
        return EXIT_FAILURE;
      }
    }
  }
  bool hint = false;
  /* Loop termination : board size is finite therefore number of playable moves if finite */
  while(!is_game_over(pboard))
  {
    /* Declare a new unallocated pointer: it will be allocated if required and manipulated in functions */
    Pcoord pcoord;
    print_board(pboard, hint);
    enum action playerAction = select_action(pboard, pcoord, &hint);
    execute_action(pboard, playerAction, pcoord);
  }
  /* Free all allocated pointers */
  free_history();
  free_board(pboard);
  return EXIT_SUCCESS;
}
      
   
 
      
      
    
  
  
