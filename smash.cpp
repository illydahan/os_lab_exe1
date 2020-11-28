/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string>
#include "commands.h"
#include "signals.h"

#define MAX_LINE_SIZE 80
#define MAXARGS 20




char* L_Fg_Cmd;
std::vector<job_entry> jobs= std::vector<job_entry>();
//void* jobs = NULL; //This represents the list of jobs. Please change to a preferred type (e.g array of char*)
char lineSize[MAX_LINE_SIZE]; 



void HandleInputLine(std::string, std::vector<std::string>& );
void saveCommand(std::vector<std::string>& , std::string );

//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
    std::vector<std::string> command_history = std::vector<std::string>();
    std::vector<std::string> command_sep;

	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	 /* add your code here */
	
	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here
	/* add your code here */

	/************************************/

	/************************************/
	// Init globals 


	
	L_Fg_Cmd =(char*)malloc(sizeof(char)*(MAX_LINE_SIZE+1));
	if (L_Fg_Cmd == NULL) 
			exit (-1); 
	L_Fg_Cmd[0] = '\0';
    while (1)
    	{
    	command_sep = std::vector<std::string>();
    	std::cout << "smash >";
    	std::string commandLine("");
    	std::cin.clear();
    	std::cin >> commandLine;
    	if(commandLine.length() == 0) continue;
		saveCommand(command_history,commandLine);
		HandleInputLine(commandLine, command_sep);

		if(!ExeCmd(jobs, command_sep, command_history, commandLine))
		{

			// built in commands
			//LOG("BUILT IN COMMAND");
		}
		else if(!BgCmd(lineSize, jobs))
		{
			// background command
			//LOG("BG COMMAND");
		}
		// perform a complicated Command
		else if(!ExeComp(lineSize));
		//std::cout <<std::endl;
	}
    return 0;
}

//**************************************************************************************
// function name: HandleInputLine
// Description: this function handles the input of the prompt.
// Input: std::string inputLine - input line as typed by the user
//        std::vector<std::string>& command_sep - separated input line to command and arguments
// will return 0 for built-in command,  1 for background, 2 for complicated
//**************************************************************************************
void HandleInputLine(std::string inputLine, std::vector<std::string>& command_sep)
{
	const char	*delimiters = " \t\n";
	char 	    *Command = (char *)inputLine.c_str();
	const char 	*cmd   = std::strtok(Command, delimiters);
	char 		*arg;
	command_sep.push_back(cmd);
	do
	{
		arg = std::strtok(NULL, delimiters);
		if(arg != '\0')
		{
			command_sep.push_back(arg);
		}
	}while(arg != '\0');

}

void saveCommand(std::vector<std::string> &command_history, std::string command)
{

	if(command_history.size() > 50)
	{
		command_history.pop_back();
		command_history.push_back(command);
	}
	else
	{
		command_history.push_back(command);
	}
}

