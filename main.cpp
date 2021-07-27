#include <stdio.h>
#include <stdlib.h>

#include "libe1701.h"

#include <unistd.h>

static int cardNum = -1;

void initial_setup() {

  // *** set general marking parameters
  // ************************************************************************************************
  E1701_set_laser_mode(cardNum, E1701_LASERMODE_CO2); // configure for CO2
  E1701_set_standby(cardNum, 20000.0,
                    5.0); // 20kHz and 5 usec standby frequency/period
  E1701_set_laser_timing(
      cardNum, 20000.0,
      25.0); // 20kHz and 50% duty cycle marking frequency/period
  E1701_set_z_correction(cardNum, 100000000, 0, 0);
  E1701_set_scanner_delays(
      cardNum, 100.0, 100.0,
      10.0); // some delay values, have to be adjusted for used scanhead
  E1701_set_laser_delays(cardNum, 20.0,
                         30.0); // laser on/off delay in microseconds, have
                                // to be adjusted for used scan-system

  // speeds in bits per milliseconds within an 26 bit range,
  // here jump speed is 1000 times faster than mark speed
  E1701_set_speeds(cardNum, 67108864.0, 67108.864);
  E1701_digi_set_motf(cardNum, 0.0, 0.0); // no marking on-the-fly enabled

  // Toggle the laser for a fixed period of time
  E1701_set_laser(cardNum, E1701_COMMAND_FLAG_DIRECT, 1);
  // turn on the laser immediately using previously
  // defined laser parameters
  sleep(1);
  E1701_set_laser(cardNum, E1701_COMMAND_FLAG_DIRECT,
                  0); // turn off the laser after about 1 second
}

void send_commands() {
  // send data to perform a normal mark operation with vector data

  // jump to mark start(laser disabled) position (using unit bits
  // within an 26 bit range)
  E1701_jump_abs(cardNum, -10000000, -10000000, -10000000);
  // mark to position(laser enabled)
  E1701_mark_abs(cardNum, -10000000, 10000000, 10000000);   // mark a square
  E1701_mark_abs(cardNum, 10000000, 10000000, -10000000);   // mark a square
  E1701_mark_abs(cardNum, 10000000, -10000000, 10000000);   // mark a square
  E1701_mark_abs(cardNum, -10000000, -10000000, -10000000); // mark a square
}

int main(int argc, char *argv[]) {

  printf("======= START ==========\r\n");
  cardNum = E1701_set_connection("192.168.2.254");
  if (cardNum < 0) {
    printf("ERROR: Could not initialise!\r\n");
    return 0;
  }
  printf("Connection is set\r\n");
  if (E1701_load_correction(cardNum, "", 0) != E1701_OK) {
    printf("ERROR: opening connection failed\r\n");
  }
  printf("Correction is loaded");

  initial_setup();

  send_commands();
  // Flush all commands and execute
  E1701_execute(cardNum);

  // *** wait until marking has started
  while ((E1701_get_card_state(cardNum) &
          (E1701_CSTATE_MARKING | E1701_CSTATE_PROCESSING)) == 0) {
    usleep(10000);
  }

  // *** wait until marking is finished
  while ((E1701_get_card_state(cardNum) &
          (E1701_CSTATE_MARKING | E1701_CSTATE_PROCESSING)) != 0) {
    usleep(500000);
  }

  sleep(1000);
  E1701_close(cardNum);

  return 0;
}
