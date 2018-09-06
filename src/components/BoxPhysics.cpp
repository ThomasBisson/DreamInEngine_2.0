//
// Created by ThomasBI on 05/03/2018.
//

#include <Box2D/Dynamics/b2Body.h>
#include "components/BoxPhysics.h"

b2Body* BoxPhysics::addRec(b2World *world, int x, int y, int w, int h, bool dyn)
{
	b2BodyDef bodydef;
	bodydef.position.Set(x, y);
	//bodydef.gravityScale = 5.0;
	if (dyn)
		bodydef.type = b2_dynamicBody;
	body = world->CreateBody(&bodydef);

	b2PolygonShape bshape;
	bshape.SetAsBox(w, h);

	b2FixtureDef fixturedef;
	fixturedef.shape = &bshape;
	fixturedef.density = 1.0;
	fixturedef.restitution = 3.0;
	body->CreateFixture(&fixturedef);

	// TODO: nullptr to remove ! should return a value (did this to Compile)
	return nullptr;
}
