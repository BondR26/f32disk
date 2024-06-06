// f32disk.cpp : Defines the entry point for the application.
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "file_manager/file_mgr.h"
#include "commands/emulator_context.h"
#include "commands/command_list.h"
#include "file_manager/file_config.h"

#include "helpers.h"

static bool isFormatted;

int main(int argc, char* argv[])
{
    // if the arvg value is not empty 
    // try openning fat32 file 
    //char* file_name = argv[1];
    string file_name;
    //initFileName(&file_name, argv[1]);
    strncpy(file_name.data, "disk.img", sizeof("disk.img"));
    file_name._size = sizeof("disk.img");
    
    // parse the cmd argument 1 string
    // hwich should be a filename
    eCmdParseResult res = parseCmd(file_name.data);

    // create a context for performing shell commands
    CommandContext available_commands;

    //depending on whether the file from shell exsits, not exist or is not fat32 fs 
    // select how to behave in the future
    handleParseResult(res, file_name.data, &available_commands);

    CommandContent content;
    initContent(&content, file_name.data);

    memset(available_commands.current_dir.data, 0, sizeof(available_commands.current_dir.data));
    getCurrentDir(&available_commands.current_dir, DEFAULT_MOUNTPOINT);

    while(1)
    {
        displayCurDir(&available_commands.current_dir, DEFAULT_MOUNTPOINT);
        
        // get the user input
        getCommandContent(&content);

        if (isExitCommand(&content))
        {
            break;
        }

        performCommand(&available_commands, &content);

        if (!isFormatted)
        {
            if (isFat32(file_name.data))
            {
                //remount_after_formatting(file_name.data, DEFAULT_MOUNTPOINT);
                setAllAvailableState(&available_commands);
                isFormatted = true;
                continue;
            }
            else
            {
                continue;
            }
        }

        updateCurDir(&available_commands.current_dir, DEFAULT_MOUNTPOINT, &content);
        // clean the garbage out of string
        memset(content.content.data, 0, sizeof(content.content.data));
    }

    // unmount and close the created file;
    // if the file from shell wasnt selected then it is default file
    if (file_name.data == NULL)
    {
        clean(DEFAULT_FILENAME, DEFAULT_MOUNTPOINT, eUnmount);
    }
    else
    {
        clean(file_name.data, DEFAULT_MOUNTPOINT, eUnmount);
    }

    return 0;
}