#include "vm_image.h"
#include "vm_utilities.h"
#include "vm_globals.h"

/**
 * Opens a binary image file and loads its contents into the LC3 virtual machine's memory.
 *
 * This function is responsible for opening a binary image file in read-binary mode.
 * If successful, it calls 'ReadFile' to read and load the file's contents into the
 * LC3 VM's memory. After reading the file, it closes the file. If the file cannot
 * be opened, the function returns 0 indicating failure.
 *
 */
int ReadImage(const char* image_path)
{
    FILE* file = fopen(image_path, "rb");
    if (!file) { return 0; };
    ReadImageFile(file);
    fclose(file);
    return 1;
}

/**
 * Reads and loads a binary file into the memory of the LC3 virtual machine.
 *
 * This function reads a binary file intended to be loaded into the LC3 VM's memory.
 * It starts by reading the origin, which indicates where in the memory the file
 * content should begin. It then reads the file contents into memory, starting at
 * that origin address. Each word read from the file is byte-swapped to match the
 * endianness of the host system. This function assumes that 'file' is already open
 * and points to a valid FILE object.
 *
 */
void ReadImageFile(FILE* file)
{
    uint16_t origin;

    fread(&origin, sizeof(origin), 1, file);
    origin = Swap16(origin);

    uint16_t max_read = MEMORY_MAX - origin;
    uint16_t* p = memory + origin;
    size_t read = fread(p, sizeof(uint16_t), max_read, file);

    while (read-- > 0)
    {
        *p = Swap16(*p);
        ++p;
    }
}
