/*
Course: 
	CS 570 - Operating Systems
		 Section 2

Professor: 
	Ben Shen 
	
Programmers: 
	Brandon Altamirano (821836022)
	Alberto Guadiana   (820784452)
	

Description:
	Program that involves the use of POSIX threads. The purpose of this program 
	is to take an input, whether it be a file/filename and have it read the amount 
	of words it consists of. A progress bar is printed as the program is reading 
	the file. Once it completes an entire progress bar is displayed followed by 
	a statement that shows the amount of words the file consists of. The program
	should be able to handle large sized files as well as smaller ones. 
*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>

using namespace std;

/*
The structure has 3 access member fields. CurrentStatus is apointer to a long. This 
represents the status of the computation done. The InitialValue member is the value 
where the computation starts and TerminationValue is the value at which the program 
completes. Once the TerminationValue is reached by CurrentStatus the loop breaks
and the progress bar completes it's printing.
*/

typedef struct{
    long *CurrentStatus;
    long InitialValue;
    long TerminationValue;
} PROGESS_STATUS;

/*
Method monitors the progress of a task that executes in a separate
separate thread. It is only invoked once by the thread where it will 
loop until the progress bar is complete. Once complete it exits.
*/
void * progress_monitor(void * progress){
    PROGESS_STATUS *progress_stat = (PROGESS_STATUS*)progress;
    double percentage = 0;
    int nextChar = 2;
    do { 
        percentage = ((double)*progress_stat->CurrentStatus / (double)progress_stat->TerminationValue) * 100;
         /*
        if-else statement that takes in the calculated percentage from 
        CurrentStatus/TerminationValue. Every 20% a "+" is to be printed, 
        else, a "-" is printed for every 2%. This loop terminates by a break once 
        CurrentStatus is equal to the Termination value and nextChar is 
        greater than 100.   
        */
        if(percentage >= nextChar){
            if((nextChar % 20) == 0){
                cout<<"+";
                fflush(stdout);
            }
            else{
                cout<<"-";
                fflush(stdout); 
            }
            nextChar += 2;
        }
        //nextChar condition below is for smaller sized files that do not have proper percentage calculations.
        if(*progress_stat->CurrentStatus == progress_stat->TerminationValue && nextChar > 100) 
            break;
    } while(1);
    return NULL;
}
/*
Method returns a long integer with the number of words where it 
takes a file/filename as parameter. It creates the progress_monitor pthread
with the PROGRESS_STATUS struct populated as the argument. The while loop will read each 
character inside the file and count it as a byte. The variable CurrentStatus 
from the struct will be the counter of these bytes. Once the progress bar is complete, 
the method will return how many words the file has. 	
*/
long int wordcount(FILE* file, long fileSize){

    PROGESS_STATUS *progress = new PROGESS_STATUS;
    progress->CurrentStatus = new long;
    progress->TerminationValue = fileSize;
    progress->InitialValue = 0;
    pthread_t pthread;
    int buffers = 0;
    long int words = 0;
    int i = 0;

    pthread_create(&pthread, NULL, &progress_monitor, (void *) progress);
    while(i<fileSize){
        char c = fgetc(file);
        if(isspace(c)){
            buffers = 0;
        }
        else if(buffers == 0){
            ++words;
            buffers = 1;
        } 
        ++*progress->CurrentStatus;
        ++i;
    }
    pthread_join(pthread, NULL);
    cout<<endl;
    return words;
}
int main(int argc, char** argv){
    
    if(argc < 2) //File must be given
        return(0);
    FILE* file = fopen(argv[argc - 1], "r");
    struct stat filestatus;
    stat(argv[1], &filestatus);
    long bytes = filestatus.st_size;

    /*
    if-else statement that returns 0 if there is no file(NULL)
    or creates a long int words and is assigned the result of wordcount(). 
    It should then print the amount of words inside a file.
    */
    if (file == NULL){ //File doesn't exist
        cout<<"Cannot open specified file";
        return(0);
    }
    else{
        long int words = wordcount(file,bytes);
        cout<<"There are " << words << " words in "<< argv[1]<<endl;
    }
    fclose(file);
    return(0);
}
