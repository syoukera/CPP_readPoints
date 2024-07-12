#ifndef WRITEDATA_H
#define WRITEDATA_H

#include <vector>
#include <string>
#include "data_point.h"

bool writeDataToFile(const std::string& filename, std::vector<std::vector<DataPoint>>& gridData, int numPoints, int GridSize);
#endif // WRITEDATA_H