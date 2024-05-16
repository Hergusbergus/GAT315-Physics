#include "render.h" // Including the render header file for declarations related to rendering.
#include "raymath.h" // Including the raymath header file for vector math functions.

// Global variables defining the screen size, zoom level, and view size.
Vector2 ncScreenSize = { 1280, 720 }; // Screen size in pixels (default: 1280x720).
float ncScreenZoom = 1.0f; // Zoom level (default: 1.0).
float ncViewSize = 25.0f; // View size in world units (default: 25.0).

// Function to convert screen coordinates to world coordinates.
Vector2 ConvertScreenToWorld(Vector2 screen)
{
    // Calculate the aspect ratio of the screen.
    float ratio = ncScreenSize.x / ncScreenSize.y;

    // Calculate the extents of the view in world units.
    Vector2 extents = { ncViewSize * ratio, ncViewSize };

    // Scale the extents by the screen zoom level.
    extents = Vector2Scale(extents, ncScreenZoom);

    // Calculate the lower and upper bounds of the view.
    Vector2 lower = Vector2Negate(extents);
    Vector2 upper = extents;

    // Normalize the screen coordinates to the range [0, 1].
    float nx = screen.x / ncScreenSize.x;
    float ny = (ncScreenSize.y - screen.y) / ncScreenSize.y;

    // Convert normalized coordinates to world coordinates.
    Vector2 world;
    world.x = ((1 - nx) * lower.x) + (nx * upper.x);
    world.y = ((1 - ny) * lower.y) + (ny * upper.y);

    return world; // Return the converted world coordinates.
}

// Function to convert world coordinates to screen coordinates.
Vector2 ConvertWorldToScreen(Vector2 world)
{
    // Calculate the aspect ratio of the screen.
    float ratio = ncScreenSize.x / ncScreenSize.y;

    // Calculate the extents of the view in world units.
    Vector2 extents = { ncViewSize * ratio, ncViewSize };

    // Scale the extents by the screen zoom level.
    extents = Vector2Scale(extents, ncScreenZoom);

    // Calculate the lower and upper bounds of the view.
    Vector2 lower = Vector2Negate(extents);
    Vector2 upper = extents;

    // Calculate normalized coordinates within the view.
    float nx = (world.x - lower.x) / (upper.x - lower.x);
    float ny = (world.y - lower.y) / (upper.y - lower.y);

    // Convert normalized coordinates to screen coordinates.
    Vector2 screen;
    screen.x = nx * ncScreenSize.x;
    screen.y = (1.0f - ny) * ncScreenSize.y;

    return screen; // Return the converted screen coordinates.
}

// Function to convert world units to pixel units.
float ConvertWorldToPixel(float world)
{
    // Calculate the aspect ratio of the screen.
    float ratio = ncScreenSize.x / ncScreenSize.y;

    // Calculate the extents of the view in world units.
    Vector2 extents = { ncViewSize * ratio, ncViewSize };

    // Scale the extents by the screen zoom level.
    extents = Vector2Scale(extents, ncScreenZoom);

    // Convert world units to pixel units.
    float nx = world / extents.x;
    float pixel = nx * ncScreenSize.x;

    return pixel; // Return the converted pixel value.
}