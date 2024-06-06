#pragma once
#include "commands/command_content.h"


CommandContent getCommandContent(CommandContent *content);

void getCurrentDir(string* dest, const char* mount_dir);

void displayCurDir(string* dir_, const char* mount_dir);

void updateCurDir(string* dir_, const char* mount_dir, CommandContent* content);

void initFileName(string* dest, const char* filename_);