#include "editor.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"
#include <body.h>
#include "render.h"

bool ncEditorActive = true;
bool ncEditorIntersect = false;
ncEditorData_t ncEditorData;

Rectangle editorRect;

Vector2 anchor01 = { 950, 50 };

bool EditorBoxActive = true;

void InitEditor()
{
    GuiLoadStyle("raygui/styles/cyber/style_cyber.rgs");

    ncEditorData.GravitationValue = 2;
    ncEditorData.MassMinValue = 0.1f;
    ncEditorData.MassMaxValue = 1;
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
        GuiSliderBar((Rectangle) { anchor01.x + 80, anchor01.y + 64, 120, 16 }, "Mass min", NULL, & ncEditorData.MassMinValue, 0, 100);
        GuiSliderBar((Rectangle) { anchor01.x + 80, anchor01.y + 104, 120, 16 }, "Mass max", NULL, & ncEditorData.MassMaxValue, 0, 100);
        GuiSlider((Rectangle) { anchor01.x + 80, anchor01.y + 152, 120, 16 }, "Gravity", NULL, & ncEditorData.GravitationValue, 0, 100);
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