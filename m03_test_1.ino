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

// you do what you want to test anything
// Or use this module as a template to create an other module.
// rename the 3 functions and declare it in the nozori.h module list

inline void test_1_init_() {
}

inline void test_1_loop_() {
  uint32_t toggle, tmp;
  filter16_nozori_68  
  test_connect_loop_68();

  toggle = get_toggle();
  tmp = get_switch();
  if (tmp != 0) module_type = tmp;

  switch (module_type) {
    case 0:
    case 1:
      set_led2(0);
      set_led4(0);
    break;
    case 2:
      led2(CV_filter16_out[index_filter_pot1]>>7);
      led4(CV_filter16_out[index_filter_pot2]>>7);  
    break;
    case 4:
      led2(CV_filter16_out[index_filter_pot3]>>7);
      led4(CV_filter16_out[index_filter_pot4]>>7);  
    break;
    case 8:
      led2(CV_filter16_out[index_filter_pot5]>>7);
      led4(CV_filter16_out[index_filter_pot6]>>7);  
    break;
    case 16:
      if (CV1_connect < 60) led2(CV_filter16_out[index_filter_CV1]>>7); else set_led2(0);
      if (CV2_connect < 60) led4(CV_filter16_out[index_filter_CV2]>>7); else set_led4(0); 
    break;
    case 32:
      if (CV3_connect < 60) led2(CV_filter16_out[index_filter_CV3]>>7); else set_led2(0);
      if (CV4_connect < 60) led4(CV_filter16_out[index_filter_CV4]>>7); else set_led4(0); 
    break;
    case 64:
      if (IN1_connect < 60)  led2(audio_inL>>23); else set_led2(0);
      if (IN2_connect < 60)  led4(audio_inR>>23); else set_led4(0);
    break;
    case 128:
      switch (toggle) {
        case 0:
          set_led2(0);
          set_led4(0);
        break;
        case 1:
          set_led2(0);
          set_led4(255);
        break;
        case 2:
          set_led2(255);
          set_led4(0);
        break;
      }
    break;
  }
  
  /*
  if (module_type == 0) {
    led2(CV_filter16_out[index_filter_pot1]>>7);
    led4(CV_filter16_out[index_filter_pot2]>>7);    
  }
  if (module_type == 1) {
    led2(CV_filter16_out[index_filter_pot3]>>7);
    led4(CV_filter16_out[index_filter_pot4]>>7);    
  }
  if (module_type == 2) {
    led2(CV_filter16_out[index_filter_pot5]>>7);
    led4(CV_filter16_out[index_filter_pot6]>>7);    
  } 
  */
}

inline void test_1_audio_() {
  uint32_t tmp;
  uint32_t freq;
  int32_t tmpS;

  increment_0 += 44739242; // 1KHz
  increment_1 += 22369621; // 500Hz

  switch (module_type) {
    case 0:
    case 1:
/*      tmpS = fast_sin(increment_0)^0x80000000;
      tmp = tmpS ^0x80000000; 
      audio_outL = tmp;
    
      tmpS = fast_sin(increment_1)^0x80000000;
      tmpS -= tmpS >> 2;
      tmp = tmpS ^0x80000000; 
      audio_outR = tmp;
      break;
    case 2:
*/
      if (IN1_connect < 60) audio_outL = audio_inL; else audio_outL = fast_sin(increment_0); // fulll scale
      if (IN2_connect < 60) audio_outR = audio_inR; else { // -5/5V out
        tmpS = fast_sin(increment_1)^0x80000000; 
        tmpS -= tmpS >> 2;
        tmp = tmpS ^0x80000000; 
        audio_outR = tmp;        
      }
    break;
    default:
      audio_outL = CV_filter16_out[index_filter_pot1]<<16;
      audio_outR = CV_filter16_out[index_filter_pot2]<<16;
    break;
  }
}

