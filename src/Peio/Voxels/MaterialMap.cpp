#define PEIO_VXL_EXPORTING
#include "MaterialMap.h"

bool Peio::Vxl::MaterialMap::Group::operator==(const Group& that) const
{
	for (size_t i = 0; i < sizeof(Group) / sizeof(size_t); i++)
		if (((const size_t*)this)[i] != ((const size_t*)&that)[i])
			return false;
	return true;
}

size_t Peio::Vxl::MaterialMap::GroupHash::operator()(const Group& group) const
{
	size_t result = 0;
	for (size_t i = 0; i < sizeof(Group) / sizeof(size_t); i++)
		result ^= std::hash<size_t>()(((const size_t*)&group)[i]);
	return result;
}

void Peio::Vxl::MaterialMap::Init(uint maxSize, uint numLayers)
{
	this->numLayers = numLayers;
	groups.resize(maxSize);
	map.reserve(maxSize);
	references.resize(maxSize, 0);
	deleted.resize(maxSize, 0);
	numDeleted = 0;
	nextFree = 1;

	path.resize(numLayers);
}

Peio::uint Peio::Vxl::MaterialMap::GetNext()
{
	if (numDeleted) {
		return deleted[--numDeleted];
	}
	else {
		if (nextFree == groups.size() - 1) {
			throw PEIO_EXCEPTION("MaterialMap overflow.");
		}
		return nextFree++;
	}
}

void Peio::Vxl::MaterialMap::AddDeleted(uint index)
{
	deleted[numDeleted++] = index;
}

void Peio::Vxl::MaterialMap::RemoveRef(uint index)
{
	if (references[index] == 0) {
		map.erase(groups[index]);
		groups[index] = Group();
		AddDeleted(index);
	}
	else {
		references[index]--;
	}
}

Peio::uint Peio::Vxl::MaterialMap::GetMaterial(Uint3 pos) const
{
	const Group* cur = &groups[0];
	for (uint l = 0; l < numLayers; l++) {
		uint mask = 1U << (numLayers - l - 1);
		uint index = cur->indices[(bool)(pos.x() & mask)][(bool)(pos.y() & mask)][(bool)(pos.z() & mask)];
		if (l == numLayers - 1)
			return index;
		if (index == ~0)
			return ~0;
		cur = &groups[index];
	}
	return ~0;
}

void Peio::Vxl::MaterialMap::SetMaterial(Uint3 pos, uint material)
{
	path[0] = 0;
	for (uint l = 0; l < numLayers - 1; l++) {
		uint mask = 1U << (numLayers - l - 1);
		if (path[l] == ~0)
			path[l + 1] = ~0;
		else
			path[l + 1] = groups[path[l]].indices[(bool)(pos.x() & mask)][(bool)(pos.y() & mask)][(bool)(pos.z() & mask)];
	}

	uint next = material;
	for (uint l = numLayers - 1;; l--) {
		uint mask = 1U << (numLayers - l - 1);

		Group group = (path[l] == ~0) ? Group() : groups[path[l]];
		uint& index = group.indices[(bool)(pos.x() & mask)][(bool)(pos.y() & mask)][(bool)(pos.z() & mask)];
		if (index == next)
			break;
		index = next;

		if (l == 0) {
			groups[path[l]] = group;
			break;
		}

		if (map.contains(group)) {
			next = map.at(group);
			if (path[l] != ~0)
				RemoveRef(path[l]);
			references[next]++;
		}
		else if (path[l] == ~0 || references[path[l]] > 0) {
			if (path[l] != ~0)
				RemoveRef(path[l]);
			next = GetNext();
			groups[next] = group;
			map.insert({ group, next });
		}
		else {
			map.erase(groups[path[l]]);
			groups[path[l]] = group;
			next = path[l];
			map.insert({ group, next });
		}
	}
}

Peio::Vxl::MaterialMap::Group* Peio::Vxl::MaterialMap::GetGroups()
{
	return &groups[0];
}

Peio::uint Peio::Vxl::MaterialMap::GetNumGroups()
{
	return groups.size();
}

Peio::Vxl::MaterialMap::Ray Peio::Vxl::MaterialMap::Trace(Double3 origin, Double3 ray, Uint3 skip)
{
    const Double3 invRay = { 1.0 / ray.x(), 1.0 / ray.y(), 1.0 / ray.z() };

    Ray result;
    result.voxel = {};
    result.side = ~0;
    result.normal = {};
    result.material = ~0;

    double curScale = 0.0;

    { // Trace to the root cube
        double rad = (double)(1U << numLayers) / 2.0;
        Double3 mid = Double3(rad, rad, rad) - origin;
        Double3 invRad = { abs(invRay.x() * rad), abs(invRay.y() * rad), abs(invRay.z() * rad) };
        Double3 minDiv = mid * invRay - invRad;
        Double3 maxDiv = mid * invRay + invRad;

        //double minScale = max(max(max(minDiv.x, minDiv.y), minDiv.z), 0.0);
        double minScale = 0.0;
        for (uint i = 0; i < 3; i++) {
            if (minDiv[i] > minScale) {
                minScale = minDiv[i];
                result.side = i;
            }
        }

        double maxScale = std::min(std::min(maxDiv.x(), maxDiv.y()), maxDiv.z());

        if (minScale > maxScale) {
            result.side = ~0;
            return result;
        }
        curScale = minScale;
    }

    Uint3 path = {};
    uint curLayer = 0;
    std::vector<uint> indices(numLayers);
    uint nextIndex = 0;
    bool down = true;

    while (true) {
        uint mask = 1U << (numLayers - curLayer - 1);
        double rad = (double)((size_t)1 << (numLayers - curLayer)) / 2.0;
        Double3 mid = (Double3)(path & Uint3(~mask, ~mask, ~mask)) + Double3(rad, rad, rad);
    
        if (down) {
            if (curLayer == numLayers) {
                if (path == skip) {
                    curLayer--;
                    down = false;
                    continue;
                }
                result.voxel = path;
                result.material = nextIndex;
                break;
            }
    
            Double3 curPos = origin + ray * curScale;
            indices[curLayer] = nextIndex;
            for (uint i = 0; i < 3; i++) {
                if (curPos[i] >= mid[i])
                    path[i] |= mask;
                else
                    path[i] &= ~mask;
            }
        }
        else {
            uint axis = -1;
            double bestScale = std::numeric_limits<double>::max();
            for (uint i = 0; i < 3; i++) {
                double s = (mid[i] - origin[i]) * invRay[i];
                if (s <= curScale) // Might cause errors because of double inaccuracy (?)
                    continue;
                if (s < bestScale) {
                    axis = i;
                    bestScale = s;
                }
            }
    
            bool inside = true;
            for (uint i = 0; i < 3; i++) {
                if (abs((origin[i] + ray[i] * bestScale) - mid[i]) > rad) {
                    inside = false;
                    break;
                }
            }
    
            if (axis == -1 || !inside) {
                if (curLayer == 0)
                    break;
                for (uint i = 0; i < 3; i++)
                    path &= ~mask;
                curLayer--;
                continue;
            }
            result.side = axis;
            curScale = bestScale;
            path[axis] ^= mask;
        }
        nextIndex = groups[indices[curLayer]].indices[(bool)(path.x() & mask)]
            [(bool)(path.y() & mask)][(bool)(path.z() & mask)];
        if (nextIndex == -1) {
            down = false;
            continue;
        }
        else {
            down = true;
            curLayer++;
        }
    }
    if (result.material != ~0) {
        result.normal = ray;
        result.normal[result.side] = -result.normal[result.side];
        result.collision = origin + ray * curScale;
    }
    return result;
}
