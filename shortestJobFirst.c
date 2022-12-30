#include <stdio.h>
#include <stdlib.h>

typedef struct Process {
	int id;
	int arrivalTime;
	int burstTime;

	int executionStartTime;
} Process;

typedef struct Window {
	int start;
	int end;
} Window;

void takeInput();
void scheduleProcesses();
void sortProcessesAccordingToArrivalTime();
void initializeUnExecutedProcessesWindow();
int unExecutedProcessesWithInCurrentTimeWindowSize();
void updateCurrentTimeToLeastArrivalTimeOfUnExecutedProcesses();
void expandUnExecutedProcessesWithInCurrentTimeWindow();
Process *findProcessWithShortestBurstTimeWithinWindow();
void executeProcess(Process *process);
void swapExecutedProcessWithWindowStart(Process *executedProcess);
void removeExecutedProcessFromUnExceutedProcessesWithInCurrentTimeWindow();
void printOutput();
void printProcess(Process process);
int max(int a, int b);

int processesCount;
Process *processes;
int currentTime = 0;
Window unExecutedProcessesWithInCurrentTime;

int main(void) {
	takeInput();
	scheduleProcesses();
	printOutput();
}

void takeInput() {
	scanf("%d", &processesCount);
	processes = (Process *) malloc(sizeof(Process) * processesCount);

	for (int i = 0; i < processesCount; i++) {
		Process *currentProcess = processes + i;

		currentProcess->id = i;
		scanf("%d", &currentProcess->arrivalTime);
		scanf("%d", &currentProcess->burstTime);
	}
}

void scheduleProcesses() {
	sortProcessesAccordingToArrivalTime();
	initializeUnExecutedProcessesWindow();
	updateCurrentTimeToLeastArrivalTimeOfUnExecutedProcesses();
	expandUnExecutedProcessesWithInCurrentTimeWindow();
	
	while (unExecutedProcessesWithInCurrentTimeWindowSize() > 0) {
		Process *nextProcessToBeExecuted = findProcessWithShortestBurstTimeWithinWindow();
		executeProcess(nextProcessToBeExecuted);
		swapExecutedProcessWithWindowStart(nextProcessToBeExecuted);
		removeExecutedProcessFromUnExceutedProcessesWithInCurrentTimeWindow();
		updateCurrentTimeToLeastArrivalTimeOfUnExecutedProcesses();
		expandUnExecutedProcessesWithInCurrentTimeWindow();
	}
}

void removeExecutedProcessFromUnExceutedProcessesWithInCurrentTimeWindow() {
	unExecutedProcessesWithInCurrentTime.start++;
}

void swapExecutedProcessWithWindowStart(Process *executedProcess) {
	Process temp = processes[unExecutedProcessesWithInCurrentTime.start];
	processes[unExecutedProcessesWithInCurrentTime.start] = *executedProcess;
	*executedProcess = temp;
}

void executeProcess(Process *process) {
	process->executionStartTime = currentTime;
	currentTime += process->burstTime;
}

Process *findProcessWithShortestBurstTimeWithinWindow() {
	int windowStart = unExecutedProcessesWithInCurrentTime.start;
	int windowEnd = unExecutedProcessesWithInCurrentTime.end;
	Process *resultProcess = &processes[windowStart];
	
	for (int i = windowStart + 1; i <= windowEnd; i++) {
		if (processes[i].burstTime < resultProcess->burstTime)
			resultProcess = &processes[i];
	}

	return resultProcess;
}

void updateCurrentTimeToLeastArrivalTimeOfUnExecutedProcesses() {
	// CPU will be idle from currentTime to arrival time of next to-be executed process.
	currentTime = max(currentTime, processes[unExecutedProcessesWithInCurrentTime.start].arrivalTime); 
}

void expandUnExecutedProcessesWithInCurrentTimeWindow() {
	int windowEnd = unExecutedProcessesWithInCurrentTime.end;
	while (windowEnd < processesCount - 1 && processes[windowEnd + 1].arrivalTime <= currentTime)
		windowEnd++;
	unExecutedProcessesWithInCurrentTime.end = windowEnd;
}

int unExecutedProcessesWithInCurrentTimeWindowSize() {
	return unExecutedProcessesWithInCurrentTime.end - unExecutedProcessesWithInCurrentTime.start + 1;
}

void initializeUnExecutedProcessesWindow() {
	unExecutedProcessesWithInCurrentTime.start = 0;
	unExecutedProcessesWithInCurrentTime.end = 0;
}

void sortProcessesAccordingToArrivalTime() {
	for (int i = 1; i < processesCount; ++i) {	
		int j = i - 1;
		while (j >= 0 && processes[j].arrivalTime > processes[i].arrivalTime) j--;
		// j + i is our destination position
		Process currentInsertingElement = processes[i];
		for (int k = i - 1; k >= j + 1; k--) {
			processes[k + 1] = processes[k];
		}
		processes[j + 1] = currentInsertingElement;
	}
}

int max(int a, int b) {
	return a > b ? a : b;
}

void printOutput() {
	for (int i = 0; i < processesCount; i++) {
		Process currentProcess = processes[i];
		printProcess(currentProcess);
		printf("\n");
	}
}

void printProcess(Process process) {
	printf("Process Id: %d\n", process.id);
	printf("Arrival Time: %d\n", process.arrivalTime);
	printf("Burst Time: %d\n", process.burstTime);
	printf("Execution Start Time: %d\n", process.executionStartTime);
	printf("Execution End Time: %d\n", process.executionStartTime + process.burstTime);
	printf("Response Time: %d\n", process.executionStartTime - process.arrivalTime);
	printf("Turn Around Time: %d\n", process.executionStartTime + process.burstTime - process.arrivalTime);
}