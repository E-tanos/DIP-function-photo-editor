#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Image.h"
#include "DIPs.h"

/* Convert image to Black & White */
Image *BlackNWhite(Image *image) {
    assert(image);
    for (unsigned int y = 0; y < image->H; y++) {
        for (unsigned int x = 0; x < image->W; x++) {
            unsigned char gray = (GetPixelR(image, x, y) + 
                                  GetPixelG(image, x, y) + 
                                  GetPixelB(image, x, y)) / 3;
            SetPixelR(image, x, y, gray);
            SetPixelG(image, x, y, gray);
            SetPixelB(image, x, y, gray);
        }
    }
    return image;
}

/* Convert image to Negative */
Image *Negative(Image *image) {
    assert(image);
    for (unsigned int y = 0; y < image->H; y++) {
        for (unsigned int x = 0; x < image->W; x++) {
            SetPixelR(image, x, y, 255 - GetPixelR(image, x, y));
            SetPixelG(image, x, y, 255 - GetPixelG(image, x, y));
            SetPixelB(image, x, y, 255 - GetPixelB(image, x, y));
        }
    }
    return image;
}

/* Apply a Color Filter */
Image *ColorFilter(Image *image, int target_r, int target_g, int target_b, 
                   int threshold, int replace_r, int replace_g, int replace_b) {
    assert(image);
    for (unsigned int y = 0; y < image->H; y++) {
        for (unsigned int x = 0; x < image->W; x++) {
            int r = GetPixelR(image, x, y);
            int g = GetPixelG(image, x, y);
            int b = GetPixelB(image, x, y);

            if (abs(r - target_r) <= threshold &&
                abs(g - target_g) <= threshold &&
                abs(b - target_b) <= threshold) {
                SetPixelR(image, x, y, replace_r);
                SetPixelG(image, x, y, replace_g);
                SetPixelB(image, x, y, replace_b);
            }
        }
    }
    return image;
}

//Edge detection
Image *Edge(Image *image) {
    assert(image);
    
    unsigned int width = ImageWidth(image);
    unsigned int height = ImageHeight(image);

    // Create a new image for the output
    Image *output = CreateImage(width, height);
    if (!output) {
        printf("Error: Could not create output image in Edge function.\n");
        return NULL;
    }

    // Apply edge detection filter
    for (unsigned int y = 1; y < height - 1; y++) {
        for (unsigned int x = 1; x < width - 1; x++) {
            int sumR = -GetPixelR(image, x - 1, y - 1) - GetPixelR(image, x, y - 1) - GetPixelR(image, x + 1, y - 1)
                       - GetPixelR(image, x - 1, y) + 8 * GetPixelR(image, x, y) - GetPixelR(image, x + 1, y)
                       - GetPixelR(image, x - 1, y + 1) - GetPixelR(image, x, y + 1) - GetPixelR(image, x + 1, y + 1);

            int sumG = -GetPixelG(image, x - 1, y - 1) - GetPixelG(image, x, y - 1) - GetPixelG(image, x + 1, y - 1)
                       - GetPixelG(image, x - 1, y) + 8 * GetPixelG(image, x, y) - GetPixelG(image, x + 1, y)
                       - GetPixelG(image, x - 1, y + 1) - GetPixelG(image, x, y + 1) - GetPixelG(image, x + 1, y + 1);

            int sumB = -GetPixelB(image, x - 1, y - 1) - GetPixelB(image, x, y - 1) - GetPixelB(image, x + 1, y - 1)
                       - GetPixelB(image, x - 1, y) + 8 * GetPixelB(image, x, y) - GetPixelB(image, x + 1, y)
                       - GetPixelB(image, x - 1, y + 1) - GetPixelB(image, x, y + 1) - GetPixelB(image, x + 1, y + 1);

            // Apply saturation (clamp values between 0 and 255)
            sumR = sumR > 255 ? 255 : (sumR < 0 ? 0 : sumR);
            sumG = sumG > 255 ? 255 : (sumG < 0 ? 0 : sumG);
            sumB = sumB > 255 ? 255 : (sumB < 0 ? 0 : sumB);

            SetPixelR(output, x, y, (unsigned char)sumR);
            SetPixelG(output, x, y, (unsigned char)sumG);
            SetPixelB(output, x, y, (unsigned char)sumB);
        }
    }

    // Set border pixels to black (0)
    for (unsigned int x = 0; x < width; x++) {
        SetPixelR(output, x, 0, 0);
        SetPixelG(output, x, 0, 0);
        SetPixelB(output, x, 0, 0);
        SetPixelR(output, x, height - 1, 0);
        SetPixelG(output, x, height - 1, 0);
        SetPixelB(output, x, height - 1, 0);
    }
    for (unsigned int y = 0; y < height; y++) {
        SetPixelR(output, 0, y, 0);
        SetPixelG(output, 0, y, 0);
        SetPixelB(output, 0, y, 0);
        SetPixelR(output, width - 1, y, 0);
        SetPixelG(output, width - 1, y, 0);
        SetPixelB(output, width - 1, y, 0);
    }

    DeleteImage(image); // Free old image memory
    return output;
}

/* Flip Image Vertically */
Image *VFlip(Image *image) {
    assert(image);
    for (unsigned int y = 0; y < image->H / 2; y++) {
        for (unsigned int x = 0; x < image->W; x++) {
            unsigned char r = GetPixelR(image, x, y);
            unsigned char g = GetPixelG(image, x, y);
            unsigned char b = GetPixelB(image, x, y);

            SetPixelR(image, x, y, GetPixelR(image, x, image->H - y - 1));
            SetPixelG(image, x, y, GetPixelG(image, x, image->H - y - 1));
            SetPixelB(image, x, y, GetPixelB(image, x, image->H - y - 1));

            SetPixelR(image, x, image->H - y - 1, r);
            SetPixelG(image, x, image->H - y - 1, g);
            SetPixelB(image, x, image->H - y - 1, b);
        }
    }
    return image;
}

Image *HMirror(Image *image) {
    assert(image);

    unsigned int width = ImageWidth(image);
    unsigned int height = ImageHeight(image);
    int x, y;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width / 2; x++) {
            // Swap pixels between left and right
            unsigned char r = GetPixelR(image, x, y);
            unsigned char g = GetPixelG(image, x, y);
            unsigned char b = GetPixelB(image, x, y);

            SetPixelR(image, width - 1 - x, y, r);
            SetPixelG(image, width - 1 - x, y, g);
            SetPixelB(image, width - 1 - x, y, b);
        }
    }
    return image;
}

/* Add border to an image */
Image *AddBorder(Image *image, char color[SLEN], int border_width) {
    assert(image);
    unsigned char borderR = 0, borderG = 0, borderB = 0;

    /* Define colors */
    if (!strcmp(color, "red")) { borderR = 255; }
    else if (!strcmp(color, "green")) { borderG = 255; }
    else if (!strcmp(color, "blue")) { borderB = 255; }
    else if (!strcmp(color, "yellow")) { borderR = borderG = 255; }
    else if (!strcmp(color, "cyan")) { borderG = borderB = 255; }
    else if (!strcmp(color, "pink")) { borderR = 255; borderG = 192; borderB = 203; }
    else if (!strcmp(color, "orange")) { borderR = 255; borderG = 165; }
    else if (!strcmp(color, "black")) { borderR = borderG = borderB = 0; }
    else if (!strcmp(color, "white")) { borderR = borderG = borderB = 255; }

    /* Apply border */
    for (unsigned int y = 0; y < image->H; y++) {
        for (unsigned int x = 0; x < image->W; x++) {
            if (x < border_width || x >= image->W - border_width ||
                y < border_width || y >= image->H - border_width) {
                SetPixelR(image, x, y, borderR);
                SetPixelG(image, x, y, borderG);
                SetPixelB(image, x, y, borderB);
            }
        }
    }
    return image;
}

/* Shuffle Image (Matches Reference Image) */
Image *Shuffle(Image *image) {
    assert(image);
    unsigned int blockW = image->W / 4;  // Width of each block
    unsigned int blockH = image->H / 4;  // Height of each block

    /* Define the new positions of each block based on the reference image */
    int new_positions[16] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

    /* Temporary buffer to store the shuffled image */
    Image *temp = CreateImage(image->W, image->H);

    for (unsigned int i = 0; i < 16; i++) {
        unsigned int oldX = (i % 4) * blockW;
        unsigned int oldY = (i / 4) * blockH;
        unsigned int newX = (new_positions[i] % 4) * blockW;
        unsigned int newY = (new_positions[i] / 4) * blockH;

        for (unsigned int y = 0; y < blockH; y++) {
            for (unsigned int x = 0; x < blockW; x++) {
                /* Copy pixel from original to new shuffled location */
                SetPixelR(temp, newX + x, newY + y, GetPixelR(image, oldX + x, oldY + y));
                SetPixelG(temp, newX + x, newY + y, GetPixelG(image, oldX + x, oldY + y));
                SetPixelB(temp, newX + x, newY + y, GetPixelB(image, oldX + x, oldY + y));
            }
        }
    }

    /* Copy shuffled image back to original image */
    for (unsigned int y = 0; y < image->H; y++) {
        for (unsigned int x = 0; x < image->W; x++) {
            SetPixelR(image, x, y, GetPixelR(temp, x, y));
            SetPixelG(image, x, y, GetPixelG(temp, x, y));
            SetPixelB(image, x, y, GetPixelB(temp, x, y));
        }
    }

    /* Free temporary image */
    DeleteImage(temp);

    return image;
}

/* Pixelate an image */
Image *Pixelate(Image *image, int block_size) {
    assert(image);
    for (unsigned int y = 0; y < image->H; y += block_size) {
        for (unsigned int x = 0; x < image->W; x += block_size) {
            unsigned long sumR = 0, sumG = 0, sumB = 0;
            int count = 0;

            /* Compute average color for block */
            for (unsigned int j = y; j < y + block_size && j < image->H; j++) {
                for (unsigned int i = x; i < x + block_size && i < image->W; i++) {
                    sumR += GetPixelR(image, i, j);
                    sumG += GetPixelG(image, i, j);
                    sumB += GetPixelB(image, i, j);
                    count++;
                }
            }
            unsigned char avgR = sumR / count;
            unsigned char avgG = sumG / count;
            unsigned char avgB = sumB / count;

            /* Assign average color to block */
            for (unsigned int j = y; j < y + block_size && j < image->H; j++) {
                for (unsigned int i = x; i < x + block_size && i < image->W; i++) {
                    SetPixelR(image, i, j, avgR);
                    SetPixelG(image, i, j, avgG);
                    SetPixelB(image, i, j, avgB);
                }
            }
        }
    }
    return image;
}

/* Shift an image */
Image *Shift(Image *image, int shiftX, int shiftY) {
    assert(image);
    Image *shifted = CreateImage(image->W, image->H);
    if (!shifted) return NULL;

    for (unsigned int y = 0; y < image->H; y++) {
        for (unsigned int x = 0; x < image->W; x++) {
            /* Calculate new positions with wrapping */
            unsigned int newX = (x + shiftX) % image->W;
            unsigned int newY = (y + shiftY) % image->H;

            SetPixelR(shifted, newX, newY, GetPixelR(image, x, y));
            SetPixelG(shifted, newX, newY, GetPixelG(image, x, y));
            SetPixelB(shifted, newX, newY, GetPixelB(image, x, y));
        }
    }

    DeleteImage(image);
    return shifted;
}
//eof
