
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
    int s, i;
    int nbytes;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;

    printf("Road Follower \r\n");

    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        perror("Socket");
        return 1;
    }

    strcpy(ifr.ifr_name, "vcan0");
    ioctl(s, SIOCGIFINDEX, &ifr);

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Bind");
        return 1;
    }

    uint32_t currentAccel = 0x00;
    uint32_t currentBrake = 0x00;
    int32_t currentTurn = 0x00;

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
            return 1;
        }

        // Récupération données
        uint32_t vehicleSpeed;
        if (frame.can_id == (0x00000C07 | CAN_EFF_FLAG))
        {
            vehicleSpeed = frame.data[frame.can_dlc - 2];
        }
        uint32_t resultMidLeft;
        uint32_t resultMidRight;
        uint32_t resultLeft;
        uint32_t resultRight;
        uint32_t resultFullLeft;
        uint32_t resultFullRight;
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

        // Prise de décision
        // on ne va pas au dessus de 50km/h
        if (vehicleSpeed <= 50)
        {
            // de base on va tout droit 
            currentTurn = 0;
            //Si jamais on voit un écart entre la route a droite et la route a gauche, on tourne en fonction de ca 
            //principe de palier pour eviter de faire de gros zig zag 
            if ((resultFullLeft - resultFullRight) > 60 || (resultFullLeft - resultFullRight) < -60)
            {
                currentTurn = resultLeft - resultRight;
            }
            else if ((resultFullLeft - resultFullRight) > 40 || (resultFullLeft - resultFullRight) < -40)
            {
                currentTurn = resultLeft - resultRight;
                if (currentTurn < 0)
                {
                    currentTurn = -20;
                }
                else
                {
                    currentTurn = 20;
                }
            }
            if ((resultFullLeft - resultFullRight) > 20 || (resultFullLeft - resultFullRight) < -20)
            {
                currentTurn = resultLeft - resultRight;
                if (currentTurn < 0)
                {
                    currentTurn = -10;
                }
                else
                {
                    currentTurn = 10;
                }
            }
            currentBrake = 0;
            //On accélere de manière propertionnelle a la route qu'on a devant nous 
            // normalement j'aurai aimé faire une moyenne entre le midleft et le midright mais j'ai eu quelques soucis avec les division
            currentAccel = resultMidLeft;
            //Si on a pas beaucoup de route devant nous, on arrete d'accélerer et surtout on tourner vers le coté ou on a le plus de route.
            if (currentAccel < 40)
            {
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
            return 1;
        }
    }

    if (close(s) < 0)
    {
        perror("Close");
        return 1;
    }

    return 0;
}
