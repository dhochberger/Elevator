#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/time.h>
#include "lift.h"
#include "shared.h"

#define SHARED_FILE "/sharedfile"

// V1 : passager = 1
typedef struct shared {
  int etageActuel;
  int nbEtages;
  int *in;
  int *out;
  sem_t maxpass;
  sem_t doors;
}shared;

char *get_time_str();

#define CHECK(x) \
  do { \
    if (!(x)) { \
      fprintf(stderr, "%s:%d: ", __func__, __LINE__); \
      perror(#x); \
      exit(-1); \
    } \
  } while (0)
