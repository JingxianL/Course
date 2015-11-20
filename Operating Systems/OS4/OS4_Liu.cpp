/*
 
 Program Name: Project OS4
 
 Purpose: To implement a first-fit memory allocation scheme, and get stats on how well it performs.
 
 Input: 
    1) Input from keyboard: 
        - Input filename

 Output:
    1) Output on console:
        - Prompt to enter input filename
        - A table showing the final configuration of memory
        - Error messages while file not found
        - Number of jobs not fit in memory in system
        - Total amount of memory in use at the end of the simulation
        - Total amount of memory remaining
        - Number of contiguous spaces (memory blocks) remaining
        - Average size of remaining memory blocks

 - I have abided by the Wheaton College honor code in this work.

*/

// include all the library needed
#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include<algorithm>
using namespace std;

// CONSTANTS:
long TOTALMEMORYSIZE = 1 * 1024 * 1024; // total memory in bytes

// Simulation class
class simulation{
public:
	//Job struct for each job (each line of the file read in)
	struct Job{
		string op;
		int pid;
		long size;
		long begin;
		long end;	
	};

	//find element structure for find element function use for find_if
	struct find_element
	{
		int y;
		find_element(int y) : y(y) {}
		bool operator()(const Job& l) const {return y == l.pid;};
	};


	string filename;
	vector<Job> incomingJobs;
	int numOfProcessFit;
	int numOfFreeSpace;
	int totalMemoryInUse;
	int totalNumOfJobs;
	vector<int> pidOfJobNotFit;
	//Constructor
	simulation(string filename){
		readAndProcess(filename);
	};

	/* This function takes each line as a job from an input file and process the job.
     - Pre-condition: the user entered an file name
     - Post-condition: a memory table after process all the jobs;
     - Return: None
     */
	void readAndProcess(string filename){
		ifstream fin;
		fin.open(filename.c_str());
		//check if the file can open
		if(!fin.is_open())
		{
			cout << "ERROR: FILE COULD NOT OPEN!" << endl;
		}

		else
		{
			Job nextJob; // a temporary job for each job read in

			char finished;
			fin >> finished;
			nextJob.op = finished; //read in the first char in the file and store it in to nextJob.op

			//variables needed for find_if function to get the index in the vector;
			vector<Job>::iterator iter;
			size_t index;

			// variable needed for final stats
			totalMemoryInUse = 0;
			numOfProcessFit = 0;
			Job thisFreeBlock;
			totalNumOfJobs = 0;

			bool haveFindFreeBlock = false;   // control variable for loop 
			int lastEnd=0;   // keep track of where the last end is in the memory

			while(fin && finished != 'Q'){ // read in from file till it is Q

				fin	>> nextJob.pid;
				haveFindFreeBlock = false;   // read in each pid 
				if (totalMemoryInUse <= TOTALMEMORYSIZE){ //check if the memory is full
					if (finished == 'A' ){ // check if it is A
						fin >> nextJob.size;
						totalNumOfJobs++;
						if (find_if(incomingJobs.begin(), incomingJobs.end(),find_element(0)) == incomingJobs.end()){// put on the end of the using memory if no freeblock found
							nextJob.begin = totalMemoryInUse;
							totalMemoryInUse += nextJob.size;
							nextJob.end = totalMemoryInUse-1;
							lastEnd = nextJob.end;
							incomingJobs.push_back(nextJob);
							numOfProcessFit++;
						}
						else{// putting it in the free block if possible
							for (int i =0; i < incomingJobs.size(); i++){// try to find a freeblock if possible
								if (nextJob.size <= incomingJobs[i].size && incomingJobs[i].op == "Free" && !haveFindFreeBlock){ // find a block that is big enough to fit the process

									thisFreeBlock.size = incomingJobs[i].size;
									thisFreeBlock.begin = incomingJobs[i].begin;
									thisFreeBlock.end = incomingJobs[i].end;
									nextJob.begin = thisFreeBlock.begin;
									nextJob.end = thisFreeBlock.begin + nextJob.size - 1;
									incomingJobs.erase(incomingJobs.begin()+i, incomingJobs.begin()+i+1);
									incomingJobs.insert(incomingJobs.begin()+i, nextJob);
									thisFreeBlock.begin = nextJob.end+1;
									thisFreeBlock.size = thisFreeBlock.size - nextJob.size;
									thisFreeBlock.pid = 0;
									thisFreeBlock.op = "Free";
									incomingJobs.insert(incomingJobs.begin()+i+1, thisFreeBlock);
									totalMemoryInUse += nextJob.size;
									haveFindFreeBlock = true;
									numOfProcessFit++;

								}
							}
							if (!haveFindFreeBlock && nextJob.size < TOTALMEMORYSIZE - lastEnd){//if can't find it, then add it to the end
								nextJob.begin = lastEnd+1;
								nextJob.end = lastEnd + nextJob.size ;
								lastEnd = nextJob.end;
								incomingJobs.push_back(nextJob);
								totalMemoryInUse += nextJob.size;
								haveFindFreeBlock = true;
								numOfProcessFit++;

							}
							else if (!haveFindFreeBlock) {// if still can't fit, then it is not able to fit
								pidOfJobNotFit.push_back(nextJob.pid);
							}
						}
					}
					else if (finished == 'D'){//check if it is deallocate

						iter = find_if(incomingJobs.begin(), incomingJobs.end(),find_element(nextJob.pid));
						index = distance(incomingJobs.begin(), iter);
						nextJob.size = incomingJobs[index].size;
						nextJob.begin = incomingJobs[index].begin;
						nextJob.end = incomingJobs[index].end;
						nextJob.pid = 0;
						nextJob.op = "Free";
						incomingJobs.erase(incomingJobs.begin()+index, incomingJobs.begin()+index+1);
						incomingJobs.insert(incomingJobs.begin()+index, nextJob);
						totalMemoryInUse -= incomingJobs[index].size;

					}
					for ( int k=0; k< incomingJobs.size() ; k++){ //check if two freeblock is together
						if (incomingJobs[k].pid == 0 && incomingJobs[k+1].pid == 0){
							nextJob.size = incomingJobs[k].size + incomingJobs[k+1].size;
							nextJob.begin = incomingJobs[k].begin;
							nextJob.end = incomingJobs[k+1].end;
							nextJob.pid = 0;
							nextJob.op = "Free";
							incomingJobs.erase(incomingJobs.begin()+k, incomingJobs.begin()+k+2);
							incomingJobs.insert(incomingJobs.begin()+k, nextJob);
						}
						if(incomingJobs[k].pid == 0 && incomingJobs[k].size==0){
							incomingJobs.erase(incomingJobs.begin()+k, incomingJobs.begin()+k+1);
						}
					}
				}
				else{
					totalNumOfJobs++;
					pidOfJobNotFit.push_back(nextJob.pid);
				}	
				fin>>finished;
				nextJob.op = finished;
				if (finished == 'Q'){//check it is Q
					nextJob.pid = 0;
					nextJob.begin = lastEnd+1;
					nextJob.end = TOTALMEMORYSIZE;
					nextJob.size = TOTALMEMORYSIZE - lastEnd;
					incomingJobs.push_back(nextJob);
				}
			}
		}
	}

	//function use to print all of the print all the result.
	void print(){
		cout << endl << "Memory Table" << endl;
		cout << "PID" << '\t' << "Begin" << '\t' << "End" << '\t' << "Size" << endl;
		cout << "-------------------------------------" << endl;
		numOfFreeSpace = 0;
		for (int  i= 0; i < incomingJobs.size();i++){//print table
			
			if (incomingJobs[i].pid == 0) {
				cout << "Free" << "\t"
					<< incomingJobs[i].begin << "\t"
					<< incomingJobs[i].end << "\t"
					<< incomingJobs[i].size << endl;
					numOfFreeSpace++;

			}
			else{
				cout << incomingJobs[i].pid << "\t"
					<< incomingJobs[i].begin << "\t"
					<< incomingJobs[i].end << "\t"
					<< incomingJobs[i].size << endl;
			}	
		}
		//print stats
		float aveRemaingBlock = (TOTALMEMORYSIZE - totalMemoryInUse + 1)/ float(numOfFreeSpace);
		cout  << endl << "Number of processes (and their PIDs) that did not fit in memory: " << totalNumOfJobs - numOfProcessFit << endl;
		cout << "pid of these jobs: ";
		for (int i=0; i<pidOfJobNotFit.size();i++){
			cout << pidOfJobNotFit[i] << " ";
		}
		cout << endl;
		cout << "Total amount of memory in use at the end of the simulation: " << totalMemoryInUse << " bytes." << endl;
		cout << "Total Amount of Memory Remaining: " << TOTALMEMORYSIZE - totalMemoryInUse + 1 << " bytes." << endl;
		cout << "Number of Free Space in System: "  << numOfFreeSpace << endl;
		cout << "Average size of remaining memory blocks: "<< aveRemaingBlock << endl << endl;
	}
};

int main()
{
	string filename;
	cout << "Please enter the file name: " << "\n";
	cin >> filename;
	// create the object Simulation 
	simulation Simulation = simulation(filename);
	//call the print function
	Simulation.print();
	return 0;
}