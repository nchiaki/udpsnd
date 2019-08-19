/* Global value definision */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

long mrate, bytmrate, pacrate;
struct timeval  itvl;
int udpdatsz;
struct sockaddr_in addr;
