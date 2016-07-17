
#include <iostream>

#define LOOPCOUNTER  100
#define INS_PER_US   200         // 5ns per instruction
#define INS_PER_DELAY_LOOP 2     // two instructions per delay loop

#define INS_PER_GPIO 12
#define TARGET_HZ 1333
#define N_GPIOS 33
#define TARGET_LOOP_TIME (1000000 / TARGET_HZ)  // us that should last each main loop

#define INS_OVERHEAD 4


// DELAY_INS = (TargetTimePerLoop - Needed Time per Loop)/LoopCounter


#define DELAY_T_USED ((INS_PER_GPIO * N_GPIOS + INS_OVERHEAD) * LOOPCOUNTER)/INS_PER_US // time us consumed by program

#define DELAY_T  (TARGET_LOOP_TIME - DELAY_T_USED)

#define DELAY_INS ((DELAY_T * INS_PER_US) / (LOOPCOUNTER * INS_PER_DELAY_LOOP))


using namespace std;

int main(int argc, char *argv[])
{
  cout << endl;
  cout << "Loopcounter/duty points: " << LOOPCOUNTER << endl;
  cout << "Target frequency: " << TARGET_HZ << " Hz" <<  endl << endl;


  cout << "Target loop time: " << TARGET_LOOP_TIME << " us" << endl;
  cout << "Delay time: " << DELAY_T << " us" << endl;
  cout << "Delay inst: " <<  DELAY_INS << endl;
  cout << "Max frequency: " << 1e6*INS_PER_US/((INS_PER_GPIO * N_GPIOS + INS_OVERHEAD) * LOOPCOUNTER) << " Hz" << endl;

  return 0;
}
