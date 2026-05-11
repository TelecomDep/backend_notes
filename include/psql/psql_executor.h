#ifndef PSQL_EXECUTOR_H_
#define PSQL_EXECUTOR_H_


#include <iostream>
#include <string>
#include <sstream>

#include <libpq-fe.h>


typedef struct psql_exec_s{
    PGconn *con;
    PGresult *res;
} psql_exec_t;


#endif