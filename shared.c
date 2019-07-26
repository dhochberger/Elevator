#include <sys/time.h>
#include "shared.h"

char *get_time_str()
{
  struct timeval now;
  char *str=malloc(33);
  gettimeofday(&now,NULL);
  snprintf(str,32,"%3ld.%06ld", (long int)now.tv_sec, (long int)now.tv_usec);
  return str;
}
