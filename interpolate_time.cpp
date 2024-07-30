#include <iostream>
#include <fstream>
#include <vector>
#include "data_point.h"
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
    
    // 読み込む流速データのパス
    string lowerReferenceUFilename = "/NAS/18/NH3_HiTAC/fuel_Xinterp/" + lowerReference + "/U";
    string upperReferenceUFilename = "/NAS/18/NH3_HiTAC/fuel_Xinterp/" + upperReference + "/U";
    
    // 読み込む流速データのvectorを定義する
    vector<DataPoint> lowerReferenceUData, upperReferenceUData;

    // "U"ファイルからデータを読み込む
    if (!readDataFromFile(lowerReferenceUFilename, lowerReferenceUData)) {
        cerr << "データを読み込めませんでした: " << lowerReferenceUFilename << endl;
        return 1;
    }
    if (!readDataFromFile(upperReferenceUFilename, upperReferenceUData)) {
        cerr << "データを読み込めませんでした: " << upperReferenceUFilename << endl;
        return 1;
    }

    for (size_t i = 0; i < lowerReferenceUData.size(); ++i) {
        cout << lowerReferenceUData[i].x << " " << upperReferenceUData[i].x << endl;
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
        // cout << interpTime << endl;

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

                // cout << lowerReference << " " << interpTime << " " << upperReference << endl;

                if (!interpolateUbyTime(interpTime, lowerReference, upperReference)) {
                    cout << "Timedata: " << interpTime << " was interpolated" << endl;
                } else {
                    cerr << "Timedata: " << interpTime << " can't be interpolated" << endl;
                }
                break;
            }
        }

    // for development only
    // break;

    }
}