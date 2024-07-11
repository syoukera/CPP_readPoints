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

// 最も近い点の値を補間する関数
void interpolateNearestNeighbor(const vector<DataPoint>& points, const vector<DataPoint>& uData, double gridSpacing, vector<vector<DataPoint>>& gridU, int GridSize) {
    for (int i = 0; i < GridSize; ++i) {
        for (int j = 0; j < GridSize; ++j) {
            // 格子点の座標を計算
            double gridX = i * gridSpacing;
            double gridY = j * gridSpacing;

            // 最も近いデータ点を見つける
            double minDist = numeric_limits<double>::max();
            DataPoint nearestU = {0.0f, 0.0f, 0.0f}; // デフォルトの初期値

            for (size_t k = 0; k < points.size(); ++k) {
                double dist = sqrt(pow(points[k].x - gridX, 2) + pow(points[k].y - gridY, 2));
                if (dist < minDist) {
                    minDist = dist;
                    nearestU = uData[k]; // pointsと同じインデックスのuDataの値を取得
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

            DataPoint point = {gridX, gridY, gridZ};

            // 格子点に最も近いデータ点の値を代入
            gridPoints[i][j] = point;
        }
    }

    // 最も近い点の値を補間する
    interpolateNearestNeighbor(pointsData, uData, gridSpacing, gridU, GridSize);

    // "points"ファイルからデータを読み込む
    if (!writeDataToFile("output/U_interpolated", gridU, numPoints, GridSize)) {
        return 1;
    }
    
    return 0;
}
