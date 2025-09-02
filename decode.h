#ifndef DECODE_H
#define DECODE_H

#include "types.h"
#include "common.h"

/* Get file extention size by decoding 32 bytes */
#define DECODE_FILE_EXTN_SIZE 32

/* Get file size by decoding 32 bytes*/
#define DECODE_FILE_SIZE 32

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4
#define MAX_FILE_SIZE 20


typedef struct _DecodeInfo
{
	/* Encoded stego image info */
	char *stego_image_fname;
	FILE *fptr_stego_image;
	uint image_data_size;
	int d_sec_file_extn_size;
	int d_sec_file_size;
	//char image_data[MAX_IMAGE_BUF_SIZE];

	/* Decoded output file info */
	char output_file_name[MAX_FILE_SIZE];
	FILE *fptr_output_file;
	char extn_output_file[MAX_FILE_SUFFIX];
//	char decode_data[MAX_SECRET_BUF_SIZE];

} DecodeInfo;



/* Decoding Function Prototypes */

/* Read and validate decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);


/* Get File pointers for i/p and o/p files */
Status open_files_dec(DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Decode Magic String */
Status decode_magic_string(const char *magic_string,DecodeInfo *decInfo);

/*decode data from image*/
Status decode_data_from_image(int size, FILE *fptr_stego_image,char *decoded_data);

/* Decode byte from lsb */
char decode_byte_from_lsb(char *image_buffer);


Status decode_output_file_extn(DecodeInfo *decInfo);

int decode_size(DecodeInfo *decInfo);

Status decode_secret_data(DecodeInfo *decInfo);

#endif


