#pragma once
#include "command_content.h"

// here i am going to use pattern State to resolve this task,
// since we can have to state accroding to task - only formate availbale ; or everything availbale
// predeclare the CommandContext
typedef struct CommandContext CommandContext;

// Function pointer type for command execution
typedef void (*CommandFunc)(const CommandContext* const, const CommandContent* const);

// State structure representing different states
typedef struct State 
{
    CommandFunc performCommand;
} State;

// CommandContext structure that contains a state
typedef struct CommandContext
{
    State state;
    string current_dir;
}CommandContext;

// Functions to perform different commands
void AllCommandsAvailable(const CommandContext* const context, const CommandContent* const CommandContext);

void FormatCommandAvailable(const CommandContext* const context, const CommandContent* const CommandContext);

// Functions to transition to different states
// One state allows for multiple command 
void setAllAvailableState(CommandContext* CommandContext);

// This state only can perfrom formatting
void setFormatOnlyState(CommandContext* CommandContext);
// Function to perform a command based on the current state
void performCommand(const CommandContext* const CommandContext, const CommandContent* const content_);