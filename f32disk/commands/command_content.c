#include "command_content.h"

#include <string.h>


void initContent(CommandContent* content_, const char* additional_data_)
{
	int legnth = strnlen(additional_data_, 256);
	strncpy(content_->additional_info.data, additional_data_, legnth);
	content_->additional_info._size = legnth;
}