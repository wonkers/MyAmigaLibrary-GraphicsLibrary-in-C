#include "gpInput.h"

struct InputEvent   *game_event;
struct IOStdReq     *game_io_msg = NULL;
struct MsgPort      *game_msg_port = NULL;

struct GamePortTrigger  joyTrigger =
{
    GPTF_UPKEYS + GPTF_DOWNKEYS,
    0,
    XMOVE,
    YMOVE
};

UBYTE   hertz = 50;
SHORT   codeval,error;
BOOL    UnitOpened, DeviceOpened = FALSE;

/*need to set hertz prior to calling*/
void BeginGameDevice()
{
    if(!(game_msg_port = CreatePort("RKM_game_port", 0)))
        cleanexit("Can't create Port\n", RETURN_FAIL);

    if(!(game_io_msg = CreateStdIO(game_msg_port)))
        cleanexit("cant create IO request\n", RETURN_FAIL);

    if(error=OpenDevice("gameport.device", 1, game_io_msg, 0))
        cleanexit("can't open gameport.device\n", RETURN_FAIL);
    else DeviceOpened = TRUE;

    game_event = (struct InputEvent *)AllocMem(sizeof(struct InputEvent), MEMF_PUBLIC);

    if(!(set_controller_type(GPCT_RELJOYSTICK)))
        cleanexit("gameport unit in use?\n", RETURN_FAIL);

    /*set trigger conditions*/
    set_trigger_conditions(&joyTrigger);
    flush_buffer();

    send_read_request();
}


BOOL set_controller_type(BYTE type)
{
    BOOL success = FALSE;
    BYTE controller_type = 0;
    Forbid();
    game_io_msg->io_Command = GPD_ASKCTYPE;
    game_io_msg->io_Length = 1;
    game_io_msg->io_Flags = IOF_QUICK;
    game_io_msg->io_Data = (APTR)&controller_type;
    DoIO(game_io_msg);

    /*get the controller if not in use*/
    if(controller_type == GPCT_NOCONTROLLER)
    {
        game_io_msg->io_Command = GPD_SETCTYPE;
        game_io_msg->io_Flags = IOF_QUICK;
        game_io_msg->io_Length = 1;
        game_io_msg->io_Data = (APTR)&type;
        DoIO(game_io_msg);
        success = TRUE;
        UnitOpened = TRUE;
    }
    Permit();
    return success;
}

void set_trigger_conditions(struct GamePortTrigger *gpt)
{
    joyTrigger.gpt_Timeout = (UWORD)hertz * 20;

    game_io_msg->io_Command = GPD_SETTRIGGER;
    game_io_msg->io_Length = (LONG)sizeof(struct GamePortTrigger);
    game_io_msg->io_Data = (APTR)gpt;
    DoIO(game_io_msg);
}

void check_move(int *x , int *y)
{
    WORD xmove, ymove;
    xmove = game_event->ie_X;
    ymove = game_event->ie_Y;

    if(xmove != 0 || ymove !=0)
    {
        if(xmove == 1 && ymove == 0)
        {
            *x = *x + 1;
        }
        if(xmove == -1 && ymove == 0)
        {
            *x = *x - 1;
        }
        if(xmove == 0 && ymove == 1)
        {
            *y = *y + 1;
        }
        if(xmove == 0 && ymove == -1)
        {
            *y = *y - 1;
        }

        if(xmove == 1 && ymove == 1)
        {
            *x = *x + 1;
            *y = *y + 1;
        }
        if(xmove == -1 && ymove == 1)
        {
            *x = *x - 1;
            *y = *y + 1;
        }
        if(xmove == 1 && ymove == -1)
        {
            *x = *x + 1;
            *y = *y - 1;
        }
        if(xmove == -1 && ymove == -1)
        {
            *x = *x - 1;
            *y = *y - 1;
        }
    }
    game_event->ie_X = 0;
    game_event->ie_Y = 0;
}

void flush_buffer()
{
    game_io_msg->io_Command = CMD_CLEAR;
    game_io_msg->io_Flags = IOF_QUICK;
    DoIO(game_io_msg);
}

void send_read_request()
{
    game_io_msg->io_Command = GPD_READEVENT;
    game_io_msg->io_Length = sizeof(struct InputEvent);
    game_io_msg->io_Data = (APTR)game_event;
    SendIO(game_io_msg); /*async*/
}

void free_gp_unit()
{
    BYTE type = GPCT_NOCONTROLLER;
    game_io_msg->io_Command = GPD_SETCTYPE;
    game_io_msg->io_Flags = IOF_QUICK;
    game_io_msg->io_Length = 1;
    game_io_msg->io_Data = (APTR)&type;
    DoIO(game_io_msg);
}
void cleanup(void)
{
    if(DeviceOpened) CloseDevice(game_io_msg);
    if(game_io_msg) DeleteStdIO(game_io_msg);
    if(game_msg_port) DeletePort(game_msg_port);
    if(game_event) FreeMem(game_event, sizeof(struct InputEvent));
}

void cleanexit(UBYTE *msg, LONG er)
{
    if(*msg) printf(msg);
    if(UnitOpened) free_gp_unit();
    cleanup();
    exit(er);
}