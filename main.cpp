#include<iostream>
#include<string>
#include<cstdio>
#include<cstdlib>
#include<cctype>
#include"MySQLClass/mysql_class.h"
#include"func.h"
using namespace mysql_class;

void table_menu(MySQL&,const Table&);
void new_table(MySQL&,const Table&);
void func_account(MySQL&,const std::string&);
void add(MySQL&,const std::string&,std::vector<std::string>);
void show(MySQL&,const std::string&,const std::vector<std::string>&);
void calc(MySQL&,const std::string&,const std::vector<std::string>&);
void help(MySQL&,const std::string&,const std::vector<std::string>&);

int main(int argc,char **argv){
	MySQL mysql,db;
	mysql.connect("localhost","root",nullptr,nullptr);
	mysql.query("show databases where `Database`='account';");
	if(mysql.get_result().size()==0)
		mysql.query("create database account;");
	db.connect("localhost","root",nullptr,"account");
	while(1){
		db.query("show tables;");
		auto tabs=db.get_result();
		if(tabs.size()==0)
			new_table(db,tabs);
		else
			table_menu(db,tabs);
	}
	return 0;
}
void table_menu(MySQL &db,const Table &tabs){
	auto size=tabs.size();
	for(size_t i=0;i<size;i++)
		printf("%zu. %s\n",i+1,tabs[i][0].c_str());
	puts("\n0. new account\nQ. exit");
	std::string tmp;
	while(1){
		printf("choose the account: ");
		std::getline(std::cin,tmp);
		switch(first_not_space(tmp)){
			case 'q':
			case 'Q':
				exit(0);
			case 0:
				continue;
		}
		size_t tab_num;
		sscanf(tmp.c_str(),"%zu",&tab_num);
		if(tab_num==0){
			new_table(db,tabs);
			break;
		}
		else if(tab_num<=size){
			func_account(db,tabs[tab_num-1][0]);
			break;
		}
		else
			fputs("error: number out of range\n",stderr);
	}
}
void new_table(MySQL &db,const Table &tabs){
	std::string tab_name;
	while(1){
		printf("enter the name of the account: ");
		std::getline(std::cin,tab_name);
		if(tab_name.size()==0 || isspace(tab_name[0])){
			fputs("error: invalid account name\n",stderr);
			continue;
		}
		for(const auto &i:tabs)
			if(tab_name==i[0]){
				fputs("warning: account already exist",stderr);
				func_account(db,tab_name);
				return;
			}
		db.query("create table `"+tab_name+"`(id bigint unsigned not null auto_increment,income bigint not null,`time` datetime not null,description varchar(16384) not null,primary key(id)) auto_increment=1 default charset=utf8");
		func_account(db,tab_name);
		break;
	}
}
void func_account(MySQL &db,const std::string &tab_name){
	puts("enter 'help' for help");
	std::string cmd;
	std::vector<std::string> args;
	while(1){
		printf("> ");
		std::getline(std::cin,cmd);
		try{
			args=str2args(cmd);
		}
		catch(std::invalid_argument &e){
			fputs(e.what(),stderr);
			continue;
		}
		if(args.size()){
			if(args[0]=="add")
				add(db,tab_name,args);
			else if(args[0]=="show")
				show(db,tab_name,args);
			else if(args[0]=="calc")
				calc(db,tab_name,args);
			else if(args[0]=="help")
				help(db,tab_name,args);
			else if(args[0]=="exit")
				break;
			else
				system(cmd.c_str());
		}
	}
}
void add(MySQL &db,const std::string &tab_name,std::vector<std::string> args){
	auto size=args.size();
	std::string tmp;
	switch(size){
		case 1:
			while(1){
				printf("enter the income (negative for expense): ");
				std::getline(std::cin,tmp);
				if(first_not_space(tmp)){
					args.push_back(tmp);
					break;
				}
			}
		case 2:
			while(1){
				printf("enter the description: ");
				std::getline(std::cin,tmp);
				if(first_not_space(tmp)){
					args.push_back(tmp);
					break;
				}
			}
		default:
			db.query("insert into `"+tab_name+"` (income,`time`,description) values ("+args[1]+",now(),'"+args[2]+"');");
	}
}
void show(MySQL &db,const std::string &tab_name,const std::vector<std::string> &args){
	db.query("select * from `"+tab_name+"`;");
	default_print_tab(db.get_result());
}
void calc(MySQL &db,const std::string &tab_name,const std::vector<std::string> &args){
	db.query("select * from `"+tab_name+"`;");
	if(db.get_result().size()){
		db.query("select sum(income) from `"+tab_name+"`;");
		default_print_tab(db.get_result());
	}
	else
		puts("0");
}
void help(MySQL &db,const std::string &tab_name,const std::vector<std::string> &args){
	puts("help\n");
	puts("enter command like");
	puts("    add [money] [description]");
	puts("\tomitted part should be input later interactively");
	puts("    show");
	puts("    calc");
	puts("    exit");
	puts("    help");
}