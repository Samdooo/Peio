#pragma once

#include "Exception.h"
#include <string>

#define PEIO_GFX_SHADER_EXCEPTION(x, y, z) Peio::Gfx::ShaderException(x, __FILE__, __LINE__, y, z)

namespace Peio::Gfx {

	struct ShaderException : public Exception {

		ShaderException(const char* msg, const char* file, int line, HRESULT ret, const char* output)
			: Exception(msg, file, line, ret), output(output) {}

		const char* output;

	};

	struct PEIO_GFX_EXPORT Shader {

		std::string data = {};
		
		void Compile(std::string path, std::string profile, std::vector<std::pair<std::string, std::string>> defines = {});

	protected:

		struct IncludeHandler : public IDxcIncludeHandler {
			IDxcLibrary* library = nullptr;
			
			HRESULT QueryInterface(REFIID riid, void** ppvObject) override;
			ULONG AddRef() override;
			ULONG Release() override;
			HRESULT LoadSource(LPCWSTR path, IDxcBlob** outBlob) override;
		};

	};

}