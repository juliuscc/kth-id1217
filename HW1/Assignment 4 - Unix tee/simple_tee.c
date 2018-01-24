#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>

#define BUF_SIZE 5

// the buffer works like a stack for
// the sake of simplicity, if needed
// we may implement a queue
typedef struct
{
	char buf[BUF_SIZE];			// the buffer
	size_t start;				// start of buffer
	size_t end;					// end of buffer
	pthread_mutex_t mutex;		// needed to add/remove data from the buffer
	pthread_cond_t can_produce; // signaled when items are removed
	pthread_cond_t can_consume; // signaled when items are added
	bool read_last;				// If the last thread that touched the buffer read
} buffer_t;

typedef struct
{
	buffer_t *buffer;
	int output;
} consumer_args;

void *consumer(void *arg)
{
	consumer_args *arguments = (consumer_args *)arg;
	buffer_t *buffer = arguments->buffer;
	int output = arguments->output;

	while (1)
	{
		pthread_mutex_lock(&buffer->mutex);

		if ((buffer->start == ((buffer->end - 1) % BUF_SIZE)) && buffer->read_last)
		{ // empty
			// wait for new items to be appended to the buffer
			pthread_cond_wait(&buffer->can_consume, &buffer->mutex);
		}

		// grab data
		buffer->read_last = true;
		write(output, &buffer->buf[buffer->start], 1);
		++buffer->start;
		buffer->start = buffer->start % BUF_SIZE;
		// printf("Consumed: %d\n", buffer->buf[buffer->len]);

		// signal the fact that new items may be produced
		pthread_cond_signal(&buffer->can_produce);
		pthread_mutex_unlock(&buffer->mutex);
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	// Get copy file
	int copy_file;
	if (argc >= 1)
	{
		// printf("File: %s\n", argv[1]);
		mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IXOTH;
		copy_file = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, mode);
		if (copy_file < 0)
		{
			copy_file = 1;
		}
	}
	else
	{
		copy_file = 1;
	}

	// Prepare arguments
	buffer_t copy_buffer = {
		.start = 0,
		.end = 0,
		.mutex = PTHREAD_MUTEX_INITIALIZER,
		.can_produce = PTHREAD_COND_INITIALIZER,
		.can_consume = PTHREAD_COND_INITIALIZER,
		.read_last = true};

	buffer_t throughput_buffer = {
		.start = 0,
		.end = 0,
		.mutex = PTHREAD_MUTEX_INITIALIZER,
		.can_produce = PTHREAD_COND_INITIALIZER,
		.can_consume = PTHREAD_COND_INITIALIZER,
		.read_last = true};

	consumer_args copy_args = {
		.buffer = &copy_buffer,
		.output = copy_file};

	consumer_args throughput_args = {
		.buffer = &throughput_buffer,
		.output = 1};

	pthread_t copy, through;
	pthread_create(&copy, NULL, consumer, (void *)&copy_args);
	pthread_create(&through, NULL, consumer, (void *)&throughput_args);

	while (1)
	{
		char ch;
		while (read(STDIN_FILENO, &ch, 1) > 0)
		{
			pthread_mutex_lock(&copy_buffer.mutex);
			if ((copy_buffer.end == copy_buffer.start) && !copy_buffer.read_last)
			{ // full
				// wait until some elements are consumed
				pthread_cond_wait(&copy_buffer.can_produce, &copy_buffer.mutex);
			}

			pthread_mutex_lock(&throughput_buffer.mutex);
			if ((throughput_buffer.end == throughput_buffer.start) && !throughput_buffer.read_last)
			{
				pthread_cond_wait(&throughput_buffer.can_produce, &throughput_buffer.mutex);
			}

			// append data to the buffer
			copy_buffer.buf[copy_buffer.end] = ch;
			throughput_buffer.buf[throughput_buffer.end] = ch;

			++copy_buffer.end;
			++throughput_buffer.end;
			copy_buffer.end = copy_buffer.end % BUF_SIZE;
			throughput_buffer.end = throughput_buffer.end % BUF_SIZE;

			copy_buffer.read_last = false;
			throughput_buffer.read_last = false;

			// signal the fact that new items may be consumed
			pthread_cond_signal(&copy_buffer.can_consume);
			pthread_cond_signal(&throughput_buffer.can_consume);
			pthread_mutex_unlock(&copy_buffer.mutex);
			pthread_mutex_unlock(&throughput_buffer.mutex);
		}
	}

	pthread_join(copy, NULL);
	pthread_join(through, NULL);

	return 0;
}