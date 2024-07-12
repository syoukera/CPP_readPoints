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

    vector<DataPoint> pointsData, uData;
    vector<vector<DataPoint>> gridPoints(GridSize, vector<DataPoint>(GridSize, {0.0f, 0.0f, 0.0f}));
    vector<vector<DataPoint>> gridU(GridSize, vector<DataPoint>(GridSize, {0.0f, 0.0f, 0.0f}));

    // "points"ファイルからデータを読み込む
    if (!readDataFromFile("data/points_4", pointsData)) {
        return 1;
    }

    // "U"ファイルからデータを読み込む
    if (!readDataFromFile("data/U_4", uData)) {
        return 1;
    }

    // 格子点のXYZ座標を計算
    for (int i = 0; i < GridSize; ++i) {
        for (int j = 0; j < GridSize; ++j) {
            // 格子点の座標を計算
            double gridX = i * gridSpacing;
            double gridY = j * gridSpacing;
            double gridZ = 0.0;

            // convert range from [0, 1] to [-0.0005, 0.0005]
            gridX = (gridX - 0.5)*0.001;
            gridY = (gridY - 0.5)*0.001;
            gridZ = (gridZ - 0.5)*0.001;

            DataPoint point = {gridX, gridY, gridZ};

            // 格子点に最も近いデータ点の値を代入
            gridPoints[i][j] = point;
        }
    }
    
    // バイリニア補間を実行する
    interpolateBilinear(pointsData, uData, gridPoints, gridU);

    cout << pointsData[0].x << " " << pointsData[0].y << " " << pointsData[0].z << endl;
    cout << pointsData[1].x << " " << pointsData[1].y << " " << pointsData[1].z << endl;
    cout << pointsData[2].x << " " << pointsData[2].y << " " << pointsData[2].z << endl;
    cout << pointsData[3].x << " " << pointsData[3].y << " " << pointsData[3].z << endl;
    cout << pointsData[4].x << " " << pointsData[4].y << " " << pointsData[4].z << endl;

    // // 最も近い点の値を補間する
    // interpolateNearestNeighbor(pointsData, uData, gridPoints, gridU);

    // // gridPointsをファイルに書き込む
    // if (!writeDataToFile("output/X_interpolated", gridPoints, numPoints, GridSize)) {
    //     return 1;
    // }

    // // gridUをファイルに書き込む
    // if (!writeDataToFile("output/U_interpolated", gridU, numPoints, GridSize)) {
    //     return 1;
    // }
    
    return 0;
}
