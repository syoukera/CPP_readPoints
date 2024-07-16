#ifndef INTERPOLATE_H
#define INTERPOLATE_H

#include <vector>
#include "data_point.h"

// void interpolateBilinear(const std::vector<DataPoint>& points, const std::vector<DataPoint>& uData, float gridSpacing, std::vector<std::vector<DataPoint>>& gridU, int GridSize);

void findNeighborIndex(double gridX, double gridY, 
    const std::vector<DataPoint>& points, 
    std::vector<std::size_t>& neighborIndex,
    std::vector<bool>& foundU);

DataPoint interpolatePointBilinear(double gridX, double gridY,
    const std::vector<DataPoint>& points,
    const std::vector<DataPoint>& uData,
    std::vector<std::size_t>& neighborIndex);

DataPoint bilinearInterpolation(
    double gridX, double gridY, 
    const std::vector<DataPoint>& points, 
    const std::vector<DataPoint>& uData);

void interpolateBilinear(
    const std::vector<DataPoint>& points, 
    const std::vector<DataPoint>& uData, 
    std::vector<std::vector<DataPoint>>& gridPoints, 
    std::vector<std::vector<DataPoint>>& gridU);

void interpolateNearestNeighbor(const std::vector<DataPoint>& refPoints, 
const std::vector<DataPoint>& refU, 
std::vector<std::vector<DataPoint>>& gridPoints, 
std::vector<std::vector<DataPoint>>& gridU);

#endif // INTERPOLATE_H
