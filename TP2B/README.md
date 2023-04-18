## Qu'est ce que MISRA : 
MISRA est une norme utilisé pour le dévéloppement. Dans notre cas nous parlerons plus de la norme MISRA-C qui est la norme pour le language C, celui que nous utilisons.
Le but de cette norme est d'améliorer la sécurité, la fiabilité, la portabilité du code que se trouve en général dans les systèmes embarqués.
En C, ces normes permettent de mieux gérer la mémoire, d'optimiser l'utilisation des pointeurs par exemple, améliorer la syntaxe, la sémantique du langage...
Utiliser MISRA peut aider & réduire les erreurs, les bugs dans les logiciels embarques qui ont besoin d'être sur.

## Erreur misra du début 
```
road_follower_misra.c:110:13: error: Uninitialized variable: vehicleSpeed [uninitvar]
        if (vehicleSpeed <= 50)
            ^
road_follower_misra.c:171:37: error: Uninitialized variable: resultMidLeft [uninitvar]
        printf("MidLeft   : %d \n", resultMidLeft);
                                    ^
road_follower_misra.c:172:37: error: Uninitialized variable: resultMidRight [uninitvar]
        printf("MidRight  : %d \n", resultMidRight);
                                    ^
road_follower_misra.c:170:37: error: Uninitialized variable: resultLeft [uninitvar]
        printf("Left      : %d \n", resultLeft);
                                    ^
road_follower_misra.c:173:37: error: Uninitialized variable: resultRight [uninitvar]
        printf("Right     : %d \n", resultRight);
                                    ^
road_follower_misra.c:169:37: error: Uninitialized variable: resultFullLeft [uninitvar]
        printf("FullLeft  : %d \n", resultFullLeft);
                                    ^
road_follower_misra.c:174:37: error: Uninitialized variable: resultFullRight [uninitvar]
        printf("FullRight : %d \n", resultFullRight);
                                    ^
road_follower_misra.c:110:13: error: Uninitialized variable: vehicleSpeed [uninitvar]
        if (vehicleSpeed <= 50)
            ^
road_follower_misra.c:73:26: note: Assuming condition is false
        if (frame.can_id == (0x00000C07 | CAN_EFF_FLAG))
                         ^
road_follower_misra.c:110:13: note: Uninitialized variable: vehicleSpeed
        if (vehicleSpeed <= 50)
            ^
road_follower_misra.c:169:37: error: Uninitialized variable: resultFullLeft [uninitvar]
        printf("FullLeft  : %d \n", resultFullLeft);
                                    ^
road_follower_misra.c:99:26: note: Assuming condition is false
        if (frame.can_id == (0x00000C00 | CAN_EFF_FLAG))
                         ^
road_follower_misra.c:169:37: note: Uninitialized variable: resultFullLeft
        printf("FullLeft  : %d \n", resultFullLeft);
                                    ^
road_follower_misra.c:170:37: error: Uninitialized variable: resultLeft [uninitvar]
        printf("Left      : %d \n", resultLeft);
                                    ^
road_follower_misra.c:91:26: note: Assuming condition is false
        if (frame.can_id == (0x00000C01 | CAN_EFF_FLAG))
                         ^
road_follower_misra.c:170:37: note: Uninitialized variable: resultLeft
        printf("Left      : %d \n", resultLeft);
                                    ^
road_follower_misra.c:171:37: error: Uninitialized variable: resultMidLeft [uninitvar]
        printf("MidLeft   : %d \n", resultMidLeft);
                                    ^
road_follower_misra.c:83:26: note: Assuming condition is false
        if (frame.can_id == (0x00000C02 | CAN_EFF_FLAG))
                         ^
road_follower_misra.c:171:37: note: Uninitialized variable: resultMidLeft
        printf("MidLeft   : %d \n", resultMidLeft);
                                    ^
road_follower_misra.c:172:37: error: Uninitialized variable: resultMidRight [uninitvar]
        printf("MidRight  : %d \n", resultMidRight);
                                    ^
road_follower_misra.c:87:26: note: Assuming condition is false
        if (frame.can_id == (0x00000C03 | CAN_EFF_FLAG))
                         ^
road_follower_misra.c:172:37: note: Uninitialized variable: resultMidRight
        printf("MidRight  : %d \n", resultMidRight);
                                    ^
road_follower_misra.c:173:37: error: Uninitialized variable: resultRight [uninitvar]
        printf("Right     : %d \n", resultRight);
                                    ^
road_follower_misra.c:95:26: note: Assuming condition is false
        if (frame.can_id == (0x00000C04 | CAN_EFF_FLAG))
                         ^
road_follower_misra.c:173:37: note: Uninitialized variable: resultRight
        printf("Right     : %d \n", resultRight);
                                    ^
road_follower_misra.c:174:37: error: Uninitialized variable: resultFullRight [uninitvar]
        printf("FullRight : %d \n", resultFullRight);
                                    ^
road_follower_misra.c:103:26: note: Assuming condition is false
        if (frame.can_id == (0x00000C05 | CAN_EFF_FLAG))
                         ^
road_follower_misra.c:174:37: note: Uninitialized variable: resultFullRight
        printf("FullRight : %d \n", resultFullRight);
                                    ^
road_follower_misra.c:20:14: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-2.7]
int main(int argc, char **argv)
             ^
road_follower_misra.c:73:41: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-10.4]
        if (frame.can_id == (0x00000C07 | CAN_EFF_FLAG))
                                        ^
road_follower_misra.c:83:41: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-10.4]
        if (frame.can_id == (0x00000C02 | CAN_EFF_FLAG))
                                        ^
road_follower_misra.c:87:41: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-10.4]
        if (frame.can_id == (0x00000C03 | CAN_EFF_FLAG))
                                        ^
road_follower_misra.c:91:41: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-10.4]
        if (frame.can_id == (0x00000C01 | CAN_EFF_FLAG))
                                        ^
road_follower_misra.c:95:41: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-10.4]
        if (frame.can_id == (0x00000C04 | CAN_EFF_FLAG))
                                        ^
road_follower_misra.c:99:41: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-10.4]
        if (frame.can_id == (0x00000C00 | CAN_EFF_FLAG))
                                        ^
road_follower_misra.c:103:41: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-10.4]
        if (frame.can_id == (0x00000C05 | CAN_EFF_FLAG))
                                        ^
road_follower_misra.c:110:26: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-10.4]
        if (vehicleSpeed <= 50)
                         ^
road_follower_misra.c:116:52: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-10.4]
            if ((resultFullLeft - resultFullRight) > 60 || (resultFullLeft - resultFullRight) < -60)
                                                   ^
road_follower_misra.c:120:57: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-10.4]
            else if ((resultFullLeft - resultFullRight) > 40 || (resultFullLeft - resultFullRight) < -40)
                                                        ^
road_follower_misra.c:132:52: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-10.4]
            if ((resultFullLeft - resultFullRight) > 20 || (resultFullLeft - resultFullRight) < -20)
                                                   ^
road_follower_misra.c:149:30: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-10.4]
            if (currentAccel < 40)
                             ^
road_follower_misra.c:152:56: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-10.4]
                if ((resultFullLeft - resultFullRight) > 0)
                                                       ^
road_follower_misra.c:22:10: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-12.3]
    int s, i;
         ^
road_follower_misra.c:30:12: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-13.4]
    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
           ^
road_follower_misra.c:33:9: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-15.5]
        return 1;
        ^
road_follower_misra.c:46:9: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-15.5]
        return 1;
        ^
road_follower_misra.c:68:13: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-15.5]
            return 1;
            ^
road_follower_misra.c:189:13: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-15.5]
            return 1;
            ^
road_follower_misra.c:196:9: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-15.5]
        return 1;
        ^
road_follower_misra.c:131:13: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-15.7]
            }
            ^
road_follower_misra.c:28:11: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-17.7]
    printf("Road Follower \r\n");
          ^
road_follower_misra.c:36:11: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-17.7]
    strcpy(ifr.ifr_name, "vcan0");
          ^
road_follower_misra.c:39:11: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-17.7]
    memset(&addr, 0, sizeof(addr));
          ^
road_follower_misra.c:169:15: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-17.7]
        printf("FullLeft  : %d \n", resultFullLeft);
              ^
road_follower_misra.c:170:15: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-17.7]
        printf("Left      : %d \n", resultLeft);
              ^
road_follower_misra.c:171:15: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-17.7]
        printf("MidLeft   : %d \n", resultMidLeft);
              ^
road_follower_misra.c:172:15: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-17.7]
        printf("MidRight  : %d \n", resultMidRight);
              ^
road_follower_misra.c:173:15: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-17.7]
        printf("Right     : %d \n", resultRight);
              ^
road_follower_misra.c:174:15: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-17.7]
        printf("FullRight : %d \n", resultFullRight);
              ^
road_follower_misra.c:175:15: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-17.7]
        printf("accel : %d \n", currentAccel);
              ^
road_follower_misra.c:176:15: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-17.7]
        printf("brake : %d \n", currentBrake);
              ^
road_follower_misra.c:177:15: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-17.7]
        printf("turn  : %d \n", currentTurn);
              ^
road_follower_misra.c:178:15: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-17.7]
        printf("\r\n");
              ^
road_follower_misra.c:2:0: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-21.6]
#include <stdio.h>
^
road_follower_misra.c:9:0: style: misra violation (use --rule-texts=<file> to get proper output) [misra-c2012-21.10]
#include <time.h>
^
```
On peut alors voir des erreurs de type `error`, `note` ou même `style`. Nous avons également tous les code d'erreur suivant : 
* [uninitvar] V
* [misra-c2012-2.7]
* [misra-c2012-10.4]
* [misra-c2012-12.3] V
* [misra-c2012-13.4] V
* [misra-c2012-15.5] V
* [misra-c2012-15.7] V
* [misra-c2012-17.7]
* [misra-c2012-21.6]
* [misra-c2012-21.10]

Il va donc falloir en corriger au minimum 3 et au mieux toute. Il faut s'aider du fichier donné qui permet d'identifier les erreurs.


