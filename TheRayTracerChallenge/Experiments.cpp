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
    auto s = Shape::MakeShared<Sphere>();
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
			IntersectionBuffer intersections;
			s->FindIntersections(ray, intersections);


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
    auto s = Shape::MakeShared<Sphere>();
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
			IntersectionBuffer intersections;
			s->FindIntersections(ray, intersections);


            //There was an intersection
            if (intersections.GetCount() != 0) {
                Intersection hit = intersections.GetFirstHit();

                Point hitPoint = ray.PositionAt(hit.t);
                Vector normal = s->SurfaceNormal(hitPoint, hit);
                Vector eye = -ray.direction;

                canvas.WritePixel(l.Lighting(s, hitPoint, eye, normal, false) , x, y);


            }
        }
        std::cout << x;
        std::cout << "\n";
    }


    canvas.SaveToFile("sphereReflections");
}



//End of chapter 7 / 8 / 9 / 10 / 11 / 12 / 13 / 14
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
	
	//auto lightSource2 = std::make_shared<LightSource>();
	//lightSource2->SetIntensity(Color(0.0f, 0.0f, 1.0f));
	//lightSource2->SetPosition(Point::CreatePoint(14.0f, 10.0f, -10.0f));
	//world.AddLightSource(lightSource2);


	Transform transform;
	Material material;

	//Shapes

	//floor
	transform = Transform();
	material = Material();
	material.pattern = std::make_shared<CheckerPattern>(Color(0.05, 0.05, 0.05), Color(0.4, 0.4, 0.4));
	material.pattern->SetTransform(Transform::CreateScale(0.4, 0.4, 0.4));
	material.specular = 0.0;
	material.reflective = 0.8;
	material.transparency = 0.01;

	auto floor = Shape::MakeShared<Plane>();
	floor->SetMaterial(material);
	floor->SetTransform(transform);
	world.AddShape(floor);

	//backdrop
	transform = Transform();
	transform.RotateX(Constants::PI / 2.0).RotateZ(Constants::PI / 5.0).Translate(0.0, 0.0, 10.0);
	material = Material();
	
	{
		auto p1 = std::make_shared<StripePattern>(
			std::make_shared<GradientPattern>(Color(0.0, 0.0, 0.1), Color(0.0, 0.0, 0.2)),
			std::make_shared<GradientPattern>(Color(0.1, 0.0, 0.0), Color(0.2, 0.0, 0.0)));
		auto p2 = std::make_shared<RingPattern>(Color(0.2, 0.2, 0.2), Color(0.0, 0.0, 0.0));
		material.pattern = std::make_shared<BlendedPattern>(p1, p2);
	}
	
	material.pattern->SetTransform(Transform::CreateTranslation(1.5, 1.0, 1.0));
	material.specular = 0.4;
	material.shininess = 200;
	material.reflective = 0.4;

	auto backdrop = Shape::MakeShared<Plane>();
	backdrop->SetMaterial(material);
	backdrop->SetTransform(transform);
	world.AddShape(backdrop);

	//middle
	//transform = Transform::CreateTranslation(-0.5f, 1.0f, 0.5f);
	transform = Transform::CreateRotationZ(Constants::PI / 2.0).RotateY(Constants::PI / 4.0).Translate(0.1f, 1.0f, 0.5f);
	//transform = Transform::CreateTranslation(-0.5f, 1.0f, 0.5f);
	material = Material();
	material.pattern = std::make_shared<StripePattern>(Color(0.2, 0.2, 0.2), Color(0.6, 0.6, 0.6));
	material.pattern->SetTransform(Transform::CreateScale(0.1, 0.1, 0.1).RotateZ(Constants::PI / 2.0));
	material.diffuse = 0.7f;
	material.specular = 0.6f;
	material.shininess = 300;
	material.reflective = 0.2;

	auto middle = Shape::MakeShared<Cylinder>(-1.0, 1.0, true);
	middle->SetMaterial(material);
	middle->SetTransform(transform);
	world.AddShape(middle);

	//right
	transform = Transform().Scale(0.5, 0.5, 0.5).RotateY(Constants::PI / 4.0).Translate(1.5, 0.5, -0.5);
		
	material = Material();
	material.pattern = std::make_shared<ColorPattern>(Color(0.7, 0.7, 0.7));
	material.pattern->SetTransform(
		Transform::CreateScale(2.0, 2.0, 2.0).RotateY(-Constants::PI / 2.0).Translate(1.0, 1.0, 1.0)
	);
	material.diffuse = 0.7;
	material.specular = 0.7;
	material.reflective = 0.1;

	auto right = Shape::MakeShared<Cube>();
	right->SetMaterial(material);
	right->SetTransform(transform);
	world.AddShape(right);

	//left
	transform = Transform::CreateRotationX(Constants::PI * 1.15).RotateZ(Constants::PI / 4.0).Translate(-1.5, 0.0, 0.0);
	material = Material();
	material.pattern = std::make_shared<ColorPattern>(Color(0.7f, 0.7f, 0.7f));
	material.diffuse = 0.2;
	material.specular = 0.9;
	material.ambient = 0.2;
	material.shininess = 300;
	material.transparency = 0.0;
	material.refractiveIndex = 2;
	material.reflective = 0.3;

	auto left = Shape::MakeShared<Cone>(-1.0, 0.0, true);
	
	left->SetMaterial(material);
	left->SetTransform(transform);
	world.AddShape(left);

	auto group = Shape::MakeShared<ShapeGroup>();
	
	for (int i = -3; i <= 3; i++) {
		auto shape = Shape::MakeShared<Sphere>();
		shape->SetTransform(Transform::CreateScale(0.1, 0.1, 0.1).Translate(static_cast<double>(i), 0.2, -1.0));
		auto m = shape->GetMaterial();
		m.pattern = std::make_shared<StripePattern>(Color((static_cast<double>(i)+6.0) / 6.0, 0.0, 0.0), Color(0.1, 0.1, 0.1));
		m.pattern->SetTransform(Transform::CreateScale(0.1, 0.1, 0.1));
		shape->SetMaterial(m);
		group->AddShape(shape);
	}
	group->PartitionChildren(3);
	world.AddShape(group);

	auto groupgroup = Shape::MakeShared<ShapeGroup>();

	for (int i = 0; i < 4; i++) {
		auto copy = group->Copy();
		copy->SetTransform(Transform::CreateTranslation(0.0, static_cast<double>(i) / 2.0, -1.5));

		groupgroup->AddShape(copy);
	}

	for (int i = 0; i < 4; i++) {
		auto groupgroupgroup = groupgroup->Copy();
		groupgroupgroup->SetTransform(Transform::CreateTranslation(0.0, static_cast<double>(i) / 4.0, static_cast<double>(i) / 4.0).RotateY(Constants::PI / 6.0));

		world.AddShape(groupgroupgroup);
	}
	

	camera.RenderFrame(world).SaveToFile("chapter14");

	std::cout << "\n";
	std::cout << "Rays: " << std::to_string(world.numberOfRaysCast) << "\n";
}

void DrawBoundingBoxScene()
{
	World world;

	//Camera
	Camera camera(100, 50, Constants::PI / 3.0f,
		Camera::CreateViewTransform(
			Point::CreatePoint(10.0, 5.0, -5.0),
			Point::CreatePoint(5.0, 3.0, 5.0),
			Vector::CreateVector(0.0, 1.0, 0.0)
		)
	);

	//Light source
	auto lightSource = std::make_shared<LightSource>();
	lightSource->SetIntensity(Color(1.0f, 1.0f, 1.0f));
	lightSource->SetPosition(Point::CreatePoint(-10.0f, 10.0f, -10.0f));
	world.AddLightSource(lightSource);

	auto group = Shape::MakeShared<ShapeGroup>();

	int size = 10;

	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			for (int z = 0; z < size; z++) {
				double fracX = static_cast<double>(x) / static_cast<double>(size);
				double fracY = static_cast<double>(y) / static_cast<double>(size);
				double fracZ = static_cast<double>(z) / static_cast<double>(size);

				auto shape = Shape::MakeShared<Sphere>();

				Material m;
				m.shininess = 300;
				m.ambient = 0.09;
				m.diffuse = 0.3;
				m.specular = 0.9;
				m.reflective = 0.3;
				m.transparency = 0.0;
				m.pattern = std::make_shared<ColorPattern>(Color(fracX, fracY, fracZ));
				shape->SetMaterial(m);

				Transform t;
				t.Scale(0.9, 0.9, 0.9);
				t.Translate(fracX * 30.0 - 15.0, fracY * 30.0 - 15.0, fracZ * 20.0 + 3.0);
				shape->SetTransform(t);

				group->AddShape(shape);
			}
		}
	}

	group->PartitionChildren(5);
	world.AddShape(group);

	camera.RenderFrame(world).SaveToFile("boundingBoxTest");

	std::cout << "\n";
	std::cout << "Rays: " << std::to_string(world.numberOfRaysCast) << "\n";
}

void DrawTriangleScene()
{
	World world;

	//Camera
	Camera camera(1080, 1920, Constants::PI / 3.0f,
		Camera::CreateViewTransform(
			Point::CreatePoint(-50.0, 260.0, -70.0),
			Point::CreatePoint(45.0, 60.0, 0.0),
			Vector::CreateVector(0.0, 1.0, 0.0)
		)
	);

	//Light source
	auto lightSource = std::make_shared<LightSource>();
	lightSource->SetIntensity(Color(1.0f, 1.0f, 1.0f));
	lightSource->SetPosition(Point::CreatePoint(-70.0f, 400.0f, -70.0f));
	world.AddLightSource(lightSource);

	OBJParser parser;
	
	parser.ParseFile("C:/Users/Monst/Desktop/ML_HP.obj");

	std::cout << "Splitting groups and generating bounding boxes ... ";

	auto group = parser.MakeGroup();
	Material groupMaterial;
	groupMaterial.ambient = 0.4;
	groupMaterial.specular = 0.4;
	groupMaterial.shininess = 200;
	group->SetMaterial(groupMaterial);
	//auto group2 = group->Copy();

	auto background = Shape::MakeShared<Plane>();
	//background->SetTransform(Transform::CreateRotationX(Constants::PI / 2.0).Translate(0.0, 0.0, 5.0));
	Material backgroundMaterial;
	backgroundMaterial.reflective = 0.0;
	backgroundMaterial.specular = 0.2;
	backgroundMaterial.pattern = std::make_shared<CheckerPattern>(Color(0.1, 0.1, 0.1), Color(0.4, 0.4, 0.4));
	backgroundMaterial.pattern->SetTransform(Transform::CreateScale(10, 10, 10));
	background->SetMaterial(backgroundMaterial);

	group->SetTransform(Transform::CreateTranslation(-60.0, 0.0, -50.0));
	group->PartitionChildren(5);
	
	/*group2->SetTransform(Transform::CreateRotationY(Constants::PI / 2.7).Translate(2.5, 0.0, 0.0));
	group2->PartitionChildren(5);
	Material m;
	m.pattern = std::make_shared<RingPattern>(Color(1.0, 1.0, 0.0), Color(0.3, 0.0, 0.0));
	m.shininess = 400;
	m.specular = 0.6;
	m.diffuse = 0.6;
	m.pattern->SetTransform(Transform::CreateScale(0.1, 0.1, 0.1));
	group2->SetMaterial(m);
	*/
	world.AddShape(group);
	world.AddShape(background);
	//world.AddShape(group2);



	std::cout << "Done\n";

	camera.RenderFrame(world).SaveToFile("Triangles");

	std::cout << "\n";
	std::cout << "Rays: " << std::to_string(world.numberOfRaysCast) << "\n";
}
