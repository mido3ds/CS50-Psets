/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 * By Mahmoud Adas
 */

 #include <stdio.h>
 #include <stdbool.h>

 typedef unsigned char BYTE;
 #define BLOCK_SIZE 512

bool is_jpeg(BYTE block[BLOCK_SIZE]);
FILE* new_jpeg(int* num);

int count = 0;      // to name files

// save temporarily one block
BYTE buffer[BLOCK_SIZE];

int main(void)
{
    // open raw and out files
    FILE* raw = fopen("card.raw", "r");
    if (raw == NULL)
    {
        printf("Couldn't open file \n'");
        return -1;
    }
    FILE* out = NULL;

    // detect if we are writing on a file already or not
    bool have_file = false;

    // write out files 
    while (fread(buffer, sizeof(buffer), 1, raw) == 1)                // read one block   
    {
        // check if block is for new JPEG
        if (is_jpeg(buffer))
        {
            if (have_file)
            {
                fclose(out);        // close the old one, if any
            }
            out = new_jpeg(&count);         // create new out file
            if (out == NULL)        // check if valid
                return -2;
            have_file = true;       // indicates we are writing on a file now
        }
        // skip process if no file is being written
        else if (!have_file)
        {
            continue;
        }
              
        // copy the block to out
        fwrite(buffer, sizeof(buffer), 1, out);

    }

    // close files
    fclose(raw);
    fclose(out);

    // it is all, tara tata :D
    return 0;
}

// know if the block is a beginning of jpg
bool is_jpeg(BYTE block[BLOCK_SIZE])
{
    // check first 4 bytes
    if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && (block[3] >= 0xe0 && block[3] <= 0xef))
        return true;
    return false;
}

// create next file in order
FILE* new_jpeg(int* num)
{
    char file_name[8];
    sprintf(file_name, "%03d.jpg", *num);     // format file name as ###.jpg
    (*num)++;     // increase the counter for the next time

    FILE* out = fopen (file_name, "a");
    if (out == NULL)
    {
        printf("couldn't open output file \n");
        return NULL;
    }

    return out;
}