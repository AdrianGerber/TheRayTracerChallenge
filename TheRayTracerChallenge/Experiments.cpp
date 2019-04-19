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
        Transform::CreateScale(1.2, 0.8, 1.0)
    );

    //Go through every pixel of the screen
    for (size_t x = 0; x < xSize; x++) {
        for (size_t y = 0; y < ySize; y++) {
            //Calculate the ray's direction for the current pixel
            double xComponent = static_cast<double>(x) / static_cast<double>(xSize) - 0.5;
            double yComponent = static_cast<double>(y) / static_cast<double>(ySize) - 0.5;

            Vector direction = Vector::CreateVector(xComponent * 1.0, yComponent * 1.0, 1.0);
            direction.Normalize();
          
            //Cast the ray
            Ray ray(origin, direction);
            IntersectionBuffer intersections = s->FindIntersections(ray);


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

    Point origin = Point::CreatePoint(0.0, 0.0, -4.0);

    //Sphere to draw
    auto s = std::make_shared<Sphere>();
    //s->SetTransform(
    //    Transform::CreateScale(1.2f, 0.8f, 1.0f)
    //);
    Material m;
	auto color = std::make_shared<ColorPattern>();
	color->SetColor(Color(1.0f, 0.2f, 1.0f));
	m.pattern = color;
    s->SetMaterial(m);

    LightSource l(Point::CreatePoint(-10.0f, 10.0f, -10.0f), Color(1.0f, 1.0f, 0.0f));


    //Go through every pixel of the screen
    for (size_t x = 0; x < xSize; x++) {
        for (size_t y = 0; y < ySize; y++) {
            //Calculate the ray's direction for the current pixel
            double xComponent = static_cast<double>(x) / static_cast<double>(xSize) - 0.5;
            double yComponent = static_cast<double>(y) / static_cast<double>(ySize) - 0.5;

            Vector direction = Vector::CreateVector(xComponent * 1.0, yComponent * 1.0, 1.0);
            direction.Normalize();

            //Cast the ray
            Ray ray(origin, direction);
            IntersectionBuffer intersections = s->FindIntersections(ray);


            //There was an intersection
            if (intersections.GetCount() != 0) {
                Intersection hit = intersections.GetFirstHit();

                Point hitPoint = ray.PositionAt(hit.t);
                Vector normal = s->SurfaceNormal(hitPoint);
                Vector eye = -ray.direction;

                canvas.WritePixel(l.Lighting(s, hitPoint, eye, normal, false) , x, y);


            }
        }
        std::cout << x;
        std::cout << "\n";
    }


    canvas.SaveToFile("sphereReflections");
}



//End of chapter 7 / 8 / 9/ 10 / 11
void DrawChapter7Scene()
{
	World world;
	
	//Camera
	Camera camera(1920, 1080, Constants::PI / 3.0f, 
		Camera::CreateViewTransform(
			Point::CreatePoint(0.0f, 1.5f, -5.0f),
			Point::CreatePoint(0.0f, 1.0f, 0.0f),
			Vector::CreateVector(0.0f, 1.0f, 0.0f)
		)
	);

	//Light sources
	auto lightSource = std::make_shared<LightSource>();
	lightSource->SetIntensity(Color(1.0f, 1.0f, 1.0f));
	lightSource->SetPosition(Point::CreatePoint(-10.0f, 10.0f, -10.0f));
	world.AddLightSource(lightSource);
	
	auto lightSource2 = std::make_shared<LightSource>();
	lightSource2->SetIntensity(Color(0.0f, 0.0f, 1.0f));
	lightSource2->SetPosition(Point::CreatePoint(14.0f, 10.0f, -10.0f));
	world.AddLightSource(lightSource2);


	Transform transform;
	Material material;

	//Shapes

	//floor
	transform = Transform();
	material = Material();
	material.pattern = std::make_shared<CheckerPattern>(Color(1.0f, 0.8f, 0.1f), Color(0.0, 0.1, 0.7));
	material.pattern->SetTransform(Transform::CreateScale(0.2, 0.2, 0.2));
	material.specular = 0.0;
	material.reflective = 0.3;

	auto floor = std::make_shared<Plane>();
	floor->SetMaterial(material);
	floor->SetTransform(transform);
	world.AddShape(floor);

	//backdrop
	transform = Transform();
	transform.RotateX(Constants::PI / 2.0).RotateZ(Constants::PI / 5.0).Translate(0.0, 0.0, 10.0);
	material = Material();
	
	{
		auto p1 = std::make_shared<StripePattern>(
			std::make_shared<GradientPattern>(Color(0.0, 0.0, 0.1), Color(0.0, 0.0, 1.0)),
			std::make_shared<GradientPattern>(Color(0.1, 0.0, 0.0), Color(1.0, 0.0, 0.0)));
		auto p2 = std::make_shared<RingPattern>(Color(0.5, 0.5, 0.5), Color(0.0, 0.0, 0.0));
		material.pattern = std::make_shared<BlendedPattern>(p1, p2);
	}
	
	material.pattern->SetTransform(Transform::CreateTranslation(1.5, 1.0, 1.0));
	material.specular = 0.1;

	auto backdrop = std::make_shared<Plane>();
	backdrop->SetMaterial(material);
	backdrop->SetTransform(transform);
	world.AddShape(backdrop);

	//middle
	//transform = Transform::CreateTranslation(-0.5f, 1.0f, 0.5f);
	transform = Transform::CreateRotationY(Constants::PI / 4.0).Translate(0.1f, 1.0f, 0.5f);
	//transform = Transform::CreateTranslation(-0.5f, 1.0f, 0.5f);
	material = Material();
	material.pattern = std::make_shared<StripePattern>(Color(0.1, 0.9, 0.9), Color(0.3, 0.5, 0.0));
	material.pattern->SetTransform(Transform::CreateScale(0.2, 0.2, 0.2));
	material.diffuse = 0.7f;
	material.specular = 0.3f;

	auto middle = std::make_shared<Sphere>();
	middle->SetMaterial(material);
	middle->SetTransform(transform);
	world.AddShape(middle);

	//right
	transform = Transform::CreateTranslation(1.5f, 0.5f, -0.5f)
		* Transform::CreateScale(0.5f, 0.5f, 0.5f);

	material = Material();
	material.pattern = std::make_shared<GradientPattern>(Color(0.1f, 0.1f, 0.3f), Color(0.1f, 0.1f, 0.0f));
	material.pattern->SetTransform(
		Transform::CreateScale(2.0, 2.0, 2.0).RotateY(-Constants::PI / 2.0).Translate(1.0, 1.0, 1.0)
	);
	material.diffuse = 0.7;
	material.specular = 0.3;
	material.reflective = 0.4;

	auto right = std::make_shared<Sphere>();
	right->SetMaterial(material);
	right->SetTransform(transform);
	world.AddShape(right);

	//left
	transform = Transform::CreateTranslation(-1.5f, 0.33f, -0.75f)
		* Transform::CreateScale(0.33f, 0.33f, 0.33f);

	material = Material();
	material.pattern = std::make_shared<StripePattern>(Color(1.0f, 0.9f, 0.9f), Color(0.0f, 0.2f, 0.9f));
	material.diffuse = 0.7f;
	material.specular = 0.3f;

	auto left = std::make_shared<Sphere>();
	left->SetMaterial(material);
	left->SetTransform(transform);
	world.AddShape(left);

	camera.RenderFrame(world).SaveToFile("chapter10");
}
