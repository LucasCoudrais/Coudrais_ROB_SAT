
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

	printf("Car listener : \r\n");

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
        //Partie écouter voiture
        struct can_filter rfilter[2];

        rfilter[1].can_id   = 0xBFF;
        rfilter[0].can_mask = 0x400;
        rfilter[0].can_id   = 0x000;

        setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

        nbytes = read(s, &frame, sizeof(struct can_frame));

        if (nbytes < 0) {
            perror("Read");
            return 1;
        }

        printf("Info voiture : 0x%03X [%d] ",frame.can_id, frame.can_dlc);

        for (i = 0; i < frame.can_dlc; i++)
            printf("%02X ",frame.data[i]);

        printf("\r\n");

    }
	
// ------------------------ fin partie filter

	if (close(s) < 0) {
		perror("Close");
		return 1;
	}

	return 0;
}
