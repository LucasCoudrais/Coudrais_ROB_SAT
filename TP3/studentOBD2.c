
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
    struct can_frame frame2;

    printf("studentOBD2 : \r\n");

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

    //----- vcan1

    if ((s2 = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        perror("Socket");
        return 1;
    }

    strcpy(ifr2.ifr_name, "vcan1");
    ioctl(s2, SIOCGIFINDEX, &ifr2);

    memset(&addr2, 0, sizeof(addr2));
    addr2.can_family = AF_CAN;
    addr2.can_ifindex = ifr2.ifr_ifindex;

    if (bind(s2, (struct sockaddr *)&addr2, sizeof(addr2)) < 0)
    {
        perror("Bind");
        return 1;
    }

    frame2.can_dlc = 8;

    //-------- on envoie une trame vide si jamais le temrinal l'écoute c'est qui est sur un read bloquant, donc ca va le débloquer. EN gros on lui envoie un truc pour lui dire qu'on est allumé

    frame2.can_id = (0x00000555 | CAN_EFF_FLAG);
    frame2.data[0] = 0xAA;
    frame2.data[1] = 0xAA;
    frame2.data[2] = 0xAA;
    frame2.data[3] = 0xAA;
    frame2.data[4] = 0xAA;
    frame2.data[5] = 0xAA;
    frame2.data[6] = 0xAA;
    frame2.data[7] = 0xAA;
    if (write(s2, &frame2, sizeof(struct can_frame)) != sizeof(struct can_frame))
    {
        perror("Write");
        return 1;
    }
    printf("Envoie signal d'allumage au term \n");

    while (1 == 1)
    {
        //-----------------------------------------Partie écouter vcan1
        struct can_filter rfilter2[2];

        // application des filtres requis
        rfilter2[1].can_id = 0xFFF;
        rfilter2[0].can_mask = 0x000;
        rfilter2[0].can_id = 0x000;

        setsockopt(s2, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter2, sizeof(rfilter2));

        nbytes2 = read(s2, &frame2, sizeof(struct can_frame));

        if (nbytes2 < 0)
        {
            perror("Read");
            return 1;
        }

        //----------- Si on recoit une demande de UserOBD2Terminal
        if (frame2.can_id == (0x000007DF | CAN_EFF_FLAG))
        {
            // mode de lecture courant et demande de vitesse du véhicule
            if (frame2.data[1] == 0x01 && frame2.data[2] == 0x0D)
            {
                printf("Demande Vehicule speed \n");
                while (recv(s, &frame, sizeof(frame), MSG_DONTWAIT) != -1)
                    ; // on vide toute la socket pour ne plus avoir des anciennes donnée

                while (1 == 1)
                {

                    //---------------------------Partie écouter vcan0
                    struct can_filter rfilter[2];

                    // application des filtres requis
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

                    if (frame.can_id == (0x00000C07 | CAN_EFF_FLAG))
                    { // lecture id C07
                      // Envoie vitesse du véhivule
                        frame2.can_id = (0x000007E8 | CAN_EFF_FLAG);
                        frame2.data[0] = 0x03;
                        frame2.data[1] = 0x41;
                        frame2.data[2] = 0x0D;
                        frame2.data[3] = frame.data[frame.can_dlc - 2];
                        frame2.data[4] = 0xAA;
                        frame2.data[5] = 0xAA;
                        frame2.data[6] = 0xAA;
                        frame2.data[7] = 0xAA;
                        if (write(s2, &frame2, sizeof(struct can_frame)) != sizeof(struct can_frame))
                        {
                            perror("Write");
                            return 1;
                        }
                        break;
                    }
                }
            }
            // mode de lecture courant et demande de vitesse du moteur
            else if (frame2.data[1] == 0x01 && frame2.data[2] == 0x0C)
            {
                printf("Demande Motor speed \n");
                while (recv(s, &frame, sizeof(frame), MSG_DONTWAIT) != -1)
                    ; // on vide toute la socket pour ne plus avoir des anciennes donnée

                while (1 == 1)
                {
                    //---------------------------Partie écouter vcan0
                    struct can_filter rfilter[2];

                    // application des filtres requis
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

                    if (frame.can_id == (0x00000C06 | CAN_EFF_FLAG))
                    { // on lit sur l'id qui correspond a la vitesse
                        frame2.can_id = (0x000007E8 | CAN_EFF_FLAG);
                        frame2.data[0] = 0x04;
                        frame2.data[1] = 0x41;
                        frame2.data[2] = 0x0C;
                        frame2.data[3] = frame.data[frame.can_dlc - 2];
                        frame2.data[4] = frame.data[frame.can_dlc - 1];
                        frame2.data[5] = 0xAA;
                        frame2.data[6] = 0xAA;
                        frame2.data[7] = 0xAA;
                        if (write(s2, &frame2, sizeof(struct can_frame)) != sizeof(struct can_frame))
                        {
                            perror("Write");
                            return 1;
                        }
                        break;
                    }
                }
            }
            else if (frame2.data[1] == 0x01 && frame2.data[2] == 0x11)
            {
                printf("Demande Acceleration pedale de gaz \n");
                while (recv(s, &frame, sizeof(frame), MSG_DONTWAIT) != -1)
                    ; // on vide toute la socket pour ne plus avoir des anciennes donnée

                uint32_t counter = 0;

                while (1 == 1)
                {
                    //---------------------------Partie écouter vcan0
                    struct can_filter rfilter[2];

                    // application des filtres requis
                    rfilter[1].can_id = 0xBFF;
                    rfilter[0].can_mask = 0x400;
                    rfilter[0].can_id = 0x000;

                    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

                    nbytes = read(s, &frame, sizeof(struct can_frame));

                    if (nbytes < 0)
                    {
                        perror("Read");
                        return 1;
                    }

                    if (frame.can_id == (0x00000321 | CAN_EFF_FLAG))
                    { // on lit sur l'id qui correspond a la THROTTLE
                        frame2.can_id = (0x000007E8 | CAN_EFF_FLAG);
                        frame2.data[0] = 0x03;
                        frame2.data[1] = 0x41;
                        frame2.data[2] = 0x11;
                        frame2.data[3] = frame.data[0];
                        frame2.data[4] = 0xAA;
                        frame2.data[5] = 0xAA;
                        frame2.data[6] = 0xAA;
                        frame2.data[7] = 0xAA;
                        if (write(s2, &frame2, sizeof(struct can_frame)) != sizeof(struct can_frame))
                        {
                            perror("Write");
                            return 1;
                        }
                        break;
                    }
                    else
                    {
                        counter = counter + 1;
                    }
                    if (counter == 100) // si on recoit rien au bout d'un certain temps, on estime que la pedale est pas enfoncé (on ne recoit que quand on donne des instrcution)
                    {
                        frame2.can_id = (0x000007E8 | CAN_EFF_FLAG);
                        frame2.data[0] = 0x03;
                        frame2.data[1] = 0x41;
                        frame2.data[2] = 0x11;
                        frame2.data[3] = 0x00;
                        frame2.data[4] = 0xAA;
                        frame2.data[5] = 0xAA;
                        frame2.data[6] = 0xAA;
                        frame2.data[7] = 0xAA;
                        if (write(s2, &frame2, sizeof(struct can_frame)) != sizeof(struct can_frame))
                        {
                            perror("Write");
                            return 1;
                        }
                        break;
                    }
                }
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
