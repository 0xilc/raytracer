#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "parser.h"
#include "ppm.h"
#include "color.h"
#include "ray.h"
#include "bvh.h"


class Camera {

public:
	Camera(parser::Camera _camera) :
		e(_camera.position),
		w(Vec3(_camera.gaze).reverse()),
		v(_camera.up),
		nearPlane{ _camera.near_plane.x, _camera.near_plane.y, _camera.near_plane.z, _camera.near_plane.w },
		nearDistance(_camera.near_distance),
		imageWidth(_camera.image_width),
		imageHeight(_camera.image_height),
		imageName(_camera.image_name)
	{
		u = v.cross(w);
		m = e - (w * nearDistance);
		q = m + (u * nearPlane[0]) + (v * nearPlane[3]);
		s_u_coefficient = (nearPlane[1] - nearPlane[0]) / imageWidth;
		s_v_coefficient = (nearPlane[3] - nearPlane[2]) / imageHeight;
		image = new unsigned char[imageWidth * imageHeight * 3];
	}

	void render(const BvhNode& world);

	Color computeColor(const Ray& ray, int depth, const BvhNode& world);
	Color applyShading(const Ray& ray, int depth, const BvhNode& world, HitRecord& red);

	void writeOnFile() { write_ppm(imageName.c_str(), image, imageWidth, imageHeight); }
	void setPixelColor(const int& i, const int& j, const Color& color);


private:
	Vec3 e;
	Vec3 w;
	Vec3 v;
	Vec3 m;
	Vec3 q;
	Vec3 u;
	double nearPlane[4];
	float nearDistance;
	int imageWidth, imageHeight;
	std::string imageName;
	double s_u_coefficient;
	double s_v_coefficient;

	unsigned char* image;

};

#endif // !CAMERA_H
