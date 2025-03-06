#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include "Image.h"
#include "Advanced.h"

/* Crop an image */
Image *Crop(Image *image, int x, int y, int W, int H) {
    assert(image);

    /* Ensure crop dimensions don't exceed the original image */
    if (x + W > image->W) W = image->W - x;
    if (y + H > image->H) H = image->H - y;

    /* Create the cropped image */
    Image *cropped = CreateImage(W, H);
    if (!cropped) return NULL;

    for (int j = 0; j < H; j++) {
        for (int i = 0; i < W; i++) {
            SetPixelR(cropped, i, j, GetPixelR(image, x + i, y + j));
            SetPixelG(cropped, i, j, GetPixelG(image, x + i, y + j));
            SetPixelB(cropped, i, j, GetPixelB(image, x + i, y + j));
        }
    }

    DeleteImage(image);
    return cropped;
}

/* Resize an image using nearest-neighbor interpolation */
Image *Resize(Image *image, int newWidth, int newHeight) {
    assert(image);
    
    /* Create a new image with resized dimensions */
    Image *resized = CreateImage(newWidth, newHeight);
    if (!resized) return NULL;

    double scaleX = (double)image->W / newWidth;
    double scaleY = (double)image->H / newHeight;

    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            /* Compute the corresponding pixel in the original image */
            int origX = (int)(x * scaleX);
            int origY = (int)(y * scaleY);

            SetPixelR(resized, x, y, GetPixelR(image, origX, origY));
            SetPixelG(resized, x, y, GetPixelG(image, origX, origY));
            SetPixelB(resized, x, y, GetPixelB(image, origX, origY));
        }
    }

    DeleteImage(image);
    return resized;
}

/* Apply Watermark to an Image*/
Image *Watermark(Image *image, const Image *watermark) {
    assert(image);
    assert(watermark);

    unsigned int img_width = ImageWidth(image);
    unsigned int img_height = ImageHeight(image);
    unsigned int wm_width = ImageWidth(watermark);
    unsigned int wm_height = ImageHeight(watermark);

    // Create a new image for the output
    Image *temp = CreateImage(img_width, img_height);
    if (!temp) {
        printf("Error: Could not create temp image in Watermark function.\n");
        return NULL;
    }

    for (unsigned int y = 0; y < img_height; y++) {
        for (unsigned int x = 0; x < img_width; x++) {
            // Get corresponding watermark pixel (tiled if smaller than image)
            unsigned int wm_x = x % wm_width;
            unsigned int wm_y = y % wm_height;

            unsigned char wm_r = GetPixelR(watermark, wm_x, wm_y);
            unsigned char wm_g = GetPixelG(watermark, wm_x, wm_y);
            unsigned char wm_b = GetPixelB(watermark, wm_x, wm_y);

            // Get original image pixel
            unsigned char img_r = GetPixelR(image, x, y);
            unsigned char img_g = GetPixelG(image, x, y);
            unsigned char img_b = GetPixelB(image, x, y);

            // If watermark pixel is black (R=0, G=0, B=0), apply brightness increase
            if (wm_r == 0 && wm_g == 0 && wm_b == 0) {
                img_r = (img_r * 1.45 > 255) ? 255 : (unsigned char)(img_r * 1.45);
                img_g = (img_g * 1.45 > 255) ? 255 : (unsigned char)(img_g * 1.45);
                img_b = (img_b * 1.45 > 255) ? 255 : (unsigned char)(img_b * 1.45);
            }

            // Set new pixel in temp image
            SetPixelR(temp, x, y, img_r);
            SetPixelG(temp, x, y, img_g);
            SetPixelB(temp, x, y, img_b);
        }
    }

    DeleteImage(image);  // Free old image memory
    return temp;
}

/* Rotate an image by 90 degrees clockwise or counterclockwise */
Image *RotateBy90(Image *image, int rotateDirection) {
    assert(image);

    /* New dimensions after rotation */
    unsigned int newWidth = image->H;
    unsigned int newHeight = image->W;
    Image *rotated = CreateImage(newWidth, newHeight);
    if (!rotated) return NULL;

    for (unsigned int y = 0; y < image->H; y++) {
        for (unsigned int x = 0; x < image->W; x++) {
            if (rotateDirection == 0) { // Clockwise
                SetPixelR(rotated, newWidth - y - 1, x, GetPixelR(image, x, y));
                SetPixelG(rotated, newWidth - y - 1, x, GetPixelG(image, x, y));
                SetPixelB(rotated, newWidth - y - 1, x, GetPixelB(image, x, y));
            } else { // Counterclockwise
                SetPixelR(rotated, y, newHeight - x - 1, GetPixelR(image, x, y));
                SetPixelG(rotated, y, newHeight - x - 1, GetPixelG(image, x, y));
                SetPixelB(rotated, y, newHeight - x - 1, GetPixelB(image, x, y));
            }
        }
    }

    DeleteImage(image);
    return rotated;
}
//eof
