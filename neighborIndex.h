#ifndef NEIGHBORINDEX_H
#define NEIGHBORINDEX_H

#include <vector>
#include "data_point.h"
#include "index_point.h"

void findNeighborIndexPoint(
                        double gridX, double gridY, 
                        const std::vector<DataPoint>& points, 
                        IndexPoint& neighborIndex, 
                        std::vector<bool>& foundU
                        );

void findNeighborIndexGrid(
                        std::vector<std::vector<DataPoint>>& gridPoints, 
                        const std::vector<DataPoint>& points,
                        std::vector<std::vector<IndexPoint>>& neighborIndex, 
                        std::vector<std::vector<std::vector<bool>>>& foundU);
 
#endif // NEIGHBORINDEX_H