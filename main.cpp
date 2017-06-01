#include <iostream>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <getopt.h>
#include <chrono>
#include <sstream>
#include <cstring>
#include <sys/time.h>

#include "logger.h"

using namespace std;

//char* getErrorMessage(data_t);
data_t makeError(char *executable, const char *errorMsg);

int main(int argc, char **argv){

	//long_options is used for getopt_long so that --help can be used in place of -h as a command line arg
	static struct option long_options[] = {
        {"help", no_argument, 0, 'h'}
    };

    //also used as a paramater in getopt_long
    int longIndex = 0;

	extern char *optarg;
	int temp; //used to hold temp return value of getopt_long

	int specialNumber = 42; //logged whenever this program is ran

	const char *logfileName = "logfile.txt"; //name of the output file

	while((temp = getopt_long(argc, argv, "hl:n:", long_options, &longIndex)) != -1){
		switch(temp){
			case 'h':
				cout << "This program accepts the following command-line arguments:" << endl;
				cout << "\t-h, -help: Get detailed information about command-line argument options." << endl;
				cout << "\t-n i: Initialize an integer that will be logged whenever \n\t\t this log utility is used (default 42)." << endl;
				cout << "\t-l 'filename': Specify the output file for the log \n\t\t utility (default 'filename.txt')." << endl;
				exit(0);
			break;

			case 'l':
			//set the logfile output name as the supplied command line arg
				logfileName = optarg;

			break;

			case 'n':
				//set the special number supplied as a command line argument
				specialNumber = atoi(optarg);

				try {
					specialNumber = stoi(optarg); 
				}catch(exception const &e){
					perror("Argument for option -n is not an integer");
					exit(1);
				}
			break;

			case '?':
				cout << "invalid option" << endl;
				exit(1);
			break;

			default:
				cout << "invalid argument" << endl;
				exit(1);
			break;

		}
	}

	cout << "specialNumber - " << specialNumber << endl;


	//Two sample error messages

	data_t data1 = makeError(argv[0], "Ouch! You pinned the tail on the wrong end.");

	data_t data2 = makeError(argv[0], "Success!...wait...nevermind...that was a fail.");

	//add one message to the log queue

	addmsg(data1);
	cout << "The log currently contains: \n";
	cout << getlog() << endl;

	//save first message to log file

	cout << "Writing to file..." << endl;
	savelog(logfileName);

	cout << "clearing the log..." << endl;
	clearlog();

	cout << "Lets try storying 2 nodes in the list!" << endl;
	addmsg(data1);
	addmsg(data2);

	cout << "The log now contains:\n";
	cout << getlog() << endl;

	cout << "Saving these errors to the file." << endl;
	savelog(logfileName);

	cout << "Clearing the log list." << endl;
	clearlog();

	cout << "Log now contains: \n";
	cout << getlog() << endl;

	return 0;	
}

//accepts the executable name and an error message string,
// creates a data_t type using current time in nanoseconds and
//and a string formmatted according to the project specification

data_t makeError(char *executable, const char *errorMsg){
	data_t data;

	char *dataString = new char[150];
	
	//Using chrono api, calculate nanoseconds since epoch
	
	chrono::system_clock::time_point now = chrono::system_clock::now();
	chrono::system_clock::duration duration = now.time_since_epoch();

	data.time = chrono::duration_cast<chrono::nanoseconds>(duration).count();


//	cout << "chrono time is " << data.time << endl;

	//FIXME: Should 'Error' be a literal or the actual error?
	//format error message to be in form:
	// [executable] : [time]: Error: [detailed error message]

	ostringstream ss;
	ss << executable;
	ss << ": ";
	ss << data.time;
	ss << ": Error: "; //FIXME
	ss << errorMsg;

	data.string = new char[150]; //FIXME: arbitrarily assigned size

	strcpy(data.string, ss.str().c_str());


	return data;
}
