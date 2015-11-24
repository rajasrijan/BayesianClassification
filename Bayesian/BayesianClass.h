#pragma once

#include <iostream>
#include <stdint.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <map>
#include "dirent.h"
#include <fstream>
#include <sstream>

using namespace std;

string const blacklist = "after,alt,anybody,anymore,anything,ask,go,able,about,ah,again,as,are,also,and,an,am,be,the,than,thats,yours,or,maybe,if,else,then,because";

class BayesianClass
{
private:
	map<string,int> wi;
	int filterString(char* ptr,int size);
	void calculateHistogram(string file_path);
	int fileCount;
	double word_count;
public:
	string className;
	BayesianClass(string name,string path);
	BayesianClass(string name);
	~BayesianClass(void);
	void serialize();
	void deserialize(string name);
	double p(map<string,int> t);
};

