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
	for (ncSpring_t* spring = springs; spring; spring = spring->next)
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

void ApplySpringForcePosition(Vector2 position, ncBody* body, float restLength, float k, float damping)
{
	// Check if the body pointer is null; if so, exit the function
	if (!body) return;

	// Calculate the direction vector from the body's position to the given position
	Vector2 direction = Vector2Subtract(position, body->position);

	// If the direction vector is zero (i.e., positions are the same), exit the function
	if (direction.x == 0 && direction.y == 0) return;

	// Calculate the length of the direction vector (distance between the two positions)
	float length = Vector2Length(direction);

	// Calculate the displacement from the rest length
	float x = length - restLength;

		// Apply Hooke's Law (f = -kx) to determine the spring force
	float force = -k * x;

		// Normalize the direction vector
	Vector2 ndirection = Vector2Normalize(direction);

		// Calculate the damping force (opposing force due to velocity)
	float dampingForce = damping * Vector2DotProduct(body->velocity, ndirection);

		// Calculate the total force by combining spring force and damping force
	float totalForce = force + dampingForce;

		// Apply the total force to the body in the direction of the normalized vector
		ApplyForce(body, Vector2Scale(ndirection, -totalForce), FM_FORCE);
}

// Partial aid by ChatGPT. Source: https://chat.openai.com/share/3f1049f8-e717-4745-b7b2-b4c9582a949e