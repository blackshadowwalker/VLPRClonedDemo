#include "StdAfx.h"
#include "VideoUtil.h"
#include "FileUtil.h"

VideoUtil::VideoUtil(void)
{
}

VideoUtil::~VideoUtil(void)
{
}

#define WIDTHSTEP(pixels_width)  (((pixels_width) *3 +3) / 4 *4)

int VideoUtil::write24BitBmpFile(char *filename, unsigned int width, unsigned int height, unsigned char *image,bool isBGR,int widthstep)  
{  
    BITMAPINFOHEADER bmpInfoHeader;  
    BITMAPFILEHEADER bmpFileHeader;  
    FILE *filep;  
    unsigned int row, column;  
    unsigned int extrabytes, bytesize;  
    unsigned char *paddedImage = NULL, *paddedImagePtr, *imagePtr;  
  
  
    /* The .bmp format requires that the image data is aligned on a 4 byte boundary.  For 24 - bit bitmaps, 
       this means that the width of the bitmap  * 3 must be a multiple of 4. This code determines 
       the extra padding needed to meet this requirement. */  
   extrabytes = (4 - (width * 3) % 4) % 4;  
  
  
    // This is the size of the padded bitmap  
    bytesize = (width * 3 + extrabytes) * height;  
  
  
    // Fill the bitmap file header structure  
    bmpFileHeader.bfType = 'MB';   // Bitmap header  
    bmpFileHeader.bfSize = 0;      // This can be 0 for BI_RGB bitmaps  
    bmpFileHeader.bfReserved1 = 0;  
    bmpFileHeader.bfReserved2 = 0;  
    bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);  
  
  
    // Fill the bitmap info structure  
    bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);  
    bmpInfoHeader.biWidth = width;  
    bmpInfoHeader.biHeight = height;  
    bmpInfoHeader.biPlanes = 1;  
    bmpInfoHeader.biBitCount = 24;            // 8 - bit bitmap  
    bmpInfoHeader.biCompression = BI_RGB;  
    bmpInfoHeader.biSizeImage = bytesize;     // includes padding for 4 byte alignment  
    bmpInfoHeader.biXPelsPerMeter = 0;  
    bmpInfoHeader.biYPelsPerMeter = 0;  
    bmpInfoHeader.biClrUsed = 0;  
    bmpInfoHeader.biClrImportant = 0;  
  
  
  
  
    // Open file  
    if ((filep = fopen(filename, "wb")) == NULL) {  
        printf("Error opening file %s\n", filename);  
        return FALSE;  
    }  
  
  
    // Write bmp file header  
    if (fwrite(&bmpFileHeader, 1, sizeof(BITMAPFILEHEADER), filep) != sizeof(BITMAPFILEHEADER)) {  
        printf("Error writing bitmap file header\n");  
        fclose(filep);  
        return FALSE;  
    }  
  
  
    // Write bmp info header  
    if (fwrite(&bmpInfoHeader, 1, sizeof(BITMAPINFOHEADER), filep) != sizeof(BITMAPINFOHEADER)) {  
        printf("Error writing bitmap info header\n");  
        fclose(filep);  
        return FALSE;  
    }  
  
  
    // Allocate memory for some temporary storage  
    paddedImage = (unsigned char *)calloc(sizeof(unsigned char), bytesize);  
    if (paddedImage == NULL) {  
        printf("Error allocating memory \n");  
        fclose(filep);  
        return FALSE;  
    }  
  
  
    /* This code does three things.  First, it flips the image data upside down, as the .bmp 
       format requires an upside down image.  Second, it pads the image data with extrabytes  
        number of bytes so that the width in bytes of the image data that is written to the 
        file is a multiple of 4.  Finally, it swaps (r, g, b) for (b, g, r).  This is another 
        quirk of the .bmp file format. */  
  
    for (row = 0; row <  height; row++) {  
       
		if(widthstep > width)
			 imagePtr = image + (height - 1 - row) * WIDTHSTEP(width) ;  
		else
			 imagePtr = image + (height - 1 - row) * width * 3;  

        paddedImagePtr = paddedImage + row * (width * 3 + extrabytes);  
	//	debug("row: %d  column ", row);
        for (column = 0; column < width; column++) {  
			if(isBGR == true)
			{
				*paddedImagePtr =   *imagePtr ;  
				*(paddedImagePtr + 1) = *(imagePtr + 1);   
				*(paddedImagePtr + 2) =  *(imagePtr + 2);

			}
			else
			{
				*paddedImagePtr = *(imagePtr + 2);  
				*(paddedImagePtr + 1) = *(imagePtr + 1);   
				*(paddedImagePtr + 2) = *imagePtr;
			}
            imagePtr += 3;  
            paddedImagePtr += 3;  
        }  
    }  
  
  
    // Write bmp data  
    if (fwrite(paddedImage, 1, bytesize, filep) != bytesize) {  
        printf("Error writing bitmap data\n");  
        free(paddedImage);  
        fclose(filep);  
        return FALSE;  
    }  
  
  
    // Close file  
    fclose(filep);  
    free(paddedImage);  
    return TRUE;  
} 