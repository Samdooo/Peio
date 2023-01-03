#define PEIO_GFX_EXPORTING
#include "CommandList.h"

void Peio::Gfx::CommandList::Init(ID3D12CommandQueue* cmdQueue, UINT numBuffers, UINT numAllocators)
{
	HRESULT ret;
	CommandList::~CommandList();

	this->numBuffers = numBuffers;
	this->numAllocators = numAllocators;

	allocators = new CommandAllocator*[numAllocators];

	for (UINT i = 0; i < numAllocators; i++) {
		allocators[i] = new CommandAllocator[numBuffers];
		for (UINT j = 0; j < numBuffers; j++)
			allocators[i][j].Init();
	}

	ret = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocators[0][0].GetAllocator(), nullptr, __uuidof(ID3D12CommandList), &cmdList);
	if (ret != 0) {
		throw PEIO_GFX_EXCEPTION("Failed to create command list.", ret);
	}

	cmdLists[0] = cmdList.Get();
}

void Peio::Gfx::CommandList::Reset(UINT allocator, UINT frameIndex)
{
	allocators[allocator][frameIndex].Reset();

	HRESULT ret = cmdList->Reset(allocators[allocator][frameIndex].GetAllocator(), nullptr);
	if (ret != 0) {
		throw PEIO_GFX_EXCEPTION("Failed to reset command list.", ret);
	}
}

void Peio::Gfx::CommandList::Close()
{
	HRESULT ret = cmdList->Close();
	if (ret != 0) {
		throw PEIO_GFX_EXCEPTION("Failed to close command list.", ret);
	}
}

Peio::Gfx::CommandAllocator& Peio::Gfx::CommandList::GetAllocator(UINT allocator, UINT frameIndex) const noexcept
{
	return allocators[allocator][frameIndex];
}

ID3D12GraphicsCommandList* Peio::Gfx::CommandList::GetCommandList() const noexcept
{
	return cmdList.Get();
}

ID3D12CommandList* const* Peio::Gfx::CommandList::GetCommandLists() const noexcept
{
	return cmdLists;
}

Peio::Gfx::CommandList::~CommandList()
{
	if (allocators) {
		for (UINT i = 0; i < numAllocators; i++)
			if (allocators[i])
				delete[] allocators[i];
		delete[] allocators;
		allocators = nullptr;
	}
}
