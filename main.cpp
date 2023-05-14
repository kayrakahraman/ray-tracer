#include <iostream>
#include <fstream>
#include <vector>
#include "rtweekend.h"
#include "color.h"
#include "vec3.h"
#include "ray.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "texture.h"
//#include "aarect.h"

//double hit_sphere(const point3& center, double radius, const ray& r) {
//	vec3 oc = r.origin() - center;
//	auto a = dot(r.direction(), r.direction());
//	auto b = 2.0 * dot(oc, r.direction());
//	auto c = dot(oc, oc) - radius*radius;
//	auto discriminant = b*b - 4 * a*c;
//
//	if (discriminant < 0) {
//		return -1.0;
//	}
//	else {
//		return (-b - sqrt(discriminant)) / (2.0*a);
//	}
//}

//hittable_list simple_light() {
//	hittable_list objects;
//
//	auto pertext = make_shared<solid_color>(4);
//	objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
//	objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));
//
//	auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
//	objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));
//
//	return objects;
//}

color ray_color(const ray& r, const color& background, const hittable& world, int depth) {
	hit_record rec;

	if (depth <= 0)
		return color(0, 0, 0);

	if (!world.hit(r, 0.001, infinity, rec))
		return background;

	ray scattered;
	color attenuation;
	color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

	if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		return emitted;

	return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
}

int main() {

	std::ofstream output;
	output.open("./render.ppm", std::ios::out | std::ios::trunc);
	if (!output.is_open())
		return 1;
	std::cout << "Outputting...";

	// Image
	const auto aspect_ratio = 4.0 / 3.0;
	const int image_width = 640;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 100;
	const int max_depth = 50;

	auto R = cos(pi / 4);
	color background(0, 0, 0);


	// World
	hittable_list world;
	/*world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(-1, -2, -3), 1));
	world.add(make_shared<sphere>(point3(2, 1, -2), 0.5));
	world.add(make_shared<sphere>(point3(2, -3, -5), 0.5));
	world.add(make_shared<sphere>(point3(-5, 3, -7), 1.5));*/

	auto material1 = make_shared<lambertian>(color(1, 1, 1));
	auto material2 = make_shared<lambertian>(color(0.5, 0.7, 1));
	auto material3 = make_shared<lambertian>(color(0.1, 0.9, 0.1));
	auto material4 = make_shared<lambertian>(color(0.9, 0.1, 0.1));
	auto material_temp = make_shared<lambertian>(color(1, 1, 0.0));
	auto material_light = make_shared<diffuse_light>(color(3, 3, 3));
	auto material_dark = make_shared<lambertian>(color(0.2, 0.2, 0.2));
	auto material_light_r = make_shared<diffuse_light>(color(0.9, 0.1, 0.1));
	auto material_light_g = make_shared<diffuse_light>(color(0.1, 0.9, 0.1));
	auto material_light_b = make_shared<diffuse_light>(color(0.5, 0.7, 1));
	auto material_light_y = make_shared<diffuse_light>(color(1, 1, 0.0));

	/*world.add(make_shared<sphere>(point3(0.0, -101, -1.0), 100.0, material_ground));
	world.add(make_shared<sphere>(point3(3, 2, -4), 1, material_center));
	world.add(make_shared<sphere>(point3(2, 3, -5), 0.5, material_left));
	world.add(make_shared<sphere>(point3(-5, 3, -7), 1.5, material_right));*/

	//world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, material_light));
	world.add(make_shared<sphere>(point3(-1, -2, -3), 1, material_light_b));
	world.add(make_shared<sphere>(point3(2, 1, -3), 0.5, material_light_g));
	world.add(make_shared<sphere>(point3(1, -2, -3), 0.5, material_light_r));
	world.add(make_shared<sphere>(point3(-2, 2, -3), 1.5, material_light_y));
	world.add(make_shared<sphere>(point3(0, 0, -3), 1, material_dark));

	/*world.add(make_shared<sphere>(point3(0.0, -101, -1.0), 100.0, material_light));
	world.add(make_shared<sphere>(point3(3, 2, -4), 1, material2));
	world.add(make_shared<sphere>(point3(2, 3, -5), 0.5, material3));
	world.add(make_shared<sphere>(point3(-5, 3, -7), 1.5, material4));
	world.add(make_shared<sphere>(point3(-1, 1, -6), 1.5, material_temp));*/
	// Camera

	camera cam;

	// Render

	output << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; --j) {
		//std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			/*auto r = double(i) / (image_width - 1);	//until the comment ends, 1st impl
			auto g = double(j) / (image_height - 1);
			auto b = 0.25;

			int ir = static_cast<int>(255.999 * r);
			int ig = static_cast<int>(255.999 * g);
			int ib = static_cast<int>(255.999 * b);

			output << ir << ' ' << ig << ' ' << ib << '\n';*/

			//color pixel_color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0.25);	//2nd impl after color.h
			/*auto u = double(i) / (image_width - 1);
			auto v = double(j) / (image_height - 1);
			ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
			color pixel_color = ray_color(r, world);
			write_color(output, pixel_color);*/			//3rd impl inside comments
			color pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; ++s) {
				auto u = (i + random_double()) / (image_width - 1);
				auto v = (j + random_double()) / (image_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, background, world, max_depth);
			}
			write_color(output, pixel_color, samples_per_pixel);
		}
	}


	output.close();
	std::cout << "\nDone.\n";

	return 0;
}