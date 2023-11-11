#ifndef VM_IMAGE_H
#define VM_IMAGE_H

#include <stdio.h>

/**
 * Opens a binary image file and loads its contents into the LC3 virtual machine's memory.
 *
 * @param image_path The file path of the binary image to be loaded.
 * @return An integer indicating success or failure: 1 for success, 0 for failure.
 *
 */
int ReadImage(const char* image_path);

/**
 * Reads and loads a binary file into the memory of the LC3 virtual machine.
 *
 * @param file A pointer to a FILE object that represents the binary file to be read.
 *
 */
void ReadImageFile(FILE* file);

#endif