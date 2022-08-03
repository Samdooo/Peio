#pragma once

#include "VoxelsHeader.h"
#include <string>

namespace Peio::Vxl {

	struct PEIO_VXL_EXPORT Shader {

		void Init(const char* path);
		void Replace(std::string regex, std::string to);

		void Compile(const char* entryPoint, const char* model);
		void CompileVS(const char* entryPoint = "main");
		void CompilePS(const char* entryPoint = "main");

		_NODISCARD D3D12_SHADER_BYTECODE GetBytecode() const;

	protected:

		struct Include : public ID3DInclude {

			std::vector<std::pair<std::string, std::string>> replacements = {};

			HRESULT Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override;
			HRESULT Close(LPCVOID pData) override;

		};

		std::string code = {};
		Include include = {};
		Microsoft::WRL::ComPtr<ID3DBlob> blob = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
		D3D12_SHADER_BYTECODE bytecode = {};

	};

}