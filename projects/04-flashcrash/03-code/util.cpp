//
//  util.cpp
//  flash
//
//  Created by Jose Luis Rodriguez on 11/15/17.
//  Copyright © 2017 Jose Luis Rodriguez. All rights reserved.
//

// icpc -qopenmp -qopt-report util.cpp
// install_name_tool -change @rpath/libiomp5.dylib /opt/intel/compilers_and_libraries_2018.1.126/mac/compiler/lib/libiomp5.dylib a.out

#include "util.hpp"
#include <iostream> // std::cout
#include <fstream>  // std::ifstream
#include <unordered_set>
#include <map>

using namespace std;

void help()
{
    fprintf(stderr,"nbody3 --help|-h --nparticles|-n --nsteps|-s --stepsize|-t\n");
}


void tagSearch(const vector<string> &data,
               vector<string> &search,
               string fixtag, size_t nsize) {
    
    char *buf = new char[fixtag.length() + 1];
    strcpy(buf, fixtag.c_str());
    
#pragma omp parallel
    {
        vector<string> vec_private;
#pragma omp for nowait
        for(int i=0; i<nsize; i++) {
            string smatch = data[i].substr(data[i].find(buf)+4,8);
            vec_private.push_back(smatch);
        }
#pragma omp critical
        search.insert(search.end(), vec_private.begin(), vec_private.end());
    }
}


void dateVolume(vector<string> &data, map<string, int> &count_dates, size_t nsize){
#pragma omp parallel for default(shared)
    for (int i = 0; i < nsize; ++i)
    {
    #pragma omp atomic update
        count_dates[data[i]]+=1;
    }
}


int main (int argc, char* argv[])
{
    
    /*
    if (argc < 2)
    {
        std::cerr << "filename not specified\n";
        return 1;
    }
        
     **/
    
    string path = "/Users/jlroo/cme/data/2010/XCME";
    //string path = "/Users/jlroo/cme/data/2010/XCME_MD_ES_20100104_20100108";
    string fixtag;
    const char * tag_start = "\x01";
    const char * tag_end = "=";
    fixtag = "52";

    vector<string> data;
    vector<string> search;
    map<string, int> volume;
    
    read_fix(path, data);
    size_t n = data.size();
    fixtag = tag_start + fixtag + tag_end;
    
    tagSearch(data, search, fixtag, n);
    dateVolume(search, volume, n);
    
    int wk_volume = 0;
    std::cout << "date, volume\n" <<std::endl;
    for (const auto &p : volume) {
        //std::cout << "volume[" << p.first << "] = " << p.second << '\n';
        std::cout << "volume[" << p.first << "] = " << p.second << '\n';
        wk_volume +=p.second;
    }
    
    std::cout << wk_volume <<std::endl;
    
    //int count = 0;
    //Counter(search,count);
    
    /**
    
     for(std::vector<string>::iterator it = search.begin(); it != search.end(); ++it)
     std::cout << *it <<std::endl;
     
     
    //cout<<fixtag<<endl;
    //cout<<n<<endl;
    //cout<<search[0]<<std::endl;
    //cout<<data[10000]<<std::endl;
    **/
    
    return 0;
}
