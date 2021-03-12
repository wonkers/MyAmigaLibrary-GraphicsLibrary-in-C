#include "entities.h"


void AddEntity(struct Entities *entities, struct EntityData *data)
{
	struct Entity *newEntity = (struct Entity*)AllocMem(sizeof(struct Entity), MEMF_CLEAR);;
	if(entities->count == 0)
	{
		newEntity->data = data;
		newEntity->next = newEntity;
		newEntity->prev = newEntity;
		entities->top = newEntity;
		entities->bottom = newEntity;

	}
	else
	{
		newEntity->data = data;
		newEntity->next = entities->bottom;
		newEntity->prev = entities->top;
		entities->top->next = newEntity;
		entities->bottom->prev = newEntity;

		entities->top = newEntity;
	}
	entities->count++;
}
int RemoveEntity(struct Entities *entities, struct Entity *entity)
{
	if(entity == entities->bottom)
	{
		entities->bottom = entities->bottom->next;
		entities->bottom->prev = entities->top;
		entities->top->next = entities->bottom;
		FreeMem(entity, sizeof(struct Entity));
		entities->count--;
	}
	else if(entity == entities->top)
	{
		entities->top = entities->top->prev;
		entities->bottom->prev = entities->top;
		entities->top->next = entities->bottom;

		FreeMem(entity, sizeof(struct Entity));
		entities->count--;
	}
	else
	{
		struct Entity tempEntity = *entities->bottom;
		
		while(tempEntity.data->name != entity->data->name)
		{
			tempEntity = *tempEntity.next;
		}
		
		entity->prev->next = entity->next;
		entity->next->prev = entity->prev;
		FreeMem(entity, sizeof(struct Entity));
		entities->count--;
	}

	return entities->count;
}

void FreeEntities(struct Entities *entities)
{
	while(entities->count != 0)
	{
		RemoveEntity(entities, entities->bottom);
	}
}

void DrawEntities(struct RastPort *rastPort, struct Entities *entities)
{
	struct Entity *entity;
	SetRast(rastPort, 0);

	/*only attempt to draw entities if any exist*/
	if(entities->count != 0)
	{
		entity = entities->bottom;
		do
		{
			SetAPen(rastPort, entity->data->colour);
			Move(rastPort, entity->data->x + 9, entity->data->y+13);
			Text(rastPort, entity->data->symbol, 1);
			entity = entity->next;

		}while(entity != entities->bottom);
	}
	
	
	
}
