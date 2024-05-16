#pragma once
#include "raylib.h"

typedef struct ncBody ncBody;

extern ncBody* ncBodies;
extern int ncBodyCount;
extern Vector2 ncGravity;

ncBody* CreateBody(Vector2 position, float mass/*, ncBodyType bodyType*/);
void AddBody(ncBody* body);
void DestroyBody(ncBody* body);
void DestroyAllBodies();