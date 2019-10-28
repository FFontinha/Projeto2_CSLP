
#include "image.h"

int main() {
    /** main function
     * @param none
     * @return nothing
     */

    Image *img = readImage("lena.ppm");
    /*toGrey(img);
    toGreySplitted(img, "red");
    writeGrey("greyred.ppm",img);
    toGreySplitted(img, "green");
    writeGrey("greygreen.ppm",img);
    toGreySplitted(img, "blue");
    writeGrey("greyblue.ppm",img);

    writeGrey("grayLena.ppm",img);*/
    changeIntensity(img, -40);
    writePPM("intensity.ppm",img);
    /*
    addFilter(img, "filtro");
    writeGrey("filteredLena.ppm",img);
    waterMark(img,"house.ppm");
    writePPM("watermarkedLena.ppm", img);


    Image *img2 = readImage("grayLena.ppm");
    changeIntensityGray(img2, 40);
    writeGrey("grayIntensity.ppm",img2);
    toBin(img2);
    writeBin("binLena.pbm",img2);
*/
    printMenu();

    while(1)
    {
        Menu();
    }

    return 0;
}