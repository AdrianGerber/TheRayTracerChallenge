#pragma once

#include "Tuple.h"
#include "Matrix.h"
#include "Transform.h"
#include "Constants.h"
#include <cmath>
#include <iostream>
#include "Canvas.h"
#include "World.h"

class Camera {
private:
	size_t xSize, ySize;
	double fieldOfView;
	Transform transform;

	double halfHeight, halfWidth, pixelSize;

public:

	Camera() {
		halfHeight = halfWidth = pixelSize = 0.0f;
		xSize = ySize = 100;
		fieldOfView = Constants::PI / 2.0f;
		transform.matrix = Matrix::IndentityMatrix4x4();
		CalculatePixelSize();
	}

	Camera(size_t initialXSize, size_t initialYSize, double initialFieldOfView, Transform initialTransform) {
		halfHeight = halfWidth = pixelSize = 0.0f;
		xSize = initialXSize;
		ySize = initialYSize;
		fieldOfView = initialFieldOfView;
		transform = initialTransform;
		CalculatePixelSize();
	}

	Camera(size_t initialXSize, size_t initialYSize, double initialFieldOfView) {
		halfHeight = halfWidth = pixelSize = 0.0f;
		xSize = initialXSize;
		ySize = initialYSize;
		fieldOfView = initialFieldOfView;
		transform = Matrix::IndentityMatrix4x4();
		CalculatePixelSize();
	}

	size_t GetXSize() { return xSize; }

	size_t GetYSize() { return ySize; }

	double GetFieldOfView() { return fieldOfView; }

	Transform GetTransform() { return transform; }
	void SetTransform(Transform newTransform) { transform = newTransform; }

	double GetPixelSize() { return pixelSize; }

	Ray CreateRayForPixel(size_t xPixel, size_t yPixel) {
		//Offset from edge of canvas to the pixel's center
		double xOffset = (static_cast<double>(xPixel) + 0.5) * pixelSize;
		double yOffset = (static_cast<double>(yPixel) + 0.5) * pixelSize;

		//Untransformed coordinates of the pixel
		double worldX = halfWidth - xOffset;
		double worldY = halfHeight - yOffset;

		//Transform the the point (-> view transform)
		//(Untransformed canvas is at z =-1) 
		Point pixel = transform.Inversion() * Point::CreatePoint(worldX, worldY, -1.0f);
		Point origin = transform.Inversion() * Point::CreatePoint(0.0f, 0.0f, 0.0f);
		Vector direction = (pixel - origin).Normalize();

		return Ray(origin, direction);
	}

	Canvas RenderFrame(World& world) {
		Canvas image(xSize, ySize);

		for (size_t x = 0; x < xSize; x++) {
			for (size_t y = 0; y < ySize; y++) {
				Ray currentRay = CreateRayForPixel(x, y);
				Color pixelColor = world.FindRayColor(currentRay);
				image.WritePixel(pixelColor, x, y);
			}

			std::cout << std::to_string(static_cast<double>(x) / static_cast<double>(xSize) * 100.0f) + "%\n";
		}

		return image;
	}

	static Transform CreateViewTransform(Point from, Point to, Vector up) {
		Vector forward = (to - from).Normalize();
		Vector left = Vector::CrossProduct(forward, up.Normalize());
		Vector trueUp = Vector::CrossProduct(left, forward);

		Matrix4x4 matrix = Matrix::Create(
			left.x, left.y, left.z, 0.0f,
			trueUp.x, trueUp.y, trueUp.z, 0.0f,
			-forward.x, -forward.y, -forward.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		Transform orientation(matrix);


		return orientation * Transform::CreateTranslation(-from.x, -from.y, -from.z);
	}

private:

	void CalculatePixelSize() {
		double halfView = tan(fieldOfView / 2.0);
		double aspectRatio = static_cast<double>(xSize) / static_cast<double>(ySize);

		if (aspectRatio >= 1.0) {
			halfWidth = halfView;
			halfHeight = halfView / aspectRatio;
		}
		else {
			halfWidth = halfView * aspectRatio;
			halfHeight = halfView;
		}

		pixelSize = (halfWidth * 2.0) / static_cast<double>(xSize);
	}
};