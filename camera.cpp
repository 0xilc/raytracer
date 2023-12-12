#include "camera.h"
#include <iostream>
#include <thread>
#include <vector>

extern parser::Scene scene;

void Camera::render(const BvhNode& world)
{

	const int numThreads = std::thread::hardware_concurrency();
	std::vector<std::thread> threads(numThreads);
	for (int threadId = 0; threadId < numThreads; threadId++)
	{
		threads[threadId] = std::thread([this, &world, threadId, numThreads]() {
			for (int j = threadId; j < imageHeight; j += numThreads)
			{
				for (int i = 0; i < imageWidth; i++)
				{
					double s_u = (i + 0.5) * s_u_coefficient;
					double s_v = (j + 0.5) * s_v_coefficient;
					Vec3 s = q + (u * s_u) - (v * s_v);

					Ray primaryRay(e, s - e);

					Color pixelColor = computeColor(primaryRay, scene.max_recursion_depth + 1, world);
					pixelColor.clamp();
					setPixelColor(i, j, pixelColor);
					
				}
				//std::cout << j << std::endl;
			}
		});
	}

	for (int threadId = 0; threadId < numThreads; ++threadId)
	{
		threads[threadId].join();
	}
}

Color Camera::computeColor(const Ray& ray, int depth, const BvhNode& world)
{
	if (depth <= 0) return Color(0, 0, 0);

	HitRecord rec;
	if (!world.hit(ray, Interval(scene.shadow_ray_epsilon, INFINITY), rec))
	{
		if (depth == scene.max_recursion_depth + 1)
			return Color(scene.background_color);
		else 
			return Color(0, 0, 0);
	}
	return applyShading(ray, depth, world, rec);
	
}

Color Camera::applyShading(const Ray& ray, int depth, const BvhNode& world, HitRecord& rec)
{
	parser::Material mat = scene.materials[rec.material_id - 1];
	Color color = Color(mat.ambient) * Color(scene.ambient_light);

	if (mat.is_mirror) {
		Vec3 wo = ray.direction * -1;
		Vec3 wr = (rec.normal * (2 * (rec.normal.dot(wo)))) - wo;
		Ray reflectedRay = Ray(rec.p + rec.normal * scene.shadow_ray_epsilon, wr);
		color += computeColor(reflectedRay, depth - 1, world) * Color(mat.mirror);
	}

	for (const parser::PointLight& light : scene.point_lights)
	{
		Vec3 wi = Vec3(light.position) - rec.p;
		double distance = wi.length();
		wi.normalize();
		Ray shadowRay = Ray(rec.p + rec.normal * scene.shadow_ray_epsilon, wi) ;
		HitRecord shadowRec;
		if (!world.hit(shadowRay, Interval(0, distance), shadowRec))
		{
			// Diffuse
			double cosTheta = std::max(double(0.0f), rec.normal.dot(wi));
			color += Color(mat.diffuse) * Color(light.intensity) * (cosTheta / (distance * distance));
			
			// Specular
			Vec3 wo = (ray.origin - rec.p);
			wo.normalize();
			Vec3 h = (wi + wo);
			h.normalize();
			double cosAlpha = std::max(double(0.0f), rec.normal.dot(h));
			color += Color(mat.specular) * Color(light.intensity) * (pow(cosAlpha, mat.phong_exponent) / (distance * distance));
		}
	}

	return color;
}

void Camera::setPixelColor(const int& i, const int& j, const Color& color) 
{
	int location = 3 * (j * imageWidth + i);
	image[location] = color.r;
	image[location + 1] = color.g;
	image[location + 2] = color.b;
}
