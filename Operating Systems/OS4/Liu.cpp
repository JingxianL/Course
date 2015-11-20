#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<vector>
#include<algorithm>
using namespace std;
// 1044976
// 1048576
// CONSTANTS:
int TOTALMEMORYSIZE = 1 * 1024 * 1024; // total memory in bytes
struct Job{
	char op;
	int pid;
	int size;
	int begin;
	int end;	
};
struct freeMem
{
	int begin;
	int size;
};

struct find_element
{
    int y;
    find_element(int y) : y(y) {}
    bool operator()(const Job& l) const {return y == l.pid;};
};


void readAndProcess(vector<Job>&, int&);
void FindExistFreeSpace(int& numOfFreeSpace, bool& findExistFree, Job& thisJob, vector<freeMem>& Free, int& totalMemoryInUse);
int main()
{
	vector<Job> incomingJobs;
	int totalMemoryInUse = 0;
	int totalNumOfJob=0;
	vector<Job>::iterator iter;
	size_t index;
	bool freeSpace = false;
	vector<freeMem> Free;
	vector<int> skippedJob;
	int numOfFreeSpace = 0;
	bool findExistFree = false;
	int numOfJobNotFit = 0;
	readAndProcess(incomingJobs, totalNumOfJob);
	int lastEnd;
	cout << totalNumOfJob << endl;
	// iter = find_if(incomingJobs.begin(), incomingJobs.end(),find_element( aJob.pid));
	// index = distance(incomingJobs.begin(), iter);
	// cout << index<< endl;
	freeMem nextFreeSpace;
	bool skippedThis = false;
	int k =0;
	for (int i=0; i < totalNumOfJob; i++){
		if (incomingJobs[i].op == 'A'){
			if (totalMemoryInUse < TOTALMEMORYSIZE){
				if (incomingJobs[i].size < (TOTALMEMORYSIZE-totalMemoryInUse)){
					cout << i << endl;
					findExistFree = false;
					if (!freeSpace){
						incomingJobs[i].begin = totalMemoryInUse;
						totalMemoryInUse += incomingJobs[i].size;
						incomingJobs[i].end = totalMemoryInUse-1;
						cout << incomingJobs[i].pid << '\t'
						<<  incomingJobs[i].begin << '\t'
						<< incomingJobs[i].end << '\t'
						<< incomingJobs[i].size << endl;
						lastEnd = incomingJobs[i].end;
					}
					else{
						FindExistFreeSpace(numOfFreeSpace, findExistFree, incomingJobs[i], Free, totalMemoryInUse);
						// if (i == 10) {
						// 		cout << "ahahahaha!!!!!!!!!!" << endl;
						// 		cout  << k <<"k" << endl;
						// 		cout << numOfFreeSpace << "numOfFreeSpace"<< endl;
						// 		cout << !findExistFree << "findExistFree" << endl;
						// }
						if(!findExistFree){
							cout << lastEnd << "lastEnd" << endl;
							incomingJobs[i].begin = lastEnd+1;
							totalMemoryInUse += incomingJobs[i].size;
							incomingJobs[i].end = lastEnd + incomingJobs[i].size;
							cout << incomingJobs[i].pid << '\t'
								<<  incomingJobs[i].begin << '\t'
								<< incomingJobs[i].end << '\t'
								<< incomingJobs[i].size << endl;
							lastEnd = incomingJobs[i].end;
							cout << lastEnd << "lastEnd" << endl;
							findExistFree = true;
							cout << i << "i"<< endl;
						}
					}
				}
				else{
					int pid = incomingJobs[i].pid;
					skippedJob.push_back(pid);
					numOfJobNotFit++;
				}
			}
		}
		else if(incomingJobs[i].op == 'D'){
			int pid = incomingJobs[i].pid;
			skippedThis = (skippedJob.end()!= (find(skippedJob.begin(), skippedJob.end(), pid)));
			if (skippedThis){
				cout << skippedThis << "skipped" << endl;
				skippedJob.push_back(pid);
				numOfJobNotFit++;
			}
			else{
				iter = find_if(incomingJobs.begin(), incomingJobs.end(),find_element( incomingJobs[i].pid));
				index = distance(incomingJobs.begin(), iter);
				cout<< endl;
				cout << "index: " << index << endl;
				nextFreeSpace.begin = incomingJobs[index].begin;
				nextFreeSpace.size = incomingJobs[index].size;
				totalMemoryInUse -= nextFreeSpace.size;
				Free.push_back(nextFreeSpace);
				cout << "Free" << '\t'
						<< Free[numOfFreeSpace].begin << '\t'
						<< incomingJobs[index].end << '\t'
						<< Free[numOfFreeSpace].size << endl;
				freeSpace = true;
				// numOfFreeSpace++;
			}
			

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
				// cout << nextJob.op << "\t"
				// 	<< nextJob.pid << "\t"
				// 	<< nextJob.size << endl;
			}
			else{
				fin	>> nextJob.pid;
				nextJob.size = 0;
				totalNumOfJob++;
				// cout << nextJob.op << "\t"
				// 	<< nextJob.pid << "\t"
				// 	<< nextJob.size << endl;
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

void FindExistFreeSpace(int& numOfFreeSpace, bool& findExistFree, Job& thisJob, vector<freeMem>& Free, int& totalMemoryInUse){
	int k=0;

	while(k <= Free.size() && !findExistFree)
	{	

		
		// if (i == 10) {
		// 	cout << "ahahahaha!!!!!!!!!!" << endl;
		// 	cout  << k <<"k" << endl;
		// 	cout << numOfFreeSpace << "numOfFreeSpace"<< endl;
		// }
		if (thisJob.size <=  Free[k].size){
			thisJob.begin = Free[k].begin;
			totalMemoryInUse += thisJob.size;
			thisJob.end = thisJob.begin + thisJob.size -1;
			cout << thisJob.pid << '\t'
			<<  thisJob.begin << '\t'
			<< thisJob.end << '\t'
			<< thisJob.size << endl;
			findExistFree = true;
			Free[k].begin += thisJob.size;
			Free[k].size -= thisJob.size;
			cout << findExistFree << "findExistFree" << endl;
			cout << numOfFreeSpace << "numOfFreeSpace"<< endl;
			cout  << k <<"k" << endl;
			// if(Free[k].size == 0) {
			// 	Free.erase(Free.begin()+k);
			// 	numOfFreeSpace--;
			// 	k++; 
			// }
		}
		k++;
	}
	cout << findExistFree << "findExistFree" << endl;
	cout << numOfFreeSpace << "numOfFreeSpace"<< endl;
	cout  << k <<"k" << endl;
	// findExistFree = false;
	
}