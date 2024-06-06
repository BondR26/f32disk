#pragma once

#include <stdbool.h>

#include "command_content.h"
#include "emulator_context.h"


typedef const char* (*command_call)(CommandContent*);

typedef struct CommandEnumPair
{
	eCommandTypes _type;
	command_call  _call;
}CommandEnumPair;

typedef struct CommandList
{
	CommandEnumPair _list[5];

}CommandList;

void execShellCommand(const CommandContext* const context_, CommandContent* command);

void execFormatCommand(const CommandContext* const context_, CommandContent* command);

bool isExitCommand(CommandContent* content_);

