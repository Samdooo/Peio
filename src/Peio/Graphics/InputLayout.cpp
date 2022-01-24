#define PEIO_GFX_EXPORTING
#include "InputLayout.h"
#include "D3DBPP.h"

D3D12_INPUT_ELEMENT_DESC Peio::Gfx::InputElement::Create(LPCSTR name, DXGI_FORMAT format, UINT byteOffset, D3D12_INPUT_CLASSIFICATION inputSlotClass, UINT semanticIndex, UINT inputSlot, UINT instanceDataStepRate)
{
	return { name, semanticIndex, format, inputSlot, byteOffset, inputSlotClass, instanceDataStepRate };
}

D3D12_INPUT_LAYOUT_DESC Peio::Gfx::InputLayout::Create(std::vector<D3D12_INPUT_ELEMENT_DESC> elements)
{
	UINT bytes = 0;
	for (UINT i = 0; i < elements.size(); i++) {
		elements[i].AlignedByteOffset = bytes;
		bytes += (UINT)BitsPerPixel(elements[i].Format) / 8U;
	}
	D3D12_INPUT_ELEMENT_DESC* pElements = new D3D12_INPUT_ELEMENT_DESC[elements.size()];
	memcpy(pElements, &elements[0], sizeof(D3D12_INPUT_ELEMENT_DESC) * elements.size());
	return { pElements, static_cast<UINT>(elements.size()) };
}