#include "body.h"
#include "mathf.h"
#include "raylib.h"
#include "raymath.h"
#include "force.h"
#include "integrator.h"
#include "render.h"
#include "editor.h"
#include "world.h"
#include "spring.h"
#include "collision.h"
#include "contact.h"

#include <stdlib.h>
#include <assert.h>

#define MAX_BODIES 10000

int main(void)
{
	ncBody* selectedBody = NULL;
	ncBody* connectBody = NULL;

	InitWindow(1280, 720, "Physics Engine");
	InitEditor();
	SetTargetFPS(60);

	// Initialize world
	ncGravity = (Vector2){ 0, 30 };

	// Body* bodies = new Body[20];
	ncBody* bodies = (ncBody*)malloc(sizeof(ncBody) * MAX_BODIES);
	assert(bodies);

	int bodyCount = 0;

	/*Define a fixedTimestep(rate that physics simulator will run at) set it to 1.0f / 50 (or number of frames you want it to run per second)
		Define a timeAccumulator(accumulates delta time)*/

	float fixedTimestep = 1.0f / 24;
	float timeAccumulator = GetFrameTime();

	// game loop
	while (!WindowShouldClose())
	{
		// Update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		ncScreenZoom += GetMouseWheelMove() * 0.2f;
		ncScreenZoom = Clamp(ncScreenZoom, 0.1f, 10);

		UpdateEditor(position);

		selectedBody = GetBodyIntersect(ncBodies, position);
		if (selectedBody)
		{
			Vector2 screen = ConvertWorldToScreen(selectedBody->position);
			DrawCircleLines(screen.x, screen.y, ConvertWorldToPixel(selectedBody->mass) + 5, YELLOW);
		}

		// create body
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) /*|| (IsMouseButtonDown(MOUSE_BUTTON_LEFT && IsKeyDown(KEY_LEFT_CONTROL)))*/)
		{
			ncBody* body = CreateBody(ConvertScreenToWorld(position), ncEditorData.MassValue, ncEditorData.BodyTypeActive);
			body->damping = 2.5f;
			body->gravityScale = -ncEditorData.GravityValue;
			body->color = (Color){ GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255 };
			body->restitution = 0.8f;

			AddBody(body);
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody)
		{
			connectBody = selectedBody;
		}
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody)
		{
			DrawLineBodyToPosition(connectBody, position);
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody)
		{
			if (selectedBody && selectedBody != connectBody)
			{
				ncSpring_t* spring = CreateSpring(connectBody, selectedBody, Vector2Distance(connectBody->position, selectedBody->position), 20);
				AddSpring(spring);
			}
		}

		if (IsKeyDown(KEY_LEFT_ALT))
		{
			if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody) connectBody = selectedBody;
			if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody) DrawLineBodyToPosition(connectBody, position);
			if (connectBody)
			{
				Vector2 world = ConvertScreenToWorld(position);
				ApplySpringForcePosition(world, connectBody, 0, 20, 5);
			}
		}

		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
		{
			selectedBody = NULL;
			connectBody = NULL;
		}

		/*Add delta time to time accumulator
			Create a while () loop that loops while the time accumulator is greater than or equal to the fixed time step
			Subtract fixed time step from time accumulator
			Apply Gravitation and other forces
			Step() each body, make sure to pass in the fixed time step for the timestep
			Destroy all contacts
			Create contacts
			Separate contacts
			Resolve contacts*/
		ncContact_t* contacts = NULL;

		while (timeAccumulator >= fixedTimestep)
		{
			timeAccumulator -= fixedTimestep;

			// Apply force
			ApplyGravitationForce(ncBodies, ncEditorData.GravitationValue);
			ApplySpringForce(ncSprings);

			// Update bodies
			for (ncBody* body = ncBodies; body; body = body->next)
			{
				Step(body, dt);
			}

			// Collision				
			DestroyAllContacts(contacts);
			CreateContacts(ncBodies, &contacts);
			SeparateContacts(contacts);
			ResolveContacts(contacts);
		}

		// Render
		BeginDrawing();
		ClearBackground(BLACK);

		// Stats
		DrawText(TextFormat("FPS: %f (%fms)", fps, 1000/fps), 10, 10, 20, LIME);
		DrawText(TextFormat("FRAME: %f", dt), 10, 30, 20, LIME);

		//DrawCircle((int)position.x, (int)position.y, 20, YELLOW);		

		// Draw bodies
		for (ncBody* body = ncBodies; body; body = body->next)
		{
			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(body->mass), body->color);
		}

		// Draw contacts
		for (ncContact_t* contact = contacts; contact; contact = contact->next)
		{
			Vector2 screen = ConvertWorldToScreen(contact->body1->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(contact->body1->mass), RED);
		}

		// Draw springs
		for (ncSpring_t* spring = ncSprings; spring; spring = spring->next)
		{
			Vector2 screen1 = ConvertWorldToScreen(spring->body1->position);
			Vector2 screen2 = ConvertWorldToScreen(spring->body2->position);
			DrawLine((int)screen1.x, (int)screen1.y, (int)screen2.x, (int)screen2.y, YELLOW);
		}


		DrawEditor(position);

		EndDrawing();
	}

	CloseWindow();

	free(bodies);
	
	return 0;
}