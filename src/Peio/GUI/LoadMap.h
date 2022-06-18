#pragma once

#include "Loadable.h"
#include "..\Files.h"
#include <unordered_map>
#include <typeindex>

namespace Peio::GUI {

	template <typename T_base>
	struct LoadMap {

	protected:

		struct Type {
			T_base* (*Load)(std::ifstream&);
			void (*Save)(const T_base*, std::ofstream&);
		};

		std::unordered_map<size_t, Type> types = {};
		std::unordered_map<std::type_index, size_t> indices = {};

	public:

		template <typename T_obj, size_t T_id>
		LoadMap<T_base>& Insert() {
			types[T_id] = {
				[](std::ifstream& ifile) {
					T_obj* obj = new T_obj;
					Loadable<T_obj>::Load(obj, ifile);
					return static_cast<T_base*>(obj);
				},
				[](const T_base* obj, std::ofstream& ofile) {
					Loadable<T_obj>::Save(dynamic_cast<const T_obj*>(obj), ofile);
				}
			};
			indices[std::type_index(typeid(const T_obj))] = T_id;
			return *this;
		}

		_NODISCARD T_base* Load(std::ifstream& ifile) const {
			size_t index = Files::ReadObject<size_t>(ifile);
			if (!types.count(index)) {
				throw PEIO_EXCEPTION("Unknown type in file.");
			}
			return types.at(index).Load(ifile);
		}

		void Save(const T_base* obj, std::ofstream& ofile) const {
			std::type_index typeIndex(typeid(*obj));
			if (!indices.count(typeIndex)) {
				throw PEIO_EXCEPTION("Unknown type in base pointer.");
			}
			size_t index = indices.at(typeIndex);
			Files::WriteObject(ofile, index);
			types.at(index).Save(obj, ofile);
		}

		_NODISCARD size_t GetSize() const noexcept {
			return types.size();
		}

	};

}