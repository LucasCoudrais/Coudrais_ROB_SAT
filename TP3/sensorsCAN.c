
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
    int s2;
    int nbytes;
    int nbytes2;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;
    struct sockaddr_can addr2;
    struct ifreq ifr2;
    struct canfd_frame frame2;

    printf("sensorsCAN : \r\n");

    //-------vcan0

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

    //----- vcan2

    int enable_canfd = 1;

    if ((s2 = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        perror("Socket");
        return 1;
    }

    strcpy(ifr2.ifr_name, "vcan2");
    ioctl(s2, SIOCGIFINDEX, &ifr2);

    memset(&addr2, 0, sizeof(addr2));
    addr2.can_family = AF_CAN;
    addr2.can_ifindex = ifr2.ifr_ifindex;

    if (bind(s2, (struct sockaddr *)&addr2, sizeof(addr2)) < 0)
    {
        perror("Bind");
        return 1;
    }

    uint32_t resultMidLeft = -1;
    uint32_t resultMidRight = -1;
    uint32_t resultLeft = -1;
    uint32_t resultRight = -1;
    uint32_t resultFullLeft = -1;
    uint32_t resultFullRight = -1;
    while (1 == 1)
    {

        //---------------------------Partie écouter vcan0
        struct can_filter rfilter[2];

        // application des filtres requis
        rfilter[1].can_id = 0xCFF;
        rfilter[0].can_mask = 0x500;
        rfilter[0].can_id = 0xC00;

        setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
        setsockopt(s2, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &enable_canfd, sizeof(enable_canfd));

        nbytes = read(s, &frame, sizeof(struct can_frame));

        if (nbytes < 0)
        {
            perror("Read");
            return 1;
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

        if (resultMidLeft != -1 && resultMidRight != -1 && resultLeft != -1 && resultRight != -1 && resultFullLeft != -1 && resultFullRight != -1)
        {
            frame2.data[0] = resultFullLeft;
            frame2.data[1] = resultLeft;
            frame2.data[2] = resultMidLeft;
            frame2.data[3] = resultMidRight;
            frame2.data[4] = resultRight;
            frame2.data[5] = resultFullRight;
            for (int i = 6; i < 64; i++)
            {
                frame2.data[i] = 0xAA;
            }
            // for (int i = 0; i < 8; i++)
            // {
            //     frame2.data[i] = resultFullLeft[i];
            // }
            // for (int i = 8; i < 16; i++)
            // {
            //     frame2.data[i] = resultLeft[i];
            // }
            // for (int i = 16; i < 24; i++)
            // {
            //     frame2.data[i] = resultMidLeft[i];
            // }
            // for (int i = 24; i < 32; i++)
            // {
            //     frame2.data[i] = resultMidRight[i];
            // }
            // for (int i = 32; i < 40; i++)
            // {
            //     frame2.data[i] = resultRight[i];
            // }
            // for (int i = 40; i < 48; i++)
            // {
            //     frame2.data[i] = resultFullRight[i];
            // }
            // for (int i = 48; i < 64; i++)
            // {
            //     frame2.data[i] = 0xAA;
            // }
            if (write(s2, &frame2, sizeof(struct canfd_frame)) != sizeof(struct canfd_frame))
            {
                perror("Write");
                return 1;
            }
        }
    }

    if (close(s) < 0)
    {
        perror("Close");
        return 1;
    }

    return 0;
}
