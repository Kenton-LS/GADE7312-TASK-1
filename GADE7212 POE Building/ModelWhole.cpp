#include "ModelWhole.h"
#include <iostream>
#include <tiny_obj_loader.h>

using namespace std;

ModelWhole::ModelWhole(string path, string name)
{
	loadModel(path, name);
}

ModelWhole::~ModelWhole()
{
	for (Texture* t : textures) // for each texture pointer in textures vector
	{
		delete t;
	}
}

void ModelWhole::drawGeometry() // Rotate the shape
{

	glPushMatrix();
	{
		glRotatef(rotationOffset, 0, 1, 0);
		glCallList(id);
	}
	glPopMatrix();
	rotationOffset += rotationSpeed;
};

void ModelWhole::loadModel(string path, string name)
{
	cout << "Loading model: " << name << endl;

	tinyobj::attrib_t attrib; // One long array containing all the values in a particular sequence. E.g. Vertex(all variables e.g. colour etc)
	// We parse a pointer to tiny_obj, then it makes up the mesh of the model.
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> objMaterials; // Materials of the shapes we are reading from this tiny obj file
	std::string warn, err;

	string objPath = path + "/" + name + ".obj";
	string mtlPath = path; // Contains info about materials used in this model

	if (!tinyobj::LoadObj(&attrib, &shapes, &objMaterials, &warn, &err, objPath.c_str(), mtlPath.c_str()))
	{
		throw std::runtime_error(warn + err);
	}

	if (!warn.empty())
	{
		cout << warn << endl;
	}

	if (!err.empty())
	{
		cout << err << endl;
	}

	cout << "Num materials: " << objMaterials.size() << endl; // Print out how many materials belong to this model
	// Loop through each material and load the texture file associated to each image. Load those textures into memory and then store it into the vectors
	for (const auto& objMaterial : objMaterials) // Const auto = equivalent to C# var
	{
		if (objMaterial.diffuse_texname != "")
		{
			string texturePath = path + "/" + objMaterial.diffuse_texname; // An image we can load
			Texture* texture = new Texture(texturePath.c_str());

			//cout << "Texture " << objMaterial.diffuse_texname << " successfully loaded " << endl; // KNOWLEDGE

			textures.push_back(texture); // Adds to list of textures
		}
	}

	//cout << "Processing num of shapes: " << shapes.size() << endl; // *Shapes of OBJ loader

	// Process geometry information (read it)
	for (const auto& shape : shapes)
	{
		vector<Vertex> vertices; // Index to where the vertex is in the Attrib
		vector<unsigned int> indices; // Grab all info out of the shapes

		for (const auto& index : shape.mesh.indices)
		{
			// Loop through all indices and grab vertex info
			Vertex vertex = {}; // Holds 3 lists -> 1 of POSITIONS, 1 of NORMALS, 1 of TEXTURE CORDS }} all in format of 1 list that can be read 3-by-3

			vertex.position = {
				attrib.vertices[3 * index.vertex_index + 0], // x
				attrib.vertices[3 * index.vertex_index + 1], // y
				attrib.vertices[3 * index.vertex_index + 2] // z
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0], // x
				attrib.texcoords[2 * index.texcoord_index + 1] // y
			};

			vertex.normal = {
				attrib.normals[3 * index.normal_index + 0], // x
				attrib.normals[3 * index.normal_index + 1], // y
				attrib.normals[3 * index.normal_index + 2] // z
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			vertices.push_back(vertex); //Push each vertex for each shape onto the vector array
			indices.push_back(indices.size()); // For each loop, adds 0, 1, 2, 3, 4 (starts from 0 number increases)
		}

		ModelPart part;
		part.indices = indices;
		part.vertices = vertices;

		parts.push_back(part); // Push onto list of parts
	}

	generateGeometry();
}

void ModelWhole::generateGeometry()
{
	int index = 0;
	id = glGenLists(1); // Range 
	//id = glGenLists(parts.size()); // Range 

	glNewList(id, GL_COMPILE);
	{
		if (index < textures.size())
		{
			textures[index]->use();
		}

		for (ModelPart part : parts)
		{
			cout << "INDEX: " << index << endl;
			cout << "SIZE: " << part.indices.size() << endl;
			
			vertexCount = part.indices.size(); // TASK 2 for calculating triangle / vert count

			// Draw Geometry
			glBegin(GL_TRIANGLES);
			{
				for (int i = 0; i < part.indices.size(); i++)
				{
					Vertex thisVertex = part.vertices[i];
					// Vertex thisVertex = part.vertices[part.indices[index]];

					glTexCoord2f(thisVertex.texCoord.x, thisVertex.texCoord.y);
					glNormal3f(thisVertex.normal.x, thisVertex.normal.y, thisVertex.normal.z);
					glColor3f(thisVertex.color.x, thisVertex.color.y, thisVertex.color.z);
					glVertex3f(thisVertex.position.x, thisVertex.position.y, thisVertex.position.z);
				}
			}
			glEnd();
		}
		index++;
	}
	glEndList();
}