#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main (int argc, char* argv[])
{
    if (argc != 2)
        {
            printf("./recover image \n");
            return 1; 
        }
     
    char *name = argv[1];    
    FILE *file = fopen(name, "r");
    
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", name);
        return 1;
    }
    
    FILE *image = NULL;
    int count = 0;
    BYTE *bytes = malloc(512);
    
    while ( fread(bytes, 512, 1, file) == 1)
    {
        if( bytes[0] == 0xff && bytes[1] == 0xd8 && bytes[2] == 0xff && ( bytes[3] & 0xf0) == 0xe0) 
            {
                if (count > 0 )
                    {
                        fclose(image);
                    }
            
                char filename[9];
                sprintf(filename, "%03i.jpg", count);
                image = fopen(filename, "w");
                fwrite(bytes, 512, 1, image);
           
            
            
                if (image == NULL)
                    {
                        fclose(file);
                        fprintf(stderr, "Could not create %s.\n", filename);
                        return 3;
                    }
            
                count++;
            
            }
            
        else if (count > 0 )
        {
            fwrite(bytes, 512, 1, image);
        }
    }
    fclose(image);
    fclose(file);
    return 0;
    
}
