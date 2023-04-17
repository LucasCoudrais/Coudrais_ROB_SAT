
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

	printf("Control car \r\n");

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


	frame.can_id =  0x123;
	frame.can_dlc = 2;
	// après analyse du trafic au lancement du vehicle_checker, on peut déterminer les données à envoyer pour faire les actions avec les feu comme ci dessous
	while (1 == 1)
	{
		//Ainsi on met directement dans frame.data qui est un tableau de byte, notre byte en hexa selon les valeurs qu'on connait à définit avant

		sleep(3);
		// cligno droit
		frame.data[0] = 0x00;
		frame.data[1] = 0x01;
		if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
			perror("Write");
			return 1;
		}

        sleep(3);
		//cligno gauche
		frame.data[0] = 0x00;
		frame.data[1] = 0x02;
			if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
			perror("Write");
			return 1;
		}

        sleep(3);
		//phare 
		frame.data[0] = 0x01;
		frame.data[1] = 0x00;
		if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
			perror("Write");
			return 1;
		}

        sleep(3);
		//plein phares
		frame.data[0] = 0x02;
		frame.data[1] = 0x00;
		if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
			perror("Write");
			return 1;
		}

        sleep(3);
		// éteint les phares
		frame.data[0] = 0x00;
		frame.data[1] = 0x00;
		if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
			perror("Write");
			return 1;
		}
	}


	if (close(s) < 0) {
		perror("Close");
		return 1;
	}


	return 0;
}
