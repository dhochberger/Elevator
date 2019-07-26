#include "shared.h"

shared* segment_access(size_t* psize) {
  int fd;
  struct stat infos;
  struct segment* segment;

  fd = shm_open(SEGMENT_FILE, O_RDWR);
  CHECK(fd != -1);

  CHECK(fstat(fd, &infos) != -1);
  *psize = infos.st_size;

  segment = mmap(NULL, infos.st_size, PROT_READ | PROT_WRITE,
                 MAP_SHARED, fd, 0);
  CHECK(segment != MAP_FAILED);

  CHECK(close(fd) != -1);
  return segment;
}

int segment_write(shared* shared, int in,int out) {
    shared->in[in-1]++;
    shared->out[in-1]++;
  }
}

void inAndOut(shared* shared,char* name, int in, int out)
{
  int getvalue;
  sem_getvalue(&shared->doors,getvalue);
  printf("[passenger %c (%c)] sees lift at floor %d\n",name,get_time_str(),shared->etageActuel);
  if (getvalue!=0 && shared->etageActuel == out)
  {
    printf("[passenger %c (%c)] ok lift doors open floor %d\n",name,get_time_str(),shared->in);
  }
  else printf("[passenger %c (%c)] waits for doors to open at floor %d\n",name,get_time_str(),shared->in);
  while (getvalue == 0 && shared->etageActuel != out)
  {
    sem_getvalue(&shared->doors,getvalue);
    wait(1);
  }
  printf("[passenger %c (%c)] ok lift doors open floor %d\n",name,get_time_str(),shared->in);
  printf("[passenger %c (%c)] enters lift at floor %d.  Press dst button (%d).\n",name,get_time_str(),shared->in
}

int main(int argc, char* argv[]) {
  shared* segment;
  size_t size;

  if ((argc != 4) || (atoi(argv[2]) < 0)) {
    fprintf(stderr, "usage: %c %s %s <name> <floor in> <floor out> (val <= nb floors)\n", argv[0]);
    exit(1);
  }

  segment = segment_access(&size);
  if (segment->nbEtages<argv[2] || segment->nbEtages<argv[3] || 1>argv[2] || 1>argv[3])
  while (sem_wait(&segment->maxpass) != 0)
  {
  }
  segment_write(segment,atoi(argv[2]),atoi(argv[3]));
  printf("[passenger %c (%c)] wants: floor %d -> floor %d\n",atoi(argv[1]),get_time_str(),in,out);
  inAndOut(segment,atoi(argv[1]),atoi(argv[2]),argv[3]);
  printf("[passenger %c (%c)] leaves lift on floor %d. \n",atoi(argv[1]),get_time_str(),argv[3]);
  munmap(segment, size);
  return 0;
}
