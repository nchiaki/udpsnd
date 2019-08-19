#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "procdef.h"
#include  "glovaldef.h"
#include  "funcdef.h"

int
main(int ac, char *av[])
{
 int sock;
 int  sz;
 char sndbuf[2000];
 struct timeval tv, strtv, trmtv, wtv;
 //long sqno = 0;
 long secttl = 0;

cmd_proc(ac, av);

 printf("%ld bps %ld byteps %ld packps interval %ld.%06ld", mrate, bytmrate, pacrate, itvl.tv_sec, itvl.tv_usec);

 sock = socket(AF_INET, SOCK_DGRAM, 0);

sz = udpdatsz;
gettimeofday(&tv, NULL);
gettimeofday(&strtv, NULL);
while (1)
{
   timersub(&tv, &strtv, &trmtv);
   if (1 <= trmtv.tv_sec)
   {
     long bpsrate = secttl*8;
     long Mbpsrate, dotrate;
      Mbpsrate = bpsrate / 100000;
      dotrate = Mbpsrate % 10;
      Mbpsrate /= 10;
      printf("%ld byteps %ld.%ld Mbps\n", secttl, Mbpsrate, dotrate);
      secttl = 0;
      gettimeofday(&strtv, NULL);
   }
   sendto(sock, sndbuf, sz, 0, (struct sockaddr *)&addr, sizeof(addr));
   secttl += (sz+MAC_HDRS+IPV4_HDRS+UDP_HDRS);
   sndbuf[0]++;

   gettimeofday(&tv, NULL);
   gettimeofday(&wtv, NULL);
   timersub(&wtv, &tv, &trmtv);
   while (timercmp(&trmtv, &itvl, <))
   {
     gettimeofday(&wtv, NULL);
     timersub(&wtv, &tv, &trmtv);
   }
 }
 close(sock);

 return 0;
}
