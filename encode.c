#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "common.h"

OperationType check_operation_type(char *argv[])
{
	if(strcmp(argv[1],"-e")==0)
	{
		return e_encode;
	}
	if(strcmp(argv[1],"-d")==0)
	{
		return e_decode;
	}
	else
	{
		return e_unsupported;
	}
}

/* Function Definitions */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo) 
{
    if(!strcmp( strstr(argv[2],"."),".bmp"))
    {
        printf("INFO: beautiful.bmp file validation is Done\n");
        encInfo->src_image_fname = argv[2];
        if(!strcmp(strstr(argv[3],"."),".txt") || !strcmp(strstr(argv[3],"."),".c") || !strcmp(strstr(argv[3],"."),".sh"))
        {
            printf("INFO: *argv[3] is having .bmp or .c or .sh extension\n");
			encInfo->secret_fname=argv[3]; 
			//store extn in the variable char extn_secret_file[MAX_FILE_SUFFIX]
			char *extensionStart = strstr(argv[3], ".");// copied base address of .(dott) 
		    if (extensionStart)
			    {
			       strcpy(encInfo -> extn_secret_file, extensionStart); // Copy the extension
				   printf("INFO: %s File extension validation is done\n",argv[3]);
				}
			else
				{
				    printf("ERROR: No extension found.Enter valid extension {.c,.sh,.txt etc}\n");
					return e_failure;
				}

            encInfo -> secret_fname = argv[3]; // passing base of 3rd arg
            if(argv[4] != NULL)
            {
                if(!strcmp(strstr(argv[4],"."),".bmp"))  //strcpm retun val=0 !(0)=1; if(1)
                {
                    encInfo -> stego_image_fname= argv[4]; //passing addr of 4th argv
					printf("INFO: Output file %s validation is done\n",argv[4]);
                    return e_success;
                }
                else
                {
                  printf("ERROR: argv[4] is not having .bmp extension\n");
                  return e_failure;
                }
            }
 			else
            {
              encInfo -> stego_image_fname = "stego.bmp";
			  printf("INFO: Output file stego_img.bmp validation is done\n");
              return e_success;
            }

        }
    }
}



Status do_encoding(EncodeInfo *encInfo)
{
	//printf("");
	printf("INFO: Opening Required Files\n");
	if(open_files(encInfo)==e_success)
	{
		printf("INFO: Opened Required Files\n");
		printf("INFO: Opened beautiful.bmp\n");
		printf("INFO: Opened secret.txt file\n");
		printf("Done\n");
	}
	else
	{
		printf("ERROR: Opening of Files Failed\n");
		return e_failure;
	}

	printf("---------------------------------------\n");
	printf("INFO: ## ENCODING PROCEDURE STARTED ##\n");
	printf("----------------------------------------\n");
	printf("INFO: Checking for %s capacity to handle %s\n", encInfo->src_image_fname, encInfo->secret_fname);
	if(check_capacity(encInfo)==e_success)
	{
		printf("INFO: Done.Found OK\n");

		printf("INFO: Copying Image Header\n");
		

			if(copy_bmp_header(encInfo -> fptr_src_image,encInfo -> fptr_stego_image)==e_success)
				{
					printf("INFO: Done\n");
					printf("INFO: Copied Image Header successfully\n");
					printf("INFO: Encoding Magic String\n");
					if(encode_magic_string(MAGIC_STRING, encInfo)==e_success)
						{
							printf("INFO: Magic string encoded sucessfully\n");
							printf("INFO: Encoding %s File Extension Size\n",encInfo->secret_fname);
							if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo)==e_success) // extn call
							{
								printf("INFO: %s File extension size is encoded sucessfully\n",encInfo->secret_fname);
								printf("INFO: Encoding %s File Extension\n",encInfo->secret_fname);
								if(encode_secret_file_extn(encInfo->extn_secret_file,encInfo)==e_success)
								{
									printf("INFO: %s File extension encoded sucessfully\n",encInfo->secret_fname);
									printf("INFO: Encoding %s File Size\n",encInfo->secret_fname);
									if(encode_secret_file_data_size(encInfo -> size_secret_file,encInfo)==e_success)
									{
			   							printf("INFO: %s File size encoded sucessfully\n",encInfo->secret_fname);
										printf("INFO: Encoding %s File Data\n",encInfo->secret_fname);
										if(encode_secret_file_data(encInfo)==e_success)
										{				    			
											printf("INFO: %s File data encoded sucessfully\n",encInfo->secret_fname);
											printf("INFO: Copying Remaining Data\n");
											if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)==e_success)
											{
				   								printf("INFO: Remaining Data copied sucessfully\n");
												// Close files
       											fclose(encInfo->fptr_src_image);
        										fclose(encInfo->fptr_secret);
        										fclose(encInfo->fptr_stego_image);
												FILE *fptr_output = fopen(encInfo->stego_image_fname, "r");
        
        										if (fptr_output != NULL) 
												{
           											// Print size of output file
            										uint output_size = get_file_size(fptr_output);
													printf("--------------------------------------------\n");
            										printf("INFO: OUTPUT ENCODED FILE SIZE: %u bytes\n", output_size);
													printf("--------------------------------------------\n");

	
            										// Close fptr_output
            										fclose(fptr_output);
												}

												return e_success;									
											}
											else
											{
												printf("ERROR: Copying of remaining datas failed!!\n");
												return e_failure;			
											}
										}
										else
										return e_failure;
									}
									else
									return e_failure;
								}
								else
								return e_failure;
							}
							else
							return e_failure;
						}
						else
					return e_failure;
				}
				else
				return e_failure;
	}
	else
	return e_failure;
}


/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR:Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR:Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR:Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;	
}

Status check_capacity(EncodeInfo *encInfo)
{
	encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);
	encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
	
	if(encInfo->image_capacity >= 54+(strlen(MAGIC_STRING)+4+strlen(encInfo->extn_secret_file)+4+encInfo->size_secret_file)*8)
	{
		printf("INFO: Image capacity size: %u\n",encInfo->image_capacity);
		FILE *fptr_output = fopen(encInfo->stego_image_fname, "r");
        
        	if (fptr_output != NULL) 
			{
           		// Print size of output file
            	uint output_size = get_file_size(fptr_output);
    			//	printf("Image Capacity size: %u bytes\n", output_size);
				printf("INFO: Encoding data size: %ld bytes\n", encInfo->size_secret_file);

				//check for greater imag size
				if(encInfo->image_capacity > output_size)
				{
					printf("INFO: Done. Founded greater image size\n");
				}
            	// Close fptr_output
            	fclose(fptr_output);
			}
			
		return e_success;
	}
	else
	{
		return e_failure;
	}
	//encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);
	//printf("Encoding data size: %u\n",encInfo->size_secret_file);
}


uint get_file_size(FILE *fptr)
{
	fseek(fptr,0,SEEK_END);
	int file_size=ftell(fptr);
	fseek(fptr,0,SEEK_SET);
	return file_size;

}



/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
   // printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
   // printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}


Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image)
{
	char str[54];	
	//rewind(fptr_src_image);
	fseek(fptr_src_image, 0, SEEK_SET);
	fread(str,1,54,fptr_src_image);
	fwrite(str,1,54,fptr_stego_image);
	return e_success;
}


Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
	encode_data_to_image(MAGIC_STRING,strlen(MAGIC_STRING),encInfo->fptr_src_image,encInfo->fptr_stego_image);
	return e_success;
}

Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
	char arr[8];
    for (int i = 0; i < size; i++)
    {
        fread(arr, 1, 8, fptr_src_image);//beautiful.bmp
        if(encode_byte_to_lsb(data[i], arr)==e_success)
		{
        	fwrite(arr, 1, 8, fptr_stego_image);
		}
		else
		{
			fprintf(stderr,"Encoding bye to lsb failed");
			return e_failure;
		}
    }
	return e_success;

}
				

Status encode_byte_to_lsb(char data, char *buffer)
{
	for(int i = 0; i < 8; i++)
	{
        buffer[i] = (0xFE&buffer[i])|((data>>i)&1);
    }
	return e_success;
}

Status encode_secret_file_extn_size(int extn_size, EncodeInfo* encInfo)
 {
    char str[32];
    fread(str, 1, 32, encInfo->fptr_src_image);
    encode_size_to_lsb(extn_size,str);
    fwrite(str, 1, 32, encInfo->fptr_stego_image);
    return e_success;
}

Status encode_size_to_lsb(int size, char*buffer)
{
	for(int i=0;i<32;i++)
	{
		buffer[i] = (0xFE&buffer[i])|((size>>i)&1);
	}
}

Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
	encode_data_to_image(file_extn,strlen(file_extn),encInfo -> fptr_src_image,encInfo -> fptr_stego_image);
	return e_success;
}



Status encode_secret_file_data_size(int file_size, EncodeInfo *encInfo)
{

    char buffer[32];
    fread (buffer, 1, 32, encInfo->fptr_src_image);
	encode_size_to_lsb(file_size,buffer);
	fwrite(buffer, 1 , 32, encInfo -> fptr_stego_image);

    return e_success;
}


Status encode_secret_file_data(EncodeInfo *encInfo)
{
	rewind(encInfo -> fptr_secret);
	char data[(int)encInfo->size_secret_file+1];
	fread(data, 1, encInfo->size_secret_file, encInfo->fptr_secret);
    if(encode_data_to_image(data,encInfo -> size_secret_file,encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success)
	{
		return e_success;
	}
	else
	{
		fprintf(stderr,"Encoding secret file data failed");
		return e_failure;
	}

}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while (fread(&ch, 1, 1, fptr_src) > 0)
    {
        fwrite(&ch, 1, 1, fptr_dest);
    }
    return e_success;
}
