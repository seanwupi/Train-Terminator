#include <stdio.h>
#include <sys/mman.h>

long memtotal() {
  char buf[1024];
  char name[256];
  long value, sum = 0;
  FILE *f = fopen("/proc/meminfo", "r");
  while (fgets(buf, sizeof(buf), f)) {
    sscanf(buf, "%s%ld", name, &value);
    if (!strcmp(name, "MemAvailable:") || !strcmp(name, "SwapFree:"))
      sum += value;
  }
  fclose(f);
  return sum * 1024;
}

int main(int argc, char **argv) {
  const int PAGE_SZ = 4096;
  const int R_SZ = PAGE_SZ * 64000;
  long i, j;
  printf("Process Name: ");
  gets(argv[0]);
  while (1) {
    sleep(1);
    long m = memtotal();
    printf("mem = %ldM\n", m / 1024 / 1024);
    if (m>R_SZ) {
      if (!fork()) {
        char *p = (char*)mmap(0, R_SZ, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
        for (j=0; j<R_SZ; j+=PAGE_SZ) p[j] = 1;
        sleep(900); // 15min
        munmap(p, R_SZ);
        return 0;
      }
    }
  }
}
