#pragma once
#include <spring.h>
typedef struct ncBody ncBody;

void ApplyGravitationForce(ncBody* bodies, float strength);

void ApplySpringForce(ncSpring_t* springs);
