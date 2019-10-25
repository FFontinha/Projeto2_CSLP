#include <stdio.h>
#include<stdlib.h>
#include "image.h"

Image *readImage(char *file){
    FILE *fp;
    Image *img;
    char buff[16];
    int rgb_comp_color;

    //Open file
    fp = fopen(file, "rb");
    if (!fp)
    {
        fprintf(stderr, "Unable to open file %s", file);
    }

    //read image format
    if (!fgets(buff, sizeof(buff) ,fp)) {
        perror("lena.ppm");
        exit(1);
    }
    /*check the image format
    if (buff[0] != 'P' || buff[1] != '6') {
        fprintf(stderr, "Invalid image format (must be 'P6')\n");
        exit(1);
    }*/


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
        fprintf(stderr, "Invalid image size (error loading '%s')\n", file);
        exit(1);
    }

    //read rgb component
    if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
        fprintf(stderr, "Invalid rgb component (error loading '%s')\n", file);
        exit(1);
    }

    while (fgetc(fp) != '\n') ;
    //memory allocation for pixel data
    if (buff[1] == '6'){
        img->dataRGB = (RGBPixel*)malloc(img->w * img->h * sizeof(RGBPixel));
        if (fread(img->dataRGB, 3 * img->w, img->h, fp) != img->h) {
            fprintf(stderr, "Error loading image '%s'\n", file);
            exit(1);
        }
    }
    else if(buff[1] == '5'){
        img->dataGray = (GrayPixel*)malloc(img->w * img->h * sizeof(GrayPixel));
        if (fread(img->dataGray, img->w, img->h, fp) != img->h) {
            fprintf(stderr, "Error loading image '%s'\n", file);
            exit(1);
        }
    }


    return img;
}

void changeIntensity(Image *img, int intensity)
{
    int i;
    if(img){
        for(i=0;i<img->w*img->h;i++){
            if((img->dataRGB[i].r + intensity) < 0){;
                img->dataRGB[i].r = 0;
            }
            else if((img->dataRGB[i].r + intensity) > 255){
                img->dataRGB[i].r = 255;
            }
            else{
                img->dataRGB[i].r += intensity;
            }
            if((img->dataRGB[i].g + intensity) < 0){
                img->dataRGB[i].g = 0;
            }
            else if((img->dataRGB[i].g + intensity) > 255){
                img->dataRGB[i].g = 255;
            }
            else{
                img->dataRGB[i].g += intensity;
            }
            if((img->dataRGB[i].b + intensity) < 0){
                img->dataRGB[i].b = 0;
            }
            else if((img->dataRGB[i].b + intensity) > 255){
                img->dataRGB[i].b = 255;
            }
            else{
                img->dataRGB[i].b += intensity;
            }
        }
    }
}

void changeIntensityGray(Image *img, int intensity)
{
    int i;
    if(img){
        for(i=0;i<img->w*img->h;i++){
            if((img->dataGray[i].gray + intensity) < 0){;
                img->dataGray[i].gray = 0;
            }
            else if((img->dataGray[i].gray + intensity) > 255){
                img->dataGray[i].gray = 255;
            }
            else{
                img->dataGray[i].gray += intensity;
            }

        }
    }
}

void toGrey(Image *img)
{
    int i;
    img->dataGray = (GrayPixel*)malloc(img->w * img->h * sizeof(GrayPixel));
    if(img){
        for(i=0;i<img->w*img->h;i++){
            img->dataGray[i].gray = (img->dataRGB[i].r + img->dataRGB[i].g + img->dataRGB[i].b)/3;
        }
    }
}

void toGreySpitted(Image *img, char *color)
{
    int i;
    img->dataGray = (GrayPixel*)malloc(img->w * img->h * sizeof(GrayPixel));
    if(img){
        for(i=0;i<img->w*img->h;i++){
            if(color == "red"){
                img->dataGray[i].gray = (img->dataRGB[i].r);
            }
            else if(color == "green"){
                img->dataGray[i].gray = (img->dataRGB[i].g);
            }
            else if(color == "blue"){
                img->dataGray[i].gray = (img->dataRGB[i].b);
            }
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
    fprintf(fp, "P5\n");

    //image size
    fprintf(fp, "%d %d\n",img->h,img->w);

    // rgb component depth
    fprintf(fp, "%d\n",255);

    // pixel data
    fwrite(img->dataGray, 1 * img->h, img->w, fp);
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

    Image *img = readImage("lena.ppm");
    toGrey(img);
    toGreySpitted(img, "red");
    writeGrey("greyred.ppm",img);
    toGreySpitted(img, "green");
    writeGrey("greygreen.ppm",img);
    toGreySpitted(img, "blue");
    writeGrey("greyblue.ppm",img);

    writeGrey("grayLena.ppm",img);
    changeIntensity(img, -40);
    writePPM("intensity.ppm",img);

    Image *img2 = readImage("grayLena.ppm");
    changeIntensityGray(img2, 40);
    writeGrey("grayIntensity.ppm",img2);



    return 0;
}