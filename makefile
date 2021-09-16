real_account: main.cpp func.cpp func.h
	g++ main.cpp func.cpp -o real_account -LMySQLClass -lmysql_class -lmysqlclient