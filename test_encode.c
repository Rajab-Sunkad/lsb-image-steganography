/*
    Name: Rajab J Sunkad
    Batch: 24018I
    Date: 11/10/2024
    Project Discription: "Digital steganography" is the practice of concealing information within digital files, such as images
                 audio, or video, to prevent detection. 
*/

// Include necessary header files
#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "common.h"

int main(int argc, char *argv[]) 
{
    EncodeInfo encInfo;  // Structure to hold encoding information
    uint img_size;        // Image size variable (unused)

    // Check if sufficient command-line arguments are provided
    if (argc < 4) 
	{
        printf("INFO: Enter valid no. of arguments\n");
        printf("Usage:-\n");
        printf("For encoding -> ./a.out -e <.bmp file> <secret file> [output file]\n");
        printf("For Decoding -> ./a.out -d <.bmp file> [output file]\n");
        return e_failure;
    } 
    // If arguments are sufficient, proceed with operation
    else {
        // Determine operation type (encoding or decoding)
        if (check_operation_type(argv) == e_encode) 
		{
            printf("----------------------------------------\n");
            printf("## ENCODING OPERATION IS SELECTED ##\n");
            printf("----------------------------------------\n");

            // Read and validate encoding arguments
            if (read_and_validate_encode_args(argv, &encInfo) == e_success) 
			{
                printf("INFO: Read and validation for encoding is success\n");
                
                // Perform encoding operation
                if (do_encoding(&encInfo) == e_success) 
				{
                    printf("ENCODING IS COMPLETED\n");
                }
            }
        } 
        // Handle decoding operation
        else if (check_operation_type(argv) == e_decode) 
		{
            printf("------------------------------------\n");
            printf("## DECODING OPERATION IS SELECTED ##\n");
            printf("------------------------------------\n");

            // Ensure sufficient arguments for decoding
            if (argc >= 2) {
                // Read and validate decoding arguments
                if (read_and_validate_decode_args(argv, &encInfo) == e_success) 
				{
                    printf("INFO: Validation Success\n");
                    
                    // Perform decoding operation
                    if (do_decoding(&encInfo) == e_success) 
					{
                        printf("------------------------------------\n");
                        printf("\n## DECODED SUCCESSFULLY ##\n");
                        printf("------------------------------------\n");
                    }
                } 
                // Handle validation failure
                else 
				{
                    printf("\n## DECODING FAILED ##\n");
                }
            } 
            // Handle insufficient arguments
            else 
			{
                printf("ERROR: Validation Error\nPlease Provide Required Files Through Command Line\n");
                printf("\n## DECODING FAILED ##\n");
                return e_failure;
            }
        } 
        // Handle invalid operation type
        else 
		{
            printf("ERROR: Pass Valid Operation Type\n");
            printf("Encoding: ./a.out -e <.bmp file> <.txt file> [Output file]\n");
            printf("Decoding: ./a.out -d <.bmp file> [output file]\n");
        }
    }

    return 0;
}
