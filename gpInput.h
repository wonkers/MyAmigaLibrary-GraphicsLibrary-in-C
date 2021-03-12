#include <proto/all.h>
#include <exec/types.h>
#include <exec/devices.h>
#include <exec/memory.h>
#include <devices/gameport.h>
#include <devices/inputevent.h> 
#include <graphics/sprite.h>

#define XMOVE 1
#define YMOVE 1

BOOL set_controller_type(BYTE);
void set_trigger_conditions(struct GamePortTrigger *);
void send_read_request(void);
void check_move(int *x, int *y);
void flush_buffer(void);
void free_gp_unit(void);
void cleanup(void);
void cleanexit(UBYTE *, LONG);
void GetJoyStick();

void BeginGameDevice();