/** @file image.c
 * @brief Contains the main() and all the other functions.
 *
 * @author Catarina Borges, 73865
 * @author Francisco Aires, 76490
 * @bug
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "image.h"

Image *readImage(char *file){
        /** Reads the image file, allocates memory for pixel data
         * for each type of image, P6, P5 and binary.
         * @param char *file
         */
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

    //alloc memory from image
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

    while (fgetc(fp) != '\n');

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
    /** Changes the intensity of RGB images
     * @param Image *img which is the image file
     * @param int intensity is the intensity we want to change it to
     */
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
     /** changeIntensityGray changes the intensity of Gray images
     * @param Image *img which is the image file
     * @param int intensity is the intensity we want to change it to
     */
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
    /** toGrey changes the image to Gray
    * @param Image *img which is the image file we want to change to gray
    */
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
    /** toBin changes an image to binary
    * @param Image *img which is the image file we want to change
    */
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


void toGreySplitted(Image *img, char *color)
{
    /** toGreySpitted splits a color image into three grayscale images for each channel
     * red, green and blue
     * @param Image *img
     * @param char *color - defines the channel color
     *
     */
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

void addFilter(Image *img)
{
    /** Changes the intensity of RGB images
     * @param Image *img which is the image file
     * @param int intensity is the intensity we want to change it to
     */
    int i,j;
    if(img){
        /*
        for(i=1;i < img->h - 1;i++){
            for(j=1;j < img->w - 1 ;j++) {
                img->dataRGB[i][j].r = (
                        img->dataRGB[i-1][j-1].r + img->dataRGB[i-1][j].r + img->dataRGB[i-1][j+1].r +
                        img->dataRGB[i][j-1].r + img->dataRGB[i][j].r + img->dataRGB[i][j+1].r +
                        img->dataRGB[i+1][j-1].r + img->dataRGB[i+1][j].r + img->dataRGB[i+1][j+1].r
                        ) / 9;
                img->dataRGB[i][j].g = (
                        img->dataRGB[i-1][j-1].g + img->dataRGB[i-1][j].g + img->dataRGB[i-1][j+1].g +
                        img->dataRGB[i][j-1].g + img->dataRGB[i][j].g + img->dataRGB[i][j+1].g +
                        img->dataRGB[i+1][j-1].g + img->dataRGB[i+1][j].g + img->dataRGB[i+1][j+1].g
                        ) / 9;
                img->dataRGB[i][j].b = (
                        img->dataRGB[i-1][j-1].b + img->dataRGB[i-1][j].b + img->dataRGB[i-1][j+1].b +
                        img->dataRGB[i][j-1].b + img->dataRGB[i][j].b + img->dataRGB[i][j+1].b +
                        img->dataRGB[i+1][j-1].b + img->dataRGB[i+1][j].b + img->dataRGB[i+1][j+1].b
                        ) / 9;
            }
        }
         */
        int difference, threshold;
        threshold = 25;
        for(i=1; i < img->h - 1;i++) {
            for (j = 1; j < img->w - 1; j++) {
                for(int x = i-1; x < i+1;x++) {
                    for (int y = j-1; y <= j+1; y++) {
                        img->dataGray[i][j].gray = 0;
                        difference = abs(img->dataRGB[i][j].r - img->dataRGB[x][y].r);
                        if (difference > threshold) {
                            img->dataGray[i][j].gray = difference;
                        }
                        if (abs(img->dataRGB[i][j].g - img->dataRGB[x][y].g) > difference && abs(img->dataRGB[i][j].g - img->dataRGB[x][y].g) > threshold){
                            difference = img->dataRGB[i][j].g - img->dataRGB[x][y].g;
                            img->dataGray[i][j].gray = difference;
                        }
                        if (abs(img->dataRGB[i][j].b - img->dataRGB[x][y].b) > difference && abs(img->dataRGB[i][j].b - img->dataRGB[x][y].b) > threshold) {
                            difference = img->dataRGB[i][j].b - img->dataRGB[x][y].b;
                            img->dataGray[i][j].gray = difference;
                        }
                    }
                }

            }
        }
    }
}

void writeGrey(const char *filename, Image *img)
{
    /** writeGrey writes/saves the new gray image
     * @param const char *filename
     * @param Image *img
     */
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
    /** writePPM writes/saves the new RGB image
     * @param const char *filename
     * @param Image *img
     */
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
    /** writeBin writes/saves the new binary image
     * @param const char *filename
     * @param Image *img
     */
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

void printMenu(void)
{
    /** Prints the Menu()
     * @param none
     *
     */
    printf("\n\n");
    printf("-------------------------------------\n");
    printf("|                MENU                |\n");
    printf("-------------------------------------\n");
    printf("| 1 : Change Image to Gray\t|\n");
    printf("| 2 : Change Image to RGB\t|\n");
    printf("| 3 : Change Image to Binary\t|\n");
    printf("| 4 : Change Image intensity\t|\n");
    printf("| 5 : Filter your Image, with an average filter|\n");
    printf("| 6 : Watermark the Image|\n");
    printf("| 0 : Exit\t\t\t|\n");
    printf("-------------------------------------\n\n\r");
}

void Menu(void)
{
    /** Menu
     * @param none
     *
     */
     unsigned int number = getchar();

     switch (number)
     {
         case 1:;
     }
}
int main() {
    /** main function
     * @param none
     * @return nothing
     */

    Image *img = readImage("lena.ppm");
    toGrey(img);
    toGreySplitted(img, "red");
    writeGrey("greyred.ppm",img);
    toGreySplitted(img, "green");
    writeGrey("greygreen.ppm",img);
    toGreySplitted(img, "blue");
    writeGrey("greyblue.ppm",img);

    writeGrey("grayLena.ppm",img);
    //changeIntensity(img, -40);
    //writePPM("intensity.ppm",img);
    addFilter(img);
    writeGrey("filteredLena.ppm",img);


    Image *img2 = readImage("grayLena.ppm");
    changeIntensityGray(img2, 40);
    writeGrey("grayIntensity.ppm",img2);
    toBin(img2);
    writeBin("binLena.pbm",img2);


    /*
    printMenu();

    while(1)
    {
        Menu();
    }
     */
    return 0;
}