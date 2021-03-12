#include <proto/exec.h>
#include <exec/types.h>
#include <devices/keyboard.h>
#include <devices/input.h>
#include <exec/memory.h>
#include <exec/ports.h>

#define ONEPRESS 1

/*keyboard defines*/
	#define ESC		0x45
	#define L_SHIFT	0x60
	#define R_SHIFT	0x61
	#define	CTL		0x63
	#define ALT		0x64
	#define BACKSPACE	0x41
	#define	KB_RTN	0x44
    #define TAB     0x42
	
	#define KB_1	0x01
	#define	KB_2	0x02
	#define	KB_3	0x03
	#define	KB_4	0x04
	#define	KB_5	0x05
	#define	KB_6	0x06
	#define	KB_7	0x07
	#define	KB_8	0x08
	#define	KB_9	0x09
	#define	KB_0	0x0A

	/*keypad defines*/
	#define KP_RTN	0x44

	#define KP_1	0x1D
	#define KP_2	0x1E
	#define KP_3	0x1F
	#define KP_4	0x2D
	#define KP_5	0x2E
	#define KP_6	0x2F
	#define KP_7	0x3D
	#define KP_8	0x3E
	#define KP_9	0x3F
	#define KP_0	0x0F

	#define	UP		0x4C
	#define	DOWN	0x4D
	#define	LEFT	0x4F
	#define	RIGHT	0x4E

int GetKey(int *pressed);