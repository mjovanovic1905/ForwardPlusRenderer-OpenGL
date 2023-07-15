#include "LightGenerator.h"

#include <iostream>
#include <fstream>
#include <sstream>


const unsigned int LightGenerator::NUM_OF_LIGHTS = 1700;
const glm::vec3 LightGenerator::LIGHT_MIN_BOUNDS = glm::vec3(0.0f, 100.0f, -500.0f);
const glm::vec3 LightGenerator::LIGHT_MAX_BOUNDS = glm::vec3(1000.0f, 400.0f, 500.0f);

LightGenerator::LightGenerator(std::string filePath)
{
    std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 1);

	for (int i = 0; i < NUM_OF_LIGHTS; i++)
    {
		PointLight light;
		light.position = GenerateRandomPosition(dis, gen);
		light.radius = 10000.f;
        light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
        light.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
        light.specular = glm::vec3(1.f, 1.f, 1.f);
        light.constant = 1.0f;
        light.linear = 0.007f;
        light.quadratic = 0.0002f;

        lights_.push_back(light);
	}
}

glm::vec3 LightGenerator::GenerateRandomPosition(std::uniform_real_distribution<>& dis, std::mt19937& gen)
{
	glm::vec3 position = glm::vec3(0.0);

	for (int i = 0; i < 3; i++)
    {
		float min = LIGHT_MIN_BOUNDS[i];
		float max = LIGHT_MAX_BOUNDS[i];
		position[i] = dis(gen) * (max - min) + min;
	}

	return position;
}

LightGenerator::~LightGenerator()
{
}
