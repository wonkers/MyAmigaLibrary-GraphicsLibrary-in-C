#include "keyInput.h"

int GetKey(int *pressed)
{
	int i,v;
	struct 	IOStdReq	*keyRequest;
	struct 	MsgPort		*keyPort;
	UBYTE	*keyMatrix;
	
	if(keyPort=CreatePort(NULL,NULL))
	{
		if(keyRequest = (struct IOStdReq *)CreateExtIO(keyPort, sizeof(struct IOStdReq)))
		{
			if(!OpenDevice("keyboard.device", NULL, (struct IORequest *)keyRequest, NULL))
			{
				if (keyMatrix =AllocMem(16, MEMF_PUBLIC|MEMF_CLEAR))
				{
					keyRequest->io_Command = KBD_READMATRIX;
					keyRequest->io_Data = (APTR)keyMatrix;
					keyRequest->io_Length = 13;
					DoIO((struct IORequest*)keyRequest);
				}
				
			}
			CloseDevice((struct IORequest *)keyRequest);
		}
		DeleteExtIO((struct IORequest *)keyRequest);
	}
	DeletePort(keyPort);
#ifdef ONEPRESS
	if(keyMatrix[*pressed/8] == 0)
		*pressed = 0;
#endif	
	for(i = 0; i < 13; i++)
	{
		if(keyMatrix[i] > 0)
		{
			v = i * 8;
			while(keyMatrix[i] != 0)
			{
				keyMatrix[i] >>= 1;
				v++;
			}
			FreeMem(keyMatrix, 16);
#ifdef ONEPRESS			
			if(v-1 == *pressed)
			{
				return 0;
			}
#endif
			*pressed = v-1;
			return v-1;
		}
	}
	FreeMem(keyMatrix, 16);
	return 0;
}