#define PEIO_GFX_EXPORTING
#include "DescriptorTable.h"

D3D12_DESCRIPTOR_RANGE Peio::Gfx::DescriptorRange::Create(D3D12_DESCRIPTOR_RANGE_TYPE type, UINT n_descriptors, UINT index)
{
	D3D12_DESCRIPTOR_RANGE range;
	range.RangeType = type;
	range.NumDescriptors = n_descriptors;
	range.BaseShaderRegister = index;
	range.RegisterSpace = 0;
	range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	return range;
}

D3D12_ROOT_DESCRIPTOR_TABLE Peio::Gfx::DescriptorTable::Create(const std::vector<D3D12_DESCRIPTOR_RANGE>& ranges)
{
	D3D12_ROOT_DESCRIPTOR_TABLE table;
	table.NumDescriptorRanges = (UINT)ranges.size();
	D3D12_DESCRIPTOR_RANGE* pRanges = new D3D12_DESCRIPTOR_RANGE[ranges.size()];
	memcpy(pRanges, &ranges[0], sizeof(D3D12_DESCRIPTOR_RANGE) * ranges.size());
	table.pDescriptorRanges = pRanges;
	//table.pDescriptorRanges = &ranges[0]; /* Might cause errors? */

	return table;
}
