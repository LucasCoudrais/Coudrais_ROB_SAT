
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <time.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#define CAN_EFF_FLAG 0x80000000U

int main(int argc, char **argv)
{
    int s;
    int i;
    int nbytes;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    printf("Road Follower \r\n");

    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s < 0)
    {
        perror("Socket");        
    }

    strcpy(ifr.ifr_name, "vcan0");
    ioctl(s, SIOCGIFINDEX, &ifr);

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Bind");        
    }

    uint32_t currentAccel = 0x00;
    uint32_t currentBrake = 0x00;
    int32_t currentTurn = 0x00;
    uint32_t vehicleSpeed = -200;
    uint32_t resultMidLeft = -200;
    uint32_t resultMidRight = -200;
    uint32_t resultLeft = -200;
    uint32_t resultRight = -200;
    uint32_t resultFullLeft = -200;
    uint32_t resultFullRight = -200;

    int32_t testEcart = 0x00;

    while (1 == 1)
    {
        struct can_filter rfilter[2];

        rfilter[1].can_id = 0xCFF;
        rfilter[0].can_mask = 0x500;
        rfilter[0].can_id = 0xC00;

        setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

        nbytes = read(s, &frame, sizeof(struct can_frame));

        if (nbytes < 0)
        {
            perror("Read");            
        }

        // Récupération données
        if (frame.can_id == (0x00000C07 | CAN_EFF_FLAG))
        {
            vehicleSpeed = frame.data[frame.can_dlc - 2];
        }
        if (frame.can_id == (0x00000C02 | CAN_EFF_FLAG))
        {
            resultMidLeft = frame.data[frame.can_dlc - 8];
        }
        if (frame.can_id == (0x00000C03 | CAN_EFF_FLAG))
        {
            resultMidRight = frame.data[frame.can_dlc - 8];
        }
        if (frame.can_id == (0x00000C01 | CAN_EFF_FLAG))
        {
            resultLeft = frame.data[frame.can_dlc - 8];
        }
        if (frame.can_id == (0x00000C04 | CAN_EFF_FLAG))
        {
            resultRight = frame.data[frame.can_dlc - 8];
        }
        if (frame.can_id == (0x00000C00 | CAN_EFF_FLAG))
        {
            resultFullLeft = frame.data[frame.can_dlc - 8];
        }
        if (frame.can_id == (0x00000C05 | CAN_EFF_FLAG))
        {
            resultFullRight = frame.data[frame.can_dlc - 8];
        }

        //On a récupère toutes les valeurs des capteurs
        if(vehicleSpeed != -200 && resultMidLeft != -200 && resultMidRight != -200 && resultLeft != -200 && resultRight != -200 && resultFullLeft != -200 && resultFullRight != -200 ){
            // Prise de décision
            // on ne va pas au dessus de 50km/h
            if (vehicleSpeed <= 50)
            {
                //On accélere de manière propertionnelle a la route qu'on a devant nous 
                // normalement j'aurai aimé faire une moyenne entre le midleft et le midright mais j'ai eu quelques soucis avec les division
                currentAccel = resultMidLeft;
                currentBrake = 0;

                //Si jamais on voit un écart entre la route a droite et la route a gauche, on tourne en fonction de ca 
                //principe de palier pour eviter de faire de gros zig zag 
                if((resultFullLeft - resultFullRight) < 25 || (resultFullRight - resultFullLeft ) < 25)
                {
                    currentTurn = 0;             
                }
                else if ((resultFullLeft - resultFullRight) < 50 || (resultFullRight - resultFullLeft) < 50)
                {
                    // currentTurn = resultLeft - resultRight;
                    testEcart = resultLeft - resultRight;
                    if (testEcart < 0)
                    {
                        currentTurn = testEcart + 25;
                    }
                    else
                    {
                        currentTurn = testEcart - 25;
                    }
                } 
                else if((resultFullLeft - resultFullRight) < 75 || (resultFullRight - resultFullLeft) < 75)
                {
                    currentTurn = resultLeft - resultRight;

                } else {
                    currentAccel = 0;
                    if ((resultFullLeft - resultFullRight) > 0)
                    {
                        currentTurn = -40;
                    }
                    else
                    {
                        currentTurn = 40;
                    }
                }

                printf("DIFF TOURNER  : %d \n", (resultLeft - resultRight));

                //Si on a pas beaucoup de route devant nous, on arrete d'accélerer et surtout on tourner vers le coté ou on a le plus de route.
                if (currentAccel < 40)
                {
                    printf("DIFF FACE AU MUR  : %d \n", (resultFullLeft - resultFullRight));

                    currentAccel = 0;
                    if ((resultFullLeft - resultFullRight) > 0)
                    {
                        currentTurn = -40;
                    }
                    else
                    {
                        currentTurn = 40;
                    }
                }
            }
            else
            {
                currentBrake = 50;
                currentAccel = 0;
            }
        }


        

        //Affichage pour aide a la compréhgensionse "débuggage" etc
        printf("FullLeft  : %d \n", resultFullLeft);
        printf("Left      : %d \n", resultLeft);
        printf("MidLeft   : %d \n", resultMidLeft);
        printf("MidRight  : %d \n", resultMidRight);
        printf("Right     : %d \n", resultRight);
        printf("FullRight : %d \n", resultFullRight);
        printf("accel : %d \n", currentAccel);
        printf("brake : %d \n", currentBrake);
        printf("turn  : %d \n", currentTurn);
        printf("\r\n");

        // controle final de la voiture
        frame.can_id = 0x321;
        frame.can_dlc = 3;
        frame.data[0] = currentAccel;
        frame.data[1] = currentBrake;
        frame.data[2] = currentTurn;
        if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
        {
            perror("Write");            
        }
    }

    if (close(s) < 0)
    {
        perror("Close");        
    }

    return 0;
}
