// #include<stdio.h>
// #include<unistd.h>
// #include<fcntl.h>
// #include<stdlib.h>

// int main(){
//     int dev = open("/dev/saief_driver",O_RDONLY);
//     if(dev ==-1){
//         printf("device opening was unsuccessful\n");
//         return -1;
//     }
//     printf("driver openning was successfull\n");
//     close(dev);
//     return 0;
// }
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<string.h>

#define MY_MAGIC 'S'
#define CLEAR_BUF _IO(MY_MAGIC,1)
#define READ_BUF _IOR(MY_MAGIC,2,int)
#define WRITE_BUF _IOW(MY_MAGIC,3,int)
int main(){
    char write_buf[]="HELLO SAIEEF";
    char READ_buf[255];
    memset(READ_buf,0,sizeof(READ_buf));
    int size;
    int limit =100;
    int dev = open("/dev/saief_driver",O_RDWR);
    if(dev ==-1){
        printf("failed to open \n");
        return dev;
    }
    printf("info:driver opened\n");

    //write 
    if(write(dev,write_buf,strlen(write_buf)) <0){
        printf("error: not able too write\n");
        return -1;
    }
    printf("info: written ->'%s'\n",write_buf);

   ioctl(dev, READ_BUF, &size);
    printf("Buffer size: %d\n", size);

    //read
    lseek(dev,0,SEEK_SET);
     if(read(dev,READ_buf,sizeof(READ_buf))<0){
        printf("error: not able too read\n");
        return -1;
    }
    printf("info: read ->'%s'\n",READ_buf);


    ioctl(dev, CLEAR_BUF, 0);
    printf("Buffer cleared!\n");

     ioctl(dev, READ_BUF, &size);
    printf("Buffer size: %d\n", size);

       ioctl(dev, WRITE_BUF, &limit);
    printf("New limit set to: %d\n", limit);

    close(dev);
      printf("Device closed!\n");
    return 0;
}