#include "shared.h"

static volatile int stop = 0;
void inthand(int signum) {
    stop = 1;
}

int isEmpty(shared* segment)
{
  for (int i=0;i<shared->nbEtages-1;i++)
  {
    if (shared->in[i] != 0 || shared->out[i] != 0)
      return 1;
  }
  return 0;
}

shared* segment_create(size_t size) {
  int fd;
  shared* segment;
  fd = shm_open(SHARED_FILE, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  CHECK(fd != -1);
  CHECK(ftruncate(fd, size) != -1);
  segment = mmap(NULL, size, PROT_READ | PROT_WRITE,
                 MAP_SHARED, fd, 0);
  CHECK(segment != MAP_FAILED);
  CHECK(close(fd) != -1);
  return segment;
}

void segment_remove(shared* segment, size_t size) {
  CHECK(munmap(segment, size) != -1);
  CHECK(shm_unlink(SHARED_FILE) != -1);
}

void lift_up(shared* shared)
{
  printf("== floor %d\n[lift] moves   floor %d -> floor %d\n",shared->etageActuel,shared->etageActuel,shared->etageActuel+1);
  printf("[lift] called [ ");
  for (int i=0;i < shared->nbEtages;i++)
  {
    if (shared->in[i] != 0)
    {
      printf("%d ",i+1);
    }
    else printf("_ ");
  }
  printf("]\n");

  printf("[lift] exit [ ");
  for (int i=0;i < shared->nbEtages;i++)
  {
    if (shared->out[i] != 0)
    {
      printf("%d ",i+1);
    }
    else printf("_ ");
  }
  printf("]\n");

  }
void lift_down(shared* shared)
{
  printf("== floor %d\n[lift] moves   floor %d -> floor %d\n",shared->etageActuel,shared->etageActuel,shared->etageActuel-1);
  printf("[lift] called [ ");
  for (int i=0;i < shared->nbEtages;i++)
  {
    if (shared->in[i] != 0)
    {
      printf("%d ",i+1);
    }
    else printf("_ ");
  }
  printf("]\n");

  printf("[lift] exit [ ");
  for (int i=0;i < shared->nbEtages;i++)
  {
    if (shared->out[i] != 0)
    {
      printf("%d ",i+1);
    }
    else printf("_ ");
  }
  printf("]\n");
}

int main(int argc, char* argv[]) {
  shared* shared;
  int nbEtages = atoi(argv[1]);
  size_t size;
  int i;
  if ((argc != 2) || (nbEtages < 2)) {
    fprintf(stderr, "usage: %s <length> (length > 1)\n", argv[0]);
    exit(1);
  }
  signal(SIGINT, inthand);
  size = sizeof(shared) + 2*nbEtages*sizeof(int) + 2*sizeof(sem_t);
  shared = segment_create(size);
  shared->in = malloc(nbEtages*sizeof(int));
  shared->out = malloc(nbEtages*sizeof(int));
  shared->nbEtages = nbEtages;
  for (int i = 0; i < nbEtages; i++)
  {
    shared->in[i] = 0;
    shared->out[i] = 0;
  }
  shared->etageActuel=1;
  shared->in[0]=2;
  shared->out[3]=4;
  sem_init(&shared->maxpass,1,1);
  sem_init(&shared->doors,1,1);
  printf("[lift] lift initialized and starts (%s)(#floors= %d)\n",get_time_str(),shared->etageActuel);
  while (!stop && !isEmpty(segment)) {

    for (i=0; i<nbEtages-1 && !stop;shared->etageActuel++,i++)
    {
      sem_wait(&shared->doors);
      lift_up(shared);
      sem_post(&shared->doors);
      if (shared->in[etageActuel-1] != 0) printf("[lift] doors open  [floor %d]\n",shared->etageActuel);
      sleep(2);
      if (shared->in[etageActuel-1] != 0) printf("[lift] doors close  [floor %d]\n",shared->etageActuel);
    }
    for (i=nbEtages;i>1 && !stop;shared->etageActuel--,i--)
    {
      sem_wait(&shared->doors);
      lift_down(shared);
      sem_post(&shared->doors);
      if (shared->in[etageActuel-1] != 0) printf("[lift] doors open  [floor %d]\n",shared->etageActuel);
      sleep(2);
      if (shared->in[etageActuel-1] != 0) printf("[lift] doors close  [floor %d]\n",shared->etageActuel);
    }
  }
  //lift_down(shared);
  sem_destroy(&shared->doors);
  sem_destroy(&shared->maxpass);
  segment_remove(shared, size);
  return 0;
}
