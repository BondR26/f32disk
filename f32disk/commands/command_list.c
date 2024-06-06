#include "command_list.h"

#include <stdio.h>
#include <string.h>

#include "emulator_context.h"
#include "file_manager/fat32_mgr_helpers.h"
#include "file_manager/file_config.h"


#define COMMAND_LENGTH 16

const char* change_dir(CommandContent* c_cont_)
{
	// empty function_ptr which is not used
	// exist to preserve order of pair
}

const char* fake_format(CommandContent* c_cont_)
{
	// empty function_ptr which is not used
	// exist to preserve order of pair
	printf("File is already formatted\n");
}

const char* format( CommandContent* c_cont_)
{
	char cmd[256];

	unmount_file(DEFAULT_MOUNTPOINT);

	snprintf(cmd, sizeof(cmd), "sudo mkfs.fat -F 32 %s", c_cont_->additional_info.data);
	// Execute the command
	int ret = system(cmd);
	if (ret == -1)
	{
		perror("system");
		return "Command execution failed";
	}
	else if (ret != 0) {
		printf("Command failed with exit code %d\n", WEXITSTATUS(ret));
		return "Success";
	}
	else if (ret == 0)
	{
		printf("File was formatted\n");
	}

	mount_file(c_cont_->additional_info.data, DEFAULT_MOUNTPOINT);
}

const char* list( CommandContent* c_cont_) {
	char cmd[256];
	snprintf(cmd, sizeof(cmd), "ls %s", c_cont_->content.data);
	// Execute the command
	int ret = system(cmd);
	if (ret == -1) {
		perror("system");
		return "Command execution failed";
	}
	else if (ret != 0) {
		printf("Command failed with exit code %d\n", WEXITSTATUS(ret));
		return "Command failed";
	}
	return "Success";
}

const char* make_dir( CommandContent* c_cont_)
{
	char cmd[256];
	snprintf(cmd, sizeof(cmd), "sudo mkdir %s", c_cont_->content.data);
	// Execute the command
	int ret = system(cmd);
	if (ret == -1) {
		perror("system");
		return "Command execution failed";
	}
	else if (ret != 0) {
		printf("Command failed with exit code %d\n", WEXITSTATUS(ret));
		return "Command failed";
	}
	return "Success";
}

const char* touch( CommandContent* c_cont_)
{
	char cmd[256];
	snprintf(cmd, sizeof(cmd), "sudo touch %s", c_cont_->content.data);
	// Execute the command
	int ret = system(cmd);
	if (ret == -1) {
		perror("system");
		return "Command execution failed";
	}
	else if (ret != 0) {
		printf("Command failed with exit code %d\n", WEXITSTATUS(ret));
		return "Command failed";
	}
	return "Success";
}

static CommandEnumPair _list[] =
{
	{eCd,	    change_dir},
	{eformat,	fake_format},
	{eLs,		list	  },
	{eMkdir,	make_dir  },
	{eTouch,    touch     }
};

typedef struct CommandStringPair
{
	eCommandTypes _type;
	char _name[COMMAND_LENGTH];
}CommandStringPair;

static CommandStringPair gCommandToNames[] = 
{
	{eCd,	    "cd"},
	{eformat,	"format"},
	{eLs,		"ls"},
	{eMkdir,	"mkdir"},
	{eTouch,    "touch"}
};

eCommandTypes find_appropriate_command(const string* const name_, CommandStringPair* commands_, size_t cmd_size)
{
	int i ;
	string args[COMMAND_LENGTH];

	string temp;
	strncpy(temp.data, name_->data, name_->_size);
	temp._size = name_->_size;

	for (i = 0; i < cmd_size; i++)
	{
		// Tokenize the string to get the first word
		char* token = strtok(temp.data, " \t\n");
		if (strncmp(commands_[i]._name, token, sizeof(commands_[i]._name)) == 0)
		{
			//printf("The command %s was selected", commands_[i]._name);
			return commands_[i]._type;
		}
		// find if the first word in the string that occurs is one of the selected commands
	}

	// if the first string in the conten is not any of the selected command -> then it is wrong intput
	return eUnknown;
}

// this helper function helps to retrieve the remaining part of the command recive from input
const char* find_remaining(const char* input, const char* command) 
{
	char* pos = strstr(input, command);
	if (pos != NULL)
	{
		pos += strlen(command);  // Move the pointer past the command
		// Skip any leading spaces
		while (*pos == ' ')
		{
			pos++;
		}
		return pos;
	}
	return NULL;
}

string formfullCommand(const CommandContext* const context_, const char* after_data)
{
	string str;
	memset(str.data, 0, DEFAULT_CAPACITY);
	// 
	strncpy(str.data, context_->current_dir.data, context_->current_dir._size);
	str._size = context_->current_dir._size;

	//append the short command to the full command
	int sz = 0;

	// case for ls after fromtating
	if(after_data != NULL)
		sz = strlen(after_data);
	
	strncat(str.data, "/", 1);
	strncat(str.data, after_data, sz);
	str._size += sz;

	return str;
}

void execShellCommand(const CommandContext* const context_, CommandContent* command)
{
	// got throug the existing enums 
	eCommandTypes cmd_type = find_appropriate_command(command, gCommandToNames, sizeof(gCommandToNames)/sizeof(CommandStringPair));
	command->_type = cmd_type;
	if (cmd_type != eUnknown)
	{
		CommandContent command_args;
		memset(command_args.content.data, 0, DEFAULT_CAPACITY);
		// retrieve the command arguments after the command iteslt
		// really not point in doing that, could just pass the whole string 
		// except for the fact that this way there is leeway to inert some additional logic
		// in here; i.e. if there would be need to parse the arguments themself to see if they are valid;
		char* after_data = find_remaining(command->content.data, gCommandToNames[cmd_type]._name);
		if (after_data[0] == '\n')
		{
			after_data = NULL;
		}
		// form full command with absolute paths
		string full_cmd = formfullCommand(context_, after_data);

		strncpy(command_args.content.data, full_cmd.data, full_cmd._size);
		command_args.content._size = full_cmd._size;
		// if the command is change dir - then we set the context; 
		// but the context itself will be chagned outside 
		// this partly bacause of idea of executing command withing the context 
		// and partly cause ther is no place for getting the name of mountpoint at this point
		if (cmd_type == eCd)
		{
			// in order to not perfrom the addiitonal extracting of the full command content
			// i prefer to update the content here although there might be situation like cd path/virtual_disk/my_dir/..
			//
			strncpy(command->content.data, full_cmd.data, full_cmd._size);
			command->content._size = full_cmd._size;
			return;
		}
		//get the ptr to command function
		command_call sh_cmd = _list[cmd_type]._call;
		sh_cmd(&command_args);
	}
	else
	{
		printf("Invalid input \n");
	}
}

static CommandStringPair gFormatCmd[] = { eformat, "format" };

void execFormatCommand(const CommandContext* const context_, CommandContent* content_)
{
	eCommandTypes cmd_type = find_appropriate_command(content_, gFormatCmd, sizeof(gFormatCmd) / sizeof(CommandStringPair));
	content_->_type = cmd_type;
	if (cmd_type == eformat)
	{
		format(content_);
		return;
	}

	printf("Unknown disk format\n");
	return ;
}

static CommandStringPair gQuitCmd[] =
{
	{eQuit,	    "q"},

};

bool isExitCommand(CommandContent* content_)
{
	eCommandTypes cmd_type = find_appropriate_command(content_, gQuitCmd, sizeof(gQuitCmd) / sizeof(CommandStringPair));
	content_->_type = cmd_type;
	if (cmd_type == eQuit)
	{
		return true;
	}

	return false;
}