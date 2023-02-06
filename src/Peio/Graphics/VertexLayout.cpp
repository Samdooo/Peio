#define PEIO_GFX_EXPORTING
#include "VertexLayout.h"
#include "D3DBPP.h"

namespace Peio::Gfx {

	D3D12_INPUT_LAYOUT_DESC VertexLayout::CreateLayout()
	{
		layoutDesc = {};

		layoutDesc.NumElements = (UINT)elements.size();
		elementDescs.resize(elements.size());

		UINT bytes = 0;
		for (UINT i = 0; i < elements.size(); i++) {
			elementDescs[i] = {
				&elements[i].name[0], 0, elements[i].format, 0, bytes, 
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			};
			bytes += (UINT)BitsPerPixel(elements[i].format) / 8U;
		}
		layoutDesc.pInputElementDescs = &elementDescs[0];

		return layoutDesc;
	}

}