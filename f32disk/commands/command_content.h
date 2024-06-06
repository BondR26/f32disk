#pragma once
#include "udt/custom_string.h"


typedef enum eCommandTypes
{
	eCd,
	eformat,
	eLs,
	eMkdir,
	eTouch,
	eQuit,
	eUnknown
}eCommandTypes;


typedef struct CommandContent
{
    string content;
    string additional_info;
    eCommandTypes _type;
}CommandContent;

void initContent(CommandContent* content_, const char* additional_data);