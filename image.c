#include <stdio.h>
#include<stdlib.h>
#include "image.h"


/*Image * LoadFromFile(char *name)
{
    FILE *fp = fopen(name, "rb");

    int n;
    fread(&n, sizeof(int), 1, fp);

    Image *tmp = CreateImage(n);
    fread(tmp->a, sizeof(int), n, fp);
    tmp->n = n;
    tmp->size = n;

    return tmp;
}*/


int main() {

    FILE *file = NULL;
    char *buffer;
    unsigned long fileLen;

    //Open file
    file = fopen("/home/francisco/UA/CSLP/Lab2/Images/lena.ppm", "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Unable to open file %s", "lena.ppm");
    }


    return 0;
}