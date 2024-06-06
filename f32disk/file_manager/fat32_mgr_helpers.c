#include "fat32_mgr_helpers.h"


void create_file(const char* filename, int size_mb)
{
    char command[256];
    snprintf(command, sizeof(command), "dd if=/dev/zero of=%s bs=1M count=%d", filename, size_mb);
    system(command);
}

void format_file_as_fat32(const char* filename) {
    char command[256];
    snprintf(command, sizeof(command), "sudo mkfs.vfat -F 32 %s", filename);
    system(command);
}

void create_mountpoint(const char* mountpoint) 
{
    char command[256];
    snprintf(command, sizeof(command), "mkdir -p %s", mountpoint);
    system(command);
}

void mount_file(const char* filename, const char* mountpoint) {
    char command[256];
    snprintf(command, sizeof(command), "sudo mount -o loop %s %s", filename, mountpoint);
    system(command);
}

void unmount_file(const char* mountpoint) {
    char command[256];
    snprintf(command, sizeof(command), "sudo umount %s", mountpoint);
    system(command);
}

void remove_mountpoint(const char* mountpoint)
{
    char command[256];
    snprintf(command, sizeof(command), "sudo rm -r %s", mountpoint);
    system(command);
}

void remove_file(const char* filename_)
{
    char command[256];
    snprintf(command, sizeof(command), "rm -f %s", filename_);
    system(command);
}
