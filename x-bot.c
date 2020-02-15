/* Channel Robot v0.3 for the Ampr Converse Network!

   By Marius Petrescu, YO2LOJ


   Based on work by
      - Carl Makin
      - Adam Roach

   with lots stolen from other sources.

   Additions & modifications by VE3OY - FEB 2020

*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#define BOTNAME		"X-Bot"
#define VERSION		"0.4"
#define CALL		"XXXXXX-X"
#define CHANNEL		"1"
#define TOPIC		"---===> XXX Convers Channel <===---"
#define SERV_TCP_PORT	3600
#define SERV_HOST_ADDR	"127.0.0.1"

int sockfd;
pid_t pid;

int lsend(char *text) {
    int len;
    int ret = -1;
    if (text)
    {
        len = strlen(text);
        if (len) ret = send(sockfd, text, len, 0);
    }
    return ret;
}

static void terminate(int sig)
{
    lsend("/q X-Bot killed by sysop.\n");
    close(sockfd);    
    exit(0);
}

int readline(register char *ptr, register int maxlen)
{
  int	n, rc;
  char c;

  for(n = 1; n < maxlen; n++) {
     if ( (rc = read(sockfd, &c, 1)) == 1) {
        *ptr++ = c;
        if (c == '\n')
           break;
     } else if (rc == 0) {
               if (n == 1)
                        return(0);    /* EOF, no data was read */
                  else
                        break;  /* EOF, some data was read */ 
             } else
                   return(-1);    /* ERROR */
   }
   *ptr = 0;
   return(n);
}

static void checktopic(int sig)
{
  char			buffer[400];
  int 			xrc;
  lsend("/topic\n");
  xrc = readline(buffer, 399);
  if (xrc && buffer[2]=='*') xrc = readline(buffer, 399);
  if (xrc && buffer[2]=='*') xrc = readline(buffer, 399);
  if (!strstr(buffer, TOPIC))
    lsend("/topic " TOPIC "\n");
  alarm(60);
}

static void settopic(int sig)
{
      lsend("/topic " TOPIC "\n");
}

int main(void)
{
  char			buffer[400];
  char *		cmd;
  int			xrc;
  int			chc;
  struct sockaddr_in	serv_addr;

  printf(BOTNAME " v" VERSION " by YO2LOJ, VE3OY and others.\n");

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family      = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
  serv_addr.sin_port 	    = htons(SERV_TCP_PORT);

/* Open the tcp socket */

  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) <0) {
        printf(BOTNAME ": Can't open stream socket.\n");
	exit(1);
  }

/* Connect to the Server */

  if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) <0) {
        printf(BOTNAME ": Can't connect to server.\n");
	exit(1);
  }
  
  signal(SIGTERM, terminate);
  
  /* =============================================================================== */
  /* X-Bot sets LOGS IN */
  lsend("/n " CALL " " CHANNEL "\n");
  /* X-Bot sets PERSONAL */
  lsend("/personal " BOTNAME " v" VERSION "\n");
  /* X-Bot sets WIDTH */
  lsend("/width 200\n");
  /* X-Bot sets TOPIC */
  lsend("/topic " TOPIC "\n");
  /* X-Bot sets AWAY with HELP message*/
  lsend("/away For HELP, send a private message to X-Bot: / msg { X-Bot call sign }:help\n");
  

  pid = fork();
  if (-1 == pid)
  {
	fprintf(stderr, BOTNAME ": cannot become a daemon.\n");
	exit(1);
  }
  else if (pid)
  {
	
        printf(BOTNAME " is now a daemon.  PID:%5d\n", pid);
	exit(0);
  }

  signal(SIGHUP, settopic);
  
  signal(SIGALRM, checktopic);
  alarm(60);


  for(;;) {
    xrc = readline(buffer, 399);
    if (xrc > 0) {
      if(cmd = strstr(buffer, ": X:")) {
	/* show the raw command string to console */
	/* puts (cmd);*/
        /* adjust our counter to strip off first 4 characters - we don't need those */
	cmd+=5;

	if (strlen(cmd)) {
	  /* adjust our counter to strip off last the character - which should be a NULL "/0" */
	  cmd--;
            /* =============================================================================== */
            /* VE3OY */
            /* Now that we have the command, check it for uppercase letters. */
            /* If any uppercase letters are found, convert them to lowercase */
            /* cmd = command string */

            /* Print out the buffer to screen */
            /* puts (buffer); */

            int i;
            char c;
            /* change any uppercase letters to lowercase */
            /* this will fix any commands like: Time or tImE */
            for(i=0;i<=strlen(cmd);i++)
            {
              c = cmd[i];
			/* putchar (tolower(c)); */
			cmd[i] = tolower(c);
            }
            /* print out the result */
            /* puts (cmd); */

	  if (strstr(cmd, "info")) {
	    lsend("X-Bot v" VERSION " >--------------------------------\n");
	    lsend("Written by YO2LOJ, VE3OY and others.>\n");
	    lsend("Source code available at: https://github.com/VE3OY/X-Bot\n");
	    lsend(".\n");
		lsend("X-Bot is currently holding channel " CHANNEL " and a topic of:\n" TOPIC "\n");
		lsend(".\n");
	  } 
	  else if (strstr(cmd, "help")) {
	    lsend("X-Bot " VERSION " >--------------------------------\n");
	    lsend("Available commands are: help, info, time or topic.\n");
	    lsend("--------------------------------------------\n");
	  }
	  else if (strstr(cmd, "topic")) {
	    lsend("X-Bot " VERSION " >--------------------------------\n");
	    lsend("X-Bot will now set the channel topic to:\n" TOPIC "\n");
	    lsend("--------------------------------------------\n");
	    lsend("/topic " TOPIC "\n");
	  }
	  else if (strstr(cmd, "time")) {
        /* VE3OY */
        /* If we have a TIME command coming in, get and store the current time */
        time_t rawtime;
        struct tm * timeinfo;
        char TBUFFER [20];
        time (&rawtime);
        timeinfo = localtime (&rawtime);
        strftime (TBUFFER,80,"%R %Z",timeinfo);
        /* puts (TBUFFER); */
        /* --- */
	    lsend("X-Bot " VERSION " >--------------------------------\n");
	    lsend("The time at this node is currently ");
	    lsend(TBUFFER);
	    lsend("\n");
	    lsend("--------------------------------------------\n");
	  }
	  else {
	    lsend("X-Bot " VERSION " >--------------------------------\n");
	    lsend("Available commands are: help, info, time or topic.\n");
	    lsend("--------------------------------------------\n");
	  }
	}
	else {
	    lsend("X-Bot " VERSION " >--------------------------------\n");
	    lsend ("I did not understand your last command.\n");
	    lsend("Available commands are: help, info, time or topic.\n");
	    lsend("Please try again.\n");
	    lsend("--------------------------------------------\n");
	}
      }
    } 
  }
  exit(0);
}
