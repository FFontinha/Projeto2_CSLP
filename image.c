#include <stdio.h>
#include<stdlib.h>
#include "image.h"


void changeColorPPM(Image *img)
{
    int i;
    if(img){
        for(i=0;i<img->w*img->h;i++){
            img->dataRGB[i].r=255-img->dataRGB[i].r;
            img->dataRGB[i].g=255-img->dataRGB[i].g;
            img->dataRGB[i].b=255-img->dataRGB[i].b;
        }
    }
}

void toGrey(Image *img)
{
    int i, media;
    if(img){
        for(i=0;i<img->w*img->h;i++){
            media = (img->dataRGB[i].r + img->dataRGB[i].g + img->dataRGB[i].b)/3;
            img->dataRGB[i].r=media;
        }
    }
}

void writeGrey(const char *filename, Image *img)
{
    FILE *fp;
    //open file for output
    fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }

    //write the header file
    //image format
    fprintf(fp, "P4\n");

    //image size
    fprintf(fp, "%d %d\n",img->h,img->w);

    // rgb component depth
    fprintf(fp, "%d\n",255);

    // pixel data
    fwrite(img->dataRGB, 1 * img->h, img->w, fp);
    fclose(fp);
}

void writePPM(const char *filename, Image *img)
{
    FILE *fp;
    //open file for output
    fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Unable to open file '%s'\n", filename);
        exit(1);
    }

    //write the header file
    //image format
    fprintf(fp, "P6\n");

    //image size
    fprintf(fp, "%d %d\n",img->h,img->w);

    // rgb component depth
    fprintf(fp, "%d\n",255);

    // pixel data
    fwrite(img->dataRGB, 3 * img->h, img->w, fp);
    fclose(fp);
}


int main() {

    FILE *fp;
    Image *img;
    char buff[16];
    int rgb_comp_color;

    //Open file
    fp = fopen("/home/francisco/UA/CSLP/Lab2/Images/lena.ppm", "rb");
    if (!fp)
    {
        fprintf(stderr, "Unable to open file %s", "lena.ppm");
    }

    //read image format
    if (!fgets(buff, sizeof(buff) ,fp)) {
        perror("lena.ppm");
        exit(1);
    }

    //check the image format
    if (buff[0] != 'P' || buff[1] != '6') {
        fprintf(stderr, "Invalid image format (must be 'P6')\n");
        exit(1);
    }


    //alloc memory form image
    img = (Image *)malloc(sizeof(Image));
    if (!img) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }

    int c;
    //check for comments
    c = getc(fp);
    while (c == '#') {
        while (getc(fp) != '\n') ;
        c = getc(fp);
    }

    ungetc(c, fp);

    //read image size information
    if (fscanf(fp, "%d %d", &img->w, &img->h) != 2) {
        fprintf(stderr, "Invalid image size (error loading '%s')\n", "lena.ppm");
        exit(1);
    }

    //read rgb component
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
        fprintf(stderr, "Invalid rgb component (error loading '%s')\n", "lena.ppm");
        exit(1);
    }

    while (fgetc(fp) != '\n') ;
    //memory allocation for pixel data
    img->dataRGB = (RGBPixel*)malloc(img->w * img->h * sizeof(RGBPixel));

    if (fread(img->dataRGB, 3 * img->w, img->h, fp) != img->h) {
        fprintf(stderr, "Error loading image '%s'\n", "lena.ppm");
        exit(1);
    }

    printf("%d",img->dataRGB[0].r);

    toGrey(img);
    writePPM("test.ppm",img);


    return 0;
}