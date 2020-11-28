#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <vector>
#include <string>
#include <cstring>
#include <climits>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
#define LOG(x) (std::cout << x << std::endl)

typedef struct job_entry {
	char *job_name; // job executable name
	uint32_t entry_time; // job entry time
	char *job_args[]; // the job arguments
	int job_pid; // the PID of the job
	int status; // 1 = stopped; 0 = running;
}job_entry;

int ExeComp(std::string commandLine);
int BgCmd(std::string line, std::vector<job_entry> jobs);
int ExeCmd(std::vector<job_entry> , std::vector<std::string>& , std::vector<std::string>&, std::string);
void ExeExternal(std::vector<std::string>& s);



#endif

