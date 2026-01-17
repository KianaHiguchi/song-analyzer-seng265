/** @file song_analyzer.c
 *  @brief A pipes & filters program that uses conditionals, loops, and string processing tools in C to process song
 *  data and printing it in a different format.
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief The following are max values relating to file line length, number of songs, and max length for 'song' struct members.
 * Usage for string size allotment.
 *
 */
#define MAX_LINE_LEN 200
#define MAX_SONGS 1000
#define MAX_KEY 4
#define MAX_MODE 6

/**
 * @brief Structure defintion for type 'song'.
 *
 */
typedef struct
{
    char track_name[MAX_LINE_LEN];
    char artist_names[MAX_LINE_LEN];
    int artist_count;
    int released_year;
    int in_spotify_playlists;
    long int streams;
    char key[MAX_KEY];
    char mode[MAX_MODE];

} song;

/**
 * @brief Function prototypes, category: parsing input
 *
 */
void parse_arg(int argc, char *argv[], int arg_index, char *arg_flag, char **arg_value);
int read_lines(FILE *filename, char list[MAX_SONGS][MAX_LINE_LEN]);
void parse_song_line(char list[MAX_SONGS][MAX_LINE_LEN], int index, song *song_ptr);
int turn_into_song_list(FILE *file, song list[]);

/**
 * @brief Function prototypes, category: finding condition targets
 *
 */
bool is_artist(char *target, song this_song);
bool is_artist_count(int target, song this_song);
bool is_key(char *target, song this_song);
bool is_mode(char *target, song this_song);
bool is_min_playlists(int target, song this_song);
bool is_year(int target, song this_song);
bool contains_artist(char *target, song this_song);

/**
 * @brief Function prototypes, category: processing output
 *
 */
void switch_case_questions(int case_number, song songs_in[], song songs_out[], int in_count, int *out_count);
void output_answers(song output[], int out_count);

/**
 * Function: main
 * --------------
 * @brief The main function and entry point of the program.
 *
 * @param argc The number of arguments passed to the program.
 * @param argv The list of arguments passed to the program.
 * @return int 0: No errors; 1: Errors produced.
 *
 */
int main(int argc, char *argv[])
{
    char *question, *data = NULL;
    parse_arg(argc, argv, 1, "--question", &question);
    parse_arg(argc, argv, 2, "--data", &data);
    int question_int = atoi(question);

    song list_of_songs[MAX_SONGS];
    FILE *song_file = fopen(data, "r");
    int song_count = turn_into_song_list(song_file, list_of_songs);
    fclose(song_file);

    song answer_to_question[MAX_SONGS];
    int answer_count = 0;
    switch_case_questions(question_int, list_of_songs, answer_to_question, song_count, &answer_count);

    output_answers(answer_to_question, answer_count);
    return 0;
}

/**
 * @brief Function to parse command line arguments.
 *
 * @param argc Number of arguments passed to the program.
 * @param argv List of arguments passed to the program.
 * @param arg_index Argument index to parse.
 * @param arg_flag Flag to look for before the actual argument value.
 * @param arg_value Value targeted by the function.
 * @return Void function has no return value.
 *
 */
void parse_arg(int argc, char *argv[], int arg_index, char *arg_flag, char **arg_value)
{
    while (arg_index < 1 || arg_index >= argc)
    {
        *arg_value = NULL;
        return;
    }

    char *token = strtok(argv[arg_index], "=");
    if (token != NULL && strcmp(token, arg_flag) == 0)
    {
        *arg_value = strtok(NULL, "=");
    }
    else
    {
        *arg_value = NULL;
    }
}

/**
 * @brief Function to read a file and store each line in a list of strings.
 *
 * @param filename The file passed into the function.
 * @param list The list of strings to be populated by the function.
 * @return Returns the int number of lines in newly populated list.
 *
 */
int read_lines(FILE *filename, char list[MAX_SONGS][MAX_LINE_LEN])
{
    int count = 0;
    char line[MAX_LINE_LEN];

    while (fgets(line, MAX_LINE_LEN, filename) != NULL && count < MAX_SONGS - 1)
    {
        if (strspn(line, " \t\n") == strlen(line))
            continue;

        line[strcspn(line, "\n")] = '\0';

        strcpy(list[count], line);
        count++;
    }
    return count;
}

/**
 * @brief Function to take a list of strings and parse a line into song structure members.
 *
 * @param list The list of strings from which 'song object' will be extracted
 * @param index The index in the list of the song
 * @param song_ptr The song whose members will be assigned.
 * @return Void function has no return value.
 *
 */
void parse_song_line(char list[MAX_SONGS][MAX_LINE_LEN], int index, song *song_ptr)
{
    char *token;

    token = strtok(list[index], ",");
    strncpy(song_ptr->track_name, token, MAX_LINE_LEN - 1);
    song_ptr->track_name[MAX_LINE_LEN - 1] = '\0';

    token = strtok(NULL, ",");
    strncpy(song_ptr->artist_names, token, MAX_LINE_LEN - 1);
    song_ptr->artist_names[MAX_LINE_LEN - 1] = '\0';

    token = strtok(NULL, ",");
    song_ptr->artist_count = atoi(token);

    token = strtok(NULL, ",");
    song_ptr->released_year = atoi(token);

    token = strtok(NULL, ",");
    song_ptr->in_spotify_playlists = atoi(token);

    // Note: if statement only added due to error in song file where streams contained random data (love grows where my rosemary...)
    token = strtok(NULL, ",");
    if (strlen(token) > 30)
    {
        song_ptr->streams = 0;
    }
    else
    {
        song_ptr->streams = atol(token);
    }

    token = strtok(NULL, ",");
    strncpy(song_ptr->key, token, MAX_KEY - 1);
    song_ptr->key[MAX_KEY - 1] = '\0';

    token = strtok(NULL, ",");
    strncpy(song_ptr->mode, token, MAX_MODE - 1);
    song_ptr->mode[MAX_MODE - 1] = '\0';
}

/**
 * @brief Function to take a file, extract lines (using read_lines), and format them as a list of 'song objects' (using parse_song_line)
 *
 * @param file File to be processed.
 * @param list list to store songs as instances of struct type 'song'.
 * @return Returns song_count, the int value to keep track of number of songs.
 *
 */
int turn_into_song_list(FILE *file, song list[])
{
    char songs_as_strings[MAX_SONGS][MAX_LINE_LEN];
    int song_count = read_lines(file, songs_as_strings);

    int count = 0;
    for (int i = 0; i < song_count; i++)
    {
        song temp;
        parse_song_line(songs_as_strings, i, &temp);
        list[count++] = temp;
    }
    return song_count;
}

/**
 * @brief The following functions check for specified targets within their respective members of struct type 'song'.
 *
 * @param target Parameter is either a pointer for strings or an int for numbers. It is the value to be searched for.
 * @param this_song The song structure containing the member to compare against.
 * @return Each function returns true if the target matches the song member value.
 *
 */
bool is_artist(char *target, song this_song)
{
    return strcmp(this_song.artist_names, target) == 0;
}

bool contains_artist(char *target, song this_song)
{
    return strstr(this_song.artist_names, target) != NULL;
}

bool is_artist_count(int target, song this_song)
{
    return this_song.artist_count == target;
}

bool is_key(char *target, song this_song)
{
    return strcmp(this_song.key, target) == 0;
}

bool is_mode(char *target, song this_song)
{
    return strcmp(this_song.mode, target) == 0;
}

bool is_min_playlists(int target, song this_song)
{
    return this_song.in_spotify_playlists >= target;
}

bool is_year(int target, song this_song)
{
    return this_song.released_year == target;
}

/**
 * @brief Function that answers questions posed using a switch case for the question options.
 *
 * This construct makes it easy to add more search cases simply by using the is_target functions from above and changing or adding cases.
 *
 * @param case_number Represents the question number.
 * @param songs_in The list of songs to which the questions are posed.
 * @param songs_out The list of songs that satisfy the conditions of the question.
 * @param in_count The counter that keeps track of songs_in index.
 * @param out_count The counter that keeps track of songs_out additions.
 * @return Void functions returns no value.
 *
 */
void switch_case_questions(int case_number, song songs_in[], song songs_out[], int in_count, int *out_count)
{

    for (int k = 0; k < in_count; k++)
    {
        switch (case_number)
        {
        case 1:
            // only artist is 'Rae Spoon'
            if (is_artist("Rae Spoon", songs_in[k]) && is_artist_count(1, songs_in[k]))
            {
                songs_out[(*out_count)++] = songs_in[k];
            }
            break;
        case 2:
            // only artist is 'Tate McRae'
            if (is_artist("Tate McRae", songs_in[k]) && is_artist_count(1, songs_in[k]))
            {
                songs_out[(*out_count)++] = songs_in[k];
            }
            break;
        case 3:
            // only artist is 'The Weeknd' && written in Major
            if (is_artist("The Weeknd", songs_in[k]) && is_mode("Major", songs_in[k]))
            {
                songs_out[(*out_count)++] = songs_in[k];
            }
            break;
        case 4:
            // in >50000 playlists && (written in D || A)
            if (is_min_playlists(5000, songs_in[k]) && (is_key("A", songs_in[k]) || is_key("D", songs_in[k])))
            {
                songs_out[(*out_count)++] = songs_in[k];
            }
            break;
        case 5:
            // (realeased in 2021||2022) && 'Drake' is included
            if ((is_year(2021, songs_in[k]) || is_year(2022, songs_in[k])) && contains_artist("Drake", songs_in[k]))
            {
                songs_out[(*out_count)++] = songs_in[k];
            }

            break;
        }
    }
}

/**
 * @brief Function to put answers into the correct output format.
 *
 * @param output The list of songs that will be formatted as output.
 * @param out_count The number of songs to output.
 * @return Void function returns no value 
 *
 */
void output_answers(song output[], int out_count)
{
    FILE *output_file = fopen("output.csv", "w");
    fprintf(output_file, "Artist(s),Song\n");

    for (int i = 0; i < out_count; i++)
    {
        if (strlen(output[i].artist_names) > 0)
        {
            fprintf(output_file, "%s,%s\n", output[i].artist_names, output[i].track_name);
        }
    }

    fclose(output_file);
}
