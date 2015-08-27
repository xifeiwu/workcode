/***********************************************************************
 * sol13.22s.c - a datagram based card dealer (the server)
 *
 * 	         usage: cardd portnum [numdecks]
 * 	        action: receives requests and sends back replies
 * 	      commands: GET #  -> a string of number suit SPACE
 * 	                       e.g.  4D AH 2D 4D KC
 *
	A server for the card dealer is
	sol13.22s.c

	A client for the card dealer is
	sol13.22c.c

	The server accepts a port number as an argument and an optional
	argument specifying the number of decks it holds.  The server
	simply deals cards until it runs out of cards.

	The client places a "GET" request to obtain a number of cards.
	The server returns a string consisting of space-separated
	card identifiers.  Each card identifier is a two-character
	string of the form VS where V is the value and S is the suit.

	The protocol needs to be expanded to allow for returning cards,
	setting up a new deck, asking the server for the number of
	cards left in the stack, or other operations.  Which operations
	you add depend on the application.  

	Requests are handled sequentially, so the server does
	not have to worry about race conditions and locks.

	If the server has fewer cards than the client asks for, the
	server dispenses what it has.  This might not be the correct
	response for some applications.  By changing the protocol
	so the server does not give out any cards if it cannot
	satisfy the entire request, the card server can be used as
	a way to explain semaphores.

 *  build: cc sol13.22s.c dgram.c -o sol13.22s
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>

#define oops(m,x)  { perror(m);exit(x);}

int make_dgram_server_socket(int);
int get_internet_address(char *, int,  int *, struct sockaddr_in *);
void say_who_called(struct sockaddr_in *);
void reply_to_sender(int,char *,struct sockaddr_in *, socklen_t);

/*
 * card interface
 */

void  cards_init();
char *card_next();

int main(int ac, char *av[])
{
	int	port;			/* use this port		*/
	int	sock;			/* for this socket		*/
	char	buf[BUFSIZ];		/* to receive data here		*/
	size_t	msglen;			/* store its length here	*/
	struct  sockaddr_in   saddr;	/* put sender's address here	*/
	socklen_t saddrlen;		/* and its length here		*/
	int	n_decks = 1;
	
	if ( ac == 1 || (port = atoi(av[1])) <= 0 ){
		fprintf(stderr,"usage: dgrecv portnumber [numdecks]\n");
		exit(1);
	}
	if ( ac > 2 && atoi(av[2]) > 0 )
		n_decks = atoi(av[2]);

    /*  get a socket and assign it a port number */

	if( (sock = make_dgram_server_socket(port)) == -1 )
		oops("cannot make socket",2);

    /* receive messaages on that socket */

	cards_init(n_decks);
	saddrlen = sizeof(saddr);
	while( (msglen = recvfrom(sock,buf,BUFSIZ,0,
		(struct sockaddr *) &saddr,&saddrlen))>0 ) {
		buf[msglen] = '\0';
		printf("cardd: got a message: %s\n", buf);
		say_who_called(&saddr);
		reply_to_sender(sock,buf,&saddr,saddrlen);
		saddrlen = sizeof(saddr);
	}
	return 0;
}
void 
reply_to_sender(int sock,char *msg,struct sockaddr_in *addrp, socklen_t len)
{
	char	reply[BUFSIZ];
	char	*card;
	int	num;
	int	msglen;

	if ( strncasecmp(msg,"GET ",4)!=0 || (num=atoi(msg+4)) <= 0 )
		strcpy(reply,"ERR Syntax Error");
	else
	{
		strcpy(reply,"OK ");
		msglen = 3;
		while( num-->0 && msglen+4<BUFSIZ && (card=card_next())!=NULL )
		{
			strcat(reply,card);
			strcat(reply," ");
			msglen += 1+strlen(card);
		}
	}
	sendto(sock, reply, strlen(reply),0,(struct sockaddr *)addrp,len);
}
void say_who_called(struct sockaddr_in *addrp)
{
	char	host[BUFSIZ];
	int	port;

	get_internet_address(host,BUFSIZ,&port,addrp);
	printf("  from: %s:%d\n", host, port);
}


/* -----------------------------------------------------------
 * card system here
 *    functions: void cards_init(int ndecks) shuffles deck
 *               char* card_next() returns next card
 *
 *    shuffling method: line up all 52*ndecks cards in order,
 *    then take one at random, remove it from the list
 *    then repeat until list is empty;
 */

#define	NC	52

static char *unshuffled = NULL;
static int n_unshuffled;

static char *shuffled = NULL;
static int n_shuffled;

void cards_init(int ndecks)
{
	int	i;
	int	pick;
	int	randnum();
	int	total_cards = ndecks * NC;

	/* initialization */
	srand(getpid());
	n_unshuffled = total_cards;
	n_shuffled   = 0;
	if ( unshuffled )
		free(unshuffled);
	if ( shuffled )
		free(shuffled);
	unshuffled = (char *) malloc( total_cards );
	shuffled   = (char *) malloc( total_cards );
	if ( unshuffled == NULL || shuffled == NULL ){
		perror("malloc");
		exit(1);
	}
	for(i=0;i<total_cards;i++)
		unshuffled[i] = (i%NC);

	/* build the new deck */
	while( n_unshuffled > 0 )
	{
		/* pick one */
		pick = randnum(n_unshuffled);

		/* copy it to shuffled deck */
		shuffled[n_shuffled++] = unshuffled[pick];

		/* remove it from unshuffled deck */
		while( ++pick<n_unshuffled )
			unshuffled[pick-1] = unshuffled[pick];
		n_unshuffled--;
	}
}

/*
 * returns a random number in range 0 .. n-1
 */
int randnum(int n)
{
	return (int)((double)rand()/((double)RAND_MAX+1)*n);
}


/*
 * take next card from list and convert it to a number and
 * suit. return NULL at end of deck
 */

char suits[] = "CDHS";
char value[] = "A23456789TJQK";

char *
card_next()
{
	static char result[3];
	int  card;

	if ( n_shuffled >= 0 ){
		card      = shuffled[--n_shuffled];
		result[0] = value[card%13];
		result[1] = suits[card/13];
		result[2] = '\0';
		return result;
	}
	return NULL;
}


