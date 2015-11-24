#include "BayesianClass.h"

map<string,int> global_wordlist;

BayesianClass::BayesianClass(string name):className(name)
{

}

BayesianClass::BayesianClass(string name,string path):className(name),fileCount(0)
{
	DIR *dir;
	struct dirent *ent;
	cout<<"Calculating "<<className<<endl;
	if ((dir = opendir (path.c_str())) != NULL)
	{
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL) 
		{
			struct _stat buf;
			string directory_name(ent->d_name);
			if((directory_name==".")||(directory_name==".."))
				continue;
			string class_path = path + "\\" + directory_name;
			int status = _stat(class_path.c_str(),&buf);
			if(status!=0)
				continue;
			if (!(buf.st_mode&S_IFREG))
				continue;
			calculateHistogram(class_path);
		}
		closedir (dir);
	}
	else
	{
		/* could not open directory */
		perror ("");
		exit(errno);
	}

}

BayesianClass::~BayesianClass(void)
{
}

int BayesianClass::filterString(char* ptr,int size)
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

void BayesianClass::calculateHistogram(string file_path)
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
	fileCount++;
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

void BayesianClass::serialize()
{
	fstream file(className+".class",ios_base::out);
	if (!file.is_open())
		exit(errno);
	file<<fileCount<<endl;
	for (auto i = wi.begin(); i != wi.end(); i++)
	{
		if(i->second>5)
			file<<i->first<<" "<<i->second<<endl;
	}
	file.close();
}

void BayesianClass::deserialize(string name)
{
	//className=name;
	className=className.substr(0,className.find(".class"));
	cout<<"loading "<<className<<endl;
	wi.clear();
	word_count=0;
	fstream file(name,ios_base::in);
	if (!file.is_open())
		exit(errno);
	file>>fileCount;
	while (!file.eof())
	{
		char key[1024]={0};
		int count=0;
		file>>key;
		file>>count;
		wi[key]=count;
		global_wordlist[key]+=count;
		word_count+=count;
	}
	file.close();
}

double BayesianClass::p(map<string,int> t)
{
	double ret=(1e256);
	double unique=global_wordlist.size();
	for (auto i = t.begin(); i != t.end(); i++)
	{
		if(i->second>5)
		{
			auto found1 = wi.find(i->first);
			auto found2 = global_wordlist.find(i->first);
			if (found1!=wi.end())
			{
				//auto found2 = global_wordlist.find(i->first);
				ret*=((double)found1->second/(double)found2->second);
			}
			else
				ret*=((double)(1)/((double)found2->second+unique));
		}
	}
	return ret;
}