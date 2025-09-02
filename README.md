# LSB Image Steganography

## Overview
This project implements **Least Significant Bit (LSB) Image Steganography** in C, allowing users to hide secret text inside BMP images and retrieve it without noticeable loss in image quality. Steganography is the art of concealing information within another medium, making it useful for secure communication and data protection.

---

## Features
- **Encode** secret text into a BMP image.
- **Decode** hidden text from the image.
- Supports basic BMP images for hiding messages.
- Easy-to-use C programs for embedding and extracting data.

---

## Project Structure
1. encode.c # Embeds secret text into BMP image
2. decode.c # Extracts hidden text from BMP image
3. common.h # Shared functions and definitions
4. types.h # Data structures used across the project
5. test_encode.c # Test program for encoding
6. beautiful.bmp # Sample BMP image
7. my.txt # Sample text file for encoding
8. secret.txt # Sample text file for decoding
9. stego.bmp # Resulting BMP image after encoding

---

## How It Works
1. **Encoding**:
   - Reads the secret text file.
   - Converts each character into bits.
   - Modifies the **least significant bit (LSB)** of each pixel in the BMP image to store the message.
2. **Decoding**:
   - Reads the LSBs of the BMP image pixels.
   - Reconstructs the hidden message bit by bit.
   - Saves the extracted message to a text file.

---

## Usage

-> **Compile**
   gcc encode.c -o encode
   gcc decode.c -o decode

-> **Encode a Message**
   ./encode beautiful.bmp my.txt stego.bmp
    - beautiful.bmp : Original image
    - my.txt : Text file to hide
    - stego.bmp : Output image with hidden message

-> **Decode a Message**
   ./decode stego.bmp secret.txt
   - stego.bmp : Image containing hidden message
   - secret.txt : Output file where hidden text will be saved

## Real-World Applications
- Secure communication without alerting third parties.
- Digital watermarking for copyright protection.
- Organizing sensitive data inside images.
