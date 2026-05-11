#ifndef DEFS_H_
#define DEFS_H_

#include <vector>

#include "psql_executor.h"
#include "gui_interface.h"

typedef struct my_global_s{
    psql_exec_t             psql;       // Работа с базой данных
    gui_runner_t            gui_runner;
} my_global_t;


#endif