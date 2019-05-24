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
uint32_t led_value;

// this module allow to calibrate the hardware without any computer.
// calibrate the 0/1V of the output

inline void calibrate_manual_2_init_() {
  led_value = 0;
  state = 1;  // 0 : on attend un mvt de toggle
              // 1 : on attend que le toggle soit a 0
}

inline void calibrate_manual_2_loop_() {

  delay(300);
  set_led2(255*led_value);
  led_value = 1 - led_value;
  set_led4(255*led_value);
  
  if((state == 0) && (get_toggle()!=1)) { // calibrate output
    state = 1;
    mode = 3; 
    curent_cal = 0; 
    timer_cal = 0; 
    audio_outL = 0;
    audio_outR = 0;
    cal_out = 1;
    while (cal_out < 5) { // on attend que la calibration auto soit fini
      delay(50);
      set_led2(255*led_value);
      led_value = 1 - led_value;
      set_led4(255*led_value);
    } 
    cal_out = 0;
  }

  if((state == 1) && (get_toggle()==1)) { // toggle au centre
    state = 0;
  }

}

inline void calibrate_manual_2_audio_() {
  calibrate_serial_audio_();
}

