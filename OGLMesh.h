/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series

Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Mesh.h"
#include "glad\gl.h"

namespace NCL::Rendering {	
	using UniqueOGLMesh		= std::unique_ptr<class OGLMesh>;
	using SharedOGLMesh		= std::shared_ptr<class OGLMesh>;

	class OGLMesh : public Mesh	{
	public:
		friend class OGLRenderer;
		OGLMesh();
		~OGLMesh();

		void RecalculateNormals();

		void UploadToGPU(Rendering::RendererBase* renderer = nullptr) override;
		void UpdateGPUBuffers(unsigned int startVertex, unsigned int vertexCount);

	protected:
		GLuint	GetVAO()			const { return m_vao;			}
		void	BindFloatVertexAttribute(int attribSlot, int bufferID, int bindingID, int elementCount, int elementSize, int elementOffset);

		void	BindIntegerVertexAttribute(int attribSlot, int bufferID, int bindingID, int elementCount, int elementSize, int elementOffset);

		GLuint m_vao;
		GLuint m_attributeBuffers[VertexAttribute::MAX_ATTRIBUTES];
		GLuint m_indexBuffer;
	};
}