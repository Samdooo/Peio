#include "..\App.h"

#include <iostream>

namespace Scene {

	void Camera::Init() 
	{
		UINT vectorSize = App::app.scene.numDims * (UINT)sizeof(float);
		UINT cameraSize = vectorSize * 2 + (-1 + 2) * (UINT)sizeof(float);
		if (!App::app.init) {
			buffer.shaderRegister = 1;
			buffer.visibility = D3D12_SHADER_VISIBILITY_ALL;
			App::app.rayRenderer.parameters.push_back(&buffer);
		}
		if (!App::app.init || buffer.buffer.GetSize() != cameraSize) {
			buffer.Init(cameraSize, 1);
			std::cout << "Initialized camera buffer" << std::endl;
		}
	}

	void Camera::Update(ID3D12GraphicsCommandList* cmdList)
	{
		UINT vectorSize = App::app.scene.numDims * (UINT)sizeof(float);
		memcpy(buffer.buffer.GetData(), &position[0], vectorSize);
		memcpy(buffer.buffer.GetData() + vectorSize, &rotation[0], vectorSize - sizeof(float));
		memcpy(buffer.buffer.GetData() + vectorSize * 2 - sizeof(float), &fov, sizeof(float));
		memcpy(buffer.buffer.GetData() + vectorSize * 2 - sizeof(float) + sizeof(float), &aspectRatio, sizeof(float));

		buffer.Upload(cmdList);
	}

}