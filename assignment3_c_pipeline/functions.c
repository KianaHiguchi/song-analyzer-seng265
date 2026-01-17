/** @file functions.c
 *  @brief Implementation of functions.h
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "emalloc.h"
#include "list.h"

/**
 * @brief Parses command-line arguments and extracts values based on specific flags.
 *
 * This function parses command-line arguments provided in the `argv` array and extracts
 * the values corresponding to specific flags. The extracted values are stored in the
 * pointers provided as arguments to the function.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing command-line arguments.
 * @param data Pointer to store the value of the "--data" argument.
 * @param filter Pointer to store the value of the "--filter" argument.
 * @param value Pointer to store the value of the "--value" argument.
 * @param order_by Pointer to store the value of the "--order_by" argument.
 * @param order Pointer to store the value of the "--order" argument.
 * @param limit Pointer to store the value of the "--limit" argument.
 */
void parse_arg(int argc, char *argv[], char **data, char **filter, char **value,
               char **order_by, char **order, char **limit)
{
    *data = *filter = *value = *order_by = *order = *limit = NULL;

    for (int i = 1; i < argc; i++)
    {
        char *token = strtok(argv[i], "=");
        if (token != NULL)
        {
            if (strcmp(token, "--data") == 0)
            {
                *data = strtok(NULL, "=");
            }
            else if (strcmp(token, "--filter") == 0)
            {
                *filter = strtok(NULL, "=");
            }
            else if (strcmp(token, "--value") == 0)
            {
                *value = strtok(NULL, "=");
            }
            else if (strcmp(token, "--order_by") == 0)
            {
                *order_by = strtok(NULL, "=");
            }
            else if (strcmp(token, "--order") == 0)
            {
                *order = strtok(NULL, "=");
            }
            else if (strcmp(token, "--limit") == 0)
            {
                *limit = strtok(NULL, "=");
            }
        }
    }
}

/**
 * @brief Read each line from a file and create a linked list of lines.
 *
 * This function reads each line from the specified file and creates a linked
 * list of lines, where each node in the list contains a line from the file.
 *
 * @param filename The name of the file to read.
 * @return node_t* A pointer to the head of the linked list.
 */
node_t *turn_data_into_list(const char *filename)
{
    FILE *file = fopen(filename, "r");
    char line[MAX_LINE_LEN];
    node_t *head = NULL;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        node_t *new_node = (node_t *)emalloc(sizeof(node_t));
        new_node->word = strdup(line);
        head = add_end(head, new_node);
    }

    fclose(file);
    return head;
}

/**
 * @brief Parses a line into a song structure.
 *
 * The purpose of the function is as a helper for the following
 * functions so as to avoid creating massive arrays of song structures.
 *
 * @param line The line to parse.
 * @param s The song structure to populate.
 */
void parse_line_to_song(const char *line, song *s)
{
    sscanf(line, "%[^,],%[^,],%d,%d,%d,%d,%d,%ld,%d",
           s->track_name, s->artists_name, &s->artist_count,
           &s->released_year, &s->released_month, &s->released_day,
           &s->in_spotify_playlists, &s->streams, &s->in_apple_playlists);
}

/**
 * @brief Checks a specific field in each node of a linked list and adds nodes with matching criteria to a new list.
 *
 * This function iterates through each node in the provided linked list and checks a specific field in the song structure
 * stored in each node. If the specified field matches the provided target value, the node is added to a new list.
 *
 * @param head The head of the linked list to be searched.
 * @param target The target field to be checked in each node. Supported values are "ARTIST" and "YEAR".
 * @param target_value The value to compare against the target field.
 * @param successful_lines A pointer to the head of the list where matching nodes will be added.
 * @return A pointer to the head of the new list containing nodes with matching criteria.
 */
node_t *check_field_in_linked_list(node_t *head, const char *target, const char *target_value, node_t *successful_lines)
{
    node_t *current = head;

    while (current != NULL)
    {
        song s;
        parse_line_to_song(current->word, &s);
        if (strcmp(target, "ARTIST") == 0)
        {
            if (strstr(s.artists_name, target_value) != NULL)
            {
                successful_lines = add_end(successful_lines, new_node(current->word));
            }
        }
        else if (strcmp(target, "YEAR") == 0)
        {
            if (s.released_year == atoi(target_value))
            {
                successful_lines = add_end(successful_lines, new_node(current->word));
            }
        }
        current = current->next;
    }
    free_list(head);
    return successful_lines;
}

/**
 * @brief Sorts a linked list in ascending order using the Merge Sort algorithm.
 *
 * This function sorts the provided linked list using the Merge Sort algorithm. It recursively divides the list into
 * halves until each sublist contains only one element, then merges the sublists in ascending order.
 *
 * @param head The head of the linked list to be sorted.
 * @param order_by The field by which the sorting should be performed. Supported values are "STREAMS", "NO_SPOTIFY_PLAYLISTS",
 *                 and "NO_APPLE_PLAYLISTS".
 * @return A pointer to the head of the sorted linked list.
 */
node_t *merge_sort(node_t *head, const char *order_by)
{
    if (head == NULL || head->next == NULL)
    {
        return head;
    }

    node_t *slow = head;
    node_t *fast = head->next;
    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    node_t *right = slow->next;
    slow->next = NULL;
    node_t *left_sorted = merge_sort(head, order_by);
    node_t *right_sorted = merge_sort(right, order_by);

    return merge(left_sorted, right_sorted, order_by);
}

/**
 * @brief Merges two sorted linked lists into a single sorted linked list.
 *
 * This function merges two sorted linked lists `left` and `right` into a single sorted linked list based on the specified
 * `order_by` field. It compares nodes of the linked lists based on the provided field and recursively merges them into
 * a single sorted list.
 *
 * @param left A pointer to the head of the left sorted linked list.
 * @param right A pointer to the head of the right sorted linked list.
 * @param order_by The field by which the sorting should be performed. Supported values are "STREAMS", "NO_SPOTIFY_PLAYLISTS",
 *                 and "NO_APPLE_PLAYLISTS".
 * @return A pointer to the head of the merged sorted linked list.
 */
node_t *merge(node_t *left, node_t *right, const char *order_by)
{
    if (left == NULL)
    {
        return right;
    }
    if (right == NULL)
    {
        return left;
    }

    song left_song, right_song;
    parse_line_to_song(left->word, &left_song);
    parse_line_to_song(right->word, &right_song);

    int comparison = 0;
    if (strcmp(order_by, "STREAMS") == 0)
    {
        comparison = left_song.streams - right_song.streams;
    }
    else if (strcmp(order_by, "NO_SPOTIFY_PLAYLISTS") == 0)
    {
        comparison = left_song.in_spotify_playlists - right_song.in_spotify_playlists;
    }
    else if (strcmp(order_by, "NO_APPLE_PLAYLISTS") == 0)
    {
        comparison = left_song.in_apple_playlists - right_song.in_apple_playlists;
    }

    // Merge the lists recursively based on the comparison
    node_t *result = NULL;
    if (comparison <= 0)
    {
        result = left;
        result->next = merge(left->next, right, order_by);
    }
    else
    {
        result = right;
        result->next = merge(left, right->next, order_by);
    }

    return result;
}

/**
 * @brief Limits the number of nodes in a sorted linked list based on the specified order and limit.
 *
 * This function limits the number of nodes in the sorted linked list `sorted_lines` based on the provided `order` and `limit`.
 * If `limit` is `NULL`, it limits the list to the number of nodes already present in the list. If `order` is "ASC", it takes
 * the top `limit` number of nodes from the list, and if `order` is "DES", it takes the bottom `limit` number of nodes from
 * the list (and reverses). The resulting list is returned with the nodes in the same order as specified.
 *
 * @param sorted_lines A pointer to the head of the sorted linked list.
 * @param order The order by which the list should be limited. Supported values are "ASC" and "DES".
 * @param limit The maximum number of nodes to include in the limited list. If `NULL`, the limit is set to the number of nodes already present in the list.
 * @return A pointer to the head of the limited sorted linked list.
 */
node_t *limit_list(node_t *sorted_lines, const char *order, const char *limit)
{
    int lim = 0;
    if (limit == NULL)
    {
        int count = 0;
        apply(sorted_lines, inccounter, &count);
        lim = count;
    }
    else
    {
        lim = atoi(limit);
    }
    node_t *current = sorted_lines;
    node_t *result = NULL;

    if (strcmp(order, "ASC") == 0)
    {
        // Traverse to the specified limit from the start of the list
        int count = 0;
        while (current != NULL && count < lim)
        {
            result = add_end(result, new_node(current->word));
            current = current->next;
            count++;
        }
    }
    else if (strcmp(order, "DES") == 0)
    {
        // Traverse to the specified limit from the end of the list
        int length = 0;
        node_t *temp = sorted_lines;
        while (temp != NULL)
        {
            length++;
            temp = temp->next;
        }
        int start = length - lim;

        int count = 0;
        while (current != NULL && count < start)
        {
            current = current->next;
            count++;
        }
        // Add nodes to a temporary list in correct order
        node_t *temp_list = NULL;
        while (current != NULL)
        {
            temp_list = add_end(temp_list, new_node(current->word));
            current = current->next;
        }
        result = reverse_list(temp_list);
    }
    return result;
}

/**
 * @brief Writes the contents of a linked list to an output file in CSV format.
 *
 * This function writes the contents of the linked list `answer` to an output file named "output.csv" in CSV format.
 * The order of the fields in each row is determined by the specified `order_by` parameter. The output file includes
 * a header row indicating the order of the fields. The fields in each row include the release date (`released`), track
 * name (`track_name`), artist(s) name (`artist(s)_name`), and the value of the field specified by `order_by`.
 *
 * @param answer A pointer to the head of the linked list containing the data to be written to the output file.
 * @param order_by A string indicating the field by which the list should be ordered. Supported values are "STREAMS",
 * "NO_SPOTIFY_PLAYLISTS", and "NO_APPLE_PLAYLISTS".
 */
void write_output_to_file(node_t *answer, const char *order_by)
{
    FILE *output_file = fopen("output.csv", "w");

    if (strcmp(order_by, "STREAMS") == 0)
    {
        fprintf(output_file, "released,track_name,artist(s)_name,streams\n");
    }
    else if (strcmp(order_by, "NO_SPOTIFY_PLAYLISTS") == 0)
    {
        fprintf(output_file, "released,track_name,artist(s)_name,in_spotify_playlists\n");
    }
    else if (strcmp(order_by, "NO_APPLE_PLAYLISTS") == 0)
    {
        fprintf(output_file, "released,track_name,artist(s)_name,in_apple_playlists\n");
    }

    node_t *current = answer;
    while (current != NULL)
    {
        song s;
        parse_line_to_song(current->word, &s);
        // Format the release date without leading zeros for months and days
        fprintf(output_file, "%d-%d-%d,%s,%s,%ld\n", s.released_year, s.released_month, s.released_day,
                s.track_name, s.artists_name, strcmp(order_by, "STREAMS") == 0 ? s.streams : (strcmp(order_by, "NO_SPOTIFY_PLAYLISTS") == 0 ? s.in_spotify_playlists : s.in_apple_playlists));
        current = current->next;
    }

    fclose(output_file);
}
