/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Copies a BMP piece by piece and resize it
 */
	   
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
	// ensure proper usage
	if (argc != 4)
	{
		printf("Usage: ./resize n infile outfile\n");
		return 1;
	}
	
	// remember n
	int n = atoi(argv[1]);
	if (n < 1 || n > 100)		// check its value
	{
		printf("Error, n must be between 1 and 100\n");
		return -1;
	}

	// remember filenames
	char* infile = argv[2];
	char* outfile = argv[3];

	

	// open input file 
	FILE* inptr = fopen(infile, "r");
	if (inptr == NULL)
	{
		printf("Could not open %s.\n", infile);
		return 2;
	}

	// open output file
	FILE* outptr = fopen(outfile, "w");
	if (outptr == NULL)
	{
		fclose(inptr);
		fprintf(stderr, "Could not create %s.\n", outfile);
		return 3;
	}

	// read infile's BITMAPFILEHEADER
	BITMAPFILEHEADER bf;
	fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

	// read infile's BITMAPINFOHEADER
	BITMAPINFOHEADER bi;
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

	// ensure infile is (likely) a 24-bit uncompressed BMP 4.0
	if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
		bi.biBitCount != 24 || bi.biCompression != 0)
	{
		fclose(outptr);
		fclose(inptr);
		fprintf(stderr, "Unsupported file format.\n");
		return 4;
	}

	/* change INFO and HEADER files */
    // remember infile width
	int in_width = bi.biWidth;		

	// padding for infile scanlines (before changing bi)
	int in_padding = (4 - (in_width * sizeof(RGBTRIPLE)) % 4) % 4;

	// update dimensions
	bi.biHeight *= n;
	bi.biWidth *= n;

	// remember outfile width
	int out_width = bi.biWidth;

	// padding for outfile scanlines (after changing bi)
	int out_padding = (4 - (out_width * sizeof(RGBTRIPLE)) % 4) % 4;

	// update sizes
	bi.biSizeImage = (bi.biWidth * sizeof(RGBTRIPLE) + out_padding) * abs(bi.biHeight);
	bf.bfSize = bi.biSizeImage + 54;

	/* assign changes to INFO and HEADER */
	// write outfile's BITMAPFILEHEADER
	fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

	// write outfile's BITMAPINFOHEADER
	fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

	/* resizing process */
	// temporary storages for one scanline
	RGBTRIPLE triple[bi.biWidth];

	// iterate over infile's scanlines
	for (int count = 0; count < abs(bi.biHeight);)
	{
		/* reading */
		// remember pixels in scanline
		for (int j = 0; j < in_width; j++)
			fread(&triple[j], sizeof(RGBTRIPLE), 1, inptr);		// read RGB triple from infile

		// skip padding in infile
		fseek(inptr, in_padding, SEEK_CUR);

		/* vertical expanding */
		// copy the same scanline n times 
		for (int k = 0; k < n; k++, count++)
		{
			/* horizontal expanding */
			for (int i = 0; i < out_width; i++)		// write pixels in outfile scanline
				for (int index = 0; index < in_width; index++)	// choose the pixel from triple array
					for (int j = 0; j < n; j++, i++)		// write chosen pixel n times
						fwrite(&triple[index], sizeof(RGBTRIPLE), 1, outptr);	// write RGB triple to outfile
			
			// add padding in outfile
			for (int k = 0; k < out_padding; k++)
				fputc(0x00, outptr);
		}
	}

	// close infile
	fclose(inptr);

	// close outfile
	fclose(outptr);

	// that's all folks
	return 0;
}
