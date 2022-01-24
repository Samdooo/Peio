#pragma once

#include "Exception.h"
#include "Array.h"

#include <fstream>
#include <filesystem>
#include <string>

namespace Peio {

	struct Files {

		static void CheckExists(const char* path) {
			if (!std::filesystem::exists(path)) {
				throw PEIO_EXCEPTION(std::string("File \"" + std::string(path) + "\" doesn't exist.").c_str());
			}
		}

		static void CheckFile(const char* path) {
			CheckExists(path);
			if (std::filesystem::is_directory(path)) {
				throw PEIO_EXCEPTION(std::string("Path \"" + std::string(path) + "\" is not a file.").c_str());
			}
		}

		static void CheckDirectory(const char* path) {
			CheckExists(path);
			if (!std::filesystem::is_directory(path)) {
				throw PEIO_EXCEPTION(std::string("Path \"" + std::string(path) + "\" is not a directory.").c_str());
			}
		}

		template <typename T>
		_NODISCARD static size_t ReadBinary(const char* path, T** dst) {
			CheckFile(path);

			size_t length = std::filesystem::file_size(path);
			*dst = reinterpret_cast<T*>(new byte[length]);

			std::ifstream ifile(path, std::ios::binary);
			ifile.read((char*)*dst, length);
			ifile.close();

			return length;
		}

		_NODISCARD static std::string ReadString(const char* path) {
			CheckFile(path);

			size_t length = std::filesystem::file_size(path);

			std::string result = "";
			result.resize(length, 0);
			
			std::ifstream ifile(path, std::ios::binary);
			ifile.read(&result[0], length);
			ifile.close();

			return result;
		}

		template <typename T_obj>
		static T_obj ReadObject(std::ifstream& ifile) {
			if (!ifile.good()) {
				throw PEIO_EXCEPTION("Invalid ifile.");
			}
			T_obj obj;
			ifile.read((char*)&obj, sizeof(T_obj));
			return obj;
		}

		static std::string ReadString(std::ifstream& ifile) {
			if (!ifile.good()) {
				throw PEIO_EXCEPTION("Invalid ifile.");
			}
			std::string result;
			result.resize(ReadObject<size_t>(ifile));
			ifile.read(&result[0], result.size());
			return result;
		}

		template <typename T_obj>
		static void WriteObject(std::ofstream& ofile, const T_obj& obj) {
			if (!ofile.good()) {
				throw PEIO_EXCEPTION("Invalid ofile.");
			}
			ofile.write((const char*)&obj, sizeof(T_obj));
		}

		static void WriteString(std::ofstream& ofile, const std::string& str) {
			if (!ofile.good()) {
				throw PEIO_EXCEPTION("Invalid ofile.");
			}
			WriteObject(ofile, str.size());
			ofile.write(&str[0], str.size());
		}

	};

}