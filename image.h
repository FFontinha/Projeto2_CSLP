/** @file image.h
 * @brief Function prototypes for the image.c file
 * This contains the prototypes for the image.c file
 * and the structures we will need.
 * @author Catarina Borges, 73865
 * @author Francisco Aires, 76490
 * @bug No known bugs, but we could improve the code
 * by adding an advanced algorithm in the toBin() function,
 * for example based on histograms, like the Otsu's method.
*/

#ifndef IMAGES_IMAGE_H
#define IMAGES_IMAGE_H


typedef struct  {
    /**
     *Data Structure of RGB Pixel
     * @param r  - Red pixel
     * @param g - Green pixel
     * @param b - Blue pixel */
    unsigned char r,g,b; // 0-255
} RGBPixel;


typedef  struct {
    /**Data Structure of Gray Pixel
     * @param gray - Gray pixel (only one pixel)*/
    unsigned char gray; // 0-255
} GrayPixel;


typedef struct {
    /**Data Structure of Binary Pixel
     * @param bin - Binary pixel takes a binary value 1 or 0*/
    unsigned char bin; // 8 bits (0 or 1)
} BinPixel;

typedef struct {
    /** Data Structure of an Image
     * @param w - image width
     * @param h - image height
     * @param dataRGB - bi-dimensional type RGPPixel
     * @param dataGray - bi-dimensional type GrayPixel
     * @param dataBin - bi-dimensional type BinPixel*/
    int w, h;
    RGBPixel **dataRGB;
    GrayPixel **dataGray;
    BinPixel **dataBin;

} Image;

//Image * CreateImage(int w, int h);
//Image * LoadFromFile(char *);
Image *readImage(char *file);

void changeIntensity(Image *img, int intensity);

void toGrey(Image *img);
void toBin(Image *img);
void toGreySpitted(Image *img, char *color);

void writeGrey(const char *filename, Image *img);
void writePPM(const char *filename, Image *img);
void writeBin(const char *filename, Image *img);

#endif //IMAGES_IMAGE_H


