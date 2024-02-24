#include "Entity.h"


BOOL CEntity::IsAlive()
{
	return this->Controller.AliveStatus == 1 && this->Controller.iHealth > 0;
}
