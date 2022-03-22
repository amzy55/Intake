#pragma once
#include "EntityController.h"

class EnemyController :
    public EntityController
{
    virtual void Update(Entity& entity);
};

