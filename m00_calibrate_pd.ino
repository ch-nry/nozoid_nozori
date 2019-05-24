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

// mode to test a module and to calibrate it thanks to a "Pure Data" patch

uint32_t serial_in, curent_cal, timer_cal;
volatile uint32_t cal_out;
uint32_t mode;


inline void calibrate_pd_init_() {
  Serial.begin(115200);
  cal_out = 0;
}

inline void calibrate_pd_loop_() {
  uint32_t debounce_tmp, debounce_tmp2, debounce_tmp3, debounce_index;
  int32_t debounce_tmpS;
  uint32_t tmp, i, accum;
  int32_t tmpS;

  filter16_nozori_68
  
  tmp = (CV_filter16_out[index_filter_pot1])>>3;
  Serial.write(128);
  Serial.write(tmp>>7);
  Serial.write(tmp & 0b01111111);

  tmp = (CV_filter16_out[index_filter_pot2])>>3;
  Serial.write(129);
  Serial.write(tmp>>7);
  Serial.write(tmp & 0b01111111);

  tmp = (CV_filter16_out[index_filter_pot3])>>3;
  Serial.write(130);
  Serial.write(tmp>>7);
  Serial.write(tmp & 0b01111111);

  tmp = (CV_filter16_out[index_filter_pot4])>>3;
  Serial.write(131);
  Serial.write(tmp>>7);
  Serial.write(tmp & 0b01111111);

  tmp = (CV_filter16_out[index_filter_pot5])>>3;
  Serial.write(132);
  Serial.write(tmp>>7);
  Serial.write(tmp & 0b01111111);

  tmp = (CV_filter16_out[index_filter_pot6])>>3;
  Serial.write(133);
  Serial.write(tmp>>7);
  Serial.write(tmp & 0b01111111);

  tmp = (CV_filter16_out[index_filter_CV1])>>3;
  Serial.write(134);
  Serial.write(tmp>>7);
  Serial.write(tmp & 0b01111111);

  tmp = (CV_filter16_out[index_filter_CV2])>>3;
  Serial.write(135);
  Serial.write(tmp>>7);
  Serial.write(tmp & 0b01111111);

  tmp = (CV_filter16_out[index_filter_CV3])>>3;
  Serial.write(136);
  Serial.write(tmp>>7);
  Serial.write(tmp & 0b01111111);

  tmp = (CV_filter16_out[index_filter_CV4])>>3;
  Serial.write(137);
  Serial.write(tmp>>7);
  Serial.write(tmp & 0b01111111);

  tmp = filtre_audioL;
  Serial.write(138);
  Serial.write(tmp>>25);
  Serial.write((tmp>>18) & 0b01111111);

  tmp = filtre_audioR;
  Serial.write(139);
  Serial.write(tmp>>25);
  Serial.write((tmp>>18) & 0b01111111);

  Serial.write(140);
  Serial.write(get_toggle());

  tmp = get_switch();
  Serial.write(141);
  Serial.write(tmp>>7);
  Serial.write(tmp & 0b01111111);

  while (Serial.available() > 0) {
    tmp = Serial.read();
    if (tmp >= 128) { serial_in = tmp-128; }
    else if (serial_in == 0) {led1(tmp<<2);}
    else if (serial_in == 1) {led2(tmp<<2);}
    else if (serial_in == 2) {led3(tmp<<2);}
    else if (serial_in == 3) {led4(tmp<<2);}
    else if (serial_in == 4) {
      if(tmp & 0b00100000) default1_5V(); else default1_0V();
      if(tmp & 0b00010000) default2_5V(); else default2_0V();
      if(tmp & 0b00001000) default3_5V(); else default3_0V();
      if(tmp & 0b00000100) default4_5V(); else default4_0V();
      if(tmp & 0b00000010) default5_5V(); else default5_0V();
      if(tmp & 0b00000001) default6_5V(); else default6_0V();      
    }
    else if (serial_in == 5) { // 133 : calibre CV1 0V
      tmp = CV_filter16_out[index_filter_cv1];
      rom_write_int32(rom_address_CV1_0V, tmp);
      delay(200);
    }
    else if (serial_in == 6) { // 134 : calibre CV2 0V
      tmp = CV_filter16_out[index_filter_cv2];
      rom_write_int32(rom_address_CV2_0V, tmp);
      delay(200);
    }
    else if (serial_in == 7) { // 135 : calibre CV3 0V
      tmp = CV_filter16_out[index_filter_cv3];
      rom_write_int32(rom_address_CV3_0V, tmp);
      delay(200);
    }
    else if (serial_in == 8) { // 136 : calibre CV4 0V
      tmp = CV_filter16_out[index_filter_cv4];
      rom_write_int32(rom_address_CV4_0V, tmp);
      delay(200);
    }
    else if (serial_in == 9) { //  calibre IN1 0V
      tmp = filtre_audioL;
      rom_write_int32(rom_address_IN1_0V, tmp);
      tmp >>= 18;
      Serial.write(142);
      Serial.write(tmp>>7);
      Serial.write(tmp & 0b01111111);
      delay(200);
    }
    else if (serial_in == 10) { //  calibre IN2 0V
      tmp = filtre_audioR;
      rom_write_int32(rom_address_IN2_0V, tmp);
      tmp >>= 18;
      Serial.write(142);
      Serial.write(tmp>>7);
      Serial.write(tmp & 0b01111111);  
      delay(200);
    }
    else if (serial_in == 11) { // calibre CV1 1V
      tmp = CV_filter16_out[index_filter_cv1] - rom_read_int32(rom_address_CV1_0V);
      rom_write_int32(rom_address_CV1_1V, tmp);
      delay(200);
    }
    else if (serial_in == 12) { // calibre CV2 1V
      tmp = CV_filter16_out[index_filter_cv2] - rom_read_int32(rom_address_CV2_0V);
      rom_write_int32(rom_address_CV2_1V, tmp);
      delay(200);
    }
    else if (serial_in == 13) { // calibre CV3 1V
      tmp = CV_filter16_out[index_filter_cv3] - rom_read_int32(rom_address_CV3_0V);
      rom_write_int32(rom_address_CV3_1V, tmp);
      delay(200);
    }
    else if (serial_in == 14) { // calibre CV4 1V
      tmp = CV_filter16_out[index_filter_cv4] - rom_read_int32(rom_address_CV4_0V);
      rom_write_int32(rom_address_CV4_1V, tmp);
      delay(200);
    }
    else if (serial_in == 15) { //  calibre IN1 1V
      tmp = filtre_audioL - rom_read_int32(rom_address_IN1_0V);
      rom_write_int32(rom_address_IN1_1V, tmp);
      tmp >>= 18;
      Serial.write(142);
      Serial.write(tmp>>7);
      Serial.write(tmp & 0b01111111);
      delay(200);
    }
    else if (serial_in == 16) { //  calibre IN2 1V
      tmp = filtre_audioL - rom_read_int32(rom_address_IN2_0V);
      rom_write_int32(rom_address_IN2_1V, tmp);
      tmp >>= 18;
      Serial.write(142);
      Serial.write(tmp>>7);
      Serial.write(tmp & 0b01111111);
      delay(200);
    }
    else if (serial_in == 17) { //  calibre auto des OUT
      mode = 3; 
      curent_cal = 0; 
      timer_cal = 0; 
      audio_outL = 0;
      audio_outR = 0;
      cal_out = 1;
      while (cal_out < 5) {delay(100);} // on attend que la calibration auto soit fini
      cal_out = 0;
    }
    else if (serial_in == 18) { // all in at 0V
      rom_write_int32(rom_address_CV1_0V, (CV_filter16_out[index_filter_cv1]));
      rom_write_int32(rom_address_CV2_0V, (CV_filter16_out[index_filter_cv2]));
      rom_write_int32(rom_address_CV3_0V, (CV_filter16_out[index_filter_cv3]));
      rom_write_int32(rom_address_CV4_0V, (CV_filter16_out[index_filter_cv4]));
      rom_write_int32(rom_address_IN1_0V, filtre_audioL);
      rom_write_int32(rom_address_IN2_0V, filtre_audioR);
      delay(200);
    }
    else if (serial_in == 19) { // all in at 1V
      rom_write_int32(rom_address_CV1_1V, CV_filter16_out[index_filter_cv1] - rom_read_int32(rom_address_CV1_0V));
      rom_write_int32(rom_address_CV2_1V, CV_filter16_out[index_filter_cv2] - rom_read_int32(rom_address_CV2_0V));
      rom_write_int32(rom_address_CV3_1V, CV_filter16_out[index_filter_cv3] - rom_read_int32(rom_address_CV3_0V));
      rom_write_int32(rom_address_CV4_1V, CV_filter16_out[index_filter_cv4] - rom_read_int32(rom_address_CV4_0V));
      rom_write_int32(rom_address_IN1_1V, filtre_audioL  - rom_read_int32(rom_address_IN1_0V));
      rom_write_int32(rom_address_IN2_1V, filtre_audioR  - rom_read_int32(rom_address_IN2_0V));
      delay(200);
    }
  }
  delay(15);
}

inline void calibrate_pd_audio_() {
  calibrate_serial_audio_();
}

