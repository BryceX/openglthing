#include "Vertex.h"




RenderObject MakeGrid(unsigned int rows, unsigned int columns)
{
	RenderObject rObject;	// stores a handle to the VBO, IBO, and the VAO

	// vertex data
	Vertex* vertices = new Vertex[rows*columns];
	for (unsigned int r = 0; r < rows; r++)
	{
		for (unsigned int c = 0; c < columns; c++)
		{
			glm::vec3 colour = glm::vec3(sinf((c / (float)(columns - 1))*(r / (float)(rows - 1))));

			vertices[r*columns + c].position = glm::vec4((float)c, 0, (float)r, 1);
			vertices[r * columns + c].colour = glm::vec4(colour, 1);
		}
	}

	// index data
	unsigned int* auiIndices = new unsigned int[(rows - 1)*(columns - 1) * 6];
	unsigned int index = 0;

	for (unsigned int r = 0; r < (rows - 1); ++r)
	{
		for (unsigned int c = 0; c < (columns - 1); ++c)
		{
			auiIndices[index++] = r*columns + c;
			auiIndices[index++] = (r + 1) * columns + c;
			auiIndices[index++] = (r + 1) * columns + (c + 1);

			auiIndices[index++] = r * columns + c;
			auiIndices[index++] = (r + 1) * columns + (c + 1);
			auiIndices[index++] = r * columns + (c + 1);
		}
	}

	glGenBuffers(1, &rObject.VBO);
	glGenBuffers(1, &rObject.IBO);
	glGenVertexArrays(1, &rObject.VAO);
	glBindVertexArray(rObject.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, rObject.VBO);
	glBufferData(GL_ARRAY_BUFFER, (rows*columns)*sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rObject.IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (columns - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);

	glBindVertexArray(0);	// unbind the VAO by setting to zero, which is NULL
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the VBO
	glBindVertexArray(rObject.VAO);

	delete[] auiIndices;
	delete[] vertices;
	//unsigned int indexCount = (rows - 1)*(columns - 1) * 6;
	//glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return rObject;
}