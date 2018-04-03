#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "filter.h"

int main(int argc, char *argv[]) {

    if (argc < 5 )
    {
        printf("\n Usage: ./denoise input.ppm output.ppm N F \n input.ppm is the name of the input file\n output.ppm is the name of the output file \n N specifies the size of the window\n F is the type of filtering");
        return 1;
    }
    const char *input = (argv[1]), *output = (argv[2]), *type = (argv[4]);

    int size = atoi(argv[3]);

    int *width, *height, *max;

    printf("Reading file %c", *input);
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    RGB *readfile = readPPM(input, width, height, max);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("***\t %c read in %e seconds", *input, cpu_time_used);

    filter rtype;

    if ((*type) == 'A')
    {
      printf("Processing %d x %d image using %d x %d and mean filter...", *width, *height, size, size);
      rtype = MEAN;
    }
    else
    {
      printf("Processing %d x %d image using %d x %d and median filter...", *width, *height, size, size);

    }

    start = clock();

    RGB *writefile = denoiseImage (*width, *height, readfile, size, rtype);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("***\t image processed in %e seconds", cpu_time_used);


    printf("Writing file %c", *output);
    start = clock();

    writePPM(output, *width, *height, *max, writefile);
    printf("\n");

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("***\t %c read in %e seconds", *output, cpu_time_used);

    /*
    deallocateBST(bst_root);
    deallocateList(list_head);
    free(A);
     */
    return 0;
}
