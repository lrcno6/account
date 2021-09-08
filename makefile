account: main.cpp func.cpp func.h
	g++ main.cpp func.cpp -o account -LMySQLClass -lmysql_class -lmysqlclient