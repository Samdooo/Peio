#define PEIO_VXL_EXPORTING
#include "MaterialMap.h"

Peio::Vxl::MaterialMap::Ray Peio::Vxl::MaterialMap::Trace(Double3 origin, Double3 ray)
{
    const Double3 invRay = Double3(1.0 / ray.x(), 1.0 / ray.y(), 1.0 / ray.z());

    Ray result;
    result.voxel = {};
    result.side = ~0;
    result.normal = {};
    result.material = ~0;

    double curScale = 0.0;
    const double minTotalScale = 0.001;

    { // Trace to the root cube
        double rad = (double)(1U << numLayers) / 2.0;
        Double3 mid = Double3(rad, rad, rad) - origin;
        Double3 invRad = invRay * rad;
        invRad = Double3(abs(invRad.x()), abs(invRad.y()), abs(invRad.z()));
        Double3 minDiv = mid * invRay - invRad;
        Double3 maxDiv = mid * invRay + invRad;
    
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
        double rad = (double)(1U << (numLayers - curLayer)) / 2.0;
        Double3 mid = (Double3)(path & Uint3(~mask, ~mask, ~mask)) + Double3(rad, rad, rad);
    
        if (down) {
            if (curLayer == numLayers) {
                if (curScale < minTotalScale) {
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
        nextIndex = nodes[indices[curLayer]].indices[(bool)(path.x() & mask)]
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
        result.collision = origin + (ray * curScale);
        result.collision[result.side] = (double)result.voxel[result.side];
        if (ray[result.side] < 0.0)
            result.collision[result.side] += 1.01f;
        else
            result.collision[result.side] -= 0.01f;
    }
    return result;
}

Peio::Vxl::MaterialMap::Ray Peio::Vxl::MaterialMap::Trace(Double3 origin, Double2 angle)
{
    Peio::Double3 ray = { 0.0, 0.0, 1.0 };
    ray = RotateX(ray, angle.y());
    ray = RotateY(ray, angle.x());
    return Trace(origin, ray);
}

Peio::Double3 Peio::Vxl::MaterialMap::RotateX(Peio::Double3 p, double angle)
{
    double c = cos(angle);
    double s = sin(angle);
    return Peio::Double3(p.x(), c * p.y() + s * p.z(), -s * p.y() + c * p.z());
}

Peio::Double3 Peio::Vxl::MaterialMap::RotateY(Peio::Double3 p, double angle)
{
    double c = cos(angle);
    double s = sin(angle);
    return Peio::Double3(c * p.x() - s * p.z(), p.y(), s * p.x() + c * p.z());
}
