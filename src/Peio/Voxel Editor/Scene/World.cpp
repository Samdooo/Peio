#include "..\App.h"

#include <iostream>

namespace Scene {

	size_t World::MaterialHash::operator()(const Material& material) const
	{
		size_t hash = 0;
		for (size_t i = 0; i < sizeof(Material) / sizeof(size_t); i++) {
			hash ^= std::hash<size_t>()(((const size_t*)&material)[i]);
		}
		return hash;
	}

	bool World::MaterialEqual::operator()(const Material& first, const Material& second) const
	{
		return first.reflection == second.reflection &&
			first.emission == second.emission;
	}

	void World::Init()
	{
		materialBuffer.Allocate(1 << 21);
		materialSrv.Init(materialBuffer.GetBufferSize(), materialBuffer.GetNumElements(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

		map.Init(14, 1 << 21);
		mapBuffer.SetBuffer(&map.nodes[0], map.nodes.size());
		mapSrv.Init(mapBuffer.GetBufferSize(), mapBuffer.GetNumElements(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

		if (std::filesystem::exists("Worlds/default.vxw")) {
			std::ifstream ifile("Worlds/default.vxw", std::ios::binary);
			LoadFromFile(ifile);
			ifile.close();
		}

		shouldUploadMap = true;
		shouldUploadMaterials = true;
	}

	void World::UploadMaterials()
	{
		materialSrv.Upload(materialBuffer.GetResourceData(), App::app.graphics.GetCommandList());
	}

	void World::UploadMap()
	{
		mapSrv.Upload(mapBuffer.GetResourceData(), App::app.graphics.GetCommandList());
	}

	void World::Update()
	{
		if (shouldUploadMaterials) {
			shouldUploadMaterials = false;
			App::app.scene.info.scene.newRays = true;
			UploadMaterials();
		}
		if (shouldUploadMap) {
			shouldUploadMap = false;
			App::app.scene.info.scene.newRays = true;
			UploadMap();
		}
	}

	void World::SetVoxel(Peio::Uint3 position, UINT material)
	{
		map.SetIndex(position, material);
		shouldUploadMap = true;
	}

	void World::SetVoxel(Peio::Uint3 position, const Material& material)
	{
		SetVoxel(position, GetMaterialIndex(material));
	}

	UINT World::AddMaterial(const Material& material)
	{
		materialBuffer[materials.size()] = material;
		materials.insert({ material, materials.size() });
		shouldUploadMaterials = true;
		return materials.size() - 1;
	}

	UINT World::GetMaterialIndex(const Material& material)
	{
		if (materials.contains(material))
			return materials.at(material);
		else
			return AddMaterial(material);
	}

	World::Material World::GetMaterial(UINT index) const
	{
		return materialBuffer[index];
	}

	UINT World::GetVoxel(Peio::Uint3 position) const
	{
		return map.GetIndex(position);
	}

	World::Ray World::Trace(Peio::Double3 origin, Peio::Double3 ray)
	{
		return map.Trace(origin, ray);
	}

	World::Ray World::Trace(Peio::Double3 origin, Peio::Double2 angle)
	{
		return map.Trace(origin, angle);
	}

	void World::SaveToFile(std::ofstream& ofile) const
	{
		map.SaveToFile(ofile);
		Peio::Files::WriteObject(ofile, (UINT)materials.size());
		ofile.write((char*)materialBuffer.GetSubresourceBuffer(), sizeof(Material) * materials.size());
	}

	void World::LoadFromFile(std::ifstream& ifile)
	{
		map.LoadFromFile(ifile);
		UINT numMaterials = Peio::Files::ReadObject<UINT>(ifile);
		ifile.read((char*)materialBuffer.GetSubresourceBuffer(), sizeof(Material) * numMaterials);
		
		materials.clear();
		for (UINT i = 0; i < numMaterials; i++) {
			materials.insert({ materialBuffer[i], i });
		}
		shouldUploadMap = true;
		shouldUploadMaterials = true;
	}

}