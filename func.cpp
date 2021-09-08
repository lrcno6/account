#include<stdexcept>
#include<cctype>
#include"func.h"
using namespace std;
char first_not_space(const string &str){
	for(auto i:str)
		if(!isspace(i))
			return i;
	return 0;
}
vector<string> str2args(const string &cmd){
	vector<string> args;
	auto iter=cmd.cend();
	for(auto i=cmd.cbegin();i<cmd.cend();i++){
		if(iter==cmd.cend()){
			if(!isspace(*i))
				iter=i;
		}
		else if(*iter!='"' && *iter!='\''){
			if(isspace(*i)){
				args.push_back(string(iter,i));
				iter=cmd.cend();
			}
		}
		else{
			if(*i==*iter && iter+1<i){
				args.push_back(string(iter+1,i));
				iter=cmd.cend();
			}
		}
	}
	if(iter!=cmd.cend()){
		if(*iter!='"' && *iter!='\'')
			args.push_back(string(iter,cmd.cend()));
		else
			throw invalid_argument(*iter=='"'?"error: '\"' is expected":"error: \"'\" is expected");
	}
	return args;
}