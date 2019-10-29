/** @file image.c
 * @brief Containes all functions.
 *
 * @author Catarina Borges, 73865
 * @author Francisco Aires, 76490
 * @bug
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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
        perror(file);
        exit(1);
    }

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
        img->type = 6;
        img->dataRGB = (RGBPixel**)malloc(img->h * sizeof(RGBPixel*));
        for(int i = 0; i <img->h; i++){
            img->dataRGB[i] = (RGBPixel*)malloc(img->w * sizeof(RGBPixel));
        }
        for(int i = 0; i <img->h; i++){
                fread(img->dataRGB[i], 3, img->w, fp);
        }
    }
    else if(buff[1] == '5'){
        img->type = 5;
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
        }
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
    for(int i = 0; i <img->w; i++)
    {
        img->dataGray[i] = (GrayPixel*)malloc(img->w * sizeof(GrayPixel));
    }
    for(i=0;i<img->h;i++)
    {
        for(j=0;j<img->w;j++)
        {
            if (strcmp(color, "red")==0)
            {
                img->dataGray[i][j].gray = (img->dataRGB[i][j].r);
            } else if (strcmp(color, "green")==0) {
                img->dataGray[i][j].gray = (img->dataRGB[i][j].g);
            } else if (strcmp(color, "blue")==0) {
                img->dataGray[i][j].gray = (img->dataRGB[i][j].b);
            }
        }
    }
}

void addFilter(Image *img , char *filter)
{
    /** Adds an average filter or edge filter to an RGB image
     * @param Image *img which is the image file
     * @param char filter is the filter's type that we want to add
     */
    int i,j;
    if(img){
        if (strcmp(filter, "average")==0){
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
        }


        if (strcmp(filter, "edge")==0) {

            img->dataGray = (GrayPixel**)malloc(img->h * sizeof(GrayPixel*));
            for(int i = 0; i <img->h; i++){
                img->dataGray[i] = (GrayPixel*)malloc(img->w * sizeof(GrayPixel));
            }

            int difference, threshold;
            threshold = 25;

            for (i = 1; i < img->h - 1; i++) {
                for (j = 1; j < img->w - 1; j++) {
                    for (int x = i - 1; x < i + 1; x++) {
                        for (int y = j - 1; y <= j + 1; y++) {
                            img->dataGray[i][j].gray = 0;
                            difference = abs(img->dataRGB[i][j].r - img->dataRGB[x][y].r);
                            if (difference > threshold) {
                                img->dataGray[i][j].gray = difference;
                            }
                            if (abs(img->dataRGB[i][j].g - img->dataRGB[x][y].g) > difference &&
                                abs(img->dataRGB[i][j].g - img->dataRGB[x][y].g) > threshold) {
                                difference = img->dataRGB[i][j].g - img->dataRGB[x][y].g;
                                img->dataGray[i][j].gray = difference;
                            }
                            if (abs(img->dataRGB[i][j].b - img->dataRGB[x][y].b) > difference &&
                                abs(img->dataRGB[i][j].b - img->dataRGB[x][y].b) > threshold) {
                                difference = img->dataRGB[i][j].b - img->dataRGB[x][y].b;
                                img->dataGray[i][j].gray = difference;
                            }
                        }
                    }
                }
            }

        }

    }
}

void waterMark(Image *img, char* watermark,int x, int y)
{
    /** Adds a watermark to an RGB image
     * @param Image *img which is the image file
     * @param char watermark is the watermark image we want to add
     * @param int x,y is the starting y coord of the watermark image
     */
    Image *img2 = readImage(watermark);
    int i,j;
    for(i=x;i < img2->h+x;i++) {
        for (j=y; j < img2->w+y; j++) {
            if(i < img->h && j < img->w) {
                img->dataRGB[i][j].r = img->dataRGB[i][j].r * img2->dataRGB[i - x][j - y].r / 255;
                img->dataRGB[i][j].g = img->dataRGB[i][j].g * img2->dataRGB[i - x][j - y].g / 255;
                img->dataRGB[i][j].b = img->dataRGB[i][j].b * img2->dataRGB[i - x][j - y].b / 255;
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
     *
     */
    printf("\n\n");
    printf("-----------------------------------------------------------------------------\n");
    printf("|\t\t                MENU                \t\t\t\t\t\t\t\t|\n");
    printf("-----------------------------------------------------------------------------\n");
    printf("| 1 : Change Image to Gray\t\t\t\t\t\t\t\t\t\t\t\t\t|\n");
    printf("| 2 : Change Image to Binary\t\t\t\t\t\t\t\t\t\t\t\t|\n");
    printf("| 3 : Change Image intensity\t\t\t\t\t\t\t\t\t\t\t\t|\n");
    printf("| 4 : Split the color image into the grayscale image for channel you choose |\n");
    printf("| 5 : Filter your Image, with an average filter\t\t\t\t\t\t\t\t|\n");
    printf("| 6 : Watermark the Image\t\t\t\t\t\t\t\t\t\t\t\t\t|\n");
    printf("| 0 : Exit\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t|\n");
    printf("----------------------------------------------------------------------------\n\r");
    printf("Choose option: ");
}

void Menu(void)
{
    /** Menu - implements the menu
     *
     */
    unsigned int number = getchar();
    unsigned char image[20], newImage[20], arg[20];

    switch (number)
    {
        case '1':
        {
            printf("\n(filenameRGB) (new_filename):");
            scanf("%s %s", image, newImage);
            Image *img = readImage(image);
            toGrey(img);
            writeGrey(newImage,img);
            printf("%s created!", newImage);
            break;
        }
        case '2':
        {
            printf("\n(filenameGray) (new_filename):");
            scanf("%s %s", image, newImage);

            Image *img = readImage(image);
            toBin(img);
            writeBin(newImage,img);
            printf("%s created!", newImage);
            break;
        }
        case '3':
        {
            int intensity;

            printf("\n(filename) (new_filename) (intensity[-255,255]):");
            scanf("%s %s %d", image, newImage, &intensity);
            if(intensity < -255 || intensity > 255)
            {
                printf("\n Number of intensity invalid!");
                break;
            }
            else{
                Image *img = readImage(image);
                //printf("%s %d",image, img->type);
                if (img->type == 6){
                    changeIntensity(img, intensity);
                    writePPM(newImage,img);
                }
                else if (img->type == 5){
                    changeIntensityGray(img,intensity);
                    writeGrey(newImage,img);
                }
                printf("%s created!", newImage);
                break;
            }
        }
       case '4':
        {
            printf("\n(filename) (new_filename) (color_channel[red,green,blue]):");
            scanf("%s %s %s", image, newImage, arg);
            Image *img = readImage(image);
            toGreySplitted(img, arg);

            writeGrey(newImage,img);
            printf("%s created!", newImage);
            break;
        }
        case '5':
        {
            printf("\n(filename) (new_filename) (filter [average or edge]):");
            scanf("%s %s %s", image, newImage, arg);
            Image *img = readImage(image);
            addFilter(img , arg);

            if (strcmp(arg, "average")==0) {
                writePPM(newImage, img);
            }
            else if (strcmp(arg, "edge")==0) {
                writeGrey(newImage, img);
            }
            printf("%s created!", newImage);
            break;
        }
        case '6':
        {
            int x,y;
            printf("\n(filename) (new_filename) (watermark_filename) (x) (y):");
            scanf("%s %s %s %d %d", image, newImage, arg, &x, &y);
            Image *img = readImage(image);
            waterMark(img, arg, x, y);
            writePPM(newImage, img);
            printf("%s created!", newImage);;
            break;
        }
        case '0':
        {
            printf("\nExiting...");
            exit(0);
        }
        default:
        {
            printMenu();
            break;
        }
    }
}
