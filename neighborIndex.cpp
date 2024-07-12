#include "neighborIndex.h"
#include <vector>
#include <cmath>
#include <limits>
#include "index_point.h"

using namespace std;

// 特定のポイントについて近傍のインデックスを取得する
void findNeighborIndexPoint(double gridX, double gridY, 
                            const vector<DataPoint>& points,
                            IndexPoint& neighborIndex, 
                            vector<bool>& foundU
                            ) {
    
    vector<double> minDist(4, numeric_limits<double>::max()); // 象限ごとの最小の距離
    // IndexPoint neighborIndex = {0, 0, 0, 0}; // 象限ごとの近傍の点のインデックス
    // vector<bool> foundU(4, false);
    
    for (size_t i = 0; i < points.size(); ++i) {
        
        double refX = points[i].x;
        double refY = points[i].y;

        if (refX >= gridX && refY >= gridY) {
            // 第1象限
            size_t i_quad = 1;
        
            // グリッドとレファレンスの点の距離を計算
            double dist = sqrt(pow(refX - gridX, 2) + pow(refY - gridY, 2));

            // より近傍の点であるかを判定して，距離と流速を保存
            if (dist < minDist[i_quad]) {
                minDist[i_quad] = dist;
                neighborIndex.i = i;
                foundU[i_quad] = true;
            }
        } else if (refX < gridX && refY >= gridY) {
            // 第2象限
            size_t i_quad = 2;
        
            // グリッドとレファレンスの点の距離を計算
            double dist = sqrt(pow(refX - gridX, 2) + pow(refY - gridY, 2));

            // より近傍の点であるかを判定して，距離と流速を保存
            if (dist < minDist[i_quad]) {
                minDist[i_quad] = dist;
                neighborIndex.j = i;
                foundU[i_quad] = true;
            }
        } else if (refX < gridX && refY < gridY) {
            // 第3象限
            size_t i_quad = 3;
        
            // グリッドとレファレンスの点の距離を計算
            double dist = sqrt(pow(refX - gridX, 2) + pow(refY - gridY, 2));

            // より近傍の点であるかを判定して，距離と流速を保存
            if (dist < minDist[i_quad]) {
                minDist[i_quad] = dist;
                neighborIndex.k = i;
                foundU[i_quad] = true;
            }
        } else if (refX >= gridX && refY < gridY) {
            // 第4象限
            size_t i_quad = 4;
        
            // グリッドとレファレンスの点の距離を計算
            double dist = sqrt(pow(refX - gridX, 2) + pow(refY - gridY, 2));

            // より近傍の点であるかを判定して，距離と流速を保存
            if (dist < minDist[i_quad]) {
                minDist[i_quad] = dist;
                neighborIndex.l = i;
                foundU[i_quad] = true;
            }
        }
    }
}


// グリッドにわたって近傍のインデックスを取得する
void findNeighborIndexGrid(
                        std::vector<std::vector<DataPoint>>& gridPoints, 
                        const std::vector<DataPoint>& points,
                        std::vector<std::vector<IndexPoint>>& neighborIndex, 
                        std::vector<std::vector<std::vector<bool>>>& foundU) {

    for (size_t i = 0; i < gridPoints.size(); ++i) {
        for (size_t j = 0; j < gridPoints[i].size(); ++j) {
            // 格子点の座標を計算
            double gridX = gridPoints[i][j].x;
            double gridY = gridPoints[i][j].y;

            // バイリニア補間を実行
            findNeighborIndexPoint(gridX, gridY, neighborIndex[i][j], foundU[i][j]);
        }
    }
}