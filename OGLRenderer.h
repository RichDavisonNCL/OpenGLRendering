/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series

Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "RendererBase.h"

#ifdef _WIN32
#include "windows.h"
#endif

#ifdef _DEBUG
#define OPENGL_DEBUGGING
#endif

namespace NCL::Rendering {	
	class Mesh;
	class Shader;
	class Texture;

	class OGLMesh;
	class OGLShader;
	class OGLTexture;
	class OGLBuffer;

	class SimpleFont;
		
	class OGLRenderer : public RendererBase	{
	public:
		friend class OGLRenderer;
		OGLRenderer(Window& w);
		~OGLRenderer();

		void OnWindowResize(int w, int h)	override;
		bool HasInitialised()				const override {
			return m_initState;
		}

		virtual bool SetVerticalSync(VerticalSyncState s);

	protected:			
		void BeginFrame()	override;
		void RenderFrame()	override;
		void EndFrame()		override;
		void SwapBuffers()  override;

		void BindShader(const OGLShader& s);
		void BindTextureToShader(const OGLTexture& t, const std::string& uniform, int texUnit) const;
		void BindMesh(const OGLMesh& m);
		void BindBufferAsUBO(const OGLBuffer& b, uint32_t slotID);
		void BindBufferAsSSBO(const OGLBuffer& b, uint32_t slotID);

		void DrawBoundMesh(int subLayer = 0, int numInstances = 1);
#ifdef _WIN32
		void InitWithWin32(Window& w);
		void DestroyWithWin32();
		HDC		m_deviceContext;		//...Device context?
		HGLRC	m_renderContext;		//Permanent Rendering Context		
#endif

		const OGLMesh*		m_boundMesh;
		const OGLShader*	m_boundShader;
	private:
		bool m_initState;
	};
}