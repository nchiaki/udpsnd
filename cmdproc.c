/*Command proc*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <libgen.h>
#include <string.h>

#include  "procdef.h"
#include  "glovaldef.h"


void
usage_proc(char *nam)
{
  char  *iam;
  iam = basename(nam);
  fprintf(stderr, "%s rate <Rate> datz <udpDataSize> dest <IPaddrPort>:<port> [ratecrct <val>]\n", iam);
}

void
cmd_proc(int ac, char *av[])
{
  int ix;
  char  *ipstr, *prtstr;
  int portnm, rtcrct;

  mrate = 0;
  udpdatsz = 0;
  ipstr = NULL;
  prtstr = NULL;
  rtcrct = 100;

  for (ix=1; ix<ac; ix++)
  {
    if (!strcmp(av[ix], "rate"))
    {
      ix++;
      mrate = atoi(av[ix]);
    }
    else if (!strcmp(av[ix], "datz"))
    {
      ix++;
      udpdatsz = atoi(av[ix]);
    }
    else if (!strcmp(av[ix], "ratecrct"))
    {
      ix++;
      rtcrct = atoi(av[ix]);
    }
    else if (!strcmp(av[ix], "dest"))
    {
      ix++;
      ipstr = av[ix];
      if (ipstr)
      {
        prtstr = strchr(ipstr, ':');
        if (prtstr)
        {
          *prtstr = '\0';
          prtstr++;
          portnm = atoi(prtstr);
        }
      }
    }
  }
  if (!mrate ||
      !udpdatsz ||
      !prtstr)
  {
    if (!mrate) {fprintf(stderr, "rate ");}
    if (!udpdatsz) {fprintf(stderr, "datz ");}
    if (!prtstr) {fprintf(stderr, "dest ");}
    fprintf(stderr, "を指定してください\n");
    exit(1);
  }
  bytmrate = mrate / 8;
  pacrate = bytmrate / UDPFRMZ;
  itvl.tv_usec = 1000000 / pacrate;
  itvl.tv_usec *= rtcrct;  /* x 0.xx */
  itvl.tv_usec /= 100;
  itvl.tv_sec = 0;

  addr.sin_family = AF_INET;
  addr.sin_port = htons(portnm);
  addr.sin_addr.s_addr = inet_addr(ipstr);

}
