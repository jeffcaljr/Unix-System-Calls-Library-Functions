#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <string.h>
#include <fstream>
#include <cerrno>

       
#include "logger.h"

using namespace std;

typedef struct list_struct {
     data_t item;
     struct list_struct *next;
} log_t;

static log_t *headptr = NULL;
static log_t *tailptr = NULL;

int addmsg(data_t data) {

	//create a new log_t object from a supplied data_t, and store it in the list

	log_t *newmsg = new log_t;

	if(newmsg == NULL){
		perror("Error allocating space for addmsg()");
		errno = ENOMEM;
		return -1;
	}
	else{
		newmsg->item = data;
		newmsg->next = NULL;

		if(headptr == NULL){
			headptr = newmsg;
			tailptr = newmsg;
		}
		else{
			tailptr->next = newmsg;
			tailptr = newmsg;
		}

		return 0;
	}
}

//TODO: Should clearlog erase the contents of the logfile?
void clearlog(void) {

	log_t *temp;

	while(headptr != NULL){

		temp = headptr;
		headptr = headptr->next;
		delete temp;

	}

	headptr = NULL;
}


//TODO: Allocate char array and properly fetch logs
char *getlog(void) {

	char* logString = new char[1000]; //FIXME: This size was chosen arbitrarily. Is there a dynamic solution?
	log_t *temp = headptr;

	if(logString == NULL){
		perror("Error allocating space for getlog()");
		errno = ENOMEM;
		return NULL;
	}
	else{
		strcpy(logString, "");
		while(temp != NULL){
			strcat(logString, (temp->item).string);
			strcat(logString, "\n");
			temp = temp->next;
		}

		return logString;
	}
}

int savelog(const char *filename) {
	
	ofstream file;
	file.open(filename, ofstream::out | ofstream::app);

	if(!file){
		perror("Error opening output stream:");
		errno = ENOENT;
		return -1;
	}
	else{
		file << getlog();
		file.close();
		return 0;
	}
}
