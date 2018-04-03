#include "filter.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


RGB *readPPM(const char *file, int *width, int *height, int *max){
  printf("Reading file %s", file);
  clock_t start = clock(), diff;
  FILE *fp;
  fp = fopen(file,"r"); //change this

  int i = 0;
  int c = 0;

  fscanf(fp, "%d %d", width, height);
  *max = fscanf(fp, "%d", &i);

  int size = (*width) * (*height);  //# of elements
  RGB *image = malloc(size*sizeof(RGB));

  while(!feof (fp)){
    fscanf(fp, "%d", &i);
    image[c].r = i;
    fscanf(fp, "%d", &i);
    image[c].g = i;
    fscanf(fp, "%d", &i);
    image[c].b = i;
    c++;
  }
  fclose(fp);
  diff = clock()-start;
  printf("%s read in %ld", file, diff);
  return (image);
}

void writePPM(const char *file, int width, int height, int max, const RGB *image){

  FILE *fp;
  fp = fopen(file,"w"); //change this

  fprintf(fp, "P3\n");
  fprintf(fp, "%d ", width);
  fprintf(fp, "%d\n", height);
  fprintf(fp, "%d\n", max);

  int size = width*height;

  for(int j = 1; j<=size; j++){
      fprintf(fp, "%d ", image[j-1].r);
      fprintf(fp, "%d ", image[j-1].g);
      fprintf(fp, "%d\t", image[j-1].b);

      if(j%(width) == 0){
        fprintf(fp, "\n");
      }
  }

  fclose(fp);
}

RGB *denoiseImage(int width, int height, const RGB *image, int n, filter f){
  int size = (width) * (height);
  RGB *image2 = malloc(size*sizeof(RGB));
  if(f==MEAN)  //mean filter
  {
    int i = 14;
    int n = 5;
    int average = 0;
    int width = 6;
    int right, left, top, bottom;
    right = 0;
    left = 0;
    top = 0;
    bottom = 0;


/*
    if(((i+width+(n/2)) > (width*height)) || ((i-width-(n/2)) < 0) || ((i%width - n/2) <= width) || ((i%width + n/2) >= width) )
    {

    }*/

    //K
    int size = width*height;
    int min = 0;
    int max = 0;
    int divid= n*n;
    for(int i = 0 ; i < size ; i++ ){

      int totalr = 0;
      int totalg = 0;
      int totalb = 0;

      totalr += image[i].r;
      totalg += image[i].g;
      totalb += image[i].b;


    for (int j = 1; j <= (n/2); j++){
      min = i - (width*j);
      if(((min+width+(n/2)) > (width*height)) || ((min-width-(n/2)) < 0) || ((min%width - n/2) <= width) || ((min%width + n/2) >= width) )
      {
        divid-=1;
      }
      else
      {
        totalr += image[min].r;
        totalg += image[min].g;
        totalb += image[min].b;
      }
      max = i + (width*j);
      if(((max+width+(n/2)) > (width*height)) || ((max-width-(n/2)) < 0) || ((max%width - n/2) <= width) || ((max%width + n/2) >= width) )
      {
        divid-=1;
      }
      else
      {
        totalr += image[max].r;
        totalg += image[max].g;
        totalb += image[max].b;
      }
      for (int a = 1; a <= (n/2); a++)
      {
        if(((max-a+width+(n/2)) > (width*height)) || ((max-a-width-(n/2)) < 0) || (((max-a)%width - n/2) <= width) || (((max-a)%width + n/2) >= width) )
        {
          divid-=1;
        }
        else
        {
          totalr += image[max - a].r;
          totalg += image[max - a].g;
          totalb += image[max - a].b;
        }
        if(((max+a+width+(n/2)) > (width*height)) || ((max+a-width-(n/2)) < 0) || (((max+a)%width - n/2) <= width) || (((max+a)%width + n/2) >= width) )
        {
          divid-=1;
        }
        else
        {
          totalr += image[max + a].r;
          totalg += image[max + a].g;
          totalb += image[max + a].b;
        }
        if(((min-a+width+(n/2)) > (width*height)) || ((min-a-width-(n/2)) < 0) || (((min-a)%width - n/2) <= width) || (((min-a)%width + n/2) >= width) )
        {
          divid-=1;
        }
        else
        {
          totalr += image[min - a].r;
          totalg += image[min - a].g;
          totalb += image[min - a].b;
        }
        if((((min+a)+width+(n/2)) > (width*height)) || ((min+a-width-(n/2)) < 0) || (((min+a)%width - n/2) <= width) || (((min+a)%width + n/2) >= width) )
        {
          divid-=1;
        }
        else
        {
          totalr += image[min + a].r;
          totalg += image[min + a].g;
          totalb += image[min + a].b;
        }
      }
    }

    for (int c = 1; c<=(n/2); c++)
        {
          if(((i-c+width+(n/2)) > (width*height)) || ((i-c-width-(n/2)) < 0) || (((i-c)%width - n/2) <= width) || (((i-c)%width + n/2) >= width) )
          {
            divid-=1;
          }
          else
          {
            totalr += image[i - c].r;
            totalg += image[i - c].g;
            totalb += image[i - c].b;
          }
          if(((i+c+width+(n/2)) > (width*height)) || ((i+c-width-(n/2)) < 0) || (((i+c)%width - n/2) <= width) || (((i+c)%width + n/2) >= width) )
          {
            divid-=1;
          }
          else{
            totalr += image[i + c].r;
            totalg += image[i + c].g;
            totalb += image[i + c].b;
          }
        }

      int avgr = (totalr)/(divid);
      int avgg = (totalg)/(divid);
      int avgb = (totalb)/(divid);

      image2[i].r = avgr;
      image2[i].g = avgg;
      image2[i].b = avgb;

    }
    return image2;
  }
}
