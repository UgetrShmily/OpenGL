#ifndef MODEL_H
#define MODEL_H
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader.h"
#include "Mesh.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model
{
private:
	/*  ģ������  */
	vector<Mesh> meshes;
	string directory;//ģ��·��
	vector<Texture> textures_loaded;//����·��
	bool gammaCorrection;
public:
	/*  ����   */
	Model(string const &path, bool gamma = false):gammaCorrection(gamma)
	{
		loadModel(path);
	}
	void Draw(Shader shader)//��������Mash,������Ӧ��Draw����
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i].Draw(shader);
	}
private:
	/*  ����   */
	void loadModel(string const &path) {
		Assimp::Importer importer;
		//�ڶ�������ΪһЩ���ڴ���ѡ�ģ��תΪ����|����Y�ᷭת��ͼƬԭ�������Ͻǣ�opengl�����½ǣ�
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);//��ȡ������aiScene *scene���ݽṹ��
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			//���scene root����Ƿ�Ϊ��	��	mFlages(�����Ƿ�����)�������κδ���ͽ����ӡ
			cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));//��ȡ���ļ�·����Ŀ¼·��
		
		processNode(scene->mRootNode, scene);//�ݹ����н��
	}

	/*
		Assimp�Ľṹ�У�ÿ���ڵ������һϵ�е�����������ÿ������ָ�򳡾������е��Ǹ��ض����񡣽�����
		����ȥ��ȡ��Щ������������ȡÿ�����񣬴���ÿ�����񣬽��Ŷ�ÿ���ڵ���ӽڵ��ظ���һ����
	*/
	void processNode(aiNode *node, const aiScene *scene) {
		// ����ڵ����е���������еĻ���
		/*
			���ȼ��ÿ���ڵ������������������������mMeshes��������ȡ��Ӧ�����񡣷��ص����񽫻ᴫ�ݵ�processMesh�����У���
			�᷵��һ��Mesh���󣬽����洢��meshes�б�/vector
		*/
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		// �������������ӽڵ��ظ���һ����
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	
	}
	Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			/* ������λ�á����ߺ���������
			   Assimpת����GLM������������
			   Assimp����һ��ģ����һ�������������8����ͬ����������
			 */
			glm::vec3 vector;
			//����
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			
			//����
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;

			//����
			if (mesh->mTextureCoords[0]) // �����Ƿ����������ꣿ��ֻ����һ������
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			// tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;

			vertices.push_back(vertex);
		}
		// ��������
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			//���������е��棬�����������������indices���vector��
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// �������
		
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

		// 1. diffuse maps
		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		

		return Mesh(vertices, indices, textures);
	}
	/*
		loadMaterialTextures���������˸����������͵���������λ�ã���ȡ��������ļ�λ��
		�����ز�����������������Ϣ��������һ��Vertex�ṹ����
	*/
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) 
	{
		vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // �������û�б����أ��������
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(),this->directory);
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				textures_loaded.push_back(texture); // ��ӵ��Ѽ��ص�������
			}
		}
		return textures;
	}
};

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
#endif