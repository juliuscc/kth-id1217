#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>

#define EPSILON (double)1.0E-4
#define LEFT (double)0.0
#define RIGHT (double)1.0
#define NP 4

/* timer */
double read_timer()
{
	static bool initialized = false;
	static struct timeval start;
	struct timeval end;
	if (!initialized)
	{
		gettimeofday(&start, NULL);
		initialized = true;
	}
	gettimeofday(&end, NULL);
	return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

/* function used */
double f(double x)
{
	return sqrt(1 - (x * x));
}

/* struct for tasks*/
typedef struct
{
	double left;
	double right;
	double f_left;
	double f_right;
	double past_area;
} task;

/* global shared vars */
double start_time, end_time;
double epsilon;
double pi = 0;
int np;
pthread_mutex_t pi_lock, np_lock;

void *calcArea(void *arg)
{
	int ok;
	/* get task */
	task *t = (task *)arg;

	/* do calculations */
	double mid = (t->left + t->right) / 2;
	double f_mid = f(mid);
	double left_area = (t->f_left + f_mid) * (mid - t->left) / 2;
	double right_area = (f_mid + t->f_right) * (t->right - mid) / 2;

	if (fabs((left_area + right_area) - t->past_area) > epsilon)
	{
		/* prepare tasks for another run of calcArea */
		/* left task */
		task *left_task = (task *)malloc(sizeof(task)); /* allocate space on heap for safekeeping */
		left_task->left = t->left;
		left_task->right = mid;
		left_task->f_left = t->f_left;
		left_task->f_right = f_mid;
		left_task->past_area = left_area;

		/* right task */
		task *right_task = (task *)malloc(sizeof(task)); /* allocate space on heap for safekeeping */
		right_task->left = mid;
		right_task->right = t->right;
		right_task->f_left = f_mid;
		right_task->f_right = t->f_right;
		right_task->past_area = right_area;

		/* check if we can create more threads*/
		pthread_mutex_lock(&np_lock);
		if (np < 1)
		{
			/* no threads are available */
			pthread_mutex_unlock(&np_lock);
			calcArea((void *)left_task);
			calcArea((void *)right_task);
		}
		else
		{
			/* we create one more thread*/
			--np;
			pthread_mutex_unlock(&np_lock);
			pthread_t new_thread;
			ok = pthread_create(&new_thread, NULL, calcArea, (void *)left_task);
			printf("Pthread create returned: %d\n", ok);

			/* we also calculate area on this thread */
			calcArea((void *)right_task);

			/* wait for other thread to end and then "free" it */
			pthread_join(new_thread, NULL);
			pthread_mutex_lock(&np_lock);
			np++;
			pthread_mutex_unlock(&np_lock);
		}

		/* free allocated resources from heap */
		free(left_task);
		free(right_task);
	}
	else
	{
		/* The calculated area was the same as past */
		pthread_mutex_lock(&pi_lock);
		pi = pi + left_area + right_area;
		pthread_mutex_unlock(&pi_lock);
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	/* read EPSILON and np */
	epsilon = (argc > 1) ? atof(argv[1]) : EPSILON;
	if (epsilon == 0)
		epsilon = EPSILON;
	np = (argc > 2) ? atoi(argv[2]) : NP;

	/* prepare first task*/
	task first_task;
	first_task.left = LEFT;
	first_task.right = RIGHT;
	first_task.f_left = f(first_task.left);
	first_task.f_right = f(first_task.right);
	first_task.past_area = (first_task.f_left + first_task.f_right) * (first_task.right - first_task.left) / 2;
	first_task.past_area = (first_task.f_left + first_task.f_right) * (first_task.right - first_task.left) / 2;

	/* start measuring time */
	start_time = read_timer();

	/* start first calculation */
	calcArea((void *)&first_task);
	pi = pi * 4;

	/* measure end time */
	end_time = read_timer();

/* print result */
#ifndef STRIPPEDPRINT
	printf("Specified epsilon:  %g\n", epsilon);
	printf("pi: %f10\n", pi);
	printf("The execution time is %g sec\n", end_time - start_time);
#endif
#ifdef STRIPPEDPRINT
	printf("%g\n", end_time - start_time);
#endif

	return 0;
}