#include <iostream>
#include <vector>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <TEXTURE/texture_s.hpp>
#include <SHADER/shader_s.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};


class Mesh
{
public:

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		setupMesh();
	}

	void render(Shader& shader, bool hasMaterial = false)
	{
		int diffuseNr = 1, specularNr = 1, normalNr = 1, heightNr = 1;

		/*
		*	Naming convention for textures:
		* 	- diffuse: texture_diffuseN
		*	- specular: texture_specularN
		*	- normal: texture_normalN
		*	- height: texture_heightN
		*/

		for (int i = 0; i < this->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			std::string number;
			std::string name = this->textures[i].getTextureType();

			switch (this->textures[i].sType)
			{
				case texture_diffuse:
				{
					number = std::to_string(diffuseNr++); break;
					name = "texture_diffuse";
				}
				case texture_specular:
				{
					number = std::to_string(specularNr++); break;
					name = "texture_specular";
				}
				case texture_normal:
				{
					number = std::to_string(normalNr++); break;
					name = "texture_normal";
				}
				case texture_height:
				{
					number = std::to_string(heightNr++); break;
					name = "texture_height";
				}
			}

			if(hasMaterial) shader.setFloatUniform(("material." + name + number).c_str(), i);
			else shader.setFloatUniform((name + number).c_str(), i);

			this->textures[i].bind();
		}

		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(this->VAO);
		if(this->indices.empty()) glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
		else glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	~Mesh()
	{
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);
		glDeleteBuffers(1, &this->EBO);
	}

private:

	GLuint VAO, VBO, EBO;

	void setupMesh()
	{
		// Framebuffers creation
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);

		// Bind VAO
		glBindVertexArray(this->VAO);

		// Load data into VBO
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

		// Load data into EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

		// Set vertex attribute pointers
		glEnableVertexArrayAttrib(this->VAO, 0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		// Set normal attribute pointers
		glEnableVertexArrayAttrib(this->VAO, 1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		// Set texture cordinates attribute pointers
		glEnableVertexArrayAttrib(this->VAO, 2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

		glBindVertexArray(0);
	}
};