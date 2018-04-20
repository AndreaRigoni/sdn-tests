#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main()
{
    int socket_nummer;
    int laenge;
    struct sockaddr_in adressinfo;
    int ergebnis;

    char buf[ 100 ];
    int sock_client = 0, i;

    socket_nummer = socket(AF_INET, SOCK_STREAM, 0);
    adressinfo.sin_family = AF_INET;
    adressinfo.sin_addr.s_addr = inet_addr("127.0.0.2");
    adressinfo.sin_port = htons(14004);
    /*adressinfo.sin_port = htons(23);*/
    laenge = sizeof(adressinfo);
    ergebnis = connect(socket_nummer, (struct sockaddr *)&adressinfo, laenge);
    if (ergebnis == 0)
    {
        printf("\nVerbindungsaufbau erfolgreich");
        printf("\n*****************************");
    }
    else
    {
        perror("Fehler beim Verbindungsaufbau: ");
    }

    for(i=1; i<10; i++)
    {
        sleep(1);
        printf("\nsende daten\n");
        write(socket_nummer, "123456789", i);

        // read data from socket
        int l = read( socket_nummer, buf, sizeof( buf ) );
        if ( !l ) {
            printf( "Client close socket." );
            close( sock_client );
            sock_client = 0;
        }
        else if ( l < 0 )
            printf( "Unable to read data from socket." );
        else
            printf( "\n %d bytes vom socket gelesen.", l );

        // send all data to stdout
        l = write( STDOUT_FILENO, buf, i );
        if ( i>8 )
        {
            i=0;
        }

    }

    close(socket_nummer);
    printf("\n\n");
}

