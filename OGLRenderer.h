/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#pragma once
#include "RendererBase.h"

#include "Vector3.h"
#include "Vector4.h"


#ifdef _WIN32
#include "windows.h"
#endif

#ifdef _DEBUG
#define OPENGL_DEBUGGING
#endif


#include <string>
#include <vector>

namespace NCL {
	class MeshGeometry;

	namespace Maths {
		class Matrix4;
	}

	namespace Rendering {
		class ShaderBase;
		class TextureBase;

		class OGLMesh;
		class OGLShader;

		class SimpleFont;
		
		class OGLRenderer : public RendererBase
		{
		public:
			friend class OGLRenderer;
			OGLRenderer(Window& w);
			~OGLRenderer();

			void OnWindowResize(int w, int h)	override;
			bool HasInitialised()				const override {
				return initState;
			}

			void ForceValidDebugState(bool newState) {
				forceValidDebugState = newState;
			}

			virtual bool SetVerticalSync(VerticalSyncState s);

		protected:			
			void BeginFrame()	override;
			void RenderFrame()	override;
			void EndFrame()		override;
			void SwapBuffers()  override;

			void BindShader(ShaderBase*s);
			void BindTextureToShader(const TextureBase*t, const std::string& uniform, int texUnit) const;
			void BindMesh(MeshGeometry*m);
			void DrawBoundMesh(int subLayer = 0, int numInstances = 1);
#ifdef _WIN32
			void InitWithWin32(Window& w);
			void DestroyWithWin32();
			HDC		deviceContext;		//...Device context?
			HGLRC	renderContext;		//Permanent Rendering Context		
#endif
		private:
			OGLMesh*	boundMesh;
			OGLShader*	boundShader;

			bool initState;
			bool forceValidDebugState;
		};
	}
}