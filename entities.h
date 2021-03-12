#include <exec/types.h>
#include <graphics/rastport.h>

#ifndef ENTITIES_H
#define ENTITIES_H

struct Entity
{
	struct Entity *next;
	struct Entity *prev;
	struct EntityData *data;
};
struct Entities
{
	int count;
	struct Entity *top;
	struct Entity *bottom;
};
struct EntityData
{
	int x;
	int y;
	char *symbol;
	char *name;
	short colour;
	/*other stuff*/
};



void AddEntity(struct Entities *entities, struct EntityData *data);
int RemoveEntity(struct Entities *entities, struct Entity *entity);
void FreeEntities(struct Entities *entities);
void DrawEntities(struct RastPort *rastPort, struct Entities *entities);


#endif