#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Image.h"

/* CreateImage: Allocate memory for an image and its color channels */
Image *CreateImage(unsigned int Width, unsigned int Height) {
    Image *image = (Image *)malloc(sizeof(Image));
    if (!image) {
        fprintf(stderr, "Error: Memory allocation for Image struct failed.\n");
        return NULL;
    }

    image->W = Width;
    image->H = Height;

    image->R = (unsigned char *)malloc(Width * Height * sizeof(unsigned char));
    image->G = (unsigned char *)malloc(Width * Height * sizeof(unsigned char));
    image->B = (unsigned char *)malloc(Width * Height * sizeof(unsigned char));

    if (!image->R || !image->G || !image->B) {
        fprintf(stderr, "Error: Memory allocation for image channels failed.\n");
        free(image->R);
        free(image->G);
        free(image->B);
        free(image);
        return NULL;
    }

    return image;
}

/* DeleteImage: Free memory allocated for an image */
void DeleteImage(Image *image) {
    if (image) {
        free(image->R);
        free(image->G);
        free(image->B);
        free(image);
    }
}

/* Get Pixel Functions */
unsigned char GetPixelR(const Image *image, unsigned int x, unsigned int y) {
    assert(image && image->R);
    return image->R[x + y * image->W];
}

unsigned char GetPixelG(const Image *image, unsigned int x, unsigned int y) {
    assert(image && image->G);
    return image->G[x + y * image->W];
}

unsigned char GetPixelB(const Image *image, unsigned int x, unsigned int y) {
    assert(image && image->B);
    return image->B[x + y * image->W];
}

/* Set Pixel Functions */
void SetPixelR(Image *image, unsigned int x, unsigned int y, unsigned char r) {
    assert(image && image->R);
    image->R[x + y * image->W] = r;
}

void SetPixelG(Image *image, unsigned int x, unsigned int y, unsigned char g) {
    assert(image && image->G);
    image->G[x + y * image->W] = g;
}

void SetPixelB(Image *image, unsigned int x, unsigned int y, unsigned char b) {
    assert(image && image->B);
    image->B[x + y * image->W] = b;
}

/* Return Image Width */
unsigned int ImageWidth(const Image *image) {
    assert(image);
    return image->W;
}

/* Return Image Height */
unsigned int ImageHeight(const Image *image) {
    assert(image);
    return image->H;
}
