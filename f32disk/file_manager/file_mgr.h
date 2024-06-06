#pragma once
#include <stdbool.h>

#include "udt/custom_string.h"
#include "commands/emulator_context.h"


typedef enum eCmdParseResult
{
	eisFat32,
	eNotFat32,
	eFileDoesntExist
}eCmdParseResult;

void emulate_fat32(const char* filename_, int file_size_, const char* dir_);

void remove_fat32(const char* file_name_, const char* dir_);

//void remount_after_formatting(const char* filename_, const char* dir_);

eCmdParseResult parseCmd(const char* file_name_);

void handleParseResult(eCmdParseResult res, const char* filename_, CommandContext* command_context_);

bool isFat32(const char* filename_);

typedef enum eCleanOption
{
	eEverything,
	eUnmount,
}eCleanOption;

void clean(const char* file_name_, const char* dir_, eCleanOption option );
