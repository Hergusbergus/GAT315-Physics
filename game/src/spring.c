#include "spring.h"
#include "body.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

ncSpring_t* ncSprings = NULL;

ncSpring_t* CreateSpring(ncBody* body1, ncBody* body2, float restLength, float k)
{
	// Allocate memory for new Body
	ncSpring_t* spring = (ncSpring_t*)malloc(sizeof(ncSpring_t));

	// Check if allocation is successful
	assert(spring);

	memset(spring, 0, sizeof(ncSpring_t));
	spring->body1 = body1;
	spring->body2 = body2;
	spring->restingLength = restLength;
	spring->k = k;
	
	return spring;
}

void AddSpring(ncSpring_t* spring)
{
	assert(spring);

	// Initialize 'prev' to NULL and 'next' to the head of the list
	spring->prev = NULL;
	spring->next = ncSprings;

	// If list is not empty, update 'prev' of existing head
	if (ncSprings != NULL)
	{
		ncSprings->prev = spring;
	}

	// Update head of the list to new Body
	ncSprings = spring;
}

void DestroySpring(ncSpring_t* spring)
{
	// Assert if provided Body is not NULL
	assert(spring);

	// If 'prev' is not NULL, set 'prev->next' to 'body->next'
	if (spring->prev)
	{
		spring->prev->next = spring->next;
	}

	// If 'next' is not NULL, set 'next->prev' to 'body->prev'
	if (spring->next)
	{
		spring->next->prev = spring->prev;
	}

	// If body is the head, update head to 'body->next'
	if (spring == ncSprings) {
		ncSprings = spring->next;
	}

	// Free the body
	free(spring);
}

void ApplySpringForce(ncSpring_t* springs)
{
	for (ncSpring_t* spring = springs; spring; spring->next)
	{
		Vector2 direction = Vector2Subtract(spring->body1->position, spring->body2->position);
		if (direction.x == 0 && direction.y == 0)
		{
			continue;
		}

		float length = Vector2Length(direction);
		float x = length - spring->restingLength;
		float force = -spring->k * x;

		Vector2 ndirection = Vector2Normalize(direction);

		ApplyForce(spring->body1, Vector2Scale(ndirection, force), FM_FORCE);
		ApplyForce(spring->body2, Vector2Scale(Vector2Negate(ndirection), force), FM_FORCE);
	}
}

// Partial aid by ChatGPT. Source: https://chat.openai.com/share/3f1049f8-e717-4745-b7b2-b4c9582a949e