/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series

Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*/////////////////////////////////////////////////////////////////////////////
#include "OGLComputeShader.h"
#include "OGLShader.h"
#include "Assets.h"

using namespace NCL;
using namespace Rendering;
using std::string;

OGLComputeShader::OGLComputeShader(const std::string& s)	{
	string fileContents = "";
	Assets::ReadTextFile(Assets::SHADERDIR + s, fileContents);

	m_programID	= glCreateProgram();
	m_shaderID	= glCreateShader(GL_COMPUTE_SHADER);

	const char* stringData		= fileContents.c_str();
	int			stringLength	= (int)fileContents.length();
	glShaderSource(m_shaderID, 1, &stringData, &stringLength);
	glCompileShader(m_shaderID);
	glAttachShader(m_programID, m_shaderID);

	glLinkProgram(m_programID);
	glGetProgramiv(m_programID, GL_LINK_STATUS, &m_programValid);

	std::cout << "Loading compute shader " << s << "\n";
	if (m_programValid != GL_TRUE) {
		std::cout << "Compute shader has failed!\n";
	}
	else {
		std::cout << "Compute shader loaded!\n";		
		glGetProgramiv(m_programID, GL_COMPUTE_WORK_GROUP_SIZE, (int*) & m_threadsInGroup);
	}
	OGLShader::PrintCompileLog(m_shaderID);
};

OGLComputeShader::~OGLComputeShader()	{
	glDetachShader(m_programID, m_shaderID);
	glDeleteShader(m_shaderID);
	glDeleteProgram(m_programID);
}

void OGLComputeShader::Execute(unsigned int x, unsigned int y, unsigned int z) const {
	glDispatchCompute(x, y, z);
}

void OGLComputeShader::Bind() const {
	glUseProgram(m_programID);
}

void OGLComputeShader::Unbind() {
	glUseProgram(0);
}
