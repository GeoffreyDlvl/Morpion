#include <stdlib.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "linux/limits.h"
#include "../headers/board.h"
#include "../headers/utils.h"


static Error* error;

/* The suite initialization function.
 * Opens a file and write the date of the execution of the test suite
 * Returns the zero on success, non-zero otherwise.
 */
int init_suite(void){
    error = malloc(sizeof(Error));
    system("date");
    return 0;
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite(void)
{
    free(error);
    return 0;
}

/************* TESTS *************/
void test_read_file_char_is_invalid(void)
{
    *error = NO_ERR;
    char resolved_path[PATH_MAX];
    realpath("test-files/board-invalid-char", resolved_path);
    FILE *fp = fopen(resolved_path, "r");
    /* Check return value */
    CU_ASSERT_FALSE(check_file(fp,error))
    /* Deeper test: check error value */
    CU_ASSERT(*error == FILE_UNKNOWN_CHAR_ERR);
    fclose(fp);
    free(fp);
}

void test_read_file_dimensions_are_invalid(void)
{
    *error = NO_ERR;
    char resolved_path[PATH_MAX];
    realpath("test-files/board-invalid-dimensions", resolved_path);
    FILE *fp = fopen(resolved_path, "r");
    /* Check return value */
    CU_ASSERT_FALSE(check_file(fp,error));
    /* Deeper test: check error value */
    CU_ASSERT(*error == FILE_DIMENSION_ERR);
    fclose(fp);
    free(fp);
}

void test_read_file_path_is_invalid(void)
{
    *error = NO_ERR;
    char resolved_path[PATH_MAX];
    realpath("path/is/invalid", resolved_path);
    FILE *fp = fopen(resolved_path, "r");
    /* Check return value */
    CU_ASSERT_FALSE(check_file(fp,error));
    /* Deeper test: check error value */
    CU_ASSERT(*error == FILE_PTR_ERR);
    if(fp) {
        fclose(fp);
        free(fp);
    }
}

void test_read_file_board_is_valid(void)
{
    *error = NO_ERR;
    char resolved_path[PATH_MAX];
    realpath("test-files/board-valid", resolved_path);
    FILE *fp = fopen(resolved_path, "r");
    /* Check return value */
    CU_ASSERT(check_file(fp,error));
    /* Deeper test: check error value */
    CU_ASSERT(*error == NO_ERR);
    fclose(fp);
    free(fp);
}

void test_adding_already_existing_points(void)
{
    Board board;
    *error = NO_ERR;
    char resolved_path[PATH_MAX];
    realpath("test-files/board-valid", resolved_path);
    FILE *fp = fopen(resolved_path, "r");
    /* Check that initialization went well */
    CU_ASSERT(initialize_file(&board, resolved_path, err))
    /* Check return value of a played move */
        /* Create the coord of the played move */
        Coord coord;
        coord.x = 3;
        coord.y = 0;
    /* I NEED TO FINISH */
    CU_ASSERT_FALSE(play_move(&board, ));
    /* Deeper test: check error value */
    CU_ASSERT(*error == NO_ERR);
    fclose(fp);
    free(fp);
    board = create_empty_board(10,10);
    int* point = (int*) malloc(sizeof(int));
    *point =1;
    pboard->points[0][0] = point;
    Coord* coord = (Coord*) malloc(sizeof(Coord));
    coord->x = 0;
    coord->y = 0;
    CU_ASSERT_EQUAL(add_point(pboard,*coord,NULL),false);
    free_board(pboard);
    free(coord);
    free(point);
}

void test_is_move_valid(void){
    Board* pboard;
    char resolved_path[PATH_MAX];
    realpath("test-files/board-valid-move", resolved_path);
    initialize_file(pboard,resolved_path,error);
}

void test_creating_empty_board(void){
    Board* pboard = (Board*) malloc(sizeof(Board));
    *pboard = create_empty_board(300,300);
    CU_ASSERT_NOT_EQUAL(pboard,NULL);
    CU_ASSERT_EQUAL(pboard->height,300);
    CU_ASSERT_EQUAL(pboard->width,300);
    free(pboard);
}

int main(void)
{
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();
    /* add a suite to the registry */
    pSuite = CU_add_suite("Board_suite", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* add the tests to the suite */
    if((NULL == CU_add_test(pSuite, "Read_board_file_invalid_char", test_read_file_char_is_invalid))
        ||
        (NULL == CU_add_test(pSuite, "Read_board_file_dimensions_invalid", test_read_file_dimensions_are_invalid))
        ||
        (NULL == CU_add_test(pSuite, "Read_board_file_path_invalid", test_read_file_path_is_invalid))
        ||
        (NULL == CU_add_test(pSuite, "Read_board_valid", test_read_file_board_is_valid))
        ||
        (NULL == CU_add_test(pSuite, "Adding_already_existing_points", test_adding_already_existing_points))
        ||
        (NULL == CU_add_test(pSuite, "Adding_already_existing_points", test_creating_empty_board)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run all tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    printf("\n");
    CU_basic_show_failures(CU_get_failure_list());
    printf("\n\n");
    /* Clean up registry and return */
    CU_cleanup_registry();
    return CU_get_error();
}
