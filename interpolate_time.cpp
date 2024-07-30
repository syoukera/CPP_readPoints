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

int interpolateUbyTime(double interpTime, string& lowerReference, string& upperReference) {
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
    }
}