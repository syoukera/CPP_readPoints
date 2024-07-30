#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

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

    for (int i = 0; i < numTimes; ++i) {

        double time = double(i)*deltaTime;
        listTime.push_back(time);
    }

    // for (int i = 0; i < listTime.size(); ++i) {
    //     cout << listTime[i] << endl;
    // }


}