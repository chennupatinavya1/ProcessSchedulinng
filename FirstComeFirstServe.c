#include <stdio.h>
#include <stdlib.h>

typedef struct Process {
	int id;
	int arrivalTime;
	int burstTime;

	int executionStartTime;
} Process;

void takeInput();
void scheduleProcesses();
void sortProcessesAccordingToArrivalTime();
void executeProcesses();
void executeProcess(Process *process);
void printOutput();
void printProcess(Process process);
int max(int a, int b);

int processesCount;
Process *processes;
int currentTime = 0;

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
	executeProcesses();
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

void executeProcesses() {
	// Execution will be idle until first process arrives
	currentTime = processes[0].arrivalTime;
	
	for (int i = 0; i < processesCount; i++) {
		Process *currentProcess = processes + i;
		// Execution will be idle until next process arrives
		currentTime = max(currentTime, currentProcess->arrivalTime);
		executeProcess(currentProcess);
		currentTime += currentProcess->burstTime;
	}
}

int max(int a, int b) {
	return a > b ? a : b;
}

void executeProcess(Process *process) {
	process->executionStartTime = currentTime;
	//Process will execute here.	
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