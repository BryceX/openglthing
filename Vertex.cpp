#include "Vertex.h"


unsigned int loadSubShader(unsigned int type, const char* path)
{
	std::ifstream in(path);	// opens a inbound-file stream to the file containing the shader source code
	std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());	// load the source code into a std::string object

	// HEAP		-> dynamically allocated memory (we have lots of this but we have to remember to delete things ourselves)
	// STACK	-> memory that variables are normally allocated and deallocated automatically to and frm
	// allocate a blank char array in the heap to copy the string into
	char *src = new char[contents.length()+1];

	// copy the string into the char array
	strncpy_s(src, sizeof(char)*contents.length()+1, contents.c_str(), sizeof(char)*contents.length()+1);

	// generate a blank shader and hold a handle to it so we can access it in the future
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, 0);	// supply the source for the shader to the shader
	glCompileShader(shader);	// attempt to compile the shader
	delete[] src;	// delete the dynamic char array from the heap. if we don't do this, we'll have a memory leak because we
					// won't be able to delete this anymore because we won't know where it is

	// return a handle to the shader
	return shader;
}

unsigned int loadShader(const char *v_path, const char *f_path)
{
	unsigned programID = glCreateProgram();	// store handle to generated shader program
	unsigned vshader = loadSubShader(GL_VERTEX_SHADER, v_path);
	unsigned fshader = loadSubShader(GL_FRAGMENT_SHADER, f_path);
	glAttachShader(programID, vshader);
	glAttachShader(programID, fshader);
	glLinkProgram(programID);
	int success;

	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	return programID;
}

void CreateOpenGLBuffers(FBXFile* name) 
{
	//creates vao/ibo/vbo for each mesh
	for (unsigned int i = 0; i < name->getMeshCount(); ++i)
	{
		FBXMeshNode* myMesh = name->getMeshByIndex(i);
		unsigned int* glData = new unsigned int[3];
	
		glGenVertexArrays(1, &glData[0]);
		glBindVertexArray(glData[0]);
		glGenBuffers(1, &glData[1]);
		glGenBuffers(1, &glData[2]);
		glBindBuffer(GL_ARRAY_BUFFER, glData[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glData[2]);
		glBufferData(GL_ARRAY_BUFFER,
			myMesh->m_vertices.size() * sizeof(FBXVertex),
			myMesh->m_vertices.data(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			myMesh->m_indices.size() * sizeof(unsigned int),
			myMesh->m_indices.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0); // position
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
			sizeof(FBXVertex), 0);
		glEnableVertexAttribArray(1); // normal
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE,
			sizeof(FBXVertex),
			((char*)0) + FBXVertex::NormalOffset);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		myMesh->m_userData = glData;

	}
}
void DeleteOpenGLBuffers(FBXFile* name)
{
	//cleans up vertex on each mesh
	for (unsigned int i = 0; i < name->getMeshCount(); ++i) 
	{
		FBXMeshNode* myMesh = name->getMeshByIndex(i);
		unsigned int* glData = (unsigned int*)myMesh->m_userData;
		glDeleteVertexArrays(1, &glData[0]);
		glDeleteBuffers(1, &glData[1]);
		glDeleteBuffers(1, &glData[2]);
		delete[] glData;
	}
}

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

RenderObject LoadFBX(const char* fileName)
{
	RenderObject rObject;
	
	FBXFile *temp = new FBXFile;
	temp->load(fileName);
	temp->initialiseOpenGLTextures();	
	rObject.m_FBX = temp;

	FBXMeshNode * mesh = temp->getMeshByIndex(0);	
	//FBXTexture* texture = temp.getTextureByName(fileName);

	glGenBuffers(1, &rObject.VBO);
	glGenBuffers(1, &rObject.IBO);
	glGenVertexArrays(1, &rObject.VAO);
	glBindVertexArray(rObject.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, rObject.VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh->m_vertices.size()*sizeof(FBXVertex), &mesh->m_vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)(FBXVertex::Offsets::PositionOffset));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex), (void*)(FBXVertex::Offsets::TexCoord1Offset));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rObject.IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->m_indices.size()*sizeof(unsigned int), &mesh->m_indices[0], GL_STATIC_DRAW);

	rObject.indexCount = mesh->m_indices.size();
	glBindVertexArray(0);	// unbind the VAO by setting to zero, which is NULL
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind the IBO
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the VBO

	//glBindTexture(GL_TEXTURE_3D, texture->path.begin);

	return rObject;
}
void RenderObject::Draw() 
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr );
}