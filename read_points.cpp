#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <limits>
#include "data_point.h"
#include "writeData.h"
#include "interpolate.h"
using namespace std;

// ファイルからデータを読み込む関数
bool readDataFromFile(const string& filename, vector<DataPoint>& data) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "ファイルを開けませんでした: " << filename << endl;
        return false;
    }

    int num_points;
    string line;
    double x, y, z;

    // 最初の行をスキップ（ファイルフォーマットによる）
    getline(infile, line);

    // ファイルからデータを読み取り
    infile >> num_points; // 最初の数値はデータの数として処理
    infile.ignore(); // 改行を読み飛ばす

    // 各行を読み取り、sscanfで解析

    while (getline(infile, line)) {

        // sscanfで解析
        if (sscanf(line.c_str(), "(%le %le %le)", &x, &y, &z) == 3) {
            data.push_back({x, y, z});
        } else {
            // cerr << " : 解析エラー" << endl;
        }
    }

    // ファイルを閉じる
    infile.close();
    return true;
}

int main() {
    // グリッドの間隔
    const double gridSpacing = 1.0 / 100.0;
    const int GridSize = 101;
    const int numPoints = GridSize*GridSize;
    const double inletRadious = 5.0e-4; // radious of inlet tube (0.5 mm)

    vector<DataPoint> pointsData, uData;
    vector<vector<DataPoint>> gridPoints(GridSize, vector<DataPoint>(GridSize, {0.0f, 0.0f, 0.0f}));
    vector<vector<DataPoint>> gridU(GridSize, vector<DataPoint>(GridSize, {0.0f, 0.0f, 0.0f}));

    // "points"ファイルからデータを読み込む
    if (!readDataFromFile("data/points", pointsData)) {
        return 1;
    }

    // "U"ファイルからデータを読み込む
    if (!readDataFromFile("data/U", uData)) {
        return 1;
    }

    // 円周上の点をpointDataに追加する
    const int numPointsCircle = 100;
    const double circleRadious = inletRadious + 1.0e-5; // 0.01 mm outside for inlet
    
    // cout << "angle, gridX, gridY" << endl;
    for (int i = 0; i < numPointsCircle; ++i) {

        double angle = 2.0*M_PI*i/(numPointsCircle); 
        double gridX = circleRadious*cos(angle);
        double gridY = circleRadious*sin(angle);

        // cout << angle << " " << gridX << " " << gridY << endl;

        // 円周上の座標をpointsDataに追加
        DataPoint circlePoint = {gridX, gridY, 0.001};
        pointsData.push_back(circlePoint);

        // 円周上の流速(0)をuDataに追加
        DataPoint circleU = {0.0, 0.0, 0.0};
        uData.push_back(circleU);
    }

    // 格子点のXYZ座標を計算し，バイリニア補間を実行
    for (int i = 0; i < GridSize; ++i) {
        for (int j = 0; j < GridSize; ++j) {
            // 格子点の座標を計算
            double gridX = i * gridSpacing;
            double gridY = j * gridSpacing;
            double gridZ = 0.0;

            // convert range from [0, 1] to [-0.0005, 0.0005]
            gridX = (gridX - 0.5)*0.001;
            gridY = (gridY - 0.5)*0.001;
            // gridZ = (gridZ - 0.5)*0.001;

            // 格子点の座標を2次元のvecterに保存
            DataPoint point = {gridX, gridY, gridZ};
            gridPoints[i][j] = point;

            // calclate neighbor index
            vector<size_t> neigbhorIndex(4, 0);
            vector<bool> foundU(4, false);
            findNeighborIndex(gridX, gridY, pointsData, neigbhorIndex, foundU);
    
            // calculate interpolatedU
            gridU[i][j] = interpolatePointBilinear(gridX, gridY, pointsData, uData, neigbhorIndex, foundU, inletRadious);
        }
    }

    // // gridPointsをファイルに書き込む
    // if (!writeDataToFile("output/X_interpolated", gridPoints, numPoints, GridSize)) {
    //     return 1;
    // }

    // gridUをファイルに書き込む
    if (!writeDataToFile("output/U_interpolated", gridU, numPoints, GridSize)) {
        return 1;
    }
    
    return 0;
}
