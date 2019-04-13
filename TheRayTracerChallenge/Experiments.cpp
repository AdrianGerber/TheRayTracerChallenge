#include "Experiments.h"

//End of chapter 4
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

//End of chapter 5
void DrawSphere() {
    const size_t xSize = 100, ySize = 100;
    Canvas canvas(xSize, ySize);

    Point origin = Point::CreatePoint(0.0f, 0.0f, -4.0f);
    
    //Sphere to draw
    auto s = std::make_shared<Sphere>();
    s->SetTransform(
        Transform::CreateScale(1.2f, 0.8f, 1.0f)
    );

    //Go through every pixel of the screen
    for (size_t x = 0; x < xSize; x++) {
        for (size_t y = 0; y < ySize; y++) {
            //Calculate the ray's direction for the current pixel
            float xComponent = static_cast<float>(x) / static_cast<float>(xSize) - 0.5f;
            float yComponent = static_cast<float>(y) / static_cast<float>(ySize) - 0.5f;

            Vector direction = Vector::CreateVector(xComponent * 1.0f, yComponent * 1.0f, 1.0f);
            direction.Normalize();
          
            //Cast the ray
            Ray ray(origin, direction);
            IntersectionBuffer intersections = ray.FindIntersections(s);


            //Color all pixels that represent the sphere
            if (intersections.GetCount() != 0) {
                canvas.WritePixel(Color(255.0f, 255.0f, 0.0f), x, y);
            }
            
        }
    }

    
    canvas.SaveToFile("sphere");
}

//End of chapter 6
void DrawSphereReflections() {
    const size_t xSize = 100, ySize = 100;
    Canvas canvas(xSize, ySize);

    Point origin = Point::CreatePoint(0.0f, 0.0f, -4.0f);

    //Sphere to draw
    auto s = std::make_shared<Sphere>();
    //s->SetTransform(
    //    Transform::CreateScale(1.2f, 0.8f, 1.0f)
    //);
    Material m;
    m.color = Color(1.0f, 0.2f, 1.0f);
    s->SetMaterial(m);

    LightSource l(Point::CreatePoint(-10.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 0.0f));


    //Go through every pixel of the screen
    for (size_t x = 0; x < xSize; x++) {
        for (size_t y = 0; y < ySize; y++) {
            //Calculate the ray's direction for the current pixel
            float xComponent = static_cast<float>(x) / static_cast<float>(xSize) - 0.5f;
            float yComponent = static_cast<float>(y) / static_cast<float>(ySize) - 0.5f;

            Vector direction = Vector::CreateVector(xComponent * 1.0f, yComponent * 1.0f, 1.0f);
            direction.Normalize();

            //Cast the ray
            Ray ray(origin, direction);
            IntersectionBuffer intersections = ray.FindIntersections(s);


            //There was an intersection
            if (intersections.GetCount() != 0) {
                Intersection hit = intersections.GetFirstHit();

                Point hitPoint = ray.PositionAt(hit.t);
                Vector normal = hit.object->SurfaceNormal(hitPoint);
                Vector eye = -ray.direction;

                canvas.WritePixel(l.Lighting(hit.object->GetMaterial(), hitPoint, eye, normal) , x, y);


            }
        }
        std::cout << x;
        std::cout << "\n";
    }


    canvas.SaveToFile("sphereReflections");
}