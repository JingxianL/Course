#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include<algorithm>
using namespace std;

// CONSTANTS:
int TOTALMEMORYSIZE = 1 * 1024 * 1024; // total memory in bytes
struct Job{
	char op;
	int pid;
	int size;
	int begin;
	int end;	
};

struct find_element
{
    int y;
    find_element(int y) : y(y) {}
    bool operator()(const Job& l) const {return y == l.pid;};
};

struct freeSpace
{
	int begin;
	int size;
};

void readAndProcess(vector<Job>&, int&);

int main()
{
	vector<Job> incomingJobs;
	int totalMemoryInUse = 0;
	int totalNumOfJob=0;
	vector<Job>::iterator iter;
	int index;
	bool existFreeSpace = false;
	vector<freeSpace> Free;
	int numOfFreeSpace = 0;
	bool findExistFree = false;
	readAndProcess(incomingJobs, totalNumOfJob);
	cout << totalNumOfJob << endl;
	// iter = find_if(incomingJobs.begin(), incomingJobs.end(),find_element( aJob.pid));
	// index = distance(incomingJobs.begin(), iter);
	Job nextJob;
	freeSpace nextFreeSpace;
	for (int i=0; i < totalNumOfJob; i++){
		nextJob = incomingJobs[i];
		if (nextJob.op == 'A'){
			if (totalMemoryInUse < TOTALMEMORYSIZE){

				if (nextJob.size <= TOTALMEMORYSIZE){
					if (!existFreeSpace){
						nextJob.begin = totalMemoryInUse;
						totalMemoryInUse += nextJob.size;
						nextJob.end = totalMemoryInUse-1;
						cout << nextJob.pid << '\t'
						<<  nextJob.begin << '\t'
						<< nextJob.end << '\t'
						<< nextJob.size << endl;
						cout << "hahaha" << incomingJobs[0].begin << endl;
					}
					else{
					int k =0;
					while(k < numOfFreeSpace && !findExistFree)
					{
						if (nextJob.size <=  Free[k].size){
							nextJob.begin = Free[k].begin;
							totalMemoryInUse += nextJob.size;
							nextJob.end = nextJob.begin + nextJob.size -1;
							cout << nextJob.pid << '\t'
							<<  nextJob.begin << '\t'
							<< nextJob.end << '\t'
							<< nextJob.size << endl;
							findExistFree = true;
						}
						k++;
					}
						
				}
				}
				
			}
		}
		else if(incomingJobs[i].op == 'D'){
			// cout << "hahaha" <<  endl;
			iter = find_if(incomingJobs.begin(), incomingJobs.end(),find_element( incomingJobs[i].pid));
			index = distance(incomingJobs.begin(), iter);
			cout << index<< "index" << endl;
			totalMemoryInUse -= incomingJobs[index].size;
			cout << "hahaha" << incomingJobs[2].begin << endl;

			nextFreeSpace.begin = incomingJobs[index].begin;
			nextFreeSpace.size = incomingJobs[index].size;
			Free.push_back(nextFreeSpace);
			// cout << "hahaha" <<  endl;
			
			cout << "Free" << '\t'
					<< Free[numOfFreeSpace].begin << '\t'
					// << incomingJobs[index].end << '\t'
					<< Free[numOfFreeSpace].size << endl;
			incomingJobs[index].pid=0;
			existFreeSpace = true;
			numOfFreeSpace++;

		}
	}
	

	return 0;
}

void readAndProcess(vector<Job>& incomingJobs, int& totalNumOfJob){
	ifstream fin;
	string filename;
	cout << "Please enter the file name: " << "\n";
	// cin >> filename;
	filename = "input.txt";
	fin.open(filename.c_str());
	if(!fin.is_open())
	{
		cout << "ERROR: FILE COULD NOT OPEN!" << endl;
	}
	else{
		Job nextJob;
		char finished;
		char deadLock;
		fin>>finished;
		nextJob.op = finished;
		while(fin && finished != 'Q'){
			if (finished != 'D'){
				fin	>> nextJob.pid
					>> nextJob.size;

				totalNumOfJob++;
			}
			else{
				fin	>> nextJob.pid;
				nextJob.size = 0;
				totalNumOfJob++;
			}
			incomingJobs.push_back(nextJob);

			fin>>finished;
			nextJob.op = finished;
			if (finished == 'Q'){
				nextJob.pid = 0;
				nextJob.size = 0;
				// cout << nextJob.op << endl;
				totalNumOfJob++;
				incomingJobs.push_back(nextJob);
			}
		}		
	}
}










while(k <= numOfFreeSpace && !findExistFree)
	{	
		cout << i << endl;
		
		if (i == 10) {
			cout << "ahahahaha!!!!!!!!!!" << endl;
			cout  << k <<"k" << endl;
			cout << numOfFreeSpace << "numOfFreeSpace"<< endl;
		}
		if (incomingJobs[i].size <=  Free[k].size){
			incomingJobs[i].begin = Free[k].begin;
			totalMemoryInUse += incomingJobs[i].size;
			incomingJobs[i].end = incomingJobs[i].begin + incomingJobs[i].size -1;
			cout << incomingJobs[i].pid << '\t'
			<<  incomingJobs[i].begin << '\t'
			<< incomingJobs[i].end << '\t'
			<< incomingJobs[i].size << endl;
			findExistFree = true;
			Free[k].begin += incomingJobs[i].size;
			Free[k].size -= incomingJobs[i].size;

			// if(Free[k].size == 0) {
			// 	Free.erase(Free.begin()+k);
			// 	numOfFreeSpace--;
			// 	k++; 
			// }
		}
		k++;
	}
}