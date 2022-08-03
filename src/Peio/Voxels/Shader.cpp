#define PEIO_VXL_EXPORTING
#include "Shader.h"
#include "..\Files.h"
#include "..\Graphics\Global.h"
#include <regex>

namespace Peio::Vxl {

	HRESULT Shader::Include::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes)
	{
		std::string code = {};
		try {
			code = Files::ReadString(pFileName);
		}
		catch (...) {
			*ppData = nullptr;
			return S_OK;
		}
		for (UINT i = 0; i < replacements.size(); i++) {
			code = std::regex_replace(code, std::regex(replacements[i].first), replacements[i].second);
		}
		char* data = new char[code.size()];
		memcpy(data, &code[0], code.size());
		*ppData = data;
		*pBytes = (UINT)code.size();
		return S_OK;
	}

	HRESULT Shader::Include::Close(LPCVOID pData)
	{
		if (pData)
			delete pData;
		return S_OK;
	}

	void Shader::Init(const char* path)
	{
		code = Files::ReadString(path);
	}

	void Shader::Replace(std::string regex, std::string to)
	{
		code = std::regex_replace(code, std::regex(regex), to);
		include.replacements.push_back({ regex, to });
	}

	void Shader::Compile(const char* entryPoint, const char* model)
	{
		HRESULT ret = D3DCompile(code.c_str(), code.size(), nullptr, nullptr, &include, "main", model, 0, 0, &blob, &errorBlob);
		if (ret != 0) {
			throw PEIO_EXCEPTION(std::string(std::string("Failed to compile shader. Error: ") + std::string((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize())).c_str());
		}
		bytecode.pShaderBytecode = blob->GetBufferPointer();
		bytecode.BytecodeLength = blob->GetBufferSize();
	}

	void Shader::CompileVS(const char* entryPoint)
	{
		Compile(entryPoint, "vs_6_0");
	}

	void Shader::CompilePS(const char* entryPoint)
	{
		Compile(entryPoint, "ps_6_0");
	}

	D3D12_SHADER_BYTECODE Shader::GetBytecode() const
	{
		return bytecode;
	}

}