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

// This module allow to test and to calibrate the hardware thanks to the arduino serial connection 

uint32_t led;
uint32_t serial_data; 
uint32_t phase, phase2;

// mode : 
//        0 : en attente de status
//        1 : On a reçu un status "C" (calibrate), on attend de savoir quoi
//        2 : on a reçu un 2eme octet designant ce que l'on calibre, on attent de savoir a quel tenssion (0, ou 1V)

// I10 : calibrate CV1 @ 0V
// I11 : calibrate CV1 @ 1V
// I50 : calibrate audio1 @ 0V
// ....

inline void rien_init_() { set_led2(0);set_led4(0);}
inline void rien_loop_() { 
  phase++; 
  if (phase & 0x20000) {
    set_led2(0);
    set_led4(255);
  }
  else {
    set_led4(0);
    set_led2(255);
  }
}
  
inline void rien_audio_() { audio_outL = 0x80000000; audio_outR = 0x80000000; }

inline void calibrate_serial_init_() {
  Serial.begin(115200);
  mode = 0;
  cal_out = 0; // auto audio out calibration 

  default1_0V();
  default2_0V();
  default3_0V();
  default4_0V();
  default5_0V();
  default6_0V();

}

inline void calibrate_serial_loop_() {
  uint32_t debounce_tmp, debounce_tmp2, debounce_tmp3, debounce_index;
  int32_t debounce_tmpS;
  uint32_t tmp, i, accum;
  filter16_nozori_68

  if (cal_out == 0) {
    tmp = (CV_filter16_out[index_filter_pot1])>>4;
    Serial.print("Pot1:");
    Serial.print(tmp);
  
    tmp = (CV_filter16_out[index_filter_pot2])>>4;
    Serial.print(" Pot2:");
    Serial.print(tmp);
  
    tmp = (CV_filter16_out[index_filter_pot3])>>4;
    Serial.print(" Pot3:");
    Serial.print(tmp);
  
    tmp = (CV_filter16_out[index_filter_pot4])>>4;
    Serial.print(" Pot4:");
    Serial.print(tmp);
  
    tmp = (CV_filter16_out[index_filter_pot5])>>4;
    Serial.print(" Pot5:");
    Serial.print(tmp);
  
    tmp = (CV_filter16_out[index_filter_pot6])>>4;
    Serial.print(" Pot6:");
    Serial.print(tmp);
  
    tmp = (CV_filter16_out[index_filter_cv1])>>4;
    Serial.print(" CV1:");
    Serial.print(tmp);
  
    tmp = (CV_filter16_out[index_filter_cv2])>>4;
    Serial.print(" CV2/Pot7:");
    Serial.print(tmp);
  
    tmp = (CV_filter16_out[index_filter_cv3])>>4;
    Serial.print(" CV3:");
    Serial.print(tmp);
  
    tmp = (CV_filter16_out[index_filter_cv4])>>4;
    Serial.print(" CV4/Pot8: ");
    Serial.print(tmp);
    
    //tmp = (audio_inL)>>20;
    tmp = (filtre_audioL)>>20;
    Serial.print(" In1: ");
    Serial.print(tmp);
  
    //tmp = (audio_inR)>>20;
    tmp = (filtre_audioR)>>20;
    Serial.print(" In2: ");
    Serial.print(tmp);
  
    tmp = get_toggle();
    Serial.print(" tgl: ");
    Serial.print(tmp);  
  
    tmp = get_switch();
    Serial.print(" switchs: ");
    Serial.print(tmp);  
  
    Serial.println();

    led++;
  
    led1((led*2) & 0x1FF);
    led2((led*3) & 0x1FF);
    led3((led*5) & 0x1FF);
    led4((led*7) & 0x1FF);

    if (((led*2)& 0x1FF)  > 0xFF) default1_5V(); else default1_0V();
    if (((led*3)& 0x1FF)  > 0xFF) default2_5V(); else default2_0V();
    if (((led*5)& 0x1FF)  > 0xFF) default3_5V(); else default3_0V();
    if (((led*7)& 0x1FF)  > 0xFF) default4_5V(); else default4_0V();
    if (((led*9)& 0x1FF)  > 0xFF) default5_5V(); else default5_0V();
    if (((led*11)& 0x1FF) > 0xFF) default6_5V(); else default6_0V();
  }
  else {
    set_led1(0);
    set_led2(0);
    set_led3(0);
    set_led4(0);
    if (cal_out == 1) { //output1 calibration at 0V
      Serial.print("auto calibration of output 1 at 0V. output : ");
      Serial.print((audio_outL));
      Serial.print(" to get : ");
      Serial.print((audio_inL));
      Serial.print("; goal is : ");
      Serial.println(IN1_0V);
    }
    if (cal_out == 2) { //output1 calibration at 1V
      Serial.print("auto calibration of output 1 at 1V. output : ");
      Serial.print((audio_outL));
      Serial.print(" to get : ");
      Serial.print((audio_inL));
      Serial.print("; goal is : ");
      Serial.println(IN1_1V_value);
    }
    if (cal_out == 3) { //output2 calibration at 0V
      Serial.print("auto calibration of output 2 at 0V. output : ");
      Serial.print((audio_outR));
      Serial.print(" to get : ");
      Serial.print((audio_inR));
      Serial.print("; goal is : ");
      Serial.println(IN2_0V);
    }
    if (cal_out == 4) { //output2 calibration at 1V
      Serial.print("auto calibration of output 2 at 1V. output : ");
      Serial.print((audio_outR));
      Serial.print(" to get : ");
      Serial.print((audio_inR));
      Serial.print("; goal is : ");
      Serial.println(IN2_1V_value);
    }
    if (cal_out == 5) { 
      Serial.println("auto calibration result :");
      Serial.print("OUT1 0V : ");
      Serial.println(OUT1_0V);
      Serial.print("OUT1 1V : ");
      Serial.println(OUT1_1V);
      Serial.print("OUT2 0V : ");
      Serial.println(OUT2_0V);
      Serial.print("OUT2 1V : ");
      Serial.println(OUT2_1V);
      cal_out = 0;
    }
  }
  
  while (Serial.available() > 0) {
    tmp = Serial.read();
    if (mode == 2) { 
      serial_data += tmp-48; 
      mode = 3;
      switch(serial_data) {
      case(2): // I10 (calibre CV1 0V)
        Serial.println("Calibrate CV1 at 0V");
        rom_write_int32(rom_address_CV1_0V, (CV_filter16_out[index_filter_cv1]));
        delay(500);
      break;
      case(3): // I11
        Serial.println("Calibrate CV1 at 1V");
        rom_write_int32(rom_address_CV1_1V, (CV_filter16_out[index_filter_CV1]) - rom_read_int32(rom_address_CV1_0V));
        delay(500);
      break;
      case(4): // I20
        Serial.println("Calibrate CV2 at 0V");
        rom_write_int32(rom_address_CV2_0V, (CV_filter16_out[index_filter_cv2]));
        delay(500);
      break;
      case(5): // I21
        Serial.println("Calibrate CV2 at 1V");
        rom_write_int32(rom_address_CV2_1V, (CV_filter16_out[index_filter_CV2]) - rom_read_int32(rom_address_CV2_0V));
        delay(500);
      break;
      case(6): // I30
        Serial.println("Calibrate CV3 at 0V");
        rom_write_int32(rom_address_CV3_0V, (CV_filter16_out[index_filter_cv3]));
        delay(500);
      break;
      case(7): // I31
        Serial.println("Calibrate CV3 at 1V");
        rom_write_int32(rom_address_CV3_1V, (CV_filter16_out[index_filter_CV3]) - rom_read_int32(rom_address_CV3_0V));
        delay(500);
      break;
      case(8): // I40
        Serial.println("Calibrate CV4 at 0V");
        rom_write_int32(rom_address_CV4_0V, (CV_filter16_out[index_filter_cv4]));
        delay(500);
      break;
      case(9): // I41
        Serial.println("Calibrate CV4 at 1V");
        rom_write_int32(rom_address_CV4_1V, (CV_filter16_out[index_filter_CV4]) - rom_read_int32(rom_address_CV4_0V));
        delay(500);
      break;
      case(10): // I50
        Serial.println("Calibrate IN1 at 0V");
        rom_write_int32(rom_address_IN1_0V, filtre_audioL);
        delay(500);
      break;
      case(11): // I51
        Serial.println("Calibrate IN1 at 1V");
        rom_write_int32(rom_address_IN1_1V, filtre_audioL - rom_read_int32(rom_address_IN1_0V));
        delay(500);
      break;
      case(12): // I60
        Serial.println("Calibrate IN2 at 0V");
        rom_write_int32(rom_address_IN2_0V, filtre_audioR);
        delay(500);
      break;
      case(13): // I61
        Serial.println("Calibrate IN2 at 1V");
        rom_write_int32(rom_address_IN2_1V, filtre_audioR - rom_read_int32(rom_address_IN2_0V));
        delay(500);
      break;
      case(14): // I70
        Serial.println("Calibrate all cv/in at 0V");
        rom_write_int32(rom_address_CV1_0V, (CV_filter16_out[index_filter_cv1]));
        rom_write_int32(rom_address_CV2_0V, (CV_filter16_out[index_filter_cv2]));
        rom_write_int32(rom_address_CV3_0V, (CV_filter16_out[index_filter_cv3]));
        rom_write_int32(rom_address_CV4_0V, (CV_filter16_out[index_filter_cv4]));
        rom_write_int32(rom_address_IN1_0V, filtre_audioL);
        rom_write_int32(rom_address_IN2_0V, filtre_audioR);
        Serial.print("CV1 0V = "); Serial.println(rom_read_int32(rom_address_CV1_0V));
        Serial.print("CV2 0V = "); Serial.println(rom_read_int32(rom_address_CV2_0V));
        Serial.print("CV3 0V = "); Serial.println(rom_read_int32(rom_address_CV3_0V));
        Serial.print("CV4 0V = "); Serial.println(rom_read_int32(rom_address_CV4_0V));
        Serial.print("IN1 0V = "); Serial.println(rom_read_int32(rom_address_IN1_0V));
        Serial.print("IN2 0V = "); Serial.println(rom_read_int32(rom_address_IN2_0V));
        delay(500);
      break;
      case(15): // I71
        Serial.println("Calibrate all cv/in at 1V");
        rom_write_int32(rom_address_CV1_1V, CV_filter16_out[index_filter_cv1] - rom_read_int32(rom_address_CV1_0V));
        rom_write_int32(rom_address_CV2_1V, CV_filter16_out[index_filter_cv2] - rom_read_int32(rom_address_CV2_0V));
        rom_write_int32(rom_address_CV3_1V, CV_filter16_out[index_filter_cv3] - rom_read_int32(rom_address_CV3_0V));
        rom_write_int32(rom_address_CV4_1V, CV_filter16_out[index_filter_cv4] - rom_read_int32(rom_address_CV4_0V));
        rom_write_int32(rom_address_IN1_1V, filtre_audioL  - rom_read_int32(rom_address_IN1_0V));
        rom_write_int32(rom_address_IN2_1V, filtre_audioR  - rom_read_int32(rom_address_IN2_0V));
        Serial.print("CV1 1V = "); Serial.println(rom_read_int32(rom_address_CV1_1V));
        Serial.print("CV2 1V = "); Serial.println(rom_read_int32(rom_address_CV2_1V));
        Serial.print("CV3 1V = "); Serial.println(rom_read_int32(rom_address_CV3_1V));
        Serial.print("CV4 1V = "); Serial.println(rom_read_int32(rom_address_CV4_1V));
        Serial.print("IN1 1V = "); Serial.println(rom_read_int32(rom_address_IN1_1V));
        Serial.print("IN2 1V = "); Serial.println(rom_read_int32(rom_address_IN2_1V));
        delay(500);
      break;
      }
    }
    if (mode == 1) { 
      serial_data = (tmp-48)<<1; 
      mode = 2;
    }
    if (tmp == 79) {// O : automatic output calibration, need to connect out to input
      mode = 3; 
      curent_cal = 0; 
      timer_cal = 0; 
      audio_outL = 0;
      audio_outR = 0;
      cal_out = 1;
      }
    if (tmp == 73) {  // "I" : input calibration
      mode = 1; 
      cal_out = 0;
      }
    if (tmp == 86) { // V : verbose : output calibration
      Serial.println("Current calibration :");
      Serial.print("CV1 0V: ");
      Serial.println(CV1_0V);
      Serial.print("CV2 0V: ");
      Serial.println(CV2_0V);
      Serial.print("CV3 0V: ");
      Serial.println(CV3_0V);
      Serial.print("CV4 0V: ");
      Serial.println(CV4_0V);
      
      Serial.print("IN1 0V: ");
      Serial.println(IN1_0V);
      Serial.print("IN2 0V: ");
      Serial.println(IN2_0V);
      
      Serial.print("CV1 1V: ");
      Serial.println(CV1_1Vminus0V);
      Serial.print("CV2 1V: ");
      Serial.println(CV2_1Vminus0V);
      Serial.print("CV3 1V: ");
      Serial.println(CV3_1Vminus0V);
      Serial.print("CV4 1V: ");
      Serial.println(CV4_1Vminus0V);

      Serial.print("IN1 1V: ");
      Serial.println(IN1_1V_value);
      Serial.print("IN2 1V: ");
      Serial.println(IN2_1V_value);

      delay(1000);
      mode = 0;
      cal_out = 0;
    }
  }
  delay(50);
}

inline void calibrate_serial_audio_() {
  uint32_t tmp;
  
  filtre_audioL -= filtre_audioL >> 12;
  filtre_audioL += audio_inL >> 12;
  
  filtre_audioR -= filtre_audioR >> 12;
  filtre_audioR += audio_inR >> 12;
  
  if (cal_out == 0) {
    phase += 39370534>>1; // 440 Hz
    phase2 += 89478485>>1; // 1KHz
    tmp = fast_sin(phase);
    audio_outR = tmp;
    analog_out_1(tmp);
    
    tmp = fast_sin(phase2);
    audio_outL = tmp;
    analog_out_2(tmp);
  }
  else {
    timer_cal++;
    if (timer_cal > 100) { // temps de latence entrée / sortie
      timer_cal = 0;
      if (cal_out == 1) { // auto calibration OUT 1 -> 0V
        tmp = IN1_0V;
        if (audio_inL < tmp) { // tant que le but n'est pas atteind
          curent_cal += (tmp - audio_inL) / 100; // on filtre pour arriver doucement a la bonne valeur
        }
        else { // on arrive au nvx de bruit, dc on s'arrete
          rom_write_int32(rom_address_OUT1_0V, audio_outL);
          OUT1_0V = audio_outL;
          cal_out = 2;
          curent_cal = 0;
        }
      }
      else if (cal_out == 2) { // auto calibration OUT 1 -> 1V
        tmp = IN1_0V + IN1_1V_value;
        if (audio_inL < tmp) { // tant que le but n'est pas atteind
          curent_cal += (tmp - audio_inL) / 100; // on filtre pour arriver doucement a la bonne valeur
        }
        else { // on arrive au nvx de bruit, dc on s'arrete
          rom_write_int32(rom_address_OUT1_1V, audio_outL);
          OUT1_1V = audio_outL;
          cal_out = 3;
          curent_cal = 0;
        }
      }
      else if (cal_out == 3) { // auto calibration OUT 2 -> 0V
        tmp = IN2_0V;
        if (audio_inR < tmp) { // tant que le but n'est pas atteind
          curent_cal += (tmp - audio_inR) / 100; // on filtre pour arriver doucement a la bonne valeur
        }
        else { // on arrive au nvx de bruit, dc on s'arrete
          rom_write_int32(rom_address_OUT2_0V, audio_outR);
          OUT2_0V = audio_outR;
          cal_out = 4;
          curent_cal = 0;
        }
      }
      else if (cal_out == 4) { // auto calibration OUT 2 -> 1V
        tmp = IN2_0V + IN2_1V_value;
        if (audio_inR < tmp) { // tant que le but n'est pas atteind
          curent_cal += (tmp - audio_inR) / 100; // on filtre pour arriver doucement a la bonne valeur
        }
        else { // on arrive au nvx de bruit, dc on s'arrete
          rom_write_int32(rom_address_OUT2_1V, audio_outR);
          OUT2_1V = audio_outR;
          cal_out = 5;
          curent_cal = 0;
        }
      }
    }
    audio_outL = curent_cal;
    audio_outR = curent_cal;
  }
}

