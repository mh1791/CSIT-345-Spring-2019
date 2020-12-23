#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>


int main()
{
	const int SIZE = 4096;
	const char *name = "OS";
	char str[128];
	int i = 0;

	int shm_fd;
	void *ptr;

	/* create the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	/* configure the size of the shared memory segment */
	ftruncate(shm_fd,SIZE);

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}

	/**
	 * Now write to the shared memory region.
 	 *
	 * Note we must increment the value of ptr after each write.
	 */
		do
		{
			printf("Please enter a number greater than 0 to run the Collatz Conjecture.\n"); 
  			scanf("%d", &i);	
		}while (i <= 0);
		sprintf(str,"%d ",i);
		sprintf(ptr, "%s", str);
		ptr += strlen(str);
printf("Collatz Conjecture written to memory.\n");

	while (i!=1)
		{
		if (i%2 == 0)
		{
			i = i/2;
		}
		else if (i%2 == 1)
		{
			i = 3 * (i) + 1;
		}	
		sprintf(str,"%d ",i);
		sprintf(ptr, "%s", str);
		ptr += strlen(str);
			}
		

	return 0;
}
