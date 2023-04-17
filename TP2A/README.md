
# Capteurs
0xC00 => full_left </br>
0xC01 => left</br>
0xC02 => middle_left</br>
0xC03 => middle_right</br>
0xC04 => right</br>
0xC05 => full_right</br>
# Class Sensor recognize
0 | road | 0 | 100</br>
1 | stop | 0 | 100</br>
2 | yield | 0 | 100 => cédez le passage</br>
3 | crossing | 0 | 100</br>
4 | car_park | 0 | 100</br>

# Example
  vcan0  00000C00   [8]  5C 00 0A 00 00 00 00 00 => beaucoup de route et un peu de cédez le passage à fond a gauche</br>
  vcan0  00000C01   [8]  64 00 01 02 00 00 00 00 => full route, peu de stop et cédez le passage à gauche </br>
  vcan0  00000C02   [8]  64 00 00 03 00 00 00 0E => full route et un peu de cédez le passage milieu gauche </br>
  vcan0  00000C03   [8]  64 00 00 01 00 00 00 1B => full route et peu de cédez le passage milieu droit </br>
  vcan0  00000C04   [8]  44 00 00 00 00 00 00 11 => route a droite (normal on roule a droite)</br>
  vcan0  00000C05   [8]  2B 00 00 00 00 00 00 03 => pas trop de route a fond a droite (encore normal) </br>
