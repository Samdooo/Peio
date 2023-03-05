#include "..\App.h"

#include <iostream>

namespace Scene {

	void HyperRectangles::Init()
	{
		//if (rects.size() == 0)
		//	return;
		UINT vectorSize = App::app.scene.numDims * (UINT)sizeof(float);
		UINT rectSize = vectorSize * 3 - sizeof(float) + (UINT)sizeof(Material);
		if (!App::app.init) {
			buffer.shaderRegister = 2;
			buffer.visibility = D3D12_SHADER_VISIBILITY_ALL;
			App::app.rayRenderer.parameters.push_back(&buffer);
		}
		if (rects.size() == 0)
			return;
		if (!App::app.init || buffer.GetElementSize() * buffer.GetNumElements() != rectSize * (UINT)rects.size()) {
			buffer.Init(rectSize, (UINT)rects.size());
			std::cout << "Initialized hyperrectangle buffer" << std::endl;
		}
	}

	void HyperRectangles::Update(ID3D12GraphicsCommandList* cmdList)
	{
		if (rects.size() == 0)
			return;
		UINT vectorSize = App::app.scene.numDims * (UINT)sizeof(float);
		UINT rectSize = vectorSize * 3 - sizeof(float) + (UINT)sizeof(Material);
		for (UINT i = 0; i < rects.size(); i++) {
			memcpy(buffer.buffer.GetData() + rectSize * i, &rects[i].low[0], vectorSize);
			memcpy(buffer.buffer.GetData() + rectSize * i + vectorSize, &rects[i].high[0], vectorSize);
			memcpy(buffer.buffer.GetData() + rectSize * i + vectorSize * 2, &rects[i].rotation[0], vectorSize - sizeof(float));
			memcpy(buffer.buffer.GetData() + rectSize * i + vectorSize * 3 - sizeof(float), &rects[i].material, sizeof(Material));
		}
		buffer.Upload(cmdList);
	}

	void HyperSpheres::Init()
	{
		//if (spheres.size() == 0)
		//	return;
		UINT vectorSize = App::app.scene.numDims * (UINT)sizeof(float);
		UINT sphereSize = vectorSize + (UINT)sizeof(float) + (UINT)sizeof(Material);
		if (!App::app.init) {
			buffer.shaderRegister = 3;
			buffer.visibility = D3D12_SHADER_VISIBILITY_ALL;
			App::app.rayRenderer.parameters.push_back(&buffer);
		}
		if (spheres.size() == 0)
			return;
		if (!App::app.init || buffer.GetElementSize() * buffer.GetNumElements() != sphereSize * (UINT)spheres.size()) {
			buffer.Init(sphereSize, (UINT)spheres.size());
			std::cout << "Initialized hypersphere buffer" << std::endl;
		}
	}

	void HyperSpheres::Update(ID3D12GraphicsCommandList* cmdList)
	{
		if (spheres.size() == 0)
			return;
		UINT vectorSize = App::app.scene.numDims * (UINT)sizeof(float);
		UINT sphereSize = vectorSize + (UINT)sizeof(float) + (UINT)sizeof(Material);
		for (UINT i = 0; i < spheres.size(); i++) {
			memcpy(buffer.buffer.GetData() + sphereSize * i, &spheres[i].center[0], vectorSize);
			memcpy(buffer.buffer.GetData() + sphereSize * i + vectorSize, &spheres[i].radius, sizeof(float));
			memcpy(buffer.buffer.GetData() + sphereSize * i + vectorSize + sizeof(float), &spheres[i].material, sizeof(Material));
		}
		buffer.Upload(cmdList);
	}

	void Objects::Init()
	{
		hyperRectangles.Init();
		hyperSpheres.Init();
	}

	void Objects::Update(ID3D12GraphicsCommandList* cmdList)
	{
		hyperRectangles.Update(cmdList);
		hyperSpheres.Update(cmdList);
	}


}

