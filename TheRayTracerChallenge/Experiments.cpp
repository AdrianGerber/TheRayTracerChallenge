#include "Experiments.h"

void DrawWatchface() {
    const size_t xSize = 100, ySize = 100;
    Canvas canvas(xSize, ySize);

    Tuple currentPoint = Tuple::CreatePoint(0.0f, 0.0f, 0.0f);
    //Start at 12h
    currentPoint = currentPoint * Transform::CreateTranslation(0.0f, ySize / 2.0f * 0.9, 0.0f);

    //Rotate by 30 degrees each step
    Transform rotation = Transform::CreateRotationZ(Constants::PI * 2.0f / 12.0f);

    //Draw the 12 points
    for (size_t i = 0; i < 12; i++) {
        //Rotate around watchface
        currentPoint = rotation * currentPoint;

        //Move the point into the canvas (Rotate to view from a different angle)
        Tuple pointOnScreen = Transform::CreateRotationY(0.0f).Translate(xSize / 2.0f, ySize / 2.0f, 0.0f) * currentPoint;

        //Draw pixel on screen
        canvas.WritePixel(Color(255.0f, 0.0f, 0.0f), static_cast<size_t>(pointOnScreen.x), static_cast<size_t>(pointOnScreen.y));

    }

    canvas.SaveToFile("watchface");
}