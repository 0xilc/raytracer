#ifndef BVH_H
#define BVH_H

#include "hittable.h"
#include "parser.h"
#include <memory>
#include <vector>
#include <algorithm>
#include <random>

class BvhNode : public Hittable{
public:
	// todo: write constructor of bvhnode from hittable list
	BvhNode(std::vector<std::shared_ptr<Hittable>>& objects, int begin, int end)
	{
		int axis = rand() % 3;
		if (begin == end)
		{
			left = objects[begin];
			right = objects[begin];
		}
		else if (begin == end - 1)
		{
			left = objects[begin];
			right = objects[end];
		}
		else
		{
			std::sort(objects.begin() + begin, objects.begin() + end + 1, 
				[axis](const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable> b)
				{
					return a->getAABB()[axis].min < b->getAABB()[axis].min;
				}
			);
			int mid = (begin + end) / 2;
			left = std::make_shared<BvhNode>(objects, begin, mid);
			right = std::make_shared<BvhNode>(objects, mid, end);
		}
		
		bounding_box = AABB(left->getAABB(), right->getAABB());
	}


	bool hit(const Ray& ray, Interval ray_t, HitRecord& rec) const override {
		if (!bounding_box.hit(ray, ray_t)) return false;
		HitRecord rec1, rec2;

		bool hit_left = left->hit(ray, ray_t, rec1);
		bool hit_right = right->hit(ray, ray_t, rec2);

		if (hit_left && hit_right) {
			if (rec1.t < rec2.t) {
				rec = rec1;
			}
			else {
				rec = rec2;
			}
			return true;
		}
		else if (hit_left) {
			rec = rec1;
			return true;
		}
		else if (hit_right) {
			rec = rec2;
			return true;
		}

		return false;
	}

	AABB getAABB() const override { return bounding_box; }

private:
	std::shared_ptr<Hittable> left;
	std::shared_ptr<Hittable> right;
	AABB bounding_box;
};

#endif // !BVH_H
