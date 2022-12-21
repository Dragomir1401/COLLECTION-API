#include "treap.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MIN_MAGNITUDE 100
#define MID_MAGNITUDE 50000
#define MAX_MAGNITUDE 5000000
#define MAX_NUMBER 9000000000
#define MID_NUMBER 50000

long randlong(long lower, long upper)
{
    if (upper - lower + 1 <= 0)
        return 0;
    return (rand() % (upper - lower + 1)) + lower;
}

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

void add_only_test(char *in_file, long magnitude)
{
    FILE *fptr = fopen(in_file, "w");
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    for (long i = 1; i < magnitude; i++)
    {
        fprintf(fptr, "%d %ld\n", 0, randlong(0, 100));
    }
    fclose(fptr);
}

long delete_from_array(long *array, long size)
{
    long randed = randlong(0, size - 1);
    long res = array[randed];

    for (long i = randed; i < size - 1; i++)
        array[i] = array[i + 1];

    return res;
}

long pick_from_array(long *array, long size)
{
    long randed = randlong(0, size - 1);
    long res = array[randed];
    return res;
}

void replace_in_array(long *array, long size, long target, long new)
{
    for (long i = 0; i < size; i++)
        if (array[i] == target)
            array[i] = new;
}

void add_and_remove_test(char *in_file, long magnitude, long delete_frequency)
{
    FILE *fptr = fopen(in_file, "w");
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    long add_counter = 0, remove_counter = 0, *added;
    added = malloc(MID_MAGNITUDE * sizeof(long));

    for (long i = 1; i < magnitude; i++)
    {
        // rand some random
        long randed = randlong(0, 100);
        fprintf(fptr, "%d %ld\n", 0, randed);
        added[add_counter] = randed;
        add_counter++;

        if (add_counter >= remove_counter)
        {

            if (add_counter >= (delete_frequency + 1) * remove_counter)
            {
                fprintf(fptr, "%d %ld\n", 1, delete_from_array(added, add_counter));
                add_counter--;
                remove_counter++;
            }
        }
    }

    free(added);
    fclose(fptr);
}

void add_remove_and_find_test(char *in_file, long magnitude, long delete_frequency, long find_frequency, long find_proportion)
{

    FILE *fptr = fopen(in_file, "w");
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    long add_counter = 0, remove_counter = 0, *added, find_counter = 0;
    added = malloc(MID_MAGNITUDE * sizeof(long));

    for (long i = 1; i < magnitude; i++)
    {
        // rand some random
        long randed = randlong(0, 100);
        fprintf(fptr, "%d %ld\n", 0, randed);

        added[add_counter] = randed;
        add_counter++;

        if (add_counter > remove_counter)
        {
            if (add_counter > (delete_frequency + 1) * remove_counter && add_counter > 0)
            {
                fprintf(fptr, "%d %ld\n", 1, delete_from_array(added, add_counter));
                add_counter--;
                remove_counter++;
            }

            if (add_counter > (find_frequency + 1) * find_counter && add_counter > 0)
            {
                if (find_proportion > 0)
                {
                    fprintf(fptr, "%d %ld\n", 2, pick_from_array(added, add_counter));
                    find_counter++;
                    find_proportion--;
                }
                else
                {
                    fprintf(fptr, "%d %ld\n", 2, randlong(0, 1000));
                    find_counter++;
                }
            }
        }
    }

    free(added);
    fclose(fptr);
}

void add_remove_find_and_replace_test(char *in_file, long magnitude, long delete_frequency,
                                      long find_frequency, long find_proportion,
                                      long replace_frequency, long replace_proportion)
{

    FILE *fptr = fopen(in_file, "w");
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    long add_counter = 0, remove_counter = 0, *added, find_counter = 0, replace_counter = 0;
    added = malloc(MID_MAGNITUDE * sizeof(long));

    for (long i = 1; i < magnitude; i++)
    {
        // rand some random
        long randed = randlong(0, 1000);
        fprintf(fptr, "%d %ld\n", 0, randed);

        added[add_counter] = randed;
        add_counter++;

        if (add_counter > remove_counter)
        {
            if (add_counter > (delete_frequency + 1) * remove_counter && add_counter > 0)
            {
                fprintf(fptr, "%d %ld\n", 1, delete_from_array(added, add_counter));
                add_counter--;
                remove_counter++;
            }

            if (add_counter > (find_frequency + 1) * find_counter && add_counter > 0)
            {
                if (find_proportion > 0)
                {
                    fprintf(fptr, "%d %ld\n", 2, pick_from_array(added, add_counter));
                    find_counter++;
                    find_proportion--;
                }
                else
                {
                    fprintf(fptr, "%d %ld\n", 2, randlong(0, 1000));
                    find_counter++;
                }
            }

            if (add_counter > (replace_frequency + 1) * replace_counter && add_counter > 0)
            {
                if (replace_proportion > 0)
                {
                    long target = pick_from_array(added, add_counter);
                    long new = randlong(0, 1000);
                    fprintf(fptr, "%d %ld %ld\n", 3, target, randlong(0, 1000));
                    replace_in_array(added, add_counter, target, new);
                    replace_counter++;
                    replace_proportion--;
                }
                else
                {
                    fprintf(fptr, "%d %ld %ld\n", 3, randlong(0, 1000), randlong(0, 1000));
                    replace_counter++;
                }
            }
        }
    }

    free(added);
    fclose(fptr);
}

void mix_test(char *in_file, long magnitude, long delete_frequency,
              long find_frequency, long find_proportion,
              long replace_frequency, long replace_proportion,
              long print_proportion, long max_element)
{

    FILE *fptr = fopen(in_file, "w");
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    long add_counter = 0, remove_counter = 0, *added, find_counter = 0, replace_counter = 0;
    added = malloc(MAX_MAGNITUDE * sizeof(long));

    for (long i = 1; i < magnitude; i++)
    {
        // rand some random elements to add
        long randed = randlong(0, max_element);
        fprintf(fptr, "%d %ld\n", 0, randed);

        added[add_counter] = randed;
        add_counter++;

        // generate remove commands with a given frequency
        if (add_counter > remove_counter)
        {
            if (add_counter > (delete_frequency + 1) * remove_counter && add_counter > 0)
            {
                fprintf(fptr, "%d %ld\n", 1, delete_from_array(added, add_counter));
                add_counter--;
                remove_counter++;
            }
        }

        // generate find with elements existing and non-existing
        if (add_counter > (find_frequency + 1) * find_counter && add_counter > 0)
        {
            if (find_proportion > 0 && i < magnitude / 4 && i > magnitude / 2)
            {
                fprintf(fptr, "%d %ld\n", 2, pick_from_array(added, add_counter));
                find_counter++;
                find_proportion--;
            }
            else
            {
                fprintf(fptr, "%d %ld\n", 2, randlong(0, max_element));
                find_counter++;
            }
        }

        // generate replace commands with elements existing and non-existing
        if (add_counter > (replace_frequency + 1) * replace_counter && add_counter > 0)
        {
            if (replace_proportion > 0 && i < magnitude / 3 && i > magnitude / 2)
            {
                long target = pick_from_array(added, add_counter);
                long new = randlong(0, max_element);
                fprintf(fptr, "%d %ld %ld\n", 3, target, randlong(0, max_element));
                replace_in_array(added, add_counter, target, new);
                replace_counter++;
                replace_proportion--;
            }
            else
            {
                fprintf(fptr, "%d %ld %ld\n", 3, randlong(0, max_element), randlong(0, max_element));
                replace_counter++;
            }
        }

        // generate print commands every now and then to the end of the test
        if (i > magnitude / 2 + magnitude / 5 && i % print_proportion == 13)
            fprintf(fptr, "%d\n", 4);
    }

    free(added);
    fclose(fptr);
}

long main()
{
    long *magnitude_array = malloc(30 * sizeof(long));
    for (long i = 0; i < 30; i++)
    {
        if (i < 10)
            magnitude_array[i] = i * 10 + 4;

        if (i == 10)
            magnitude_array[i] = 255;

        if (i > 10 && i <= 20)
            magnitude_array[i] = i * (i - 2) * (i - 5) + 357;

        if (i > 20 && i < 24)
            magnitude_array[i] = i * (i + 1) * (i + 3) + 987;

        if (i >= 24 && i < 26)
            magnitude_array[i] = (i - 15) * (i - 12) * (i - 11) * (i - 9) * 7 + i * i * i + i * i + 2312;

        if (i >= 26)
            magnitude_array[i] = (i - 8) * (i + 20) * i * i * 4 + (i - 3) * i * (i - 2) + i * i + 23712;
    }

    for (long i = 0; i < 30; i++)
    {
        // create input file string
        char *in_string;
        in_string = parse_input_file(i);

        // generate some add only tests
        if (i + 1 == 1 || i + 1 == 3 || i + 1 == 5)
            add_only_test(in_string, magnitude_array[i]);

        // generate add and remove tests with fluctuent frequency
        if (i + 1 == 4 || i + 1 == 2 || i + 1 == 7)
            add_and_remove_test(in_string, magnitude_array[i], (i + 1) % 2);

        // generate add and remove with fix frequency
        if (i + 1 == 8 || i + 1 == 6)
            add_and_remove_test(in_string, magnitude_array[i], 3);

        // generate add, remove and find tests
        if (i + 1 == 9 || i + 1 == 10 || i + 1 == 11)
            add_remove_and_find_test(in_string, magnitude_array[i], randlong(2, 7), randlong(2, 5), magnitude_array[i] / 4);

        // create some add, remove, find and replace tests
        if (i + 1 == 12 || i + 1 == 13 || i + 1 == 14 || i + 1 == 15)
            add_remove_find_and_replace_test(in_string, magnitude_array[i], randlong(2, 9), randlong(2, 5),
                                             magnitude_array[i] / 6, randlong(1, 3), magnitude_array[i] / 3);

        // create mixed commands tests with normal size numbers
        if (i + 1 > 15 && i + 1 <= 20)
            mix_test(in_string, magnitude_array[i], randlong(8, 14), randlong(4, 8), 100, randlong(10, 15), 120, randlong(35, 45), MID_NUMBER);

        // create some mixed commands with bigger numbers of mid size
        if (i + 1 > 20 && i + 1 <= 25)
            mix_test(in_string, magnitude_array[i], randlong(9, 18), randlong(5, 15), 200, randlong(8, 12), 420, randlong(9000, 11000), MAX_NUMBER);

        // create some mixed commands with bigger numbers of big sizes
        if (i + 1 > 25 && i + 1 <= 30)
            mix_test(in_string, magnitude_array[i], randlong(9, 18), randlong(5, 15), 200, randlong(8, 12), 420, randlong(10000, 15000), MAX_NUMBER);

        free(in_string);
    }

    free(magnitude_array);
    return 0;
}