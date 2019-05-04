#pragma once
#include "Matrix.h"
#include <cmath>

namespace RayTracer {
	//Definition of transforms and implementation of basic operations on them
	class Transform {
	public:


		Transform();
		Transform(Matrix4x4 initialMatrix);

		~Transform() = default;

		//Invert this transform
		Transform Invert();
		//Get the inversion of this transform
		Transform Inversion();

		//Creating Transforms
		static Transform CreateTranslation(double x, double y, double z);
		static Transform CreateScale(double x, double y, double z);
		static Transform CreateRotationX(double angle);
		static Transform CreateRotationY(double angle);
		static Transform CreateRotationZ(double angle);
		static Transform CreateShear(double xy, double xz, double yx, double yz, double zx, double zy);

		//Applying translations to an existing transform
		Transform& Translate(double x, double y, double z);
		Transform& Scale(double x, double y, double z);
		Transform& RotateX(double angle);
		Transform& RotateY(double angle);
		Transform& RotateZ(double angle);
		Transform& Shear(double xy, double xz, double yx, double yz, double zx, double zy);

		void SetMatrix(Matrix4x4 newMatrix) { inverseIsCached = false; matrix = newMatrix; }
		Matrix4x4 GetMatrix() { return matrix; }

	private:
		//The transformation matrix
		Matrix4x4 matrix;

		//Caching the transforms results in a 2x performance increase...
		Matrix4x4 cachedInverse;
		bool inverseIsCached;
	};


	inline Transform::Transform() : matrix{ Matrix::IndentityMatrix4x4() }, cachedInverse{ Matrix::IndentityMatrix4x4() } {
		//Identity matrix is it's own inversion
		inverseIsCached = true;
	}

	inline Transform::Transform(Matrix4x4 initialMatrix) {
		matrix = initialMatrix;
		inverseIsCached = false;
	}


	inline Transform Transform::Invert() {
		matrix = matrix.Inversion();
		inverseIsCached = false;
		return *this;
	}

	inline Transform Transform::Inversion() {

		//Return cached inverse if available
		if (inverseIsCached) {
			return cachedInverse;
		}

		//Only calculate the inverse if needed
		cachedInverse = matrix.Inversion();
		inverseIsCached = true;

		return cachedInverse;
	}

	//Creation of transforms

	inline Transform Transform::CreateTranslation(double x, double y, double z) {
		Matrix4x4 matrix = Matrix::IndentityMatrix4x4();

		matrix.elements[0][3] = x;
		matrix.elements[1][3] = y;
		matrix.elements[2][3] = z;
		return Transform(matrix);
	}

	inline Transform Transform::CreateScale(double x, double y, double z) {
		Matrix4x4 matrix = Matrix::IndentityMatrix4x4();

		matrix.elements[0][0] = x;
		matrix.elements[1][1] = y;
		matrix.elements[2][2] = z;

		return Transform(matrix);
	}

	inline Transform Transform::CreateRotationX(double angle) {
		Matrix4x4 matrix = Matrix::IndentityMatrix4x4();

		matrix.elements[1][1] = cos(angle);
		matrix.elements[1][2] = -sin(angle);
		matrix.elements[2][1] = sin(angle);
		matrix.elements[2][2] = cos(angle);

		return Transform(matrix);
	}

	inline Transform Transform::CreateRotationY(double angle) {
		Matrix4x4 matrix = Matrix::IndentityMatrix4x4();

		matrix.elements[0][0] = cos(angle);
		matrix.elements[0][2] = sin(angle);
		matrix.elements[2][0] = -sin(angle);
		matrix.elements[2][2] = cos(angle);

		return Transform(matrix);
	}

	inline Transform Transform::CreateRotationZ(double angle) {
		Matrix4x4 matrix = Matrix::IndentityMatrix4x4();

		matrix.elements[0][0] = cos(angle);
		matrix.elements[0][1] = -sin(angle);
		matrix.elements[1][0] = sin(angle);
		matrix.elements[1][1] = cos(angle);

		return Transform(matrix);
	}

	inline Transform Transform::CreateShear(double xy, double xz, double yx, double yz, double zx, double zy) {
		Matrix4x4 matrix = Matrix::IndentityMatrix4x4();

		matrix.elements[0][1] = xy;
		matrix.elements[0][2] = xz;

		matrix.elements[1][0] = yx;
		matrix.elements[1][2] = yz;

		matrix.elements[2][0] = zx;
		matrix.elements[2][1] = zy;

		return Transform(matrix);
	}

	inline bool operator==(Transform transform1, Transform transform2) {
		return transform1.GetMatrix() == transform2.GetMatrix();
	}

	inline bool operator!=(Transform transform1, Transform transform2) {
		return !(transform1 == transform2);
	}

	//Multiplication of transforms with matrices and tuples
	inline Tuple operator*(Tuple tuple, Transform transform) {
		return tuple * transform.GetMatrix();
	}

	inline Tuple operator*(Transform transform, Tuple tuple) {
		return transform.GetMatrix()* tuple;
	}

	inline Matrix4x4 operator*(Transform transform, Matrix4x4 matrix) {
		return transform.GetMatrix()* matrix;
	}

	inline Matrix4x4 operator*(Matrix4x4 matrix, Transform transform) {
		return matrix * transform.GetMatrix();
	}

	inline Transform operator*(Transform transform1, Transform transform2) {
		return Transform(transform1.GetMatrix() * transform2.GetMatrix());
	}

	//Applying transformations to an existing transform

	inline Transform& Transform::Translate(double x, double y, double z) {
		matrix = Transform::CreateTranslation(x, y, z) * matrix;
		inverseIsCached = false;
		return *this;
	}

	inline Transform& Transform::Scale(double x, double y, double z) {
		matrix = Transform::CreateScale(x, y, z) * matrix;
		inverseIsCached = false;
		return *this;
	}

	inline Transform& Transform::RotateX(double angle) {
		matrix = Transform::CreateRotationX(angle) * matrix;
		inverseIsCached = false;
		return *this;
	}

	inline Transform& Transform::RotateY(double angle) {
		matrix = Transform::CreateRotationY(angle) * matrix;
		inverseIsCached = false;
		return *this;
	}

	inline Transform& Transform::RotateZ(double angle) {
		matrix = Transform::CreateRotationZ(angle) * matrix;
		inverseIsCached = false;
		return *this;
	}

	inline Transform& Transform::Shear(double xy, double xz, double yx, double yz, double zx, double zy) {
		matrix = Transform::CreateShear(xy, xz, yx, yz, zx, zy) * matrix;
		inverseIsCached = false;
		return *this;
	}
}
