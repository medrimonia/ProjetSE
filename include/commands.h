#ifndef COMMANDS_H
#define COMMANDS_H

typedef int8_t command_id

#define CMD_GET_CAPS       0
#define CMD_RESET          1
#define CMD_PING           2
#define CMD_READ           3
#define CMD_WRITE          4
#define CMD_GET_TYPE       5
#define CMD_SET_TYPE       6
#define CMD_GET_FAILSAFE   7
#define CMD_SET_FAILSAFE   8
// This command number is reserved for the specific purpose of spontaneous
// messages from the device in a future version
#define CMD_RESERVED      15

#endif//COMMANDS_H
