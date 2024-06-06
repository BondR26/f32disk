#pragma once

#include <stdio.h>
#include <stdlib.h>


// creates file using shell command "dd if=/dev/zero of=%s";
// where dd: is a command - line utility for copying and converting data.
// if = /dev/zero : This specifies the input file as dev/zero, a special device that provides a continuous stream of zeros.
// of = % s : This defines the output filename using a placeholder(% s) which will be replaced by the actual filename argument.
// "bs = 1M" : This sets the block size for dd to 1 Megabyte(MB).
// "count=%d" : This defines the number of blocks to write, which is determined by the size_mb argument.The% d placeholder is replaced with the actual size_mb value.
void create_file(const char* filename, int size_mb);

// uses mkfs.vfat - command line utility used to create and format filesystems.
// - F 32 : This option specifies that the filesystem should be formatted as FAT32.
// % s : This placeholder is replaced with the actual filename argument.
void format_file_as_fat32(const char* filename);

// mkdir: is a command - line utility used to create directories.
// - p : This option tells mkdir to create any missing parent directories along the path if necessary.
// % s : This placeholder is replaced with the actual mountpoint argument.
void create_mountpoint(const char* mountpoint);

//snprintf(command, sizeof(command), "sudo mount -o loop %s %s", filename, mountpoint); : This line constructs the mount command using snprintf.It formats the command string by replacing% s with the provided filenameand mountpoint.
// The resulting command looks like sudo mount - o loop filename mountpoint.
//system(command); : This line executes the constructed command using the system function.The system function runs the command string as if it were typed on the command line.In this case, it mounts the virtual disk file as a loop device at the specified mount point.
void mount_file(const char* filename, const char* mountpoint);

// this unmounts the file from the folder
void unmount_file(const char* mountpoint);

// removes the mounting directory
void remove_mountpoint(const char* mountpoint);

void remove_file(const char* filename_);