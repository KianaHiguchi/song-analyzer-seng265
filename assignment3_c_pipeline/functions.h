/** @file functions.h
 *  @brief Function prototypes for song_analyzer.c solution
 */
#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#define MAX_LINE_LEN 200
#include "list.h"

// track_name,artist(s)_name,artist_count,released_year,released_month,released_day,in_spotify_playlists,streams,in_apple_playlists
/**
 * @brief An struct that represents a song
 */
typedef struct
{
    char track_name[MAX_LINE_LEN];
    char artists_name[MAX_LINE_LEN];
    int artist_count;
    int released_year;
    int released_month;
    int released_day;
    int in_spotify_playlists;
    long int streams;
    int in_apple_playlists;
} song;

/**
 * Function protypes associated with a song analyzer program for csv format
 * 
 */
void parse_arg(int argc, char *argv[], char **data, char **filter, char **value, char **order_by, char **order, char **limit);
node_t *turn_data_into_list(const char *filename);
void parse_line_to_song(const char *line, song *s);
node_t *check_field_in_linked_list(node_t *head, const char *target, const char *target_value, node_t *successful_lines);
node_t *merge_sort(node_t *head, const char *order_by);
node_t *merge(node_t *left, node_t *right, const char *order_by);
node_t *limit_list(node_t *sorted_lines, const char *order, const char *limit);
void write_output_to_file(node_t *answer, const char *order_by);

#endif
