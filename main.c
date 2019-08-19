#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAC_HDRS  14
#define IPV4_HDRS 20
#define UDP_HDRS  8
#define RTP_HDRS  12

#define RDPTSDTZ  (RTP_HDRS+188)
#define UDPFRMZ (MAC_HDRS+IPV4_HDRS+UDP_HDRS+RDPTSDTZ)

int
main(int ac, char *av[])
{
 int sock;
 int  sz;
 struct sockaddr_in addr;
 char sndbuf[2000];
 struct timeval tv, strtv, trmtv, itvl, wtv;
 //long sqno = 0;
 long secttl = 0;
 long mrate, bytmrate, pacrate;

 mrate = 20000000;
 if (2 <= ac)
 {
   mrate = atoi(av[1]);
 }
 bytmrate = mrate / 8;
 pacrate = bytmrate / UDPFRMZ;
 itvl.tv_usec = 1000000 / pacrate;
 itvl.tv_usec *= 93;  /* x 0.93 */
 itvl.tv_usec /= 100;
 itvl.tv_sec = 0;

 printf("%ld bps %ld byteps %ld packps interval %ld.%06ld", mrate, bytmrate, pacrate, itvl.tv_sec, itvl.tv_usec);

 sock = socket(AF_INET, SOCK_DGRAM, 0);

 addr.sin_family = AF_INET;
 addr.sin_port = htons(55000);
 addr.sin_addr.s_addr = inet_addr("192.168.113.212");

sz = RDPTSDTZ;
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
#if 0
   sprintf(sndbuf, "%ld,%06ld %ld UDP send data", tv.tv_sec, tv.tv_usec, sqno);
   sz = strlen(sndbuf);
   if (sz < (188+12))
    sz = (188+12);
#endif
   sendto(sock, sndbuf, sz, 0, (struct sockaddr *)&addr, sizeof(addr));
   secttl += (sz+MAC_HDRS+IPV4_HDRS+UDP_HDRS);

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
