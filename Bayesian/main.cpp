#include <iostream>
#include <stdint.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <map>
#include "dirent.h"
#include "BayesianClass.h"
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

vector<BayesianClass> classes;

void learn(string root_path)
{
	DIR *dir;
	struct dirent *ent;
	//string root_path(argv[2]);
	if ((dir = opendir (root_path.c_str())) != NULL)
	{
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) 
		{
			//printf ("%s\n", ent->d_name);
			string directory_name(ent->d_name);
			if((directory_name==".")||(directory_name==".."))
				continue;
			string class_path = root_path + "\\" + directory_name;
			classes.push_back(BayesianClass(directory_name,class_path));
			classes.back().serialize();
		}
		closedir (dir);
	}
	else
	{
		/* could not open directory */
		perror ("");
		return;
	}
}
void test(string root_path)
{
	DIR *dir;
	struct dirent *ent;
	//string root_path(argv[2]);
	if ((dir = opendir (root_path.c_str())) != NULL)
	{
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) 
		{
			//printf ("%s\n", ent->d_name);
			string directory_name(ent->d_name);
			if((directory_name==".")||(directory_name==".."))
				continue;
			int x = directory_name.find(".class");
			if (x>=0)
			{
				string class_path = root_path + "\\" + directory_name;
				classes.push_back(BayesianClass(directory_name));
				classes.back().deserialize(class_path);
			}

		}
		closedir (dir);
	}
	else
	{
		/* could not open directory */
		perror ("");
		return;
	}
}
map<string,int> wi;
int filterString(char* ptr,int size)
{
	int non_alpha_count=0;
	for(int i=0;(i<size) && (ptr[i]) ;i++)
	{
		if(isascii((uint8_t)ptr[i]))
			if (isalpha((uint8_t)ptr[i]))
			{
				ptr[i]=tolower(ptr[i]);
			}
			else
			{
				ptr[i]=' ';
				non_alpha_count++;
				if (non_alpha_count>1)
				{
					return 1;
				}
			}
		else
			return 1;
	}
	return 0;
}

void calculateHistogram(string file_path)
{
	fstream file(file_path,ios_base::in);
	if (!file.is_open())
		exit(errno);
	char word[256]={0};
	while (!file.eof())
	{
		char line[1024]={0};

		file.getline(line,1024);
		if(line[0]==0)
			break;
	}

	while (!file.eof())
	{
		int res=0;
		file>>word;
		res = filterString(word,256);
		if(res)
			continue;
		stringstream ss;
		ss<<word;

		while (!ss.eof())
		{
			ss>>word;
			if(word[0] && word[1])
			{
				if(blacklist.find(word)==-1)
					wi[word]++;
			}
		}
	}
	file.close();
}

void printHelp()
{
	cout<<"Bayesian"<<" <option> \"dataset path\"";
	cout<<"\noptions:\n";
	cout<<"\t-l\tLearn from the given dataset.\n";
	cout<<"\t-t\tClassify test document.\n";
	exit(0);
}

int main(int argc,char* argv[])
{
	if (argc<3)
	{
		printHelp();
	}
	if (!strcmp(argv[1],"-L"))
	{
		learn(argv[2]);
	}
	else if (!strcmp(argv[1],"-t"))
	{
		test(".");
		calculateHistogram(argv[2]);
		for (int i = 0; i < classes.size(); i++)
		{
			cout<<classes[i].className<<":"<<classes[i].p(wi)<<endl;
		}
	}
	else
		printHelp();
}
