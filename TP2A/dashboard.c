
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#define CAN_EFF_FLAG 0x80000000U

int main(int argc, char **argv)
{
	int s, i; 
	int nbytes;
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame frame;

	printf("DashBoard : \r\n");

	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Socket");
		return 1;
	}

	strcpy(ifr.ifr_name, "vcan0" );
	ioctl(s, SIOCGIFINDEX, &ifr);

	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Bind");
		return 1;
	}

// --------------------- partie filter

    while (1 == 1)
    {
        //Partie écouter dashboard
        struct can_filter rfilter[2];

        rfilter[1].can_id   = 0xCFF;
        rfilter[0].can_mask = 0x500;
        rfilter[0].can_id   = 0xC00;

        setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

        nbytes = read(s, &frame, sizeof(struct can_frame));

        if (nbytes < 0) {
            perror("Read");
            return 1;
        }
        
        if(frame.can_id == (0x00000C06 | CAN_EFF_FLAG)){
            printf("Motor speed : ");
            uint32_t result = frame.data[frame.can_dlc-2] | (frame.data[frame.can_dlc-1]<<8);
            printf("%" PRIu32 "\n",result);
            // for (i = 0; i < frame.can_dlc; i++)
            //     printf("%02X",frame.data[i]);
            // printf("\r\n");
        }

        if(frame.can_id == (0x00000C07 | CAN_EFF_FLAG)){
            printf("Vehicle speed : ");
            uint32_t result = frame.data[frame.can_dlc-2];
            printf("%" PRIu32 "\n",result);
        }

        if(frame.can_id == (0x00000C07 | CAN_EFF_FLAG)){
            printf("Gear : ");
            uint32_t result = frame.data[frame.can_dlc-1];
            printf("%" PRIu32 "\n",result);
            printf("\r\n");

        }  

        uint32_t resultLeft;
        uint32_t resultRight;
        if(frame.can_id == (0x00000C01 | CAN_EFF_FLAG)){
            resultLeft = frame.data[frame.can_dlc-8];
        }      
        if(frame.can_id == (0x00000C04 | CAN_EFF_FLAG)){
            resultRight = frame.data[frame.can_dlc-8];
            printf("Left : ");
            printf("%" PRIu32 "\n",resultLeft);
            printf("Right : ");
            printf("%" PRIu32 "\n",resultRight);
            printf("Trun intruction : ");

            if(resultLeft > 75 && resultRight > 75 ){
                printf("^\n");
            } else if(resultLeft < 25 && resultRight > 75){
                printf("-> \n");
            } else if(resultRight < 25 && resultLeft > 75){
                printf("<-\n");
            } else {
                printf("^\n");
            } 
        }      
    }
	
// ------------------------ fin partie filter

	if (close(s) < 0) {
		perror("Close");
		return 1;
	}

	return 0;
}
