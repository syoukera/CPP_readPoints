#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <limits>
using namespace std;

// データポイントの構造体
struct DataPoint {
    float x, y, z;
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
    float x, y, z;

    // 最初の行をスキップ（ファイルフォーマットによる）
    getline(infile, line);

    // ファイルからデータを読み取り
    infile >> num_points; // 最初の数値はデータの数として処理
    infile.ignore(); // 改行を読み飛ばす

    // 各行を読み取り、sscanfで解析
    for (int i = 0; i < num_points; ++i) {
        getline(infile, line); // 1行読み取り

        // sscanfで解析
        if (sscanf(line.c_str(), "(%f %f %f)", &x, &y, &z) == 3) {
            data.push_back({x, y, z});
        } else {
            cerr << "行 " << i+2 << " : 解析エラー" << endl; // 行数は1から始まるため、インデックスに+2を指定
        }
    }

    // ファイルを閉じる
    infile.close();
    return true;
}

// 最も近い点の値を補間する関数
void interpolateNearestNeighbor(const vector<DataPoint>& points, const vector<DataPoint>& uData, float gridSpacing, vector<vector<DataPoint>>& gridU, int GridSize) {
    for (int i = 0; i < GridSize; ++i) {
        for (int j = 0; j < GridSize; ++j) {
            // 格子点の座標を計算
            float gridX = i * gridSpacing;
            float gridY = j * gridSpacing;

            // 最も近いデータ点を見つける
            float minDist = numeric_limits<float>::max();
            DataPoint nearestU = {0.0f, 0.0f, 0.0f}; // デフォルトの初期値

            for (size_t k = 0; k < points.size(); ++k) {
                float dist = sqrt(pow(points[k].x - gridX, 2) + pow(points[k].y - gridY, 2));
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
    const float gridSpacing = 1.0 / 100.0;
    const int GridSize = 101;

    vector<DataPoint> pointsData, uData;
    vector<vector<DataPoint>> gridU(GridSize, vector<DataPoint>(GridSize, {0.0f, 0.0f, 0.0f}));

    // "points"ファイルからデータを読み込む
    if (!readDataFromFile("data/points", pointsData)) {
        return 1;
    }

    // "U"ファイルからデータを読み込む
    if (!readDataFromFile("data/U", uData)) {
        return 1;
    }

    // 最も近い点の値を補間する
    interpolateNearestNeighbor(pointsData, uData, gridSpacing, gridU, GridSize);

    // 補間したデータの出力（例として、gridUの値を出力）
    for (int i = 0; i < GridSize; ++i) {
        for (int j = 0; j < GridSize; ++j) {
            cout << "Grid Point (" << i * gridSpacing << ", " << j * gridSpacing << ") Value: ("
                 << gridU[i][j].x << ", " << gridU[i][j].y << ", " << gridU[i][j].z << ")" << endl;
        }
    }

    return 0;
}
