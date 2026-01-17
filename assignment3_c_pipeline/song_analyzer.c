/** @file song_analyzer.c
 *  @brief A small program to analyze songs data.
 *  @author Mike Z.
 *  @author Felipe R.
 *  @author Hausi M.
 *  @author Juan G.
 *  @author Angadh S.
 *  @author STUDENT: Kiana Higuchi
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "functions.h"

/**
 * @brief The main function and entry point of the program.
 *
 * @param argc The number of arguments passed to the program.
 * @param argv The list of arguments passed to the program.
 * @return int 0: No errors; 1: Errors produced.
 *
 */
int main(int argc, char *argv[])
{
    // process command line
    char *data, *filter, *value, *order_by, *order, *limit;
    parse_arg(argc, argv, &data, &filter, &value, &order_by, &order, &limit);

    // read data
    const char *data_file = "data.csv";
    fopen(data_file, "r");
    node_t *data_file_lines = turn_data_into_list(data_file);

    // filter data
    node_t *filtered_lines = NULL;
    filtered_lines = check_field_in_linked_list(data_file_lines, filter, value, filtered_lines);

    // sort data
    node_t *sorted_lines = NULL;
    sorted_lines = merge_sort(filtered_lines, order_by);
    node_t *limited_result = limit_list(sorted_lines, order, limit);

    // write output
    write_output_to_file(limited_result, order_by);

    free_list(filtered_lines);
    free_list(limited_result);

    exit(0);
}
