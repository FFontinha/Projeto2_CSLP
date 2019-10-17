//
// Created by francisco on 10/17/19.
//

#ifndef IMAGES_IMAGE_H
#define IMAGES_IMAGE_H



typedef struct {
    int w, h;
    unsigned char type;

} Image;

typedef struct  {
    unsigned char r,g,b; // 0-255
} RGBPixel;

typedef  struct {
    unsigned char gray; // 0-255
} GrayPixel;

typedef struct {
    unsigned char bin; // 0 or 1
} BinPixel;



//Image * CreateImage(int w, int h);

Image * LoadFromFile(char *);

#endif //IMAGES_IMAGE_H


