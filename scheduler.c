#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// FIFO scheduler that uses dynamically allocated linked lists where each job has information stored by the user.

//////////////////////
// data definitions //
//////////////////////

#define NAME_LEN 20
#define SIZE_LEN 3

struct job
{
	char job_name[NAME_LEN + 1], user_name[NAME_LEN + 1];
	int num_cpus, num_gpus, memory, priority;
	double time;
	struct job *next;
};

/////////////////////////
// function prototypes //
/////////////////////////

void help();
void read(char *job_name, char *user_name, int *num_cpus, int *num_gpus, int *memory, double *time, int *priority);
struct job *add_job(struct job *scheduler, char *job_name, char *user_name, int num_cpus, int num_gpus, int memory, double time, int priority);
struct job *pop_job(struct job *scheduler);
void list_user(struct job *scheduler, char *user_name);
void list_jobs(struct job *scheduler, int num_cpus, int num_gpus, int memory, double time);
void list_all_jobs(struct job *scheduler);
struct job *clear_jobs(struct job *scheduler);

///////////////////
// main function //
///////////////////

int main()
{
	char code;
	char job_name[NAME_LEN + 1], user_name[NAME_LEN + 1];
	int num_cpus, num_gpus, memory, priority;
	double time;

	struct job *scheduler = NULL;

	help();
	printf("\n");

	for (;;)
	{
		// read operation code
		printf("Enter operation code: ");
		scanf(" %c", &code);
		while (getchar() != '\n') /* skips to end of line */
			;

		// run operation
		switch (code)
		{
		case 'h':
			help();
			break;
		case 'a':
			read(job_name, user_name, &num_cpus, &num_gpus, &memory, &time, &priority);
			scheduler = add_job(scheduler, job_name, user_name, num_cpus, num_gpus, memory, time, priority);
			break;
		case 'p':
			scheduler = pop_job(scheduler);
			break;
		case 'u':
			read(NULL, user_name, NULL, NULL, NULL, NULL, NULL);
			list_user(scheduler, user_name);
			break;
		case 'j':
			read(NULL, NULL, &num_cpus, &num_gpus, &memory, &time, NULL);
			list_jobs(scheduler, num_cpus, num_gpus, memory, time);
			break;
		case 'l':
			list_all_jobs(scheduler);
			break;
		case 'q':
			scheduler = clear_jobs(scheduler);
			return 0;
		default:
			printf("Illegal operation code!\n");
		}
		printf("\n");
	}
}

//////////////////////////
// function definitions //
//////////////////////////

void help()
{
	printf("List of operation codes:\n");
	printf("\t'h' for help;\n");
	printf("\t'a' for adding a job to the scheduler;\n");
	printf("\t'p' for removing a job from the scheduler;\n");
	printf("\t'u' for searching jobs per user;\n");
	printf("\t'j' for searching jobs per capacity;\n");
	printf("\t'l' for listing all jobs;\n");
	printf("\t'q' for quit.\n");
}

void read(char *job_name, char *user_name, int *num_cpus, int *num_gpus, int *memory, double *time, int *priority)
{
	if (job_name != NULL)
	{
		printf("Enter the name of the job: ");
		scanf("%s", job_name);
	}
	if (user_name != NULL)
	{
		printf("Enter the name of the user: ");
		scanf("%s", user_name);
	}
	if (num_cpus != NULL)
	{
		printf("Enter the number of CPUs: ");
		scanf("%d", num_cpus);
	}
	if (num_gpus != NULL)
	{
		printf("Enter the number of GPUs: ");
		scanf("%d", num_gpus);
	}
	if (memory != NULL)
	{
		printf("Enter the amount of memory: ");
		scanf("%d", memory);
	}
	if (time != NULL)
	{
		printf("Enter the amount of time: ");
		scanf("%lf", time);
	}
	if (priority != NULL)
	{
		printf("Enter the priority: ");
		scanf("%d", priority);
	}
}

/////////////////////////////////////////////////////////
// WARNING - WARNING - WARNING - WARNING - WARNING     //
/////////////////////////////////////////////////////////
// Do not modify anything before this point, otherwise //
// your solution will be considered incorrect.         //
/////////////////////////////////////////////////////////

struct job *add_job(struct job *scheduler, char *job_name, char *user_name, int num_cpus, int num_gpus, int memory, double time, int priority)
{
	struct job *new_job = (struct job *)malloc(sizeof(struct job)); // allocate memory for new job
	if (new_job == NULL)
	{
		printf("Error: failed to allocate memory for new job.");
		return scheduler;
	}

	// copy job information into new job
	strcpy(new_job->job_name, job_name);
	strcpy(new_job->user_name, user_name);
	new_job->num_cpus = num_cpus;
	new_job->num_gpus = num_gpus;
	new_job->memory = memory;
	new_job->time = time;
	new_job->priority = priority;
	new_job->next = NULL;

	if (scheduler == NULL) // if the scheduler is empty, set the scheduler to the new job
	{
		scheduler = new_job;
	}
	else
	{
		// find the last job in the list
		struct job *current_job = scheduler;
		while (current_job->next != NULL)
		{
			current_job = current_job->next;
		}
		current_job->next = new_job; // add the new job to the end of the list
	}
	return scheduler;
}

struct job *pop_job(struct job *scheduler)
{
	if (scheduler == NULL) // the scheduler is empty
	{
		return NULL;
	}
	struct job *next_job = scheduler;
	scheduler = scheduler->next;

	// output format
	printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
	printf("| Job name             | User name            | CPUs | GPUs | Mem. | Time   | Priority |\n");
	printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
	printf("| %-20s | %-20s | %4d | %4d | %4d | %6.2f | %8d |\n", next_job->job_name, next_job->user_name, next_job->num_cpus, next_job->num_gpus, next_job->memory, next_job->time, next_job->priority);
	printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
	free(next_job);	  // free memory
	return scheduler; // return the updated scheduler
}

void list_user(struct job *scheduler, char *user_name)
{
	int found = 0;

	while (scheduler != NULL) // Iterate through the scheduler list
	{
		if (strcmp(scheduler->user_name, user_name) == 0) // Check if they equal 0
		{
			if (!found) // If this is the first job from the user, print the header
			{
				printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
				printf("| Job name             | User name            | CPUs | GPUs | Mem. | Time   | Priority |\n");
				printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
				found = 1;
			}
			// Print the job details
			printf("| %-20s | %-20s | %4d | %4d | %4d | %6.2f | %8d |\n", scheduler->job_name, scheduler->user_name, scheduler->num_cpus, scheduler->num_gpus, scheduler->memory, scheduler->time, scheduler->priority);
			printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
		}

		scheduler = scheduler->next; // Move to the next job in the list
	}
	if (!found) // If no jobs were found, do nothing
	{
		return;
	}
}

void list_jobs(struct job *scheduler, int num_cpus, int num_gpus, int memory, double time)
{
	int found = 0;
	struct job *p = scheduler;
	while (p != NULL) // loops till p is not null
	{
		if (p->num_cpus <= num_cpus && p->num_gpus <= num_gpus && p->memory <= memory && p->time <= time)
		{
			if (!found) // If this is the first job from the user, print the header
			{
				printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
				printf("| Job name             | User name            | CPUs | GPUs | Mem. | Time   | Priority |\n");
				printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
				found = 1;
			}
			printf("| %-20s | %-20s | %4d | %4d | %4d | %6.2f | %8d |\n", p->job_name, p->user_name, p->num_cpus, p->num_gpus, p->memory, p->time, p->priority);
			printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
		}
		p = p->next;
	}

	if (!found) // If no jobs met the criteria, does nothing
	{
		return;
	}
}

void list_all_jobs(struct job *scheduler)
{
	if (scheduler == NULL) // If the scheduler is empty, return immediately
	{
		return;
	}
	printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
	printf("| Job name             | User name            | CPUs | GPUs | Mem. | Time   | Priority |\n");
	printf("|----------------------|----------------------|------|------|------|--------|----------|\n");

	struct job *job_ptr = scheduler; // Traverse the scheduler list and print each job's information

	while (job_ptr != NULL)
	{
		printf("| %-20s | %-20s | %4d | %4d | %4d | %6.2f | %8d |\n", job_ptr->job_name, job_ptr->user_name, job_ptr->num_cpus, job_ptr->num_gpus, job_ptr->memory, job_ptr->time, job_ptr->priority);
		job_ptr = job_ptr->next;
		printf("|----------------------|----------------------|------|------|------|--------|----------|\n");
	}
}

struct job *clear_jobs(struct job *scheduler)
{
	struct job *p;			  // Declare a pointer to a job struct
	while (scheduler != NULL) // Loops till the scheduler pointer is not NULL
	{
		p = scheduler;				 // Assign the scheduler pointer to the temporary pointer p
		scheduler = scheduler->next; // Move the scheduler pointer to the next job in the list
		if (scheduler != NULL)		 // If the scheduler pointer is not NULL
			free(p);				 // Free the memory associated with the job struct pointed to by p
	}
	return NULL; // Return NULL as the new scheduler pointer, since all jobs have been cleared
}
