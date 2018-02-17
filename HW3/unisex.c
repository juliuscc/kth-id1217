#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <semaphore.h>

#define MAXNUMBEROFMEN 5   /* maximum number of men */
#define MAXNUMBEROFWOMEN 5 /* maximum number of women */
#define MAXVISITS 20	   /* maximum number of visits tp bath each person makes */

#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define CYN "\x1B[36m"
#define RESET "\x1B[0m"

// int numMen, numWomen, numVisits;

sem_t *critical_mutex;
sem_t *men_delay;
sem_t *women_delay;

int men_in_bathroom = 0;
int women_in_bathroom = 0;
int men_waiting = 0;
int women_waiting = 0;

/* When the first man leaves no man can enter before women has gotten a chanse and vise versa */
bool men_shrinking = false;   /* Used for 2 phase locking */
bool women_shrinking = false; /* Used for 2 phase locking */

int numMen = 0;	/* How many men */
int numWomen = 0;  /* How many women */
int numVisits = 0; /* How many times does each person visit the toilet */

void *Man(void *arg)
{
	int id = (int)arg;
	int i;
	for (i = 0; i < numVisits; i++)
	{
		/* Do things before entering bathroom */
		sleep(rand() % 4);

		/* Should I wait for bathroom + other stuff */
		sem_wait(critical_mutex);
		if (women_in_bathroom > 0 || men_shrinking)
		{
			men_waiting++;
			sem_post(critical_mutex);
			sem_wait(men_delay);
		}
		men_in_bathroom++;
		if (men_waiting > 0)
		{
			men_waiting--;
			sem_post(men_delay);
		}
		else
		{
			sem_post(critical_mutex);
		}

		/* Go to bathroom */
		printf("🚹  %d enters the bathroom. Visit: " CYN "%d\n" RESET, id, i + 1);
		sleep(rand() % 3);

		/* What to do after finished with bathroom visit */
		sem_wait(critical_mutex);
		men_in_bathroom--;
		men_shrinking = true;

		/* Make sure no more men enter while men leave */
		if (men_in_bathroom == 0)
			men_shrinking = false;

		/* Prioritize women */
		if (men_in_bathroom == 0 && women_waiting > 0)
		{
			women_waiting--;
			sem_post(women_delay);
		}
		else
		{
			sem_post(critical_mutex);
		}
	}
	pthread_exit(NULL);
}

void *Woman(void *arg)
{
	int id = (int)arg;
	int i;
	for (i = 0; i < numVisits; i++)
	{
		/* Do things before entering bathroom */
		sleep(rand() % 4);

		/* Should I wait for bathroom + other stuff */
		sem_wait(critical_mutex);
		if (men_in_bathroom > 0 || women_shrinking)
		{
			women_waiting++;
			sem_post(critical_mutex);
			sem_wait(women_delay);
		}
		women_in_bathroom++;
		if (women_waiting > 0)
		{
			women_waiting--;
			sem_post(women_delay);
		}
		else
		{
			sem_post(critical_mutex);
		}

		/* Go to bathroom */
		printf("🚺  %d enters the bathroom. Visit:" YEL "%d\n" RESET, id, i + 1);
		sleep(rand() % 3);

		/* What to do after finished with bathroom visit */
		sem_wait(critical_mutex);
		women_in_bathroom--;

		/* Make sure no more women enter while women leave */
		women_shrinking = true;
		if (women_in_bathroom == 0)
			women_shrinking = false;

		/* Prioritize men */
		if (women_in_bathroom == 0 && men_waiting > 0)
		{
			men_waiting--;
			sem_post(men_delay);
		}
		else
		{
			sem_post(critical_mutex);
		}
	}
	pthread_exit(NULL);
}

int main()
{
	/* Get arguments */
	while (numMen <= 0 || numMen > MAXNUMBEROFMEN)
	{
		printf("Enter the number of men: (max %d) ", MAXNUMBEROFMEN);
		scanf("%d", &numMen);
	}
	while (numWomen <= 0 || numWomen > MAXNUMBEROFWOMEN)
	{
		printf("Enter the number of women: (max %d) ", MAXNUMBEROFWOMEN);
		scanf("%d", &numWomen);
	}
	while (numVisits <= 0 || numVisits > MAXVISITS)
	{
		printf("Enter the number of visits per person: (max %d) ", MAXVISITS);
		scanf("%d", &numVisits);
	}
	printf("\n");

	/* Init semaphores */
	critical_mutex = sem_open("/critical_mutex", O_EXCL, S_IWOTH, 1);
	men_delay = sem_open("/men_delay", O_EXCL, S_IWOTH, 0);
	women_delay = sem_open("/women_delay", O_EXCL, S_IWOTH, 0);

	/* Create threads */
	pthread_t threads_men[numMen], threads_women[numWomen];
	int i;
	for (i = 0; i < numMen; i++)
		pthread_create(&threads_men[i], NULL, Man, (void *)i);
	for (i = 0; i < numWomen; i++)
		pthread_create(&threads_women[i], NULL, Woman, (void *)i);

	/* Join threads */
	for (i = 0; i < numMen; i++)
		pthread_join(threads_men[i], NULL);
	for (i = 0; i < numWomen; i++)
		pthread_join(threads_women[i], NULL);

	/* Free semaphore */
	sem_close(critical_mutex);
	sem_close(men_delay);
	sem_close(women_delay);
}