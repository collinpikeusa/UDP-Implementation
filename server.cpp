#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>

#define SERVICE_PORT	2000
#define BUFSIZE 2048
using namespace std;

int main(int argc, char **argv)
{
    struct sockaddr_in myaddr;	/* our address */
    struct sockaddr_in remaddr;	/* remote address */
    socklen_t addrlen = sizeof(remaddr);		/* length of addresses */
    int recvlen;			/* # bytes received */
    int fd;				/* our socket */
    int msgcnt = 0;			/* count # of messages we received */
    char buf[BUFSIZE];	/* receive buffer */


    /* create a UDP socket */

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("cannot create socket\n");
        return 0;
    }

    /* bind the socket to any valid IP address and a specific port */

    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(SERVICE_PORT);

    if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        perror("bind failed");
        return 0;
    }
    /* now loop, receiving data and printing what we received */
    for (;;) {
        printf("waiting on port %d\n", SERVICE_PORT);
        recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
        if (recvlen > 0) {
            buf[recvlen] = 0;
            printf("received message: \"%s\" (%d bytes)\n", buf, recvlen);
        }
        else
            printf("uh oh - something went wrong!\n");
        printf("ack %d", buf);
        msgcnt++;
        string str(buf);
        str += "Signed: Bob (Seller)  Date: November 29, 2018.";
        strcpy(buf, str.c_str());
        printf("sending response \"%s\"", buf);
        if (sendto(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, addrlen) < 0)
            perror("sendto");
    }
}
