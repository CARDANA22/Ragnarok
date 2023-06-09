#pragma once

struct memory_data {
    // The process id to read/write memory from
    int pid;
    // The address to look at
    unsigned long address;
    // The input/output buffer
    void* buffer;
    // The amount of bytes to read/write
    int size;
};

struct command_data {
    // The command type
    int command;
    // The arguments for the functions
    struct memory_data data;
};

namespace Netlink {
    int SendCommand(struct command_data& cmdData);   
}