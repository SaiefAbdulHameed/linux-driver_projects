#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

int main(){
    int dev = open("/dev/saief_dev",O_RDONLY);
    if(dev ==-1){
        printf("device opening was unsuccessful\n");
        return -1;
    }
    printf("driver openning was successfull\n");
    close(dev);
    return 0;
}