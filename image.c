#include <stdio.h>
#include<stdlib.h>
#include<stdbool.h>
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
        img->dataRGB = (RGBPixel**)malloc(img->h * sizeof(RGBPixel*));
        for(int i = 0; i <img->h; i++){
            img->dataRGB[i] = (RGBPixel*)malloc(img->w * sizeof(RGBPixel));
        }
        for(int i = 0; i <img->h; i++){
                fread(img->dataRGB[i], 3, img->w, fp);
        }
    }
    else if(buff[1] == '5'){
        img->dataGray = (GrayPixel**)malloc(img->h * sizeof(GrayPixel*));
        for(int i = 0; i <img->h; i++){
            img->dataGray[i] = (GrayPixel*)malloc(img->w * sizeof(GrayPixel));
        }
        for(int i = 0; i <img->w; i++){
            fread(img->dataGray[i], 1 , img->w, fp);
        }
    }

    return img;
}

void changeIntensity(Image *img, int intensity)
{
    int i,j;
    if(img){
        for(i=0;i<img->h;i++){
            for(j=0;j<img->w;j++) {
                if ((img->dataRGB[i][j].r + intensity) < 0) { ;
                    img->dataRGB[i][j].r = 0;
                } else if ((img->dataRGB[i][j].r + intensity) > 255) {
                    img->dataRGB[i][j].r = 255;
                } else {
                    img->dataRGB[i][j].r += intensity;
                }
                if ((img->dataRGB[i][j].g + intensity) < 0) {
                    img->dataRGB[i][j].g = 0;
                } else if ((img->dataRGB[i][j].g + intensity) > 255) {
                    img->dataRGB[i][j].g = 255;
                } else {
                    img->dataRGB[i][j].g += intensity;
                }
                if ((img->dataRGB[i][j].b + intensity) < 0) {
                    img->dataRGB[i][j].b = 0;
                } else if ((img->dataRGB[i][j].b + intensity) > 255) {
                    img->dataRGB[i][j].b = 255;
                } else {
                    img->dataRGB[i][j].b += intensity;
                }
            }
        }
    }
}

void changeIntensityGray(Image *img, int intensity)
{
    int i,j;
    for(i=0;i<img->h;i++){
        for(j=0;j<img->w;j++) {
            if ((img->dataGray[i][j].gray + intensity) < 0) { ;
                img->dataGray[i][j].gray = 0;
            } else if ((img->dataGray[i][j].gray + intensity) > 255) {
                img->dataGray[i][j].gray = 255;
            } else {
                img->dataGray[i][j].gray += intensity;
            }
        }
    }
}

void toGrey(Image *img)
{
    int i,j;
    img->dataGray = (GrayPixel**)malloc(img->h * sizeof(GrayPixel*));
    for(i = 0; i <img->w; i++){
        img->dataGray[i] = (GrayPixel*)malloc(img->w * sizeof(GrayPixel));
    }
    for(i=0;i<img->h;i++){
        for(j=0;j<img->w;j++) {
            img->dataGray[i][j].gray = (img->dataRGB[i][j].r + img->dataRGB[i][j].g + img->dataRGB[i][j].b) / 3;
        }
    }

}

/*
array;
for(i=0;i<img->h;i++){
    for(j=0;j<img->w;j++) {
        array[img->dataGray[i][j]] += 1;
    }
}
 */





void toBin(Image *img)
{
    int i,j,l;
    int bits = 0;
    unsigned char byte[8] ;
    unsigned char output = 0;
    img->dataBin = (BinPixel**)malloc(img->h * sizeof(BinPixel*));
    for(i = 0; i <img->w; i++){
        img->dataBin[i] = (BinPixel*)malloc(img->w * sizeof(BinPixel));
    }

    for(i=0;i<img->h;i++) {
        l = 0;
        for (j = 0; j < img->w; j++) {
            if (img->dataGray[i][j].gray < 127) {
                byte[bits] = 1;
            } else if (img->dataGray[i][j].gray >= 127) {
                byte[bits] = 0;
            }
            bits += 1;
            if (bits == 8) {
                bits = 0;
                for (int k = 0; k < 8; k++) {
                    //printf("%d",byte[k]);
                    if (byte[k] == 1) {
                        output |= (1 << (7 - k));
                    }
                }
                img->dataBin[i][l].bin = output;
                l += 1;
                output = 0;

            };
            //printf("\n");
        }
        printf("\n");
    }
}


void toGreySpitted(Image *img, char *color)
{
    int i,j;
    img->dataGray = (GrayPixel**)malloc(img->h * sizeof(GrayPixel*));
    for(int i = 0; i <img->w; i++){
        img->dataGray[i] = (GrayPixel*)malloc(img->w * sizeof(GrayPixel));
    }

    for(i=0;i<img->h;i++){
        for(j=0;j<img->w;j++) {
            if (color == "red") {
                img->dataGray[i][j].gray = (img->dataRGB[i][j].r);
            } else if (color == "green") {
                img->dataGray[i][j].gray = (img->dataRGB[i][j].g);
            } else if (color == "blue") {
                img->dataGray[i][j].gray = (img->dataRGB[i][j].b);
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
    for(int i = 0; i <img->h; i++){
        fwrite(img->dataGray[i], sizeof(GrayPixel), img->w, fp);
    }
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
    for(int i = 0; i <img->h; i++){
        fwrite(img->dataRGB[i], sizeof(RGBPixel), img->w, fp);
    }
    fclose(fp);
}

void writeBin(const char *filename, Image *img)
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

    // pixel data
    for(int i = 0; i < img->h; i++){
        fwrite(img->dataBin[i], sizeof(BinPixel), img->w / 8, fp);
    }
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
    toBin(img2);
    writeBin("binLena.pbm",img2);


    return 0;
}