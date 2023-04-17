
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

	printf("Test send frame + listen and print filtered frame \r\n");

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

// --------------------------------------------- partie send
/* comment : on envoie un id trop grand qui devient un id sur 29bit, ainsi on doit rajouter CAN_EFF_FLAG à l'id pour qu'il puisse bien etre envoyé */
	frame.can_id =  0x8123 | CAN_EFF_FLAG;
	frame.can_dlc = 8;
	sprintf(frame.data, "Test");
	if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
		perror("Write");
		return 1;
	}
// --------------------------------------------- fin partie send


// --------------------- partie filter

while (1 == 1)
{
	struct can_filter rfilter[2];

	// rfilter[0].can_id   = 0x000;
	rfilter[0].can_id   = 0x100;

	/* comment : on choisit ici 0xF00 comme masque pour que ça complète la condition donnée <received_can_id> & mask == can_id & mask
	ici on choisit ce masque un peu comme les masque qu'on établit en réseau
	par exemple : avec notre masque   1111 1111  0000 0000  0000 0000
	avec qqch invalide comme 0x300 => 0000 0011  0000 0000  0000 0000
	application du & logique =>       0000 0011  0000 0000  0000 0000
	borne moins (0x100) avec et logique  masque : 0000 0001  0000 0000  0000 0000 => condition borne min <= valeur ok 
	borne max (0x1FF) avec et logique masque : 0000 0001  0000 0000  0000 0000 => condition borne max >= valeur pas ok
	en fait il faut choisir un masque qui fera, par un ou logique avec la valeur, toujours une valeur comprise entre nos bornes
	*/

	// rfilter[0].can_mask = 0x400;
	rfilter[0].can_mask = 0xF00;

	// rfilter[1].can_id   = 0xBFF;
	rfilter[1].can_id   = 0x1FF;

	setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

	nbytes = read(s, &frame, sizeof(struct can_frame));

	if (nbytes < 0) {
		perror("Read");
		return 1;
	}

	printf("0x%03X [%d] ",frame.can_id, frame.can_dlc);

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
