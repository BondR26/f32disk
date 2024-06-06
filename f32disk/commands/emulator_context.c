#include "emulator_context.h"
#include <stdlib.h>
#include <stdio.h>

#include "command_list.h"


void AllCommandsAvailable(const CommandContext* const context_, const CommandContent* const content_)
{
    execShellCommand(context_ , content_);
}

void FormatCommandAvailable(const CommandContext* const context_, const CommandContent* const content_)
{
    printf("File is Not Fat32\n");
    execFormatCommand(context_, content_);
}

// Functions to transition to different states
// One state allows for multiple command 
void setAllAvailableState(CommandContext* CommandContext)
{
    CommandContext->state.performCommand = AllCommandsAvailable;
}

// This state only can perfrom formatting
void setFormatOnlyState(CommandContext* CommandContext)
{
    CommandContext->state.performCommand = FormatCommandAvailable;
}

// Function to perform a command based on the current state
void performCommand(const CommandContext* const context_, const CommandContent* const content_)
{
    context_->state.performCommand(context_, content_);
}
