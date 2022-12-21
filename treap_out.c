#include <stdio.h>
#include <stdlib.h>
#include "treap.h"
#include <string.h>
#define BUFFER_MAX 100
#define MAX_NODES 1000000
char *parse_input_file(long i)
{
    char *string = (char *)malloc(20 * sizeof(char));

    strcpy(string, "./in/test");

    long i_cop = i + 1;
    char c = i_cop + '0';
    if (i_cop <= 9)
        string[9] = c;

    if (i_cop > 9 && i_cop <= 19)
    {
        c = c - 10;
        strcat(string, "10");
        string[10] = c;
    }

    if (i_cop > 19 && i_cop <= 29)
    {
        c = c - 20;
        strcat(string, "20");
        string[10] = c;
    }

    if (i_cop == 30)
        strcat(string, "30");

    if (i_cop <= 9)
        string[10] = '\0';
    else
        string[11] = '\0';

    strcat(string, ".in");

    if (i_cop <= 9)
        string[13] = '\0';
    else
        string[14] = '\0';

    return string;
}

char *parse_output_file(long i)
{
    char *string = (char *)malloc(20 * sizeof(char));

    strcpy(string, "./out/test");

    long i_cop = i + 1;
    char c = i_cop + '0';
    if (i_cop <= 9)
        string[10] = c;

    if (i_cop > 9 && i_cop <= 19)
    {
        c = c - 10;
        strcat(string, "10");
        string[11] = c;
    }

    if (i_cop > 19 && i_cop <= 29)
    {
        c = c - 20;
        strcat(string, "20");
        string[11] = c;
    }

    if (i_cop == 30)
        strcat(string, "30");

    if (i_cop <= 9)
        string[11] = '\0';
    else
        string[12] = '\0';

    strcat(string, ".out_t");

    if (i_cop <= 9)
        string[17] = '\0';
    else
        string[18] = '\0';

    return string;
}

int main()
{

    // iterate through tests
    for (long i = 0; i < 30; i++)
    {
        Treap_tree *treap = treap_create(compare_ints);
        if (i == 25)
        {
            char *in_file = parse_input_file(i);
            char *out_file = parse_output_file(i);
            // open input and output file
            FILE *in = fopen(in_file, "r");
            FILE *out = fopen(out_file, "w");
            char *chunk = malloc(BUFFER_MAX);

            // read line by line
            while (fgets(chunk, BUFFER_MAX, in) != NULL)
            {
                // separate each line
                char *token = strtok(chunk, " ");
                int command = atoi(token);

                int arg1 = 0, arg2 = 0;
                // loop through the string to extract all other tokens
                if (command >= 0 && command <= 3)
                {
                    token = strtok(NULL, " \n");
                    if (token != NULL)
                        arg1 = atoi(token);
                }

                if (token != NULL && command == 3)
                {
                    token = strtok(NULL, " \n");
                    if (token != NULL)
                        arg2 = atoi(token);
                }

                Node *removed_node;
                switch (command)
                {
                case 0:
                    treap_insert(&treap->root, arg1, treap->compar);
                    break;
                case 1:
                    removed_node = treap_pop(&treap->root, treap->compar);
                    free(removed_node);
                    break;
                case 2:
                    if (get_key(treap->root, arg1, treap->compar))
                        fprintf(out, "%d\n", 1);
                    else
                        fprintf(out, "%d\n", 0);

                    break;
                case 3:
                    treap_insert(&treap->root, arg2, treap->compar);
                    break;
                case 4:
                    printTreap(out, treap->root);
                    break;

                default:
                    break;
                }
            }

            free(chunk);
            free(in_file);
            free(out_file);
            fclose(in);
            fclose(out);
        }
        treap_free(treap);
    }
    return 0;
}