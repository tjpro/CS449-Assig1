#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
        unsigned short start_file;
        unsigned short app_marker;
        unsigned short app_length;
        unsigned short null_term;
        unsigned short endianness;
        unsigned short version_num;
        unsigned int offset;      //Apparently you have to have all of these types in a proper order
        unsigned int exif_string; //so that sizeof() works properly
} header;

typedef struct {
        unsigned short identifier;
        unsigned short data_type;
        unsigned int num_items;
        unsigned int offset;
} tiff;

int main(int argc, char **argv) {

        unsigned short count;
        header head;
        tiff img_tiff;
        FILE* infile = fopen(argv[1], "rb");

        fread(&head, sizeof(head), 1, infile);

        fread(&count, sizeof(count), 1, infile);

        int i;
        for(i = 0; i < count; i++){
                fread(&img_tiff, sizeof(img_tiff), 1, infile); // Read in tiff tag
                unsigned long holder_pos = ftell(infile); //Hold location for next interation
                fseek(infile, 12+img_tiff.offset, SEEK_SET); //find new set point

                char str_array[img_tiff.num_items];

                if(img_tiff.identifier == 0x010f){
                        fread(&str_array, sizeof(str_array), 1, infile);
                        printf("Manufacturer:\t\t%s",str_array);
                        return 0;
                }
                else if(img_tiff.identifier == 0x0110) {
                        printf("0x0110");
                }
                else if(img_tiff.identifier == 0x8769){
                        printf("0x8769");
                }
                fseek(infile, holder_pos, SEEK_SET);
        }

        printf("\n");
    fclose(infile);
    return 0;
}