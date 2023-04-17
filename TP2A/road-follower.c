
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

	printf("Road Follower \r\n");

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

    uint32_t currentAccel = 0x00;
    uint32_t currentBrake = 0x00;
    int32_t currentTurn = 0x00;

    while (1 == 1)
    {
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
        
        //Récupération données
        uint32_t vehicleSpeed;
        if(frame.can_id == (0x00000C07 | CAN_EFF_FLAG)){
            vehicleSpeed = frame.data[frame.can_dlc-2];
        }        
        uint32_t resultMidLeft;
        uint32_t resultMidRight;
        uint32_t resultLeft;
        uint32_t resultRight;
        uint32_t resultFullLeft;
        uint32_t resultFullRight;
        if(frame.can_id == (0x00000C02 | CAN_EFF_FLAG)){
            resultMidLeft = frame.data[frame.can_dlc-8];
        }      
        if(frame.can_id == (0x00000C03 | CAN_EFF_FLAG)){
            resultMidLeft = frame.data[frame.can_dlc-8];
        }
        if(frame.can_id == (0x00000C01 | CAN_EFF_FLAG)){
            resultLeft = frame.data[frame.can_dlc-8];
        }      
        if(frame.can_id == (0x00000C04 | CAN_EFF_FLAG)){
            resultRight = frame.data[frame.can_dlc-8];
        }  
        if(frame.can_id == (0x00000C00 | CAN_EFF_FLAG)){
            resultFullLeft = frame.data[frame.can_dlc-8];
        }      
        if(frame.can_id == (0x00000C05 | CAN_EFF_FLAG)){
            resultFullRight = frame.data[frame.can_dlc-8];
        }

        //Prise de décision
        if(vehicleSpeed <= 50){
            if(currentBrake > 10){
                currentBrake -= 1;
            }else if(currentAccel < 90){
                currentAccel += 5;
            }
            else if((resultLeft < 40 && resultRight < 40)  && (resultLeft > 20 && resultRight > 20)){
                if(currentAccel > 10){
                    currentAccel -= 1;
                }
                if(currentBrake < 90){
                    currentBrake += 5;
                }
                if(resultMidLeft < resultMidRight){
                    if(currentTurn > -90){
                        currentTurn -= 1;
                    }
                }else {
                    if(currentTurn < 90){
                        currentTurn += 1;
                    }
                }
                if(resultFullLeft < resultFullLeft){
                    if(currentTurn > -90){
                        currentTurn -= 5;
                    }
                }else {
                    if(currentTurn < 90){
                        currentTurn += 5;
                    }
                }
            }
            else if((resultLeft < 20 && resultRight < 20) && (resultLeft > 0 && resultRight > 0)){
                if(currentAccel > 10){
                    currentAccel -= 5;
                }
                if(currentBrake < 90){
                    currentBrake += 5;
                }
                if(resultMidLeft < resultMidRight){
                    if(currentTurn > -90){
                        currentTurn -= 5;
                    }
                }else {
                    if(currentTurn < 90){
                        currentTurn += 5;
                    }
                }
                if(resultFullLeft < resultFullLeft){
                    if(currentTurn > -90){
                        currentTurn -= 10;
                    }
                }else {
                    if(currentTurn < 90){
                        currentTurn += 10;
                    }
                }
            }
            else if((resultLeft > 60 && resultRight > 60) && (resultLeft < 80 && resultRight < 80)){
                if(currentBrake > 10){
                    currentBrake -= 1;
                }
                if(currentAccel < 90){
                    currentAccel += 1;
                }
                if(resultMidLeft < resultMidRight){
                    if(currentTurn > -90){
                        currentTurn -= 5;
                    }
                }else {
                    if(currentTurn < 90){
                        currentTurn += 5;
                    }
                }
                if(resultFullLeft < resultFullLeft){
                    if(currentTurn > -90){
                        currentTurn -= 10;
                    }
                }else {
                    if(currentTurn < 90){
                        currentTurn += 10;
                    }
                }
            }
            else if((resultLeft > 80 && resultRight > 80) && (resultLeft < 90 && resultRight < 90)){
                if(currentBrake > 10){
                    currentBrake -= 5;
                }
                if(currentAccel < 90){
                    currentAccel += 5;
                }
                if(resultMidLeft < resultMidRight){
                    if(currentTurn > -90){
                        currentTurn -= 5;
                    }
                }else {
                    if(currentTurn < 90){
                        currentTurn += 5;
                    }
                }
                if(resultFullLeft < resultFullLeft){
                    if(currentTurn > -90){
                        currentTurn -= 10;
                    }
                }else {
                    if(currentTurn < 90){
                        currentTurn += 10;
                    }
                }
            } else{
                if(currentTurn > -90){
                    currentTurn -= 5;
                }
            }           
        } else {
            if(currentAccel > 10){
                currentAccel -= 1;
            }
            if(currentBrake < 90){
                currentBrake += 1;
            }
        }

        //controle final de la voiture
        printf("%d \n", currentAccel);
        printf("%d \n", currentBrake);
        printf("%d \n", currentTurn);
        printf("\r\n");
        frame.can_id =  0x321;
	    frame.can_dlc = 3;
        frame.data[0] = currentAccel;
        frame.data[1] = currentBrake;
        frame.data[2] = currentTurn;
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
