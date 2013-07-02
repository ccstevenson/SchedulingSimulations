#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

void firstComeFirstServed(vector<int> processID, vector<int> arrivalTime, vector<int> timeToCompletion);
void shortestJobFirst(vector<int> processID, vector<int> arrivalTime, vector<int> timeToCompletion);
void shortestRemainingTimeNext(vector<int> processID, vector<int> arrivalTime, vector<int> timeToCompletion);
void roundRobin(vector<int> processID, vector<int> arrivalTime, vector<int> timeToCompletion, const float CONTEXTSWITCH);

const int LARGENUM = 9999999;

int main() {
	const float NOCONTEXTSWITCH = 0.0;
	const float CONTEXTSWITCH = 0.4;
	vector<int> processID;
	vector<int> arrivalTime;
	vector<int> timeToCompletion;
	int currentProcessID;
	int currentArrivalTime;
	int currentTimeToCompletion;
	string currentLine;

	// read from stdin
	while (getline(cin, currentLine)) { // copy the current line. Break from loop on EOF (ctrl-d on Unix, ctrl-z on Windows)
		stringstream line; // create a string stream local to the loop
		line.str(currentLine); // copy the line into string stream
		// read each integer from the stream into the appropriate variable
		line >> currentProcessID; 
		line >> currentArrivalTime;
		line >> currentTimeToCompletion;
		// copy the variables into their appropriate vectors
		processID.push_back(currentProcessID);
		arrivalTime.push_back(currentArrivalTime);
		timeToCompletion.push_back(currentTimeToCompletion);
	}

	firstComeFirstServed(processID, arrivalTime, timeToCompletion);
	shortestJobFirst(processID, arrivalTime, timeToCompletion);
	shortestRemainingTimeNext(processID, arrivalTime, timeToCompletion);
	roundRobin(processID, arrivalTime, timeToCompletion, NOCONTEXTSWITCH);
	roundRobin(processID, arrivalTime, timeToCompletion, CONTEXTSWITCH);

	return 0;
}

void firstComeFirstServed(vector<int> processID, vector<int> arrivalTime, vector<int> timeToCompletion) {
	int wait = 0;
	int turnaround = 0;
	int timeOfCompletion = 0;
	float averageWait = 0;
	float averageTurnaround = 0;

	cout << "First Come First Served\nPID\tWait\tTurnaround\n--------------------------" << endl;

	for(int i = 0; i < processID.size(); i++) {
		if (i > 0) {
			timeOfCompletion += timeToCompletion[i];
			wait = (timeOfCompletion - (arrivalTime[i] + timeToCompletion[i]));
			if (wait < 0)
				wait = 0;
		}
		else {
			wait = 0;
			timeOfCompletion = timeToCompletion[i];
		}

		turnaround = wait + timeToCompletion[i];
		averageWait += wait;
		averageTurnaround += turnaround;

		cout << processID[i] << "\t" << wait << "\t" << turnaround << endl;
	}

	averageWait = (averageWait / (processID.size())); 
	averageTurnaround = (averageTurnaround / (processID.size()));

	cout << "Average wait: " << averageWait << " Average turnaround: " << averageTurnaround << "\n" << endl;
}

void shortestJobFirst(vector<int> processID, vector<int> arrivalTime, vector<int> timeToCompletion) {
	int timeOfCompletion = 0;
	int shortestExecutionTime = LARGENUM; 
	int shortestJob = 0;
	int alreadyExecuted = 0;
	int wait;
	int turnaround;
	float averageWait = 0;
	float averageTurnaround = 0;
	vector<int> orderOfExecution; // Since processes are printed in the order of their process id, we cannot print as we execute processes; instead, we save the execution order here.
	vector<int> waitTimes;
	vector<int> turnaroundTimes;

	// Initialize vectors
	for (int i = 0; i < processID.size(); i++) {
		waitTimes.push_back(LARGENUM);
		turnaroundTimes.push_back(LARGENUM);
	}

	cout << "Shortest Job First\nPID\tWait\tTurnaround\n--------------------------" << endl;

	for (int i = 0; i < processID.size(); i++) {
		for(int j = 0; j < processID.size(); j++) {
			if (arrivalTime[j] <= timeOfCompletion) { // The process is currently in the queue and is eligible to be checked in the algorithm.
				if (timeToCompletion[j] < shortestExecutionTime) { // Determine if the eligible process requires less time than those previously checked.
					for (int k = 0; k < orderOfExecution.size(); k++) { // Determine if the eligible process has not already been executed.
						if (j == orderOfExecution[k])
							alreadyExecuted = 1;
					}

					if (alreadyExecuted == 0) { // The process has the shortest execution time and has not previously been executed.
						shortestExecutionTime = timeToCompletion[j];
						shortestJob = j;
					}
					else
						alreadyExecuted = 0;
				}
			}
		}
		timeOfCompletion += timeToCompletion[shortestJob];
		orderOfExecution.push_back(shortestJob); // Add the shortest job to the executed vector.
		shortestExecutionTime = LARGENUM; // Reset shortest execution time.
	}

	// We now have the execution order. Determine wait and turnaround times.
	for(int i = 0; i < processID.size(); i++) {
		int n = orderOfExecution[i];
		if (i > 0) {
			timeOfCompletion += timeToCompletion[n];
			wait = (timeOfCompletion - (arrivalTime[n] + timeToCompletion[n]));
		}
		else {
			wait = 0;
			timeOfCompletion = timeToCompletion[n];
		}

		turnaround = wait + timeToCompletion[n];
		averageWait += wait;
		averageTurnaround += turnaround;

		waitTimes[n] = wait;
		turnaroundTimes[n] = turnaround;
	}

	// Print results
	for(int i = 0; i < processID.size(); i++) {
		cout << processID[i] << "\t" << waitTimes[i] << "\t" << turnaroundTimes[i] << endl;
	}
	
	averageWait = (averageWait / (processID.size())); 
	averageTurnaround = (averageTurnaround / (processID.size()));
	cout << "Average wait: " << averageWait << " Average turnaround: " << averageTurnaround << "\n" << endl;
}

void shortestRemainingTimeNext(vector<int> processID, vector<int> arrivalTime, vector<int> timeToCompletion) {
	vector<int> readyList; // Holds the indices of the processes which have arrived.
	vector<int> turnaround; // Holds the turnaround times for each process.
	vector<int> wait; // Holds the wait times for each process.
	vector<int> processesRemaining = processID;
	vector<int> ranOnce;
	int milliseconds = 0; // The current simulated time.
	int leastTimeRemaining = LARGENUM; // The currently running process' time remaining.
	int currentlyRunning; // The PID of the currently running process.
	float averageWait = 0;
	float averageTurnaround = 0;

	// Initialize vectors.
	for (int i = 0; i < processID.size(); i++) {
		readyList.push_back(0);
		turnaround.push_back(0);
		wait.push_back(0);
		ranOnce.push_back(0);
	}

	while (!processesRemaining.empty()) { // We remove the process ID from processesRemaining each time a process is completed.
		for (int i = 0; i < readyList.size(); i++) { // Check all processes for ready list eligibility.
			if (arrivalTime[i] <= milliseconds && readyList[i] != LARGENUM) { // Determine if the process is eligible to be put in the ready list.
				readyList[i] = 1; // Set the readyList flag to true. The process is in the queue.
			}
		}
	
		for (int i = 0; i < readyList.size(); i++) { // Check all ready processes for the one with the least time remaining.
			if (readyList[i] == 1) { // The i'th process is in the queue.
				if (timeToCompletion[i] < leastTimeRemaining) { // Strictly less than. If equal to, we remain with the current process and don't perform a context switch.
					leastTimeRemaining = timeToCompletion[i];
					currentlyRunning = i;
				}
			}
		}

		ranOnce[currentlyRunning] = 1; // The process is no longer waiting.

		for (int i = 0; i < readyList.size(); i++) { 
			if (readyList[i] == 1) { // Determine if the process is in the ready queue.
				turnaround[i]++; // Increment the turnaround time of all processes in the ready queue.
				if (ranOnce[i] != 1) { // The process has never been run
					wait[i]++; // Increment the wait time of all processes waiting in the ready queue.
				}
			}
		}

		timeToCompletion[currentlyRunning]--; // We have performed 1 ms of work on the currently running process.
		milliseconds++; // A millisecond has passed.

		if (timeToCompletion[currentlyRunning] == 0) { // If the currently running process is now finished, remove it from the ready queue.
			readyList[currentlyRunning] = LARGENUM;
			timeToCompletion[currentlyRunning] = LARGENUM;
			for (int i = 0; i < processesRemaining.size(); i++) {
				if (processesRemaining[i] == (currentlyRunning + 1)) {
					processesRemaining.erase(processesRemaining.begin()+i);
				}
			}
			leastTimeRemaining = LARGENUM; // Reset the least time remaining.
		}
	}

	cout << "Shortest Remaining Time Next\nPID\tWait\tTurnaround\n--------------------------" << endl;

	// Print results
	for(int i = 0; i < processID.size(); i++) {
		cout << processID[i] << "\t" << wait[i] << "\t" << turnaround[i] << endl;
		averageWait += wait[i];
		averageTurnaround += turnaround[i];
	}

	averageWait = (averageWait / (processID.size())); 
	averageTurnaround = (averageTurnaround / (processID.size()));

	cout << "Average wait: " << averageWait << " Average turnaround: " << averageTurnaround << "\n" << endl;
}

void roundRobin(vector<int> processID, vector<int> arrivalTime, vector<int> timeToCompletion, const float CONTEXTSWITCH) {
	vector<float> turnaround; // Holds the turnaround times for each process.
	vector<float> wait; // Holds the wait times for each process.
	vector<int> readyList; // The ready 'queue'.
	vector<int> previouslyQueued; // Indicates that the process has entered the queue at some point and should not do so again.
	vector<int> processesRemaining = processID;
	vector<int> ranOnce;
	float averageWait = 0;
	float averageTurnaround = 0;
	float milliseconds = 0;
	float msElapsed = 0;


	for (int i = 0; i < processID.size(); i++) { // Initialize vectors.
		turnaround.push_back(0);
		wait.push_back(0);
		previouslyQueued.push_back(0);
		ranOnce.push_back(0);
	}

	// Perform an initial check for processes ready at 0 ms.
	for (int i = 0; i < processID.size(); i++) { // Check all processes for ready list eligibility.
		if (arrivalTime[i] <= milliseconds && previouslyQueued[i] != 1) { // Determine if the process is eligible to be put in the ready list.
			readyList.push_back(i); // Put the index of the ready process in the queue. Since file is in ascending arrival time, don't need to worry about ordering.
			previouslyQueued[i] = 1; // Set the previouslyQueued flag to true. The process is in the queue.
		}
	}

	while (!processesRemaining.empty()) { // We remove the process ID from the list of processes each time a process is completed.

		while (timeToCompletion[readyList[0]] > 0 && msElapsed < 4) { // Give the process at the head of the ready queue up to 4 ms of processing time.
			timeToCompletion[readyList[0]]--;
			msElapsed++;
		}

		ranOnce[readyList[0]] = 1; // The process is no longer waiting.

		milliseconds += msElapsed; // Update the time.

		// Put any new processes in the ready queue.
		for (int i = 0; i < processID.size(); i++) { // Check all processes for ready list eligibility.
			if (arrivalTime[i] <= milliseconds && previouslyQueued[i] != 1) { // Determine if the process is eligible to be put in the ready list.
				readyList.push_back(i); // Put the index of the ready process in the queue. Since file is in ascending arrival time, don't need to worry about ordering.
				previouslyQueued[i] = 1; // Set the previouslyQueued flag to true. The process is in the queue.
			}
		}

		for (int i = 0; i < readyList.size(); i++) { 
			if (turnaround[readyList[i]] == 0) // The process arrived within the last quantum.
				turnaround[readyList[i]] += (milliseconds - arrivalTime[readyList[i]]);
			else
				turnaround[readyList[i]] += (msElapsed + CONTEXTSWITCH); // Increment the turnaround time of all processes in the ready queue.

			if (ranOnce[readyList[i]] != 1) { // The process has never been run
				if (wait[readyList[i]] == 0) // The process arrived within the last quantum.
					wait[readyList[i]] += (milliseconds - arrivalTime[readyList[i]] + CONTEXTSWITCH); // Increment the wait time.
				else
					wait[readyList[i]] += (msElapsed + CONTEXTSWITCH);
			}
		}

		if (timeToCompletion[readyList[0]] > 0) { // If the process was not completed in this time quantum . . .
			// Put the recently executed process at the end of the queue and remove it from the head.
			readyList.push_back(readyList[0]);
			readyList.erase(readyList.begin()); // Remove the head of the queue.
		}
		else { // The process was completed in this time quantum.
			for (int i = 0; i < processesRemaining.size(); i++) { // Remove the process from processesRemaining.
				if (processesRemaining[i] == (readyList[0] + 1)) {
					processesRemaining.erase(processesRemaining.begin()+i);
				}
			}
			readyList.erase(readyList.begin()); // Remove the head of the queue.
		}
		milliseconds += CONTEXTSWITCH;
		msElapsed = 0; // Reset ms elapsed during this time quantum.
	}

	if (CONTEXTSWITCH > 0)
		cout << "Round Robin with Context Switches\nPID\tWait\tTurnaround\n--------------------------" << endl;
	else 
		cout << "Round Robin\nPID\tWait\tTurnaround\n--------------------------" << endl;

	// Print results
	for(int i = 0; i < processID.size(); i++) {
		cout << processID[i] << "\t" << wait[i] << "\t" << turnaround[i] << endl;
		averageWait += wait[i];
		averageTurnaround += turnaround[i];
	}

	averageWait = (averageWait / (processID.size())); 
	averageTurnaround = (averageTurnaround / (processID.size()));

	cout << "Average wait: " << averageWait << " Average turnaround: " << averageTurnaround << "\n" << endl;
}