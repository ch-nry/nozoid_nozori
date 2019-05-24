// --------------------------------------------------------------------------
// This file is part of the NOZORI firmware.
//
//    NOZORI firmware is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    NOZORI firmware is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with NOZORI firmware. If not, see <http://www.gnu.org/licenses/>.
// --------------------------------------------------------------------------


// dual ADSR 
// Pot 1 : A 1
// Pot 2 : A 2
// Pot 3 : D 1
// Pot 4 : D 2
// Pot 5 : S 1
// Pot 6 : S 2
// Pot 7 : R 1
// Pot 8 : R 2
// IN 1 : GATE 1
// IN 2 : GATE 2
// Selecteur3 : out mode (both positive ADSR / positive and negative ADSR / both negative ADSR)
// OUT 1 : ADSR 1
// OUT 2 : ADSR 2


// ADSR : Attack curve
// this variable can be in betwwen this 2 exterm value:
// 0X47000000 : for slow curve
// 0x7FFFFFFF : for hard curve
// default is 0x7FFFFFFF
#define ADSR_Goal_value 0x4FFFFFFF

inline void ADSR_Dual_init_() {
  ADSR1_status = 2;
  ADSR1_goal = 0;  
  ADSR2_status = 2;
  ADSR2_goal = 0;
}

inline void ADSR_Dual_loop_() {
  uint32_t tmp, tmp2, toggle_value;
  bool GATE1_local, GATE2_local;
  uint32_t ADSR1_status_local, ADSR1_goal_local;
  uint32_t ADSR2_status_local, ADSR2_goal_local;
  
  filter16_nozori_84
  test_connect_loop_84();
  toggle_value = get_toggle();
  toggle_global = toggle_value;
  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // ADSR1

  if (IN1_connect < 60) { // une prise branché
    if (ADSR1_gate == true) { // on ete en mode gate
      GATE1_local = audio_inL > 0xA0000000;
    } else { // on n'avait pas de gate
      GATE1_local = audio_inL > 0xB0000000;
    }
  }
  else  { // pas de prise branché
    GATE1_local = false;
  }
  ADSR1_gate = GATE1_local;
  
  ADSR1_status_local = ADSR1_status;

  // did we change state?
  if (!GATE1_local) { // no gate -> release
    ADSR1_status_local = 2;  // release status
    ADSR1_goal_local = 0;
  }
  else {
    if ((ADSR1_status_local == 0) & (ADSR1_out >= 0x3FFFFFFF)) { // attack and signal is high, so we start a decay
      ADSR1_status_local = 1; // decay status
    }
    else if (ADSR1_status_local == 2) { // we got a gate, but use to be in release mode, so we start an attack
      ADSR1_status_local=0;
      ADSR1_goal_local = ADSR_Goal_value;
    }
  }

  switch (ADSR1_status_local) {
  case 0 : // attack
    tmp = CV_filter16_out[index_filter_pot1]; 
    break;
  case 1 : // decay -> sustain
    ADSR1_goal_local = CV_filter16_out[index_filter_pot5] << 14; // 0x3FFFFFFF max
    tmp = CV_filter16_out[index_filter_pot3];
    break;
  case 2 : // release
    tmp = CV_filter16_out[index_filter_pot7];
    break;
  }

  // curve fader -> filter coef
  tmp = (0xFFFF-tmp) / 80;
  tmp += 0xB0;
  tmp = table_CV2increment[tmp];

  noInterrupts();
  ADSR1_status = ADSR1_status_local;
  ADSR1_filter = tmp; 
  ADSR1_goal = ADSR1_goal_local;
  interrupts();

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // ADSR2

  if (IN2_connect < 60) { // une prise branché
    if (ADSR2_gate == true) { // on ete en mode gate
      GATE2_local = audio_inR > 0xA0000000;
    } else { // on n'avait pas de gate
      GATE2_local = audio_inR > 0xB0000000;
    }
  }
  else {
    GATE2_local = GATE1_local;
  }
  ADSR2_gate = GATE2_local;
  
  ADSR2_status_local = ADSR2_status;

  // did we change state?
  if (!GATE2_local) { // no gate -> release
    ADSR2_status_local = 2;  // release status
    ADSR2_goal_local = 0;
  }
  else {
    if ((ADSR2_status_local == 0) & (ADSR2_out >= 0x3FFFFFFF)) { // attack and signal is high, so we start a decay
      ADSR2_status_local = 1; // decay status
    }
    else if (ADSR2_status_local == 2) { // we got a gate, but use to be in release mode, so we start an attack
      ADSR2_status_local = 0;
      ADSR2_goal_local = ADSR_Goal_value;
    }
  }

  switch (ADSR2_status_local) {
  case 0 : // attack
    tmp = CV_filter16_out[index_filter_pot2]; 
    break;
  case 1 : // decay -> sustain
    ADSR2_goal_local = CV_filter16_out[index_filter_pot6] << 14; // 0x3FFFFFFF max
    tmp = CV_filter16_out[index_filter_pot4];
    break;
  case 2 : // release
    tmp = CV_filter16_out[index_filter_pot8];
    break;
  }

  tmp = (0xFFFF-tmp) / 80;
  tmp += 0xB0;
  tmp = table_CV2increment[tmp];
  
  noInterrupts();
  ADSR2_status = ADSR2_status_local;
  ADSR2_filter = tmp;
  ADSR2_goal = ADSR2_goal_local;
  interrupts();

  
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  if(toggle_value > 1) {
    led2(512-(ADSR1_out >> 21));
  }
  else {
    led2(ADSR1_out >> 21);
  }
  if(toggle_value > 0) {
    led4(512-(ADSR2_out >> 21));
  }
  else {
    led4(ADSR2_out >> 21);
  }
}

inline void ADSR_Dual_audio_() { 
  uint32_t ADSR1_out_tmp, ADSR2_out_tmp, toggle_value;
  //int32_t tmp1, tmp2;

  toggle_value = toggle_global;
  
  ADSR1_out_tmp = ADSR1_out;
  ADSR1_out_tmp += m_s32xs32_s32H(((int32_t)ADSR1_goal - (int32_t)(ADSR1_out_tmp)), ADSR1_filter); 
  ADSR1_out_tmp = min(ADSR1_out_tmp, 0x3FFFFFFF);
  ADSR1_out = ADSR1_out_tmp;
  ADSR1_out_tmp += (ADSR1_out_tmp>>1);
  if(toggle_value > 1) ADSR1_out_tmp *= -1;
  audio_outL = ADSR1_out_tmp + OUT1_0V;

  ADSR2_out_tmp = ADSR2_out;
  ADSR2_out_tmp += m_s32xs32_s32H(((int32_t)ADSR2_goal - (int32_t)(ADSR2_out_tmp)), ADSR2_filter); 
  ADSR2_out_tmp = min(ADSR2_out_tmp, 0x3FFFFFFF);
  ADSR2_out = ADSR2_out_tmp;
  ADSR2_out_tmp += (ADSR2_out_tmp>>1);
  if(toggle_value > 0) ADSR2_out_tmp *= -1;
  audio_outR = ADSR2_out_tmp + OUT2_0V;
}
