//Ian Johnson
//C335
//Lab 2
//1/20/2018
//Memory Dump

void dump_memory(void *p, int len)
{
  printf("address\t     char hexCh\tshort\tinteger\t\tfloat\t\tdoubleFloat\n");

  int i;

  for (i = 0; i < len; i++) {
    printf("%8p: %c 0x%-2x %7d %12d\t%12.6e\t%16.6e\n",
	   p + i,
	   ((*(char *)(p + i) >= 32 && *(char *)(p + i) < 127) ? *(char *)(p + i) : '?'),
	   *(unsigned char *)(p + i),
	   *(short *)(p + i),
	   *(int *)(p + i),
	   *(float *)(p + i),
	   *(double *)(p + i));
  }
}
