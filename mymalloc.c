#include <stddef.h>

typedef struct data {
	size_t size;
	int free;
	struct data *next;
	struct data *prev;
}data;

data *start=NULL;
char mem[25000];

void initialize() {

	data *inifreespace;
	inifreespace = (data *)mem;
	inifreespace->free = 1;//if free 1,if allocated 0
	inifreespace->size = 25000 - sizeof(data);
	inifreespace->prev = NULL;
	inifreespace->next = NULL;
	start = inifreespace;
}

data *newdata(data *block, size_t size, size_t freesize) {

	data *newdata = (data *)((char *)block + size + sizeof(data) + 1);
	newdata->size = freesize - size - sizeof(data) - 1;
	newdata->free = 1;
	newdata->next = block->next;
	newdata->prev = block;
	return newdata;
}

void* MyMalloc(size_t size) {

	if(start==NULL)
		initialize();
	data *travel = start;
	while (1) {

		if (travel == NULL) {
			return NULL;
		}
		else if (travel->size >= (size + sizeof(data)) && travel->free == 1) {
			travel->next = newdata(travel, size, travel->size);
			travel->size = size;
			travel->free = 0;
			break;
		}
		travel = travel->next;
	}
	return (char *)travel + sizeof(data) + 1;
}

void MyFree(void *ptr) {

	data *info = (data *)((char *)ptr - 1) - 1;
	info->free = 1;
	if (info->next != NULL && info->next->free == 1) {
		
		info->size += info->next->size+sizeof(data)+1;
		if(info->next->next!=NULL)
			info->next->next->prev=info;
		info->next=info->next->next;		
}

	if(info->prev!=NULL && info->prev->free==1 ){

		info->prev->size+=info->size+sizeof(data)+1;
		if(info->next!=NULL)
			info->next->prev=info->prev;
		info->prev->next=info->next;
	}
}
/*
void print() {

	data *traverse = start;
	printf("--------------------------------------------\n");
	printf("%27s", "HEAP LAYOUT\n");
	printf("--------------------------------------------\n");
	while (traverse!=NULL) {
		printf("%p-%p     ", (char *)traverse + sizeof(data) + 1, (char *)traverse + sizeof(data) + traverse->size);
		if (traverse->free == 1)
			printf("FREE          ");
		else
			printf("ALLOCATED     ");
		printf("%lu\n", traverse->size);
		traverse = traverse->next;
	}
	printf("--------------------------------------------\n");
}*/

