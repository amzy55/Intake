#pragma once

#include "EntityController.h"
#include "Entity.h"

class EnemyController : public EntityController
{
public:

    virtual void Update(Entity& entity, Tmpl8::vec2 moveBy);

protected:

private:

};

