#include <iostream>
#include <cmath>

using namespace std;

void firstComeFirstServed(int inputValues[], int headMovement[], int traversed[]);
void shortestSeekTimeFirst(int inputValues[], int headMovement[], int traversed[], int numInputs);
void elevator(int inputValues[], int headMovement[], int traversed[]);
void printResults(int headMovement[], int traversed[]);

/*  Pre: None
	Post: All array values are set to -1. */
void reinitialize(int headMovement[], int traversed[], const int MOVEMENTS, const int ARRAYSIZE);

const int NEGATIVE = -1;
const int initialPosition = 15;
const int ARRAYSIZE = 100;
const int MOVEMENTS = ARRAYSIZE + 1;
const int LARGENUM = 9999999;

int main() {
	int currentInput = 0;
	int numInputs = 0;
	int inputValues[ARRAYSIZE];
	int headMovement[MOVEMENTS];
	int traversed[ARRAYSIZE];

	// Initialize all arrays to ensure all values are negative.
	reinitialize(headMovement, traversed, MOVEMENTS, ARRAYSIZE);
	reinitialize(headMovement, inputValues, MOVEMENTS, ARRAYSIZE);
	
	while (currentInput >= 0 && numInputs < ARRAYSIZE) {
		cin >> currentInput; // Might want to use getline instead, test on server with standard input redirection.
		inputValues[numInputs] = currentInput;
		numInputs++;
	}

	firstComeFirstServed(inputValues, headMovement, traversed);
	cout << "\nFirst Come First Served\n" << endl;
	printResults(headMovement, traversed);
	reinitialize(headMovement, traversed, MOVEMENTS, ARRAYSIZE);

	shortestSeekTimeFirst(inputValues, headMovement, traversed, numInputs);
	cout << "\nShortest Seek Time First\n" << endl;
	printResults(headMovement, traversed);
	reinitialize(headMovement, traversed, MOVEMENTS, ARRAYSIZE);

	elevator(inputValues, headMovement, traversed);
	cout << "\nElevator (Scan-Look)\n" << endl;
	printResults(headMovement, traversed);
	reinitialize(headMovement, traversed, MOVEMENTS, ARRAYSIZE);

	return 0;
}

void reinitialize(int headMovement[], int traversed[], const int MOVEMENTS, const int ARRAYSIZE) {
	for (int i = 0; i < ARRAYSIZE; i++) {
		traversed[i] = NEGATIVE;
	}

	for (int i = 0; i < MOVEMENTS; i++) {
		headMovement[i] = NEGATIVE;
	}
}

void printResults(int headMovement[], int traversed[]) {
	int total = 0;
	cout << "Head Movement\t\tTracks Traversed" << endl;
	
	int i = 0;
	while (headMovement[i+1] >= 0) {
		cout << "Tracks " << headMovement[i] << " - " << headMovement[i+1] << "\t\t" << traversed[i] << endl;
		total += traversed[i];
		i++;
	}
	cout << "\nTotal Tracks Traversed\t" << total << endl;
}

void firstComeFirstServed(int inputValues[], int headMovement[], int traversed[]) {
	headMovement[0] = initialPosition;

	int i = 0;
	while (inputValues[i] >= 0) {
		headMovement[i+1] = inputValues[i];
		traversed[i] = (abs(headMovement[i] - headMovement[i+1]));
		i++;
	}
}

void shortestSeekTimeFirst(int inputValues[], int headMovement[], int traversed[], int numInputs) {
	headMovement[0] = initialPosition;
	int i = 0;
	int j = 0;
	int currentMin = LARGENUM;
	int foundAt = 0;
	int inputRemaining[ARRAYSIZE];

	memcpy(inputRemaining, inputValues, ARRAYSIZE); // Copy the input values into a modifiable array.

	while (inputRemaining[i] >= 0) { // Visit every value in the input array.
		while (inputRemaining[j] >= 0) { // For each value in the input array, check it against every other value in the input array. Complexity is n^2. 
			if (inputRemaining[j] != LARGENUM && (abs(headMovement[i] - inputRemaining[j])) < currentMin) { // If we have not already used this value and the value provides a new minimum.
				currentMin = abs(headMovement[i] - inputRemaining[j]); // The new minimum.
				foundAt = j; // The index at which the current minimum was found.
			}
			j++;
		}
		traversed[i] = currentMin; // Save the minimum traversal.
		headMovement[i+1] = inputRemaining[foundAt]; // Save the track we're traversing to.
		inputRemaining[foundAt] = LARGENUM; // Mark the current value as visited by replacing it with arbitrarily large value LARGENUM.
		currentMin = LARGENUM; // Reset the current minimum.
		j = 0; // Reset the inner loop's index.
		i++;
	}
	return;
}

void elevator(int inputValues[], int headMovement[], int traversed[]) {
	bool direction = 1; // 1 for moving up through increasing tracks, 0 for moving down through decreasing tracks. We begin moving up.
	bool foundValue = 0;
	bool foundValuePreviousDirection = 0;
	headMovement[0] = initialPosition;
	int currentMin = LARGENUM;
	int foundAt = 0;
	int inputRemaining[ARRAYSIZE];
	int i = 0;
	int j = 0;

	memcpy(inputRemaining, inputValues, ARRAYSIZE); // Copy the input values into a modifiable array.

	while (i <= MOVEMENTS) { // Could do inputRemaining[i] >= 0, but unnecessary since loops control the return.
		if (direction == 1) {
			while (inputRemaining[j] >= 0) { // For each value in the input array, check it against every other value in the input array. 
				if (inputRemaining[j] != LARGENUM && (abs(headMovement[i] - inputRemaining[j])) < currentMin && inputRemaining[j] >= headMovement[i]) { // If we have not already used this value and the value provides a new minimum.
					currentMin = abs(headMovement[i] - inputRemaining[j]); // The new minimum.
					foundAt = j; // The index at which the current minimum was found.
					foundValue = 1;
				}
				j++;
			}

			if (foundValue == 1) {
				traversed[i] = currentMin; // Save the minimum traversal.
				headMovement[i+1] = inputRemaining[foundAt]; // Save the track we're traversing to.
				inputRemaining[foundAt] = LARGENUM; // Mark the current value as visited by replacing it with arbitrarily large value LARGENUM.
				currentMin = LARGENUM; // Reset the current minimum.
				foundValuePreviousDirection = 1;
				foundValue = 0;
				j = 0; // Reset the inner loop's index.
				i++;
			}

			else if (foundValue == 0 && foundValuePreviousDirection == 0) {
				return;
			}

			else {
				direction = 0;
				foundValuePreviousDirection = 0;
				j = 0; // Reset test loop index.
			}
		}

		else if (direction == 0) {
			while (inputRemaining[j] >= 0) { // For each value in the input array, check it against every other value in the input array. Complexity is n^2. 
				if (inputRemaining[j] != LARGENUM && (abs(headMovement[i] - inputRemaining[j])) < currentMin && inputRemaining[j] <= headMovement[i]) { // If we have not already used this value and the value provides a new minimum.
					currentMin = abs(headMovement[i] - inputRemaining[j]); // The new minimum.
					foundAt = j; // The index at which the current minimum was found.
					foundValue = 1;
				}
				j++;
			}

			if (foundValue == 1) {
				traversed[i] = currentMin; // Save the minimum traversal.
				headMovement[i+1] = inputRemaining[foundAt]; // Save the track we're traversing to.
				inputRemaining[foundAt] = LARGENUM; // Mark the current value as visited by replacing it with arbitrarily large value LARGENUM.
				currentMin = LARGENUM; // Reset the current minimum.
				foundValuePreviousDirection = 1;
				foundValue = 0;
				j = 0; // Reset the inner loop's index.
				i++;
			}

			else if (foundValue == 0 && foundValuePreviousDirection == 0) {
				return;
			}

			else {
				direction = 1;
				foundValuePreviousDirection = 0;
				j = 0; // Reset test loop index.
			}
		}
	}
}