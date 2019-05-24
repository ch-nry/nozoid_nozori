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

inline void test_2_init_() {
}

inline void test_2_loop_() {
  uint32_t toggle, switch_in;
  filter16_nozori_68  
  test_connect_loop_68();

  switch_in = get_switch();
  toggle = get_toggle();

  switch(toggle) {
    case 0:
      switch(switch_in) {
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
        case 4:
          set_led2(255);
          set_led4(255);
        break;
        default:
          set_led2(10);
          set_led4(10);
        break;         
      }
    break;
    case 1:
      switch(switch_in>>3) {
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
        case 4:
          set_led2(255);
          set_led4(255);
        break;
        default:
          set_led2(10);
          set_led4(10);
        break;         
      }
    break;
    case 2:
      switch(switch_in>>6) {
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
        case 4:
          set_led2(255);
          set_led4(255);
        break;
        default:
          set_led2(10);
          set_led4(10);
        break;         
      }
    break;
  }

/*
  toggle = get_toggle();

  if (toggle == 0) {
    if (CV1_connect < 60) led2(CV_filter16_out[index_filter_CV1]>>7); else set_led2(0);
    if (CV2_connect < 60) led4(CV_filter16_out[index_filter_CV2]>>7); else set_led4(0);
  }
  if (toggle == 1) {
    if (CV3_connect < 60) led2(CV_filter16_out[index_filter_CV3]>>7); else set_led2(0);
    if (CV4_connect < 60) led4(CV_filter16_out[index_filter_CV4]>>7); else set_led4(0);
  }
  if (toggle == 2) {
    if (IN1_connect < 60)  led2(audio_inL>>23); else set_led2(0);
    if (IN2_connect < 60)  led4(audio_inR>>23); else set_led4(0);
  } 
  */
   
}

inline void test_2_audio_() {
  audio_outL = CV_filter16_out[index_filter_pot1]<<16;
  audio_outR = CV_filter16_out[index_filter_pot2]<<16;
}
