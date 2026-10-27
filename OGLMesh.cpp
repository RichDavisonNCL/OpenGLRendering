/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series

Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*/////////////////////////////////////////////////////////////////////////////
#include "OGLMesh.h"

using namespace NCL;
using namespace NCL::Rendering;
using namespace NCL::Maths;

OGLMesh::OGLMesh() {
	m_vao			= 0;

	for (int i = 0; i < VertexAttribute::MAX_ATTRIBUTES; ++i) {
		m_attributeBuffers[i] = 0;
	}
	m_indexBuffer = 0;
}

OGLMesh::~OGLMesh()	{
	glDeleteVertexArrays(1, &m_vao);			//Delete our VAO
	glDeleteBuffers(VertexAttribute::MAX_ATTRIBUTES, m_attributeBuffers);	//Delete our VBOs
	glDeleteBuffers(1, &m_indexBuffer);	//Delete our indices
}

void CreateVertexBuffer(GLuint& buffer, int byteCount, char* data) {
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, byteCount, data, GL_STATIC_DRAW);
}

void OGLMesh::BindFloatVertexAttribute(int attribSlot, int buffer, int bindingID, int elementCount, int elementSize, int elementOffset) {
	glEnableVertexAttribArray(attribSlot);
	glVertexAttribFormat(attribSlot, elementCount, GL_FLOAT, false, 0);
	glVertexAttribBinding(attribSlot, bindingID);
	
	glBindVertexBuffer(bindingID, buffer, elementOffset, elementSize);
}

void OGLMesh::BindIntegerVertexAttribute(int attribSlot, int buffer, int bindingID, int elementCount, int elementSize, int elementOffset) {
	glEnableVertexAttribArray(attribSlot);
	glVertexAttribIFormat(attribSlot, elementCount, GL_INT, 0);
	glVertexAttribBinding(attribSlot, bindingID);

	glBindVertexBuffer(bindingID, buffer, elementOffset, elementSize);
}

void OGLMesh::UploadToGPU(Rendering::RendererBase* renderer) {
	if (!ValidateMeshData()) {
		return;
	}
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	int numVertices = GetVertexCount();
	int numIndices	= GetIndexCount();

	if (!GetPositionData().empty()) {
		CreateVertexBuffer(m_attributeBuffers[VertexAttribute::Positions], numVertices * sizeof(Vector3), (char*)GetPositionData().data());
		BindFloatVertexAttribute(VertexAttribute::Positions, m_attributeBuffers[VertexAttribute::Positions], VertexAttribute::Positions, 3, sizeof(Vector3), 0);
	}

	if (!GetColourData().empty()) {	//buffer colour data
		CreateVertexBuffer(m_attributeBuffers[VertexAttribute::Colours], numVertices * sizeof(Vector4), (char*)GetColourData().data());
		BindFloatVertexAttribute(VertexAttribute::Colours, m_attributeBuffers[VertexAttribute::Colours], VertexAttribute::Colours, 4, sizeof(Vector4), 0);
	}
	if (!GetTextureCoordData().empty()) {	//Buffer texture data
		CreateVertexBuffer(m_attributeBuffers[VertexAttribute::TextureCoords], numVertices * sizeof(Vector2), (char*)GetTextureCoordData().data());
		BindFloatVertexAttribute(VertexAttribute::TextureCoords, m_attributeBuffers[VertexAttribute::TextureCoords], VertexAttribute::TextureCoords, 2, sizeof(Vector2), 0);
	}

	if (!GetNormalData().empty()) {	//Buffer normal data
		CreateVertexBuffer(m_attributeBuffers[VertexAttribute::Normals], numVertices * sizeof(Vector3), (char*)GetNormalData().data());
		BindFloatVertexAttribute(VertexAttribute::Normals, m_attributeBuffers[VertexAttribute::Normals], VertexAttribute::Normals, 3, sizeof(Vector3), 0);
	}

	if (!GetTangentData().empty()) {	//Buffer tangent data
		CreateVertexBuffer(m_attributeBuffers[VertexAttribute::Tangents], numVertices * sizeof(Vector4), (char*)GetTangentData().data());
		BindFloatVertexAttribute(VertexAttribute::Tangents, m_attributeBuffers[VertexAttribute::Tangents], VertexAttribute::Tangents, 4, sizeof(Vector4), 0);
	}

	if (!GetSkinWeightData().empty()) {	//Skeleton weights
		CreateVertexBuffer(m_attributeBuffers[VertexAttribute::JointWeights], numVertices * sizeof(Vector4), (char*)GetSkinWeightData().data());
		BindFloatVertexAttribute(VertexAttribute::JointWeights, m_attributeBuffers[VertexAttribute::JointWeights], VertexAttribute::JointWeights, 4, sizeof(Vector4), 0);
	}

	if (!GetSkinIndexData().empty()) {	//Skeleton joint indices
		CreateVertexBuffer(m_attributeBuffers[VertexAttribute::JointIndices], numVertices * sizeof(Vector4i), (char*)GetSkinIndexData().data());
		BindIntegerVertexAttribute(VertexAttribute::JointIndices, m_attributeBuffers[VertexAttribute::JointIndices], VertexAttribute::JointIndices, 4, sizeof(Vector4i), 0);
	}

	if (!GetGeneralVec4Data().empty()) {	//General purpose vec4s
		CreateVertexBuffer(m_attributeBuffers[VertexAttribute::General_Vec4], numVertices * sizeof(Vector4), (char*)GetGeneralVec4Data().data());
		BindFloatVertexAttribute(VertexAttribute::General_Vec4, m_attributeBuffers[VertexAttribute::General_Vec4], VertexAttribute::General_Vec4, 4, sizeof(Vector4), 0);
	}

	if (!GetGeneralIntegerData().empty()) {	//General purpose integers
		CreateVertexBuffer(m_attributeBuffers[VertexAttribute::General_Integer], numVertices * sizeof(int), (char*)GetGeneralIntegerData().data());
		BindIntegerVertexAttribute(VertexAttribute::General_Integer, m_attributeBuffers[VertexAttribute::General_Integer], VertexAttribute::General_Integer, 1, sizeof(int), 0);
	}

	if (!GetIndexData().empty()) {		//buffer index data
		glGenBuffers(1, &m_attributeBuffers[VertexAttribute::MAX_ATTRIBUTES]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_attributeBuffers[VertexAttribute::MAX_ATTRIBUTES]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), (int*)GetIndexData().data(), GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}

void OGLMesh::UpdateGPUBuffers(unsigned int startVertex, unsigned int vertexCount) {
	if (!GetPositionData().empty()) {
		glBindBuffer(GL_ARRAY_BUFFER, m_attributeBuffers[VertexAttribute::Positions]);
		glBufferSubData(GL_ARRAY_BUFFER, startVertex * sizeof(Vector3), vertexCount * sizeof(Vector3), (char*)&GetPositionData()[startVertex]);
	}

	if (!GetColourData().empty()) {	//buffer colour data
		glBindBuffer(GL_ARRAY_BUFFER, m_attributeBuffers[VertexAttribute::Colours]);
		glBufferSubData(GL_ARRAY_BUFFER, startVertex * sizeof(Vector4), vertexCount * sizeof(Vector4), (char*)&GetColourData()[startVertex]);
	}
	if (!GetTextureCoordData().empty()) {	//Buffer texture data
		glBindBuffer(GL_ARRAY_BUFFER, m_attributeBuffers[VertexAttribute::TextureCoords]);
		glBufferSubData(GL_ARRAY_BUFFER, startVertex * sizeof(Vector2), vertexCount * sizeof(Vector2), (char*)&GetTextureCoordData()[startVertex]);
	}

	if (!GetNormalData().empty()) {	//Buffer normal data
		glBindBuffer(GL_ARRAY_BUFFER, m_attributeBuffers[VertexAttribute::Normals]);
		glBufferSubData(GL_ARRAY_BUFFER, startVertex * sizeof(Vector3), vertexCount * sizeof(Vector3), (char*)&GetNormalData()[startVertex]);
	}

	if (!GetTangentData().empty()) {	//Buffer tangent data
		glBindBuffer(GL_ARRAY_BUFFER, m_attributeBuffers[VertexAttribute::Tangents]);
		glBufferSubData(GL_ARRAY_BUFFER, startVertex * sizeof(Vector4), vertexCount * sizeof(Vector4), (char*)&GetTangentData()[startVertex]);
	}

	if (!GetSkinWeightData().empty()) {	//Skeleton weights
		glBindBuffer(GL_ARRAY_BUFFER, m_attributeBuffers[VertexAttribute::JointWeights]);
		glBufferSubData(GL_ARRAY_BUFFER, startVertex * sizeof(Vector4), vertexCount * sizeof(Vector4), (char*)&GetSkinWeightData()[startVertex]);
	}

	if (!GetSkinIndexData().empty()) {	//Skeleton joint indices
		glBindBuffer(GL_ARRAY_BUFFER, m_attributeBuffers[VertexAttribute::JointIndices]);
		glBufferSubData(GL_ARRAY_BUFFER, startVertex * sizeof(Vector4i), vertexCount * sizeof(Vector4i), (char*)&GetSkinIndexData()[startVertex]);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void OGLMesh::RecalculateNormals() {
	normals.clear();

	if (indices.size() > 0) {
		for (size_t i = 0; i < positions.size(); i++) {
			normals.emplace_back(Vector3());
		}

		for (size_t i = 0; i < indices.size(); i += 3) {
			Vector3& a = positions[indices[i+0]];
			Vector3& b = positions[indices[i+1]];
			Vector3& c = positions[indices[i+2]];

			Vector3 normal = Vector::Cross(b - a, c - a);

			normal = Vector::Normalise(normal);

			normals[indices[i + 0]] += normal;
			normals[indices[i + 1]] += normal;
			normals[indices[i + 2]] += normal;
		}
		for (size_t i = 0; i < normals.size(); ++i) {
			normals[i] = Vector::Normalise(normals[i]);
		}
	}
	else {

	}
}