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

// 1KHz  for audio out L
// 440Hz for audio out R
// 1V/O sur IN1 et CV1 qd connecté

inline void tune68_init_() {
  set_led1(0);
  set_led2(0);
  set_led3(0);
  set_led4(0);
}

inline void tune68_loop_() {
  uint32_t tmp;
  uint32_t addresse;
  filter16_nozori_68  
  test_connect_loop_68();
  
  if (CV1_connect < 60) set_led2(255);
  else set_led2(0);
  if (IN1_connect < 60) set_led4(255);
  else set_led4(0);
  
}

inline void tune68_audio_() {
  uint32_t tmp;
  uint32_t freq;
  int32_t tmpS;
  
  phase2 += 89478485>>1; // 1KHz
  tmpS = fast_sin(phase2)^0x80000000;
  tmpS -= tmpS >> 2;
  tmp = tmpS ^0x80000000; 
  audio_outL = tmp;
  analog_out_1(tmp);


  // frequency control
  if ((IN1_connect < 60) || (CV1_connect < 60)) {
    freq = 191870000;  // about 440 Hz
    macro_1VOct_IN1
    macro_1VOct_CV1
    macro_fq2increment 
    phase += increment1<<3; 
  }
  else {  
    phase += 39370534>>1; // 440 Hz
  }
  tmpS = fast_sin(phase)^0x80000000;
  tmpS -= tmpS >> 2;
  tmp = tmpS ^0x80000000;
  audio_outR = tmp;
  analog_out_1(tmp);
}


inline void tune84_init_() {
  set_led1(0);
  set_led2(0);
  set_led3(0);
  set_led4(0);
}

inline void tune84_loop_() {
  uint32_t tmp;
  uint32_t addresse;
  filter16_nozori_84  
  test_connect_loop_84();

  if (IN1_connect < 60) set_led2(255);
  else set_led2(0);
  if (IN2_connect < 60) set_led4(255);
  else set_led4(0);
}

inline void tune84_audio_() {
  uint32_t tmp;
  int32_t tmpS;
  uint32_t freq;

  phase2 += 89478485>>1; // 1KHz
  tmpS = fast_sin(phase2)^0x80000000;
  tmpS -= tmpS >> 2;
  tmp = tmpS ^0x80000000; 
  audio_outL = tmp;
  //analog_out_1(tmp);
  
  // frequency control
  if (IN1_connect < 60) {
    freq = 191870000;  
    macro_1VOct_IN1
    macro_fq2increment 
    phase += increment1<<3; 
  }
  else {
    phase += 39370534>>1; // 440 Hz
  }
  tmpS = fast_sin(phase)^0x80000000;
  tmpS -= tmpS >> 2;
  tmp = tmpS ^0x80000000;
  audio_outR = tmp;
  //analog_out_2(tmp);  
}

