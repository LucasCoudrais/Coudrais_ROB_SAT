
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

    printf("UserOBD2Terminal :  \r\n");

    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        perror("Socket");
        return 1;
    }

    strcpy(ifr.ifr_name, "vcan1");
    ioctl(s, SIOCGIFINDEX, &ifr);

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Bind");
        return 1;
    }

    frame.can_dlc = 8;

    while (1 == 1)
    {
        // Demande vitesse véhicule
        frame.can_id = (0x000007DF | CAN_EFF_FLAG);
        frame.data[0] = 0x02;
        frame.data[1] = 0x01;
        frame.data[2] = 0x0D;
        frame.data[3] = 0xAA;
        frame.data[4] = 0xAA;
        frame.data[5] = 0xAA;
        frame.data[6] = 0xAA;
        frame.data[7] = 0xAA;
        if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
        {
            perror("Write");
            return 1;
        }


        while (1 == 1)
        {
            struct can_filter rfilter[2];

            // application des filtres requis
            rfilter[1].can_id = 0xFFF;
            rfilter[0].can_mask = 0x000;
            rfilter[0].can_id = 0x000;

            setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

            nbytes = read(s, &frame, sizeof(struct can_frame));

            if (nbytes < 0)
            {
                perror("Read");
                return 1;
            }

            if (frame.can_id == (0x000007E8 | CAN_EFF_FLAG))
            {
                if (frame.data[1] == 0x41 && frame.data[2] == 0x0D)
                {
                    printf("Vehicle speed : ");
                    uint32_t result = frame.data[3]; // on prend que le 2eme octet dans les données
                    printf("%" PRIu32 "\n", result);
                    break;
                }
            }
            if (frame.can_id == (0x00000555 | CAN_EFF_FLAG))
            {
                printf("Student allumé, on peut continuer \n ");
                break;
            }
        }
        sleep(3);

        // Demande vitesse moteur
        frame.can_id = (0x000007DF | CAN_EFF_FLAG);
        frame.data[0] = 0x02;
        frame.data[1] = 0x01;
        frame.data[2] = 0x0C;
        frame.data[3] = 0xAA;
        frame.data[4] = 0xAA;
        frame.data[5] = 0xAA;
        frame.data[6] = 0xAA;
        frame.data[7] = 0xAA;
        if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
        {
            perror("Write");
            return 1;
        }

        while (1 == 1)
        {
            struct can_filter rfilter[2];

            // application des filtres requis
            rfilter[1].can_id = 0xFFF;
            rfilter[0].can_mask = 0x000;
            rfilter[0].can_id = 0x000;

            setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

            nbytes = read(s, &frame, sizeof(struct can_frame));

            if (nbytes < 0)
            {
                perror("Read");
                return 1;
            }

            if (frame.can_id == (0x000007E8 | CAN_EFF_FLAG))
            {
                if (frame.data[1] == 0x41 && frame.data[2] == 0x0C)
                {
                    printf("Motor speed : ");
                    uint32_t result = frame.data[3] | (frame.data[4] << 8); // On inverse l'ordre des bits pour la lecture et on les tranforme en uint32
                    printf("%" PRIu32 "\n", result);
                    break;
                }
            }
        }
        sleep(3);

        // Demande position pédale de gaz
        frame.can_id = (0x000007DF | CAN_EFF_FLAG);
        frame.data[0] = 0x02;
        frame.data[1] = 0x01;
        frame.data[2] = 0x11;
        frame.data[3] = 0xAA;
        frame.data[4] = 0xAA;
        frame.data[5] = 0xAA;
        frame.data[6] = 0xAA;
        frame.data[7] = 0xAA;
        if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame))
        {
            perror("Write");
            return 1;
        }

        while (1 == 1)
        {
            struct can_filter rfilter[2];

            // application des filtres requis
            rfilter[1].can_id = 0xFFF;
            rfilter[0].can_mask = 0x000;
            rfilter[0].can_id = 0x000;

            setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

            nbytes = read(s, &frame, sizeof(struct can_frame));

            if (nbytes < 0)
            {
                perror("Read");
                return 1;
            }

            if (frame.can_id == (0x000007E8 | CAN_EFF_FLAG))
            {
                if (frame.data[1] == 0x41 && frame.data[2] == 0x11)
                {
                    printf("Accélération throttle : ");
                    uint32_t result = frame.data[3]; // On inverse l'ordre des bits pour la lecture et on les tranforme en uint32
                    printf("%" PRIu32 "\n", result);
                    break;
                }
            }
        }
        sleep(3);
    }

    if (close(s) < 0)
    {
        perror("Close");
        return 1;
    }

    return 0;
}
