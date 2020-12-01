//		commands.cpp
//********************************************
#include "commands.h"
#include <sys/stat.h>
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************

int ExeCmd(std::vector<job_entry> jobs,
		std::vector<std::string>& sep_command ,
		std::vector<std::string>& command_history,
		std::string commandLine,
		std::string former_location)
{
	const char* cmd = sep_command[0].c_str();
	int num_arg = sep_command.size() - 1;
	bool illegal_cmd = false; // illegal command
	/*************************************************/
	// Built in Commands PLEASE NOTE NOT ALL REQUIRED
	// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
	// MORE IF STATEMENTS AS REQUIRED
	/*************************************************/
	if (!strcmp(cmd, "cd") ) 
	{
		const char* path = sep_command[1].c_str();
		//this section deals with demand to go to the former location.
		if (!strcmp(path, "-") {
			if (former_location.length() == 0) {
				LOG("smash > this is your first location");
					return 1;
			}
			std::string tempstring();
			get_current_dir_name() >> tempstring;
			chdir(former_location.c_str());
			tempstring.c_str() >> former_location;
			return 0;
		}
		//this section finds the existence of the directory, and sends you there if it does.
		int exists = dpns_access(sep_command[1].c_str(), F_OK);
		if (exists) {
			std::cout << "smash error: > " << job_args[1] << " - No such file or directory" << std::endl;
			return 1;
		}
		former_location = sep_command[1];
		chdir(sep_command[1].c_str());
		return 0;	
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		char *pwd = get_current_dir_name();
		LOG(pwd);
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "mkdir"))
	{
	}
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
 		if (jobs.size() == 0) {
			LOG("no jobs submitted yet");
			return 1;
		}

		for (int i = 1; i <= jobs.size(); i++) {
			job_entry* the_job = jobs[i];
			std::cout << "[" << i << "] " << the_job->job_name << " : " << the_job.job_pid << " ";
			std::cout << the_job.entry_time << " secs ";
			if (the_job.status == 1) {
				dts::cout << "(Stopped)";
			}
			std::cout << std::endl;
		}
		return 0;
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		std::cout << "smash pid is " << getpid() << std::endl;
		return 0;	
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		// check if no job index specified and there's a job in the job buffer
		// or job is specified and its valid. (i.e job_index < jobs.size())
		int ret_val = 0;
		int job_index = atoi(sep_command[1].c_str());
		if(num_arg < 1 && jobs.size() > 0)
		{
			job_entry job = jobs.back();
			// job is not stopped
			LOG(job.job_name);
			if(!job.status)
			{
				ret_val = execvp(job.job_name, job.job_args);
			}
		}
		else if(num_arg == 1 && (int)jobs.size() < job_index)
		{
			job_entry job = jobs[job_index];
			// job is not stopped
			LOG(job.job_name);
			if(!job.status)
			{
				ret_val = execvp(job.job_name, job.job_args);
			}
		}
		if(ret_val != 0)
		{
			perror("smash error: > “fg” – process execution failed");
		}

	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
 		int pid;
		if ((pid = fork()) == -1) {
			perror("forking failed");
			exit(1);
		}
		else if (pid == 0) {
			int ret_val = 0;
			int job_index = atoi(sep_command[1].c_str());
			if (num_arg < 1 && jobs.size() > 0)
			{
				//checking all the list for one with maximal ID and is also stopped
				int check = -1;
				int ID = 0;
				for (int i = 1; i <= jobs.size(); i++) {
					if (jobs[i]->status) {
						if ((jobs[i]->job_pid) > check) {
							check = jobs[i]->job_pid;
							ID = i;
						}
					}
				}
				if (check == -1) {
					perror("no available process to run in B.G.");
					exit(1);
				}
				job_entry job = jobs[ID];
				// by this point, job is found, or error was announced
				LOG(job.job_name);
				ret_val = execvp(job.job_name, job.job_args);
			}
			else if (num_arg == 1 && (int)jobs.size() < job_index)
			{
				job_entry job = jobs[job_index];
				// job is not stopped
				LOG(job.job_name);
				if (!job.status)
				{
					perror("the process you gave wasn't stopped, and therefore couldn't be executed in BG");
					exit(1);
				}
				
				ret_val = execvp(job.job_name, job.job_args);
			}
			if (ret_val != 0)
			{
				perror("smash error: > “bg” – process execution failed");
			}
			//take the job out of the vector.
			exit(0);
		}	
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		if(num_arg < 1)
   		{
   			// quit command
   			exit(1);
   		}
   		else if(!strcmp(sep_command[1].c_str(), "kill"))
   		{
   			// quit kill command
   			for(unsigned int i = 0; i < jobs.size(); i++)
   			{
   				std::cout << "[" << i+1<< "] - Sending SIGTERM...";
   				job_entry job = jobs[i];
   				time_t    sig_time = time(NULL);
   				time_t    elapsed_time = 0;
   				int       status;
   				kill(job.job_pid, SIGTERM);
   				while(elapsed_time < 5 && wait(&status) != job.job_pid)
   				{
   					elapsed_time = time(NULL) - sig_time;
   				}
   				if(elapsed_time >= 5)
   				{
   					std::cout << "(5 sec passed) Sending SIGKILL...";
   					kill(job.job_pid, SIGKILL);

   				}
   				std::cout << "Done" << std::endl;
   			}
   			exit(1);
   		}
	} 
	/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
		if(num_arg < 2)
		{
			LOG("smash error: > kill - too few arguments");
			return 1;
		}
		int sig_num = atoi(sep_command[1].c_str() + 1);
		unsigned int job_index = atoi(sep_command[2].c_str());

		if(job_index >= jobs.size())
		{
			std::cout << "smash error:> kill " << job_index << " - job does not exist" << std::endl;
			return 1;
		}

		job_entry requested_job = jobs[job_index];
		int sig_ret = kill(requested_job.job_pid, sig_num);
		if(sig_ret)
		{
			std::cout <<"smash error: > kill " << job_index << " - cannot send signal" << std::endl;
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "history"))
	{
		for(std::string cmd : command_history)
		{
			LOG(cmd);
		}
	} 
	/*************************************************/
	else if (!strcmp(cmd, "cp"))
	{
		if (num_arg < 2) {
			LOG("smash error: > cp - too few arguments");
			return 1;
		}
		int FD1 = open(sep_command[1].c_str());
		if (FD1 == -1) {
			perror("something wrong with the file.");
			return 1;
		}
		int FD2 = open(sep_command[2].c_str());
		int check = dup2(FD1, FD2);
		if (check == -1) {
			perror("failed");
			return 1;
		}

		//copying systemcall.
	std::cout << sep_command[1] << "has been copied to" << sep_command[2] << std::endl;
	return 0;
	 }
	/*************************************************/
	else if (!strcmp(cmd, "diff"))
	{
		if (num_arg < 2) {
			LOG("smash error: > cp - too few arguments");
			return 1;
		}
		int read_access1 = dpns_access(sep_command[1].c_str(), R_OK);
		int read_access2 = dpns_access(sep_command[2].c_str(), R_OK);
		if ((read_access1 != 0) || (read_access2 != 0)) {
			perror("can't access file/s");;

		}
		//run them through something that compares them. as strings?
	}
	/*************************************************/
	else // external command
	{
 		ExeExternal(sep_command);
	 	return 0;
	}
	if (illegal_cmd == true)
	{
		//printf("smash error: > \"%s\"\n", commandLine);
		return 1;
	}
    return 0;
}


//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(std::vector<std::string>& sep_command)
{
	int pID;
    switch(pID = fork())
	{
    	case -1:
			// Add your code here (error)

			/*
			your code
			*/
    	case 0 :
            // Child Process
           	setpgrp();

	        // Add your code here (execute an external command)

			/*
			your code
			*/

    	default:
            // Add your code here
			return;
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(std::string commandLine)
{
	const char *lineSize = commandLine.c_str();
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		// Add your code here (execute a complicated command)
					
		/* 
		your code
		*/
	} 
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(std::string line, std::vector<job_entry> jobs)
{
	return -1;
}





