/* www.chrisheydrick.com
     
   June 23 2012

   CanonicalArduinoRead write a byte to an Arduino, and then
   receives a serially transmitted string in response.

   The call/response Arduino sketch is here:
   https://gist.github.com/2980344

   Arduino sketch details at www.chrisheydrick.com
*/

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <signal.h>

#define DEBUG 1
  
//pthread_mutex_t lock;
pthread_t tid;
int previousMode = 1.0;
int dontSendAgain = 0;
float tempUsage[47];
int mode = 3.0; // 0 = off
              // 1 = rainbow
              // 2 = temp
              // 3 = usage

void* sendInfo(void *arg){
  
  tempUsage[1] = 3.0;
  tempUsage[0] = -1000;
  char temp[4];  
  temp[0] = ' '; temp[1] = ' '; temp[2] = ' '; temp[3] = ' ';
  FILE *fp;
  long lSize;
  char *buffer;

  sigset_t set, isSignal, allSigs;
  sigfillset(&allSigs);
  if (sigprocmask(SIG_SETMASK, &allSigs, NULL) == -1) {
     printf("sigprocmask: %m\n");
     exit(1);
  }
  sigemptyset(&set);
  if (sigaddset(&set, SIGUSR1) == -1) {
     printf("sigaddset: %m\n");
     exit(1);
  }

  while (1) {
  if (sigpending(&isSignal) != 0) {
     printf("sigpending: %m\n");
     exit(1);
  }
  int intI = 2;
  int wordI = 0;
  fp = fopen ( "/home2/peter/Documents/sysInfo/sysInfo.txt" , "rb" );
  if( !fp ) perror("/home2/peter/Documents/sysInfo/sysInfo.txt"),exit(1);

  fseek( fp , 0L , SEEK_END);
  lSize = ftell( fp );
  rewind( fp );

  

  /* allocate memory for entire content */
  buffer = calloc( 1, lSize+1 );
  if( !buffer ) fclose(fp),fputs("memory alloc fails",stderr),exit(1);

  /* copy the file into the buffer */
  if( 1!=fread( buffer , lSize, 1 , fp) )
    fclose(fp),free(buffer),fputs("entire read fails",stderr),exit(1);

  /* do your work here, buffer is a string contains the whole text */
  for (int i = 0; i < lSize; i++){
    if (((buffer[i] != '%') && (buffer[i] != ','))){
      temp[wordI] = buffer[i];
      //printf("Notcomma   buffer: %x   comma: %x\n", buffer[i], ',');
      wordI++;
    } else if ( buffer[i] == ','){
      if (isdigit((int) temp[0])){
        tempUsage[intI] = atof(temp);
        intI++;
      }
      //printf("currentInt: %f\n", tempUsage[intI-1]);
      temp[0] = ' '; temp[1] = ' '; temp[2] = ' '; temp[3] = ' ';
      wordI = 0;
    }
    //printf("%c", buffer[i]);
  }
  //printf("\n");
  fclose(fp);
  
  //printf("numberThings: %d\n", intI);
  //for (int i = 0; i < intI; i++){
  //  printf("%f ", tempUsage[i]);
  //}
  //printf("\n");


  int fd, n, i;
  float buf[64];
  struct termios toptions;

  /* open serial port */
  fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
  ////printf("fd opened as %i\n", fd);
  
  /* wait for the Arduino to reboot */
  usleep(1500000);

  /* get current serial port settings */
  tcgetattr(fd, &toptions);
  /* set 9600 baud both ways */
  cfsetispeed(&toptions, B57600);
  cfsetospeed(&toptions, B57600);
  /* 8 bits, no parity, no stop bits */
  toptions.c_cflag &= ~PARENB;
  toptions.c_cflag &= ~CSTOPB;
  toptions.c_cflag &= ~CSIZE;
  toptions.c_cflag |= CS8;
  /* Canonical mode */
  toptions.c_lflag |= ICANON;
  /* commit the serial port settings */
  tcsetattr(fd, TCSANOW, &toptions);

  /* Send byte to trigger Arduino to send string back */
  if (tempUsage[1] == 3.0){
    write(fd, &tempUsage, 16);
    write(fd, &tempUsage[11], 8);
    write(fd, &tempUsage[20], 8);
    write(fd, &tempUsage[29], 8);
    write(fd, &tempUsage[38], 8);
  } else if (tempUsage[1] == 4.0){
    write(fd, &tempUsage, 8);
    write(fd, &tempUsage[13], 28);
    usleep(1000);
    write(fd, &tempUsage[22], 28);
    write(fd, &tempUsage[31], 28);
    //usleep(0);
    write(fd, &tempUsage[40], 28);
    printf("%f, ", tempUsage[40]);
    printf("%f, ", tempUsage[41]);
    printf("%f, ", tempUsage[42]);
    printf("%f, ", tempUsage[43]);
  } else {
    if (dontSendAgain == 1){
      write(fd, &tempUsage, 8);
      dontSendAgain = 0;
    }
  }
  
  // Receive string from Arduino 
  n = read(fd, buf, 5);
  // insert terminating zero in the string 
  buf[n] = 0.0;

  //printf("%i bytes read, buffer contains: %s\n", n, buf);
 
  if(DEBUG)
    {
      ////printf("Printing individual floats...\n\n");
      for(i=0; i<n; i++)
	{
	  //printf("Byte %i:%i, ",i+1, (int)buf[i]);
          ////printf("numbeer %d:%f ", i, buf[i]);
	}
      ////printf("\n");
    } 
  if (mode == 0.0){
    return NULL;
  }
  usleep(2000000);
  if (previousMode != mode){
     previousMode = mode;
     dontSendAgain = 1;
  }
  }
}


int main(int argc, char *argv[])
{
  pthread_create(&tid, NULL, &sendInfo, NULL);
  //int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
  //int pthread_mutex_lock(pthread_mutex_t *mutex);
  while (1) {
    char input[100];
    printf( "1 - Rainbow\n2 - Chase\n3 - Temperatures\n4 - Usages\n5 - Fade\n6 - Gradient\nAbove 6 - Turn off\n0 - Exit\nEnter number: ");
    scanf("%s", input);
    mode = atof(input);
    if (mode == 0.0){
      printf("waiting for sender thread to terminate\n");
      pthread_join(tid, NULL);
      return 0;
    } else {
      tempUsage[1] = mode;
    }
  }
  return 0;
}
