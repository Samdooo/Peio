#pragma once

#include "..\..\Voxels\Material.h"
#include "..\..\Voxels\MaterialMap.h"
#include "..\..\Graphics\SubresourceBuffer.h"
#include "..\..\Graphics\ShaderResourceView.h"

namespace Scene {

	struct World {

		using Material = Peio::Vxl::Material;
		using MaterialMap = Peio::Vxl::MaterialMap;
		using Ray = MaterialMap::Ray;

		struct MaterialHash {
			_NODISCARD size_t operator()(const Material& material) const;
		};
		struct MaterialEqual {
			_NODISCARD bool operator()(const Material& first, const Material& second) const;
		};

		void Init();
		
		void UploadMaterials();
		void UploadMap();

		void Update();

		void SetVoxel(Peio::Uint3 position, UINT material);
		void SetVoxel(Peio::Uint3 position, const Material& material);
		UINT AddMaterial(const Material& material);

		_NODISCARD UINT GetMaterialIndex(const Material& material);
		_NODISCARD Material GetMaterial(UINT index) const;
		_NODISCARD UINT GetVoxel(Peio::Uint3 position) const;
		
		_NODISCARD Ray Trace(Peio::Double3 origin, Peio::Double3 ray);
		_NODISCARD Ray Trace(Peio::Double3 origin, Peio::Double2 angle);

		void SaveToFile(std::ofstream& ofile) const;
		void LoadFromFile(std::ifstream& ifile);

		Peio::Gfx::BufferSRV materialSrv = {};
		Peio::Gfx::BufferSRV mapSrv = {};

	protected:

		MaterialMap map = {};

		Peio::Gfx::SubresourceBuffer<Material> materialBuffer = {};
		bool shouldUploadMaterials = false;
		std::unordered_map<Material, UINT, MaterialHash, MaterialEqual> materials = {};

		Peio::Gfx::SubresourceBuffer<MaterialMap::Node> mapBuffer = {};
		bool shouldUploadMap = false;

	};

}