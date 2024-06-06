#include "helpers.h"

#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include "commands/command_list.h"
#include "file_manager/file_config.h"
#include "file_manager/fat32_mgr_helpers.h"


#define MAX_PATH_LENGTH 256

CommandContent getCommandContent(CommandContent* content)
{
    char input_[256];
    fgets(input_, sizeof(input_), stdin);
    // check if the input is one of the allowed commands from the list ; 
    size_t input_command_size = strnlen(input_, 256);
    strncpy(content->content.data, input_, input_command_size);
    content->content._size = input_command_size;
}

void getCurrentDir(string* dest, const char* mount_dir)
{
	char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		//printf("Current Directory: %s\n", cwd);
	}
    // copy the current dir (we would mount the virtual_disk right where the progrma lays)
    size_t input_command_size = strnlen(cwd, MAX_PATH_LENGTH);
    strncpy(dest->data, cwd, input_command_size);
    dest->_size = input_command_size;

    size_t mount_dir_sz = strnlen(mount_dir, MAX_PATH_LENGTH);
    //printf("Directory with MountDir: %s\n", dest->data);
    // append the /virtual_disk
    strncat(dest->data, "/", 1);
    strncat(dest->data, mount_dir, mount_dir_sz);
    mount_dir_sz++;
    dest->_size += mount_dir_sz;

    //printf("Full Directory with MountDir: %s\n", dest->data);
}

void displayCurDir(string* dir_, const char* mount_dir)
{
    // Find the position where virtual_disk dir ends; 
    char* cur_location = strstr(dir_->data, mount_dir);
    int mnt_size = strlen(mount_dir);
    if(cur_location != 0)
    { 
        char* relativePath = cur_location + mnt_size;
        if(strncmp(relativePath, "", 1))
        {
            printf("/>");
        }
        else
        {
            printf("%s>", relativePath);
        }
        // If the relative path is empty, it means we are at the base directory
    }
    else 
    {
        // If dir_ doesn't start with baseDir, print the full dir_ as fallback
        printf("Exiting from the virtual disk directory\n");
        unmount_file(mount_dir);
        exit(1);
    }
}

bool directoryExists(const char* path)
{
    DIR* dir = opendir(path);
    if (dir != NULL)
    {
        closedir(dir);
        return true;
    }
    return false;
}

void removeSubstring(string* full_dir_, const char* sub1)
{
    // Find the position of the second occurrence of sub2 from the end
    char* begining_of_previous_dir = strstr(full_dir_->data, sub1);
    size_t sz = strnlen(begining_of_previous_dir, DEFAULT_CAPACITY);

    char next_slash = '/';
    
    // get the reverse beginning of last dir name
    // - 1 cause the beg_of_prev points to the '/'
    int count = full_dir_->_size - sz - 1;

    // go back to find the '/' which designates the previous dir
    while (full_dir_->data[count] != next_slash)
    {
        count--;
    }
    
    // copy the shorted version of the string into itself
    memcpy(full_dir_->data, full_dir_->data, count);
    // this is to save the '/' symbol 
    full_dir_->data[count] = '\0'; // Null-terminate the destination string
    full_dir_->_size = count;
}

void updateCurDir(string* dir_, const char* mount_dir, CommandContent* content)
{
    // Check if the command type is eCd
    if (content->_type != eCd)
    {
        return;
    }

    if (strstr(content->content.data, "..") != 0)
    {
        //if the directory is 
        char temp[DEFAULT_CAPACITY];
       
        // copy virtual disk 
        strncpy(temp, mount_dir, strnlen(mount_dir, DEFAULT_CAPACITY));
        // add virtual_disk/..
        strncat(temp, "..", 2);
        
        char* ptr = strstr(content->content.data, temp);
        if (ptr != NULL)
        {
            //means that we want to escape the virtual disk;
            // we cannot do that
            printf("Cant escape out virtual disk\n");
            
        }
        else
        {
            // we need to escape current dir
            removeSubstring(&content->content, "/..");
            memset(dir_->data, '\0', dir_->_size);
            strncpy(dir_->data, content->content.data, content->content._size);
            dir_->_size = content->content._size;
        }

    }
    // Handle special cases for '.' and '..'
    else if (strstr(content->content.data, ".") != 0)
    {
        // No change needed for current directory
        // remove the '.' 

        return;
    }
    else
    {
        // Construct the new directory path
        char temp_path[DEFAULT_CAPACITY];
        memset(temp_path, 0, DEFAULT_CAPACITY);

        char *s = strncpy(temp_path, content->content.data, content->content._size);
        
        //char * s = strncat(dir_, , sizeof(newDir), arg);

        // Resolve the new directory path
        if (directoryExists(temp_path) == NULL)
        {
            perror("directory doenst exist\n");
            return;
        }

        // Update the current directory
        size_t new_dir_size = strnlen(s, DEFAULT_CAPACITY);
        strncpy(dir_->data, temp_path, new_dir_size);
        dir_->_size = new_dir_size;
    }

    printf("Updated Current Directory: %s\n", dir_->data);
}

void initFileName(string* dest, const char* filename_)
{
    memset(dest->data, 0, DEFAULT_CAPACITY);

    if(filename_ != NULL)
    {
        size_t sz = strlen(filename_);
        strncpy(dest->data, filename_, sz);
        dest->_size = sz;
    }
    else
    {
        size_t sz = strnlen(DEFAULT_FILENAME, DEFAULT_CAPACITY);
        strncpy(dest->data, DEFAULT_FILENAME, sz);
        dest->_size = sz;
    }
}