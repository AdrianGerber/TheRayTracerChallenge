#pragma once

#include "Tuple.h"
#include "Matrix.h"
#include "Transform.h"
#include "Constants.h"
#include <cmath>
#include "Canvas.h"
#include "World.h"

class Camera {
private:
	size_t xSize, ySize;
	float fieldOfView;
	Transform transform;

	float halfHeight, halfWidth, pixelSize;

public:

	Camera() {
		halfHeight = halfWidth = pixelSize = 0.0f;
		xSize = ySize = 100;
		fieldOfView = Constants::PI / 2.0f;
		transform.matrix = Matrix::IndentityMatrix4x4();
		CalculatePixelSize();
	}

	Camera(size_t initialXSize, size_t initialYSize, float initialFieldOfView, Transform initialTransform) {
		halfHeight = halfWidth = pixelSize = 0.0f;
		xSize = initialXSize;
		ySize = initialYSize;
		fieldOfView = initialFieldOfView;
		transform = initialTransform;
		CalculatePixelSize();
	}

	Camera(size_t initialXSize, size_t initialYSize, float initialFieldOfView) {
		halfHeight = halfWidth = pixelSize = 0.0f;
		xSize = initialXSize;
		ySize = initialYSize;
		fieldOfView = initialFieldOfView;
		transform = Matrix::IndentityMatrix4x4();
		CalculatePixelSize();
	}

	size_t GetXSize() { return xSize; }

	size_t GetYSize() { return ySize; }

	float GetFieldOfView() { return fieldOfView; }

	Transform GetTransform() { return transform; }
	void SetTransform(Transform newTransform) { transform = newTransform; }

	float GetPixelSize() { return pixelSize; }

	Ray CreateRayForPixel(size_t xPixel, size_t yPixel) {
		//Offset from edge of canvas to the pixel's center
		float xOffset = (xPixel + 0.5f) * pixelSize;
		float yOffset = (yPixel + 0.5f) * pixelSize;

		//Untransformed coordinates of the pixel
		float worldX = halfWidth - xOffset;
		float worldY = halfHeight - yOffset;

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
		float halfView = tanf(fieldOfView / 2.0f);
		float aspectRatio = static_cast<float>(xSize) / static_cast<float>(ySize);

		if (aspectRatio >= 1.0f) {
			halfWidth = halfView;
			halfHeight = halfView / aspectRatio;
		}
		else {
			halfWidth = halfView * aspectRatio;
			halfHeight = halfView;
		}

		pixelSize = (halfWidth * 2.0f) / static_cast<float>(xSize);
	}
};