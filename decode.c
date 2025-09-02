#include<stdio.h>
#include "decode.h"
#include "types.h"
#include <string.h>
#include "common.h"


Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
	if(strstr(argv[2],".bmp") != NULL)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
	{
		printf("Error: Not bmp file");
        return e_failure;
	}
    if (argv[3] != NULL)
	{
		strcpy(decInfo->output_file_name,argv[3]);
	}
    else
	{
        strcpy(decInfo->output_file_name,"output");
	}
    return e_success;
}


Status open_files_dec(DecodeInfo *decInfo)
{
    //Stego Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");

    //Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
        return e_failure;
    }
	else
	{
		printf("INFO: Opened %s\n", decInfo->stego_image_fname);
	}
	return e_success;
}


Status decode_magic_string(const char *magic_string,DecodeInfo *decInfo)
 {
    char input_magic_string[strlen(MAGIC_STRING) + 1];
    printf("INFO: Enter magic string of 2 characters: ");
    scanf("%s", input_magic_string);

    if (strcmp(input_magic_string, MAGIC_STRING) == 0) 
	{
		printf("INFO: User entered Magic string and Decoded Magic String both are matched sucessfully\n");
        printf("INFO: Magic string validated successfully\n");
        // Decode magic string from image
        char magic_data[strlen(MAGIC_STRING) + 1];
        decode_data_from_image(strlen(MAGIC_STRING), decInfo->fptr_stego_image, magic_data);
        return e_success;
    } 
	else 
	{
        printf("INFO: User entered Magic string and Decoded Magic String both are not Matched\n");
        return e_failure;
    }
}



Status decode_data_from_image(int size,FILE *fptr_stego_image,char *decoded_data)
{
	int i;
	char image_buffer[8];
	for(i=0;i<size;i++)
	{
		fread(image_buffer,1,8,fptr_stego_image);
		decoded_data[i]=decode_byte_from_lsb(image_buffer);
	}
	decoded_data[i]='\0';
	return e_success;
}


char decode_byte_from_lsb(char *image_buffer)
{
	unsigned char ch=0;
	for(int i=0;i<8;i++)
	{
		ch=((image_buffer[i] & 0x01) << i) | ch;
	}
	return ch;
}


int decode_size(DecodeInfo *decInfo)
{
	char image_buffer[32];
	int size=0;
	fread(image_buffer,1,32,decInfo->fptr_stego_image);
	for(int i=0;i<32;i++)
	{
		size = ((image_buffer[i] & 0x01)<<i) | size;
	}
	//printf("%d is size\n",size);
	return size;
}


Status decode_output_file_extn(DecodeInfo *decInfo) 
{
    char arr[100];
    decode_data_from_image(decInfo->d_sec_file_extn_size, decInfo->fptr_stego_image, arr);
    arr[decInfo->d_sec_file_extn_size] = '\0';  // Null-terminate the extension
    strcpy(decInfo->extn_output_file, arr);
    strcat(decInfo->output_file_name, arr);
    return e_success;
}





Status decode_secret_data(DecodeInfo *decInfo) 
{
    char buffer[decInfo->d_sec_file_size + 1];
    decInfo->fptr_output_file = fopen(decInfo->output_file_name, "w");
    
    // Decode data from image
    decode_data_from_image(decInfo->d_sec_file_size, decInfo->fptr_stego_image, buffer);
    
    // Null-terminate the buffer
    buffer[decInfo->d_sec_file_size] = '\0';
    
    // Write decoded data to output file
    fwrite(buffer, 1, decInfo->d_sec_file_size, decInfo->fptr_output_file);
    
    // Close the output file
    fclose(decInfo->fptr_output_file);
    
    return e_success;
}



Status do_decoding(DecodeInfo *decInfo)
{
    printf("INFO: Opening required files\n");
    if (open_files_dec(decInfo) == e_success)
    {
		rewind(decInfo -> fptr_stego_image);
	    fseek(decInfo -> fptr_stego_image, 54, SEEK_SET);
		printf("-------------------------------------\n");
		printf("## Decoding Procedure Started ##\n");
		printf("-------------------------------------\n");
        printf("\nINFO: Decoding Magic String\n");
		if (decode_magic_string(MAGIC_STRING,decInfo) == e_success)
        {
			printf("Magic string length is %zu\n", strlen(MAGIC_STRING));
				printf("INFO: Done\n");
				printf("INFO: Decoding Output File Extension Size\n");
				if(decInfo -> d_sec_file_extn_size=decode_size(decInfo))
				{
					printf("INFO: Decoded output file extension size sucessfully\n");
					printf("INFO: Decoding Output File Extension\n");
					if(decode_output_file_extn(decInfo)==e_success)
					{
						printf("INFO: Decoded output file extension sucessfully\n");
						printf("INFO: Decoding File Size\n");
						if(decInfo -> d_sec_file_size=decode_size(decInfo))
						{
							printf("INFO: Decoded File size sucessfully\n");
							printf("INFO: Decoding File Data\n");
							if(decode_secret_data(decInfo)==e_success)
							{
								printf("INFO: Decoded data size sucessfully\n");
								return e_success;
							}
							else
								printf("ERROR: Decoding Secret Data Failed\n");
						}
						
					}
					else
						printf("ERROR: Decoding File Data Failed\n");
				}
							

		}

	}
	else
	printf("ERROR: Opening of Files Failed\n");
}

