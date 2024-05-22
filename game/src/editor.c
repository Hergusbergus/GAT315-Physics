#include "editor.h"
#define RAYGUI_IMPLEMENTATION
#define EDITOR_DATA(data) TextFormat("%0.2f", data), &data
#include "../../raygui/src/raygui.h"
#include <body.h>
#include "render.h"

bool ncEditorActive = true;
bool ncEditorIntersect = false;
ncEditorData_t ncEditorData;

Rectangle editorRect;

Vector2 anchor01 = { 672, 56 };
Vector2 anchor02 = { 728, 120 };
Vector2 anchor03 = { 728, 360 };

bool EditorBoxActive = true;

void InitEditor()
{
    GuiLoadStyle("raygui/styles/cyber/style_cyber.rgs");

    ncEditorData.GravityValue = 0.0f;
    ncEditorData.GravitationValue = 0.0f;
    ncEditorData.BodyTypeActive = 0;
    ncEditorData.MassValue = 0.0f;
}

void UpdateEditor(Vector2 position)
{
    //
}

void DrawEditor()
{
    if (EditorBoxActive)
    {
        EditorBoxActive = !GuiWindowBox((Rectangle) { anchor01.x + 0, anchor01.y + 0, 256, 656 }, "Editor");
        GuiGroupBox((Rectangle) { anchor02.x + 0, anchor02.y + 8, 344, 176 }, "Body");
        GuiGroupBox((Rectangle) { anchor03.x + 0, anchor03.y + 8, 344, 208 }, "World");
        GuiSliderBar((Rectangle) { anchor03.x + 96, anchor03.y + 48, 120, 16 }, "Gravity", NULL, & ncEditorData.GravityValue, 0, 100);
        GuiSliderBar((Rectangle) { anchor03.x + 96, anchor03.y + 88, 120, 16 }, "Gravitation", NULL, & ncEditorData.GravitationValue, 0, 1);
        GuiComboBox((Rectangle) { anchor02.x + 32, anchor02.y + 32, 128, 32 }, "Dynamic;Kinematic;Static", & ncEditorData.BodyTypeActive);
        GuiSliderBar((Rectangle) { anchor02.x + 112, anchor02.y + 112, 120, 16 }, "Mass", NULL, & ncEditorData.MassValue, 0, 1);
        GuiSliderBar((Rectangle) { anchor01.x + 120, anchor01.y + 168, 120, 16 }, "Gravity Scale", EDITOR_DATA(ncEditorData.GravityValue), 0, 10);
    }
}

ncBody* GetBodyIntersect(ncBody* bodies, Vector2 position)
{
    for (ncBody* body = bodies; body; body = body->next)
    {
        Vector2 screen = ConvertWorldToScreen(body->position);
        if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass * 0.5f)))
        {
            return body;
        }
    }

    return NULL;
}

void DrawLineBodyToPosition(ncBody* body, Vector2 position)
{
    Vector2 screen = ConvertWorldToScreen(body->position);
    DrawLine((int)screen.x, (int)screen.y, (int)position.x /*- cursorTexture.width / 2*/, (int)position.y /*- cursorTexture.height / 2*/, YELLOW);
}