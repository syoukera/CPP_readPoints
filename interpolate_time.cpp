#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <filesystem>
#include "data_point.h"
#include "writeData.h"
using namespace std;

// ファイルからデータを読み込む関数
bool readDataFromFile(const string& filename, vector<vector<DataPoint>>& gridData, int GridSize) {
    
    ifstream infile(filename);
    if (!infile) {
        cerr << "ファイルを開けませんでした: " << filename << endl;
        return false;
    }

    int num_points;
    string line;
    double x, y, z;
    vector<DataPoint> data;

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

    // assign in grid data
    for (int i = 0; i < GridSize; ++i) {
        for (int j = 0; j < GridSize; ++j) {

            gridData[i][j] = data[i*GridSize + j];
            // outfile << "(" << gridData[i][j].x << " " << gridData[i][j].y << " " << gridData[i][j].z << ")" << endl; 
            // if (j < GridSize - 1) outfile << " "; // 各行の最後にスペースを追加しない
        }
    }

    return true;
}

// list_timesファイルに記載されたすべてのファイル名をベクターに読み込む関数
bool readListOfFiles(const string& listFilename, vector<string>& filenames) {
    ifstream listFile(listFilename);
    if (!listFile) {
        cerr << "ファイルを開けませんでした: " << listFilename << endl;
        return false;
    }

    string filename;
    while (getline(listFile, filename)) {
        filenames.push_back(filename);
    }

    listFile.close();
    return true;
}

int interpolateUbyTime(double interpTime, string& lowerReference, string& upperReference) {

    double lowerReferenceTime = stod(lowerReference);
    double upperReferenceTime = stod(upperReference);

    // 読み込む流速データのパス
    string lowerReferenceUFilename = "/NAS/18/NH3_HiTAC/fuel_Xinterp/" + lowerReference + "/U";
    string upperReferenceUFilename = "/NAS/18/NH3_HiTAC/fuel_Xinterp/" + upperReference + "/U";

    const int GridSize = 101;
    const int numPoints = GridSize*GridSize;

    // 読み込む流速データのvectorを初期化する
    vector<vector<DataPoint>> lowerReferenceUData(GridSize, vector<DataPoint>(GridSize, {0.0f, 0.0f, 0.0f}));
    vector<vector<DataPoint>> upperReferenceUData(GridSize, vector<DataPoint>(GridSize, {0.0f, 0.0f, 0.0f}));


    // "U"ファイルからデータを読み込む
    if (!readDataFromFile(lowerReferenceUFilename, lowerReferenceUData, GridSize)) {
        cerr << "データを読み込めませんでした: " << lowerReferenceUFilename << endl;
        return 1;
    }
    if (!readDataFromFile(upperReferenceUFilename, upperReferenceUData, GridSize)) {
        cerr << "データを読み込めませんでした: " << upperReferenceUFilename << endl;
        return 1;
    }

    // Define vector for interpolated Velocity
    vector<vector<DataPoint>> interpUGridData(GridSize, vector<DataPoint>(GridSize, {0.0f, 0.0f, 0.0f}));

    for (int i = 0; i < GridSize; ++i) {
        
        vector<DataPoint> interpUData;
        for (int j = 0; j < GridSize; ++j) {

            DataPoint interpU = {
                lowerReferenceUData[i][j].x + (interpTime - lowerReferenceTime)*(upperReferenceUData[i][j].x - lowerReferenceUData[i][j].x) / (upperReferenceTime - lowerReferenceTime),
                lowerReferenceUData[i][j].y + (interpTime - lowerReferenceTime)*(upperReferenceUData[i][j].y - lowerReferenceUData[i][j].y) / (upperReferenceTime - lowerReferenceTime),
                lowerReferenceUData[i][j].z + (interpTime - lowerReferenceTime)*(upperReferenceUData[i][j].z - lowerReferenceUData[i][j].z) / (upperReferenceTime - lowerReferenceTime)
            };
            interpUGridData[i][j] = interpU;
            // cout << lowerReferenceUData[i].x << " " << upperReferenceUData[i].x << endl;
        }
    }

    // set output file name
    char interpTimeString[10];  // maximum expected length of the float
    std::snprintf(interpTimeString, 10, "%.7f", interpTime);
    std::string str(interpTimeString);

    // cout << interpTimeString << endl;

    // 補間したデータを出力するフォルダのパス
    string outputUFoldername = "/NAS/18/NH3_HiTAC/fuel_tinterp/" + std::string(interpTimeString);
    if (!std::filesystem::create_directory(outputUFoldername)) {
        cerr << "フォルダを作成できませんでした: " << outputUFoldername << endl;
    }

    // 補間したデータをスペース区切りでテキストファイルに出力
    const string outputUFilename = outputUFoldername + "/U";
    cout << outputUFilename << endl;

    // gridUをファイルに書き込む
    if (!writeDataToFile(outputUFilename, interpUGridData, numPoints, GridSize)) {
        cerr << "データを読み込めませんでした: " << outputUFilename << endl;
        return 1;
    }    

    return 0;
}


int main() {

    string listFilename = "data/list_times";
    
    // "list_times" ファイルに記載されたすべてのファイル名をベクターに読み込む
    vector<string> filenames;
    if (!readListOfFiles(listFilename, filenames)) {
        return 1;
    }

    // for (int i = 0; i < filenames.size(); ++i) {
    //     cout << filenames[i] << endl;
    // }

    const double deltaTime = 1.0e-7; // seconds
    const double endTime   = 5.0e-2; // seconds
    const int numTimes     = int(endTime/deltaTime) + 1; // number of time steps

    vector<double> listTime;

    // define timesteps for interpolation
    for (int i = 0; i < numTimes; ++i) {

        double time = double(i)*deltaTime;
        listTime.push_back(time);
    }

    // for saving time on reference loop
    size_t referenceIndex = 0;

    // for loop for interpolation time step
    for (size_t i = 0; i < listTime.size(); ++i) {
        double interpTime = listTime[i];
        
        // std::ostringstream oss;
        // oss << std::setprecision(10) << interpTime;
        // std::string interpTimeString = oss.str();

        char interpTimeString[10];  // maximum expected length of the float
        std::snprintf(interpTimeString, 10, "%.7f", interpTime);
        std::string str(interpTimeString);

        // cout << interpTimeString << endl;

        // for loop for referenced time steps
        for (size_t j = referenceIndex; j < filenames.size(); ++j) {
            double referenceTime = stod(filenames[j]);
            // cout << referenceTime << endl;

            // check upperReference
            if (referenceTime > interpTime) {

                // save current reference index
                referenceIndex = j;

                // file name of time for linear interpolation
                string lowerReference = filenames[j-1];
                string upperReference = filenames[j];

                cout << lowerReference << " " << interpTime << " " << upperReference << endl;

                if (!interpolateUbyTime(interpTime, lowerReference, upperReference)) {
                    cout << "Timedata: " << interpTime << " was interpolated" << endl;
                } else {
                    cerr << "Timedata: " << interpTime << " can't be interpolated" << endl;
                }
                break;
            }
        }

    // for development only
    break;

    }
}