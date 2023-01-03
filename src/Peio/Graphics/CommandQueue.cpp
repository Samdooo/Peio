#define PEIO_GFX_EXPORTING
#include "CommandQueue.h"

void Peio::Gfx::CommandQueue::Init()
{
	CommandQueue::~CommandQueue();

	D3D12_COMMAND_QUEUE_DESC desc = {};
	HRESULT ret = device->CreateCommandQueue(&desc, __uuidof(ID3D12CommandQueue), &cmdQueue);
	if (ret != 0) {
		throw PEIO_GFX_EXCEPTION("Failed to create command queue.", ret);
	}
}

void Peio::Gfx::CommandQueue::Execute(ID3D12CommandList* const* lists, UINT numLists)
{
	cmdQueue->ExecuteCommandLists(numLists, lists);
}

ID3D12CommandQueue* Peio::Gfx::CommandQueue::GetQueue() const noexcept
{
	return cmdQueue.Get();
}

Peio::Gfx::CommandQueue::~CommandQueue()
{
	
}
