#define PEIO_GFX_EXPORTING
#include "VertexBuffer.h"

namespace Peio::Gfx {

    const D3D12_VERTEX_BUFFER_VIEW& VertexBuffer::GetView()
    {
        view = {};
        view.BufferLocation = ArrayBufferResource::GetGPUAddress();
        view.SizeInBytes = ArrayBufferResource::buffer.GetSize();
        view.StrideInBytes = ArrayBufferResource::srvDesc.Buffer.StructureByteStride;
        return view;
    }

}