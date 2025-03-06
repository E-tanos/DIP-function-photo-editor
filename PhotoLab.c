#include <stdio.h>
#include <string.h>
#include "Image.h"
#include "DIPs.h"
#include "Advanced.h"
#include "FileIO.h"
#include "Constants.h"
#include "Test.h"

/*** function declarations ***/
/* print a menu */
void PrintMenu();

int main() {
    int rc;
    Image *image = NULL;
    
#ifdef DEBUG
    AutoTest();
    rc = 0;
#else
    const char watermarkImage[SLEN] = "watermark_template";
    int option;
    char fname[SLEN];
    char colorOption[SLEN];

    rc = 1;
    PrintMenu();
    printf("Please make your choice: ");
    scanf("%d", &option);

    int target_r, target_g, target_b, threshold;
    double factor_r, factor_g, factor_b;
    int border_width;
    int x, y, W, H;
    int newWidth, newHeight;
    int shiftX, shiftY;
    int rotateDirection;
    int block_size; 

    while (option != 18) {
        if (option == 1) {
            printf("Please input the file name to load: ");
            scanf("%s", fname);
            if (image) {
                DeleteImage(image);
            }
            image = LoadImage(fname);
        } else if (option >= 2 && option <= 17) {
            if (!image) {
                printf("No image is read.\n");
            } else {
                switch (option) {
                    case 2:
                        printf("Please input the file name to save: ");
                        scanf("%s", fname);
                        SaveImage(fname, image);
                        break;
                    case 3:
                        image = BlackNWhite(image);
                        printf("\"Black & White\" operation is done!\n");
                        break;
                    case 4:
                        image = Negative(image);
                        printf("\"Negative\" operation is done!\n");
                        break;
                    case 5:
                        printf("Enter Red component for the target color: ");
                        scanf("%d", &target_r);
                        printf("Enter Green component for the target color: ");
                        scanf("%d", &target_g);
                        printf("Enter Blue component for the target color: ");
                        scanf("%d", &target_b);
                        printf("Enter threshold for the color difference: ");
                        scanf("%d", &threshold);
                        printf("Enter factor for Red component: ");
                        scanf("%lf", &factor_r);
                        printf("Enter factor for Green component: ");
                        scanf("%lf", &factor_g);
                        printf("Enter factor for Blue component: ");
                        scanf("%lf", &factor_b);
                        image = ColorFilter(image, target_r, target_g, target_b, threshold, factor_r, factor_g, factor_b);
                        printf("\"Color Filter\" operation is done!\n");
                        break;
                    case 6:
                        image = Edge(image);
                        printf("\"Edge\" operation is done!\n");
                        break;
                    case 7:
                        image = Shuffle(image);
                        printf("\"Shuffle\" operation is done!\n");
                        break;
                    case 8:
                        image = VFlip(image);
                        printf("\"VFlip\" operation is done!\n");
                        break;
                    case 9:
                        image = HMirror(image);
                        printf("\"Mirror\" operation is done!\n");
                        break;
                    case 10:
                        printf("Enter border width: ");
                        scanf("%d", &border_width);
                        printf("Available border colors: black, white, red, green, blue, yellow, cyan, pink, orange\n");
                        printf("Select border color: ");
                        scanf("%s", colorOption);
                        image = AddBorder(image, colorOption, border_width);
                        printf("\"Border\" operation is done!\n");
                        break;
                    case 11:
                        printf("Enter block size: ");
                        scanf("%d", &block_size);
                        image = Pixelate(image, block_size);
                        printf("\"Pixelate\" operation is done!\n");
                        break;
                    case 12:
                        printf("Enter shift X: ");
                        scanf("%d", &shiftX);
                        printf("Enter shift Y: ");
                        scanf("%d", &shiftY);
                        image = Shift(image, shiftX, shiftY);
                        printf("\"Shift\" operation is done!\n");
                        break;
                    case 13:
                        printf("Please enter the X offset value: ");
                        scanf("%d", &x);
                        printf("Please enter the Y offset value: ");
                        scanf("%d", &y);
                        printf("Please input the crop width: ");
                        scanf("%d", &W);
                        printf("Please input the crop height: ");
                        scanf("%d", &H);
                        image = Crop(image, x, y, W, H);
                        printf("\"Crop\" operation is done!\n");
                        break;
                    case 14:
                        printf("Please input the new image width: ");
                        scanf("%d", &newWidth);
                        printf("Please input the new image height: ");
                        scanf("%d", &newHeight);
                        image = Resize(image, newWidth, newHeight);
                        printf("\"Resize\" operation is done!\n");
                        break;
                    case 15: {
                        Image *watermark_image = LoadImage(watermarkImage);
                        image = Watermark(image, watermark_image);
                        DeleteImage(watermark_image);
                        printf("\"Watermark\" operation is done!\n");
                        break;
                    }
                    case 16:
                        printf("Enter rotation direction (0: clockwise, 1: counterclockwise): ");
                        scanf("%d", &rotateDirection);
                        image = RotateBy90(image, rotateDirection);
                        printf("\"RotateBy90\" operation is done!\n");
                        break;
                    case 17:
                        AutoTest();
                        rc = SUCCESS;
                        break;
                    default:
                        printf("Invalid selection!\n");
                        break;
                }
            }
        }
        PrintMenu();
        printf("Please make your choice: ");
        scanf("%d", &option);
    }
    if (image) {
        DeleteImage(image);
    }
    printf("You exit the program.\n");
#endif
    return 0;
    
    // Check return code
    if (rc == 0) {
        printf("Program executed successfully.\n");
    } else {
        printf("Program encountered an error.\n");
    }
}

/* Menu */
void PrintMenu() {
	printf("\n----------------------------\n");
	printf(" 1: Load a PPM image\n");
	printf(" 2: Save an image in PPM and JPEG format\n");
	printf(" 3: Change a color image to Black & White\n");
	printf(" 4: Make a negative of an image\n");
	printf(" 5: Color filter an image\n");
	printf(" 6: Sketch the edge of an image\n");
	printf(" 7: Shuffle an image\n");
	printf(" 8: Flip an image vertically\n");
	printf(" 9: Mirror an image vertically\n");
	printf("10: Add border to an image\n");
	printf("11: Pixelate an image\n");
	printf("12: Shift an image\n");
	printf("13: Crop an image\n");
	printf("14: Resize an image\n");
	printf("15: Add Watermark to an image\n");
	printf("16: Rotate an image by 90 degree\n");
	printf("17: Test all functions\n");
	printf("18: Exit\n");
}
