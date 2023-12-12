#include <iostream>
#include "parser.h"
#include "vec3.h"
#include "camera.h"
#include "bvh.h"
#include "sphere.h"
#include "triangle.h"
#include <vector>

typedef unsigned char RGB[3];

parser::Scene scene;


int main(int argc, char* argv[])
{
    // Sample usage for reading an XML scene file
    scene.loadFromXml(argv[1]);
    std::vector<std::shared_ptr<Hittable>> objects;
    for (const parser::Sphere& sphere : scene.spheres)
    {
        objects.push_back(std::make_shared<Sphere>(sphere));
    }
    for (const parser::Triangle& triangle : scene.triangles)
    {
        objects.push_back(std::make_shared<Triangle>(triangle));
    }
    for (const parser::Mesh& mesh : scene.meshes)
    {
        for (const parser::Face& face : mesh.faces)
        {
            objects.push_back(std::make_shared<Triangle>(face, mesh.material_id));
        }
    }
    BvhNode world(objects, 0, objects.size() - 1);
    
    for (const parser::Camera& _cam : scene.cameras)
    {
        Camera camera(_cam);
        camera.render(world);
        camera.writeOnFile();
    }
   
}
