#include "force.h"
#include "body.h"

void ApplyGravitationForce(ncBody* bodies, float strength)
{
    for (ncBody* body1 = bodies; body1; body1 = body1->next)
    {
        for (ncBody* body2 = bodies; body2; body2 = body2->next)
        {
            // Skip calculation if bodies are the same
            if (body1 == body2)
            {
                continue;
            }

            Vector2 direction = Vector2Subtract(body2->position, body1->position);
            float distance = Vector2Length(direction);

            // Clamping at 1
            distance = fmaxf(1.0f, distance);

            float force = (body1->mass * body2->mass / (distance * distance)) * strength;

            // Scale normalized direction vector by force
            direction = Vector2Scale(Vector2Normalize(direction), force);

            // Apply equal and opposite forces to both bodies
            ApplyForce(body1, direction, FM_FORCE);
            ApplyForce(body2, Vector2Negate(direction), FM_FORCE);
        }
    }
}