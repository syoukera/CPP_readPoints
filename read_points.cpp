#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <limits>
using namespace std;

// データポイントの構造体
struct DataPoint {
    double x, y, z;
};

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
    for (int i = 0; i < num_points; ++i) {
        getline(infile, line); // 1行読み取り

        // sscanfで解析
        if (sscanf(line.c_str(), "(%le %le %le)", &x, &y, &z) == 3) {
            data.push_back({x, y, z});
        } else {
            cerr << "行 " << i+2 << " : 解析エラー" << endl; // 行数は1から始まるため、インデックスに+2を指定
        }
    }

    // ファイルを閉じる
    infile.close();
    return true;
}

// ファイルにデータを書き込む関数
bool writeDataToFile(const string& filename, vector<vector<DataPoint>>& gridData, int numPoints, int GridSize) {
    
    // 補間したデータをスペース区切りでテキストファイルに出力
    ofstream outfile(filename);
    if (!outfile) {
        cerr << "出力ファイルを開けませんでした。" << endl;
        return false;
    }

    outfile << endl;
    outfile << numPoints << endl;
    outfile << "(" << endl;
    
    for (int i = 0; i < GridSize; ++i) {
        for (int j = 0; j < GridSize; ++j) {
            outfile << "(" << gridData[i][j].x << " " << gridData[i][j].y << " " << gridData[i][j].z << ")" << endl; 
            // if (j < GridSize - 1) outfile << " "; // 各行の最後にスペースを追加しない
        }
    }

    outfile << ")" << endl;
    
    // ファイルを閉じる
    outfile.close();
    return true;
}

// バイリニア補間を行う関数
DataPoint bilinearInterpolation(double gridX, double gridY, const vector<DataPoint>& points, const vector<DataPoint>& uData) {

    // bool foundU1 = false, foundU2 = false, foundU3 = false, foundU4 = false;
    vector<bool> foundU(4, false);

    // DataPoint neighborU1 = {0, 0, 0};
    // DataPoint neighborU2 = {0, 0, 0};
    // DataPoint neighborU3 = {0, 0, 0};
    // DataPoint neighborU4 = {0, 0, 0};
    
    // double minDist1 = numeric_limits<double>::max();
    // double minDist2 = numeric_limits<double>::max();
    // double minDist3 = numeric_limits<double>::max();
    // double minDist4 = numeric_limits<double>::max();
    vector<double> minDist(4, numeric_limits<double>::max()); // 象限ごとの最小の距離
    vector<size_t> neighborIndex(4, 0); // 象限ごとの近傍の点のインデックス
    
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
                neighborIndex[i_quad] = i;
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
                neighborIndex[i_quad] = i;
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
                neighborIndex[i_quad] = i;
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
                neighborIndex[i_quad] = i;
                foundU[i_quad] = true;
            }
        }
    }


    // // 4つのデータ点を見つけるための変数
    // DataPoint Q11, Q21, Q12, Q22;
    // bool foundQ11 = false, foundQ21 = false, foundQ12 = false, foundQ22 = false;
    // double x1, y1, x2, y2;

    // // 周囲の4つのデータ点を見つける
    // for (size_t i = 0; i < points.size(); ++i) {
    //     if (points[i].x <= x && points[i].y <= y && (!foundQ11 || (points[i].x > x1 && points[i].y > y1))) {
    //         Q11 = uData[i];
    //         x1 = points[i].x;
    //         y1 = points[i].y;
    //         foundQ11 = true;
    //     }
    //     if (points[i].x >= x && points[i].y <= y && (!foundQ21 || (points[i].x < x2 && points[i].y > y1))) {
    //         Q21 = uData[i];
    //         x2 = points[i].x;
    //         y1 = points[i].y;
    //         foundQ21 = true;
    //     }
    //     if (points[i].x <= x && points[i].y >= y && (!foundQ12 || (points[i].x > x1 && points[i].y < y2))) {
    //         Q12 = uData[i];
    //         x1 = points[i].x;
    //         y2 = points[i].y;
    //         foundQ12 = true;
    //     }
    //     if (points[i].x >= x && points[i].y >= y && (!foundQ22 || (points[i].x < x2 && points[i].y < y2))) {
    //         Q22 = uData[i];
    //         x2 = points[i].x;
    //         y2 = points[i].y;
    //         foundQ22 = true;
    //     }
    // }
    
    // vector<DataPoint> neighborU(4, {0.0f, 0.0f, 0.0f});
    // vector<DataPoint> neighborX(4, {0.0f, 0.0f, 0.0f});

    // 象限のindexからバイリニアのインデックスが異なるので変換している
    // points バイリニア:象限, 1:2, 2:3, 3:1, 4:0
    // Q      バイリニア:象限, 11:2, 21:3, 12:1, 22:0

    DataPoint point1 = points[neighborIndex[2]];
    DataPoint point2 = points[neighborIndex[3]];
    DataPoint point3 = points[neighborIndex[1]];
    DataPoint point4 = points[neighborIndex[0]];
    
    DataPoint Q11 = uData[neighborIndex[2]];
    DataPoint Q21 = uData[neighborIndex[3]];
    DataPoint Q12 = uData[neighborIndex[1]];
    DataPoint Q22 = uData[neighborIndex[0]];

    // 4つのデータ点が見つからない場合のデフォルト値
    if (!foundU[2]) Q11 = {0, 0, 0};
    if (!foundU[3]) Q21 = {0, 0, 0};
    if (!foundU[1]) Q12 = {0, 0, 0};
    if (!foundU[0]) Q22 = {0, 0, 0};

    // バイリニア補間の計算
    double denom = (point2.x - point1.x) * (point4.y - point1.y);
    if (denom == 0) {
        return {0, 0, 0}; // 分母がゼロの場合はデフォルト値を返す
    }

    DataPoint result;
    // result.x = (Q11.x * (point2.x - gridX) * (point4.y - gridY) + Q21.x * (gridX - point1.x) * (point4.y - gridY) +
    //             Q12.x * (point2.x - gridX) * (gridY - point1.y) + Q22.x * (gridX - point1.x) * (gridY - point1.y)) / denom;
    // result.y = (Q11.y * (point2.x - gridX) * (point4.y - gridY) + Q21.y * (gridX - point1.x) * (point4.y - gridY) +
    //             Q12.y * (point2.x - gridX) * (gridY - point1.y) + Q22.y * (gridX - point1.x) * (gridY - point1.y)) / denom;
    // result.z = (Q11.z * (point2.x - gridX) * (point4.y - gridY) + Q21.z * (gridX - point1.x) * (point4.y - gridY) +
    //             Q12.z * (point2.x - gridX) * (gridY - point1.y) + Q22.z * (gridX - point1.x) * (gridY - point1.y)) / denom;

    result.x = (Q11.x * (point2.x - gridX) * (point4.y - gridY) + 
                Q21.x * (gridX - point1.x) * (point4.y - gridY) + 
                Q12.x * (point2.x - gridX) * (gridY - point1.y) + 
                Q22.x * (gridX - point1.x) * (gridY - point1.y)) / denom;

    result.y = (Q11.y * (point2.x - gridX) * (point4.y - gridY) + 
                Q21.y * (gridX - point1.x) * (point4.y - gridY) + 
                Q12.y * (point2.x - gridX) * (gridY - point1.y) + 
                Q22.y * (gridX - point1.x) * (gridY - point1.y)) / denom;

    result.z = (Q11.z * (point2.x - gridX) * (point4.y - gridY) + 
                Q21.z * (gridX - point1.x) * (point4.y - gridY) + 
                Q12.z * (point2.x - gridX) * (gridY - point1.y) + 
                Q22.z * (gridX - point1.x) * (gridY - point1.y)) / denom;

    return result;
}

// 最も近い点の値を補間する関数
void interpolateBilinear(const vector<DataPoint>& points, const vector<DataPoint>& uData, vector<vector<DataPoint>>& gridPoints, vector<vector<DataPoint>>& gridU) {

    for (size_t i = 0; i < gridPoints.size(); ++i) {
        for (size_t j = 0; j < gridPoints[i].size(); ++j) {
            // 格子点の座標を計算
            double gridX = gridPoints[i][j].x;
            double gridY = gridPoints[i][j].y;

            // バイリニア補間を実行
            gridU[i][j] = bilinearInterpolation(gridX, gridY, points, uData);
        }
    }
}

// 最も近い点の値を補間する関数
void interpolateNearestNeighbor(const vector<DataPoint>& refPoints, const vector<DataPoint>& refU, vector<vector<DataPoint>>& gridPoints, vector<vector<DataPoint>>& gridU) {
    
    for (size_t i = 0; i < gridPoints.size(); ++i) {
        for (size_t j = 0; j < gridPoints[i].size(); ++j) {

            // 格子点の座標を取得
            double gridX = gridPoints[i][j].x;
            double gridY = gridPoints[i][j].y;
            double gridZ = gridPoints[i][j].z;

            
            // 最も近いデータ点を見つける
            double minDist = numeric_limits<double>::max();
            DataPoint nearestU = {0.0f, 0.0f, 0.0f}; // デフォルトの初期値

            for (size_t k = 0; k < refPoints.size(); ++k) {
                double dist = sqrt(pow(refPoints[k].x - gridX, 2) + pow(refPoints[k].y - gridY, 2) + pow(refPoints[k].z - gridZ, 2));
                if (dist < minDist) {
                    minDist = dist;
                    nearestU = refU[k]; // pointsと同じインデックスのuDataの値を取得
                }
            }

            // 格子点に最も近いデータ点の値を代入
            gridU[i][j] = nearestU;
        }
    }
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
    if (!readDataFromFile("data/points", pointsData)) {
        return 1;
    }

    // "U"ファイルからデータを読み込む
    if (!readDataFromFile("data/U", uData)) {
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


    // // 最も近い点の値を補間する
    // interpolateNearestNeighbor(pointsData, uData, gridPoints, gridU);

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
