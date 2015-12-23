#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

long memtotal() {
  char buf[1024];
  char name[256];
  long value, sum;
  FILE *f = fopen("/proc/meminfo", "r");
  sum = 0;
  while (fgets(buf, sizeof(buf), f)) {
    sscanf(buf, "%s%ld", name, &value);
    if (!strcmp(name, "MemAvailable:")) {
      sum += value;
    } else if(!strcmp(name, "SwapFree:")) {
      sum += value;
    }
  }
  fclose(f);
  return sum * 1024;
}

int main(int argc, char **argv) {
  const int PAGE_SZ = 4096;
  const int R_SZ = PAGE_SZ * 128000;
  long i, j, n = memtotal() / R_SZ / 2;
  printf("N = %d\n", n);
  for (i=0; argv[0][i]; ++i) argv[0][i] = '\r';
  for (i=0; i<n; ++i) {
    if (!fork()) {
      sleep(1);
      char *p = (char*)mmap(0, R_SZ, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
      for (j=0; j<R_SZ; j+=PAGE_SZ) p[j] = 1;
      sleep(60);
      munmap(p, R_SZ);
      return 0;
    }
  }
}
