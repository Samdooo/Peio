#pragma once

#include "ValueAnimation.h"
#include "LoadMap.h"
#include "LoadAnimation.h"
#include "..\FunctionHash.h"

namespace Peio::GUI {

	template <>
	struct PEIO_GUI_EXPORT Loadable<AnimationFunction> {

		static LoadMap<AnimationFunction> functionMap;

	};

	template <>
	struct PEIO_GUI_EXPORT Loadable<J_Function> {
		static void Load(J_Function* obj, std::ifstream& ifile) {
			obj->multiplier = Files::ReadObject<double>(ifile);
		}
		static void Save(const J_Function* obj, std::ofstream& ofile) {
			Files::WriteObject(ofile, obj->multiplier);
		}
	};

	template <>
	struct PEIO_GUI_EXPORT Loadable<S_Function> {
		static void Load(S_Function* obj, std::ifstream& ifile) {
			obj->multiplier = Files::ReadObject<double>(ifile);
		}
		static void Save(const S_Function* obj, std::ofstream& ofile) {
			Files::WriteObject(ofile, obj->multiplier);
		}
	};
	
	template <typename T>
	struct Loadable<ValueAnimation<T>> {

		inline static std::unordered_map<std::string, std::function<void(const T*)>> updateFunctions = {};
		inline static std::unordered_map<std::function<void(const T*)>, std::string> updateNames = {};

		static void Load(ValueAnimation<T>* obj, std::ifstream& ifile) {
			Loadable<Animation>::Load(obj, ifile);
			obj->numValues = Files::ReadObject<size_t>(ifile);
			obj->from.resize(obj->numValues);
			obj->to.resize(obj->numValues);
			for (size_t i = 0; i < obj->numValues; i++) {
				obj->from[i] = Files::ReadObject<T>(ifile);
				obj->to[i] = Files::ReadObject<T>(ifile);
			}
			obj->function = Loadable<AnimationFunction>::functionMap.Load(ifile);
			std::string updateName = Files::ReadString(ifile);
			if (updateFunctions.count(updateName)) {
				obj->UpdateValues = updateFunctions.at(updateName);
			}
		}

		static void Save(const ValueAnimation<T>* obj, std::ofstream& ofile) {
			Loadable<Animation>::Save(obj, ofile);
			Files::WriteObject(ofile, obj->numValues);
			for (size_t i = 0; i < obj->numValues; i++) {
				Files::WriteObject(ofile, obj->from[i]);
				Files::WriteObject(ofile, obj->to[i]);
			}
			Loadable<AnimationFunction>::functionMap.Save(obj->function, ofile);
			std::string updateName = "";
			if (updateNames.count(obj->UpdateValues)) {
				updateName = updateNames.at(obj->UpdateValues);
			}
			Files::WriteString(ofile, updateName);
		}

	};
	template struct PEIO_GUI_EXPORT Loadable<ValueAnimation<float>>;
	template struct PEIO_GUI_EXPORT Loadable<ValueAnimation<double>>;
	template struct PEIO_GUI_EXPORT Loadable<ValueAnimation<int>>;

}