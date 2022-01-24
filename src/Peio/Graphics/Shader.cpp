#define PEIO_GFX_EXPORTING
#include "Shader.h"

D3D12_SHADER_BYTECODE Peio::Gfx::Shader::Load(const char* path)
{
	D3D12_SHADER_BYTECODE shader;
	void* ptr = nullptr;
	shader.BytecodeLength = Files::ReadBinary(path, &ptr);
	shader.pShaderBytecode = ptr;
	return shader;
}
