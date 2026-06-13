#include "tuner.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "zobrist.h"
#include "board.h"
#include "evaluation.h"
#include "tuner.h"
using namespace std;
using namespace Tuner;

int main(int argc, char** argv) {
	Zobrist::initialize_keys();
    vector<DataSource> sources;
    {
        string csv_path = "sources.csv";
        if (argc > 1)
        {
            csv_path = argv[1];
        }

        cout << "Current working directory: " << filesystem::current_path().string() << endl;

        ifstream csv(csv_path);
        if(!csv)
        {
            cout << "Unable to open data source list " << csv_path << endl;
            return -1;
        }

        while(!csv.eof())
        {
            string line;
            getline(csv, line);

            if(line.empty() || line.starts_with('#'))
            {
                continue;
            }

            DataSource source;
            stringstream ss(line);
            if(!getline(ss, source.path, ','))
            {
                cout << "CSV misformatted" << endl;
                return -1;
            }

            string flipped_wdl_str;
            if (!getline(ss, flipped_wdl_str, ','))
            {
                cout << "CSV misformatted" << endl;
                return -1;
            }
            try
            {
                source.side_to_move_wdl = stoul(flipped_wdl_str);
            }
            catch (const std::invalid_argument&)
            {
                cout << flipped_wdl_str << " is not valid for a WDL flip flag";
                return -1;
            }

            string position_limit_str;
            if (!getline(ss, flipped_wdl_str, ','))
            {
                cout << "CSV misformatted" << endl;
                return -1;
            }
            try
            {
                source.position_limit = stoll(flipped_wdl_str);
            }
            catch (const std::invalid_argument&)
            {
                cout << position_limit_str << " is not a valid position limit";
                return -1;
            }

            sources.push_back(source);
        }
    }

    if(sources.empty())
    {
        cout << "Data source list is empty";
        return -1;
    }

    run(sources);

    return 0;
}