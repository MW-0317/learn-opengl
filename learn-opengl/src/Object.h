#pragma once
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb.h"

#include "Material.h"

class Point
{
public:
	float x;
	float y;
	float z;

	Point(float x, float y)
	{
		this->x = x;
		this->y = y;
		this->z = 0.0;
	}

	Point(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

class Object
{
public:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	Material* material;
	std::vector<Point*> points;

	Object(std::vector<Point*> points, unsigned int elements[],
				Material* material)
	{
		this->material = material;
		this->points = points;

		

		//float* vertices = getVertices();

		std::vector<float> vertices = {
			//     |Positions         |Colors           |TexCoords
					 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
					 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
					-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left
					-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		};

		float* a = &vertices[0];

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 32, a, GL_DYNAMIC_DRAW);

		//unsigned int EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, elements, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}
	/*
	~Object()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
	*/

	void draw()
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	float* getVertices()
	{
		const int FINAL_SIZE = points.size() * (2 + material->getSize());
		std::cout << FINAL_SIZE << std::endl;
		float* finalVertices = (float*) malloc(sizeof(float) * FINAL_SIZE);
		if (!finalVertices)
		{
			std::cerr << "OBJECT::MEMORY_NOT_ALLOCATED";
			return NULL;
		}
			

		int index = 0;
		for (int i = 0; i < points.size(); i++)
		{
			finalVertices[index] = points[i]->x;
			index++;
			finalVertices[index] = points[i]->y;
			index++;
			finalVertices[index] = points[i]->z;
			index++;
			finalVertices[index] = material->color->color[0];
			index++;
			finalVertices[index] = material->color->color[1];
			index++;
			finalVertices[index] = material->color->color[2];
			index++;
			//finalVertices[index] = material->color->color[3];
			//index++;
			finalVertices[index] = material->texture->texCoords[2 * i];
			index++;
			finalVertices[index] = material->texture->texCoords[2 * i + 1];
			index++;
		}

		for (int i = 0; i < FINAL_SIZE; i++)
		{
			std::cout << finalVertices[i] << " ";
		}
		std::cout << std::endl;

		return finalVertices;
	}
};