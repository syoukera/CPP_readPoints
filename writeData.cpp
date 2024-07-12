#include "writeData.h"
#include <fstream>
#include <iostream>

using namespace std;

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