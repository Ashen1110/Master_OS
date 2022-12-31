#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <map>
#include <utility>
using namespace std;
string typelist[6] = {"binary_semphore", "mcs", "pthread_spin", "ticket", "only inuse", "pthread_mutex"};

string typeCheck(char* temp){
	for(int i = 0; i < 6;i++){
		if(!strcmp(temp, typelist[i]))
			return typelist[i];
	}
	return "";
}


int main(int argc, char *argv[]){
	
	if(argc < 1){
		printf("usage: ./codeGen <compareTarget>\n");
		printf("Target include: user_num, nCS_size, rs_size\n");
		return 0;
	}
	target = argv[1];
	if(!target.compare("user_num") || !target.compare("nCS_size") || !target.compare("rs_size\n")){
		perror("argument error");
		return 0;
	}
	FILE* fp = fopen("result.txt", "r");
	char* tempC = malloc(sizeof(char) * 256);
	map<pair<string, long long int>, double> data;
	pair<string, long long int> index;
	string title = "=======================", type = "", target = "";
	long long int index_value = 0, tempI = 0;

	while(fp){
		fscanf(fp, "%s", tempC);//=========
		if(!strcmp(tempC, title)){
			fscanf(fp, "%s", tempC);//type
			target = typeCheck(tempC);
			if(!target.compare("")){
				perror("typeCheck error");
				return 0;
			}
			fscanf(fp, "%s", tempC);//threads : 64
			fscanf(fp, "%s %lld", tempC, tempI);//user_num
			if(!target.compare("user_num"))
				index_value = tempI;
			fscanf(fp, "%s %lld", tempC, tempI);//nCS_size
			if(!target.compare("nCS_size"))
				index_value = tempI;
			fscanf(fp, "%s", tempC);//run_second : 10
			index = {target, index_value};
			fscanf(fp, "%s count/time: %lf", tempC, data[index]);
			printf("%lf catched\n", data[index]);
		}
	}
}
