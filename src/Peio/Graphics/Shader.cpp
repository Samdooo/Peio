#define PEIO_GFX_EXPORTING
#include "Shader.h"

#include <atlbase.h>
#include <atlconv.h>

#include <filesystem>

namespace Peio::Gfx {

	HRESULT Shader::IncludeHandler::QueryInterface(REFIID riid, void** ppvObject)
	{
		return 0;
	}

	ULONG Shader::IncludeHandler::AddRef()
	{
		return 0;
	}

	ULONG Shader::IncludeHandler::Release()
	{
		return 0;
	}

	HRESULT Shader::IncludeHandler::LoadSource(LPCWSTR path, IDxcBlob** outBlob)
	{
		UINT32 codePage = CP_UTF8;
		ComPtr<IDxcBlobEncoding> sourceBlob = nullptr;
		HRESULT ret = library->CreateBlobFromFile(path, &codePage, (IDxcBlobEncoding**)outBlob);
		if (ret != 0) {
			throw PEIO_GFX_EXCEPTION("Failed to load shader include file.", ret);
		}
		return 0;
	}

	void Shader::Compile(std::string path, std::string profile, std::vector<std::pair<std::string, std::string>> defines)
	{
		HRESULT ret;

		std::filesystem::path oldPath = std::filesystem::current_path();
		std::filesystem::current_path(std::filesystem::path(path).parent_path());

		path = std::filesystem::path(path).filename().string();

		ComPtr<IDxcLibrary> library = nullptr;
		ret = DxcCreateInstance(CLSID_DxcLibrary, __uuidof(IDxcLibrary), &library);
		if (ret != 0) {
			throw PEIO_GFX_EXCEPTION("Failed to create dxc library.", ret);
		}

		ComPtr<IDxcCompiler> compiler = nullptr;
		ret = DxcCreateInstance(CLSID_DxcCompiler, __uuidof(IDxcCompiler), &compiler);
		if (ret != 0) {
			throw PEIO_GFX_EXCEPTION("Failed to create dxc compiler.", ret);
		}

		std::wstring wPath = std::wstring(CA2W(path.c_str()));
		UINT32 codePage = CP_UTF8;
		ComPtr<IDxcBlobEncoding> sourceBlob = nullptr;
		ret = library->CreateBlobFromFile(wPath.c_str(), &codePage, &sourceBlob);
		if (ret != 0) {
			throw PEIO_GFX_EXCEPTION("Failed to load shader file.", ret);
		}

		IncludeHandler includeHandler = {};
		includeHandler.library = library.Get();

		std::vector<std::pair<std::wstring, std::wstring>> wDefines(defines.size());
		std::vector<DxcDefine> dxcDefines(defines.size());
		for (UINT i = 0; i < defines.size(); i++) {
			wDefines[i] = { std::wstring(CA2W(defines[i].first.c_str())), std::wstring(CA2W(defines[i].second.c_str())) };
			dxcDefines[i].Name = wDefines[i].first.c_str();
			dxcDefines[i].Value = wDefines[i].second.c_str();
		}

		std::wstring wProfile = std::wstring(CA2W(profile.c_str()));
		ComPtr<IDxcOperationResult> result = nullptr;
		ret = compiler->Compile(sourceBlob.Get(), wProfile.c_str(), L"main", 
			wProfile.c_str(), nullptr, 0, &dxcDefines[0], (UINT)dxcDefines.size(), &includeHandler, &result);
		if (result)
			result->GetStatus(&ret);
		if (ret != 0) {
			if (result) {
				ComPtr<IDxcBlobEncoding> errorBlob = nullptr;
				HRESULT errRet = result->GetErrorBuffer(&errorBlob);
				char* output = nullptr;
				if (errRet == 0) {
					output = new char[errorBlob->GetBufferSize()];
					memcpy(output, errorBlob->GetBufferPointer(), errorBlob->GetBufferSize());
				}
				throw PEIO_GFX_SHADER_EXCEPTION("Failed to compile shader.", ret, output);
			}
			else {
				throw PEIO_GFX_EXCEPTION("Failed to compile shader.", ret);
			}
		}
		ComPtr<IDxcBlob> resultData = nullptr;
		result->GetResult(&resultData);

		data.resize(resultData->GetBufferSize());
		memcpy(&data[0], resultData->GetBufferPointer(), resultData->GetBufferSize());

		std::filesystem::current_path(oldPath);
	}

}