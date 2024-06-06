#include "file_mgr.h"

#include <stdio.h>
#include <string.h>

#include "fat32_mgr_helpers.h"
#include "file_config.h"


void emulate_fat32(const char* filename_, int file_size_, const char* dir_)
{
    printf("Creating a virtual disk file...\n");

    create_file(filename_, file_size_);

    printf("Formatting the file as FAT32...\n");
    format_file_as_fat32(filename_);

    printf("Creating a mount point...\n");
    create_mountpoint(dir_);

    printf("Mounting the file as a loop device...\n");
    mount_file(filename_, dir_);

    printf("Done! The virtual disk is mounted at %s\n", dir_);
}

void remove_fat32(const char* file_name_, const char* dir_)
{
    printf("Unmounting the file...\n");

    unmount_file(dir_);

    printf("Cleaning up...\n");
    char command[256];
    
    remove_mountpoint(dir_);
    
    remove_file(file_name_);

    printf("Done!\n");
}

eCmdParseResult parseCmd(const char* file_name_)
{
    //printf("Opening existing file : %s\n", file_name_);
    FILE* fp;
    char buffer[1024];

    fp = fopen(file_name_, "r");
    if (fp == NULL)
    {
        //printf("File %s doesn`t exist\n", file_name_);
        return eFileDoesntExist;
    }
    // this double closing is designing flaw , i know;
    fclose(fp);

    // here , run this cmmand in shell
    char cmd_and_filename[128] = "blkid ";
    strncat(cmd_and_filename, file_name_, sizeof(cmd_and_filename) - strlen(cmd_and_filename) - 1);
    
    fp = popen(cmd_and_filename, "r"); // Open for reading
    if (fp == NULL) 
    {
        printf("File couldnt be opened\n");
        return eFileDoesntExist;
    }
    //printf("File exists. Checking if it is fat32...\n");

    // Read output from fsck.vfat
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        //printf("%s", buffer); // Print for debugging (optional)

        char* res_1 = strstr(buffer, "vfat");
        if (res_1 != NULL)
        {
            //printf("The file is Fat32\n");
        }
        else
        {
            //printf("The selected file is NOT FAT32\n");
            pclose(fp);
            return eNotFat32;
        }
        // Parse the buffer to check for FAT32 keywords (e.g., "FAT32")
    }

    pclose(fp); // Close the pipe
    return eisFat32;
}

void handleParseResult(eCmdParseResult res, const char* filename_, CommandContext* cmd_context_)
{
    switch (res)
    {
    case eisFat32:
    {
         //in this case the file exists we just need to mount to it the default directory
        create_mountpoint(DEFAULT_MOUNTPOINT);
        mount_file(filename_, DEFAULT_MOUNTPOINT);
        // in this case we are setting our availbale command context to all
        setAllAvailableState(cmd_context_);
        
    }break;
    case eNotFat32:
    {
        // if the file is not fat32 we need 
        printf("File is NOT Fat32 ; No command will be processed except for 'format'\n");
        // in this case we are setting our availbale command context only to Format
        create_mountpoint(DEFAULT_MOUNTPOINT);
        mount_file(filename_, DEFAULT_MOUNTPOINT);
        setFormatOnlyState(cmd_context_);
    }break;
    case eFileDoesntExist:
    {
        printf("File with such name doenst exist; Creating default file\n");
        emulate_fat32(filename_, DEFAULT_FILESIZE_MB, DEFAULT_MOUNTPOINT);
        // in this case we are setting our availbale command context to all
        setAllAvailableState(cmd_context_);
    }break;
    }
}

bool isFat32(const char* filename_)
{
    //printf("Opening existing file : %s\n", filename_);
    FILE* fp;
    char buffer[1024];

    fp = fopen(filename_, "r");
    if (fp == NULL)
    {
        printf("File couldnt be opened\n");
        return false;
    }
    // this double closing is designing flaw , i know;
    fclose(fp);

    // here , run this cmmand in shell
    char cmd_and_filename[128] = "blkid ";
    strncat(cmd_and_filename, filename_, sizeof(cmd_and_filename) - strlen(cmd_and_filename) - 1);

    fp = popen(cmd_and_filename, "r"); // Open for reading
    if (fp == NULL)
    {
        printf("File couldnt be opened\n");
        return false;
    }
    printf("File exists. Checking if it is fat32...\n");

    // Read output from fsck.vfat
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        printf("%s", buffer); // Print for debugging (optional)

        char* res_1 = strstr(buffer, "vfat");
        if (res_1 != NULL)
        {
            printf("The file is Fat32\n");
            return true;
        }
        else
        {
            printf("The selected file is NOT FAT32\n");
            pclose(fp);
            return false;
        }
        // Parse the buffer to check for FAT32 keywords (e.g., "FAT32")
    }

    pclose(fp); // Close the pipe
    return false;
}


void clean(const char* file_name_, const char* dir_, eCleanOption option)
{
    switch(option)
    {
    case eUnmount:
    {
        unmount_file(dir_);
    }break;
    case eEverything:
    {
        remove_fat32(file_name_, dir_);
    }break;
    }
}