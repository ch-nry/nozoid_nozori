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

// this module allow to calibrate the hardware without any computer.
// calibrate the 0/1V of the input


#define blink_time 10000

inline void calibrate_manual_1_init_() {
}

inline void calibrate_manual_1_loop_() {
  //uint32_t debounce_tmp, debounce_tmp2, debounce_tmp3, debounce_index;
  //int32_t debounce_tmpS;
  //uint32_t tmp, i, accum;
  //int32_t tmpS;
  uint32_t blinkled, state, i, toggle;

// state : 0 : on attend que toggle soit a 1
//      1 : on es a 1, on attend que ca bouge
//#      2 : on fait une mesure a 0V;
//#      3 : on fait une mesure a 1V;

  state = 1;

  while (true) {
    filter16_nozori_68
    test_connect_loop_68();
    toggle = get_toggle();

    blinkled = (blinkled+1)%blink_time;
    if (blinkled < blink_time/2) {
      set_led2(0);
      set_led4(0);
    }
    else {
      set_led2(255);
      set_led4(255);
    }
    
    if ((state == 0) && (toggle == 1))
        state = 1;
    if ((state == 1) && (toggle == 0)) { // toggle en haut
        // on fait une mesure a 1V
        state = 0;
      
        if (CV1_connect < 60) rom_write_int32(rom_address_CV1_1V, (CV_filter16_out[index_filter_CV1]) - rom_read_int32(rom_address_CV1_0V));
        if (CV2_connect < 60) rom_write_int32(rom_address_CV2_1V, (CV_filter16_out[index_filter_CV2]) - rom_read_int32(rom_address_CV1_0V));
        if (CV3_connect < 60) rom_write_int32(rom_address_CV3_1V, (CV_filter16_out[index_filter_CV3]) - rom_read_int32(rom_address_CV1_0V));
        if (CV4_connect < 60) rom_write_int32(rom_address_CV4_1V, (CV_filter16_out[index_filter_CV4]) - rom_read_int32(rom_address_CV1_0V));
        if (IN1_connect < 60) rom_write_int32(rom_address_IN1_1V, filtre_audioL - rom_read_int32(rom_address_IN1_0V));
        if (IN2_connect < 60) rom_write_int32(rom_address_IN2_1V, filtre_audioR - rom_read_int32(rom_address_IN2_0V));
        
        for (i=0; i<10; i++) {
          set_led2(255);
          set_led4(255);
          delay(50);
          set_led2(0);
          set_led4(0);
          delay(10);
        }
    }
    if ((state == 1) && (toggle == 2)) {
        // on fait une mesure a 0V
        state = 0;

        if (CV1_connect < 60) rom_write_int32(rom_address_CV1_0V, (CV_filter16_out[index_filter_CV1]));
        if (CV2_connect < 60) rom_write_int32(rom_address_CV2_0V, (CV_filter16_out[index_filter_CV2]));
        if (CV3_connect < 60) rom_write_int32(rom_address_CV3_0V, (CV_filter16_out[index_filter_CV3]));
        if (CV4_connect < 60) rom_write_int32(rom_address_CV4_0V, (CV_filter16_out[index_filter_CV4]));
        if (IN1_connect < 60) rom_write_int32(rom_address_IN1_0V, filtre_audioL);
        if (IN2_connect < 60) rom_write_int32(rom_address_IN2_0V, filtre_audioR);

        for (i=0; i<10; i++) {
          set_led2(255);
          set_led4(255);
          delay(10);
          set_led2(0);
          set_led4(0);
          delay(50);
       }
    }
  }
}

inline void calibrate_manual_1_audio_() {
  calibrate_serial_audio_();
}

