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

// this file regroupe fonction of maini perifericals

// DAC
#define codec_ctl_port_start PIOD
#define codec_ctl_port_clock PIOC
#define codec_ctl_port_data PIOC
#define codec_ctl_start (1<<10) // 32 : PD10
#define codec_ctl_clock (1<<4) // 36 : PC4
#define codec_ctl_data  (1<<2) // 34 : PC2

inline void send_dac(uint32_t dac_register, uint32_t dac_data) {
  // 3 wire protocole to send 9 bit to a 7bit adresse
  
  uint32_t i, j, to_send;

  to_send = (dac_register & 0x7F) << 9; // 7bit addresse
  to_send += dac_data & 0x1FF; // 9 bit of data

  for(j=0; j<5000; j++){nop();}; // wait

  codec_ctl_port_clock->PIO_CODR = codec_ctl_clock; // clock to 0
  codec_ctl_port_start->PIO_CODR = codec_ctl_start; // latch to 0

  for (i=0; i<16; i++) {
    if (to_send & 1<<15)  
      codec_ctl_port_data->PIO_SODR = codec_ctl_data; // prepare the bit to send
    else
      codec_ctl_port_data->PIO_CODR = codec_ctl_data;
    
    codec_ctl_port_clock->PIO_SODR = codec_ctl_clock; 
    to_send <<= 1;
    for(j=0; j<5000; j++){nop();}; // wait
    codec_ctl_port_clock->PIO_CODR = codec_ctl_clock; // latch the bit
    for(j=0; j<5000; j++){nop();}; // wait
  }
  codec_ctl_port_start->PIO_SODR = codec_ctl_start; // latch the data
}

inline void init_dac() {
  // configuration of the 3 wire control
  
  // set the 3 control pins as output
  codec_ctl_port_start->PIO_PER = codec_ctl_start;
  codec_ctl_port_start->PIO_OER = codec_ctl_start;

  codec_ctl_port_clock->PIO_PER = codec_ctl_clock;
  codec_ctl_port_clock->PIO_OER = codec_ctl_clock;

  codec_ctl_port_data->PIO_PER = codec_ctl_data;
  codec_ctl_port_data->PIO_OER = codec_ctl_data;

  // configure register using 3 wire protocol
  send_dac(0x0F,0); // reset dac

  send_dac(0x06,0b000010010); // powerdown
  send_dac(0x00,0b000011111); // left line in 
  send_dac(0x01,0b000011111); // right line in 
  send_dac(0x02,0b000000000); // left headphone to 0
  send_dac(0x03,0b000000000); // right headphone
  send_dac(0x04,0b011010010); // analogue audio path
  if (DAC_clear == 1)
    send_dac(0x05,0b000010001); // digital audio path (high pass , no desenphasis, store offset)
  else
    send_dac(0x05,0b000010111); // digital audio path (high pass , desenphasis, store offset)

  //send_dac(0x07,0b001000110); // digital audio interface (I2S / 32 bit / right channel when daclrc is low / not swap / master/ don't invert blck)
  send_dac(0x07,0b001100110); // digital audio interface (I2S / 32 bit / right channel when daclrc is low / not swap / master/ invert blck)
  send_dac(0x08,0b000011101); // sampling control (usb , 250fs,  96K, clock div 2, clk out, active)
  //send_dac(0x08,0b000000001); // sampling control (usb , 250fs,  48K, clock div 2, clk out, active)
  //send_dac(0x08,0b000100011); // sampling control (usb , 250fs,  44.1K, clock div 2, clk out, active)
  //send_dac(0x08,0b000011001); // sampling control (usb , 250fs,  32K, clock div 2, clk out, active)
  send_dac(0x09,0b000000001);

  // configuration of the 12S slave communication
  // TK : transmiter clock (input)    : PA 14 (B)
  // TF : transmitter synchro (input) : PA 15 (B)
  // TD : transmit data (output)      : PA 16 (B)
  // RF : receive synchro (input)     : PB 17 (A)
  // RD : receive data : input)       : PB 18 (A)
  
}

inline void start_dac() {
    PIOA->PIO_PDR  = (1 << 14) + (1 << 15) + (1 << 16); // peripheral control of the pin
    PIOB->PIO_PDR  = (1 << 17) + (1 << 18);
    PIOA->PIO_PUDR = (1 << 14) + (1 << 15) + (1 << 16); // disable pull_up
    PIOB->PIO_PUDR = (1 << 17) + (1 << 18); 
    PIOA->PIO_ABSR =  PIOA->PIO_ABSR | ((1 << 14) + (1 << 15) + (1 << 16)); // peripherique B 
    PIOB->PIO_ABSR =  PIOB->PIO_ABSR & (!((1 << 17) + (1 << 18)));
    
    pmc_enable_periph_clk(ID_SSC); 
    NVIC_EnableIRQ(SSC_IRQn);
  
    REG_SSC_TCMR = 0x00010702;
    //REG_SSC_TFMR = 0x0000019F;
    REG_SSC_TFMR = 0x00000197; // 24 bits
    REG_SSC_RCMR = 0x00010121;
    REG_SSC_RFMR = 0x0000009F;
    
    ssc_enable_interrupt(SSC, SSC_IER_RXRDY);
    //ssc_enable_interrupt(SSC, SSC_IER_TXSYN);
  
    ssc_enable_rx(SSC);
    ssc_enable_tx(SSC);
   
    send_dac(0x06,0b000000010); // switch dac on
}

// ------------------------------------------------------------------------------------------------------------
// test : debug out

#define PORTTEST1 PIOA // pin 4
#define TEST1 29
#define PORTTEST2 PIOC // pin 5
#define TEST2 25

inline void init_test() {
  PORTTEST1->PIO_PER  = 1 << TEST1;
  PORTTEST1->PIO_OER  = 1 << TEST1;
  PORTTEST1->PIO_OWER = 1 << TEST1;
  PORTTEST1->PIO_PUDR = 1 << TEST1;
  PORTTEST2->PIO_PER  = 1 << TEST2;
  PORTTEST2->PIO_OER  = 1 << TEST2;
  PORTTEST2->PIO_OWER = 1 << TEST2;
  PORTTEST2->PIO_PUDR = 1 << TEST2;
}

inline void test1_toggle() {
  PORTTEST1->PIO_ODSR ^=  1 << TEST1;
}
inline void test1_on() {
  PORTTEST1->PIO_SODR =  1 << TEST1;
}
inline void test1_off() {
  PORTTEST1->PIO_CODR =  1 << TEST1;
}
inline void test2_on() {
  PORTTEST2->PIO_SODR =  1 << TEST2;
}
inline void test2_off() {
  PORTTEST2->PIO_CODR =  1 << TEST2;
}
inline void test2_toggle() {
  PORTTEST2->PIO_ODSR ^=  1 << TEST2;
}
inline void test1(uint32_t value) {
  if (value == 0) 
    PORTTEST1->PIO_CODR =  1 << TEST1;
  else
    PORTTEST1->PIO_SODR =  1 << TEST1;
}
inline void test2(uint32_t value) {
  if (value == 0) 
    PORTTEST2->PIO_CODR =  1 << TEST2;
  else
    PORTTEST2->PIO_SODR =  1 << TEST2;
}

// ------------------------------------------------------------------------------------------------------------
// random

volatile uint32_t rnd_seed;

inline void init_random() {
  REG_PMC_PCER1 = 1 << 9; // enable la clock du port PIO A median_sizepour les entree
  REG_TRNG_CR = 0x524e4701; // start random number generation 
}
inline uint32_t pseudo_rnd(uint32_t seed) {
  uint32_t rnd_bit;
  uint32_t tmp = seed;

  rnd_bit  = ((seed >> 0) ^ (seed >> 2) ^ (seed >> 3) ^ (seed >> 5) );
  tmp =  (seed >> 1) | (rnd_bit << 31);
  return(tmp);
}
inline uint32_t random32() {
  rnd_seed =  (REG_TRNG_ISR & 0x01)? REG_TRNG_ODATA : pseudo_rnd(rnd_seed); 
  // return a random value generated by the pross
  // if no random is available, then compute a pseudo - random number based on last one
  return(rnd_seed);
}

// ------------------------------------------------------------------------------------------------------------
// LEDS

/*
#define PORTPWM4 PIOC 
#define PWM4 22
#define PORTPWM3 PIOC
#define PWM3 24
#define PORTPWM1 PIOC
#define PWM1 21
#define PORTPWM2 PIOC
#define PWM2 23
*/

inline void init_led() {

  analogWrite(6, 0);  // lazy PWM configuration
  analogWrite(7, 0);
  analogWrite(8, 0);
  analogWrite(9, 0);

  // speed pwm clock
  REG_PWM_CLK = REG_PWM_CLK & 0xFFFFFF00;
  REG_PWM_CLK = REG_PWM_CLK | 0x00000001;
}

inline void set_led1(uint32_t value) {  
  //PWMC_SetDutyCycle(PWM_INTERFACE, 4, value); // direct led control (from 0 to 255)
  PWM_INTERFACE->PWM_CH_NUM[4].PWM_CDTYUPD = value;
} 
inline void led1(uint32_t value) {  
  //PWMC_SetDutyCycle(PWM_INTERFACE, 4, table_led[value]);  // nice curve for the led conntrol (from 0 to 511)
  PWM_INTERFACE->PWM_CH_NUM[4].PWM_CDTYUPD = table_led[value];
}
inline void set_led2(uint32_t value) {  
  //PWMC_SetDutyCycle(PWM_INTERFACE, 7, value);  
  PWM_INTERFACE->PWM_CH_NUM[7].PWM_CDTYUPD = value;
}
inline void led2(uint32_t value) {  
  //PWMC_SetDutyCycle(PWM_INTERFACE, 7, table_led[value]);  
  PWM_INTERFACE->PWM_CH_NUM[7].PWM_CDTYUPD = table_led[value];
}
inline void set_led3(uint32_t value) {  
  //PWMC_SetDutyCycle(PWM_INTERFACE, 5, value);  
  PWM_INTERFACE->PWM_CH_NUM[5].PWM_CDTYUPD = value;
}
inline void led3(uint32_t value) {  
  //PWMC_SetDutyCycle(PWM_INTERFACE, 5, table_led[value]);  
  PWM_INTERFACE->PWM_CH_NUM[5].PWM_CDTYUPD = table_led[value];
}
inline void set_led4(uint32_t value) {  
  //PWMC_SetDutyCycle(PWM_INTERFACE, 6, value); 
  PWM_INTERFACE->PWM_CH_NUM[6].PWM_CDTYUPD = value;
}
inline void led4(uint32_t value) {  
  //PWMC_SetDutyCycle(PWM_INTERFACE, 6, table_led[value]); 
  PWM_INTERFACE->PWM_CH_NUM[6].PWM_CDTYUPD = table_led[value];
}

// ------------------------------------------------------------------------------------------------------------
// ANA_OUT onboard 12 bit DAC

inline void init_analog_out() {
  REG_PMC_PCER1 = 0X00000040; // switch on peripherical clock
  REG_DACC_MR = 0x01000000;
  REG_DACC_CHER = 0X00000003; 
  REG_DACC_ACR = 0x0000001A;
  REG_DACC_CDR = 0; // premiere conversion
}

inline void analog_out_1(uint32_t signal_out) {
  while ((REG_DACC_ISR & 0x00000002) == 0); // wait the converter to be ready
  REG_DACC_MR &= ~(0x00010000); // channel 0
  REG_DACC_CDR = (signal_out >> 20) & 0xFFF; // 12 MSB
}
inline void analog_out_2(uint32_t signal_out) {
  while ((REG_DACC_ISR & 0x00000002) == 0); // wait the converter to be ready
  REG_DACC_MR |= 0x00010000; // channel 1
  REG_DACC_CDR = (signal_out >> 20) & 0xFFF; // 12 MSB
}

// ------------------------------------------------------------------------------------------------------------
// ANA_IN

#define PORTDEFAULT1 PIOC // pin PWM10
#define DEFAULT1 29
#define PORTDEFAULT2 PIOD // pin PWM12
#define DEFAULT2 8
#define PORTDEFAULT3 PIOB // pin PWM13
#define DEFAULT3 27
#define PORTDEFAULT4 PIOD // pin PWM11
#define DEFAULT4 7

#define PORTDEFAULT6 PIOC // pin PWM3
#define DEFAULT6 28
#define PORTDEFAULT5 PIOB // pin PWM2
#define DEFAULT5 25

/* // rapel des define du .h
#define pot1 13; // A11   // PB20
#define pot2 12; // A10   // PB19
#define pot3 14; // PIN52 (AD14) // PB21
#define pot4 1; // A6    // PA3
#define pot5 0; // A7    // PA2
#define pot6 2; // A5    // PA4
#define CV1 4;  // A3    // PA22
#define CV2 3;  // A4    // PA6
#define CV3 5;  // A2    // PA23
#define CV4 6;  // A1    // PA24
*/

inline void init_analog_in() {

/*
  REG_PMC_PCER1 = 1 << 5; // switch on ADC peripheral clock

  // disable pullup on analog input
  PIOA->PIO_PUDR = (1 << 2) + (1 << 3) + (1 << 4) + (1 << 6) + (1 << 22) + (1 << 23) + (1 << 24);
  PIOB->PIO_PUDR = (1 << 19) + (1 << 20) + (1 << 21);
  
  REG_ADC_EMR = 0x00000000;
  REG_ADC_CR = 1<<1; // start
*/
  ADC->ADC_WPMR &= 0xFFFE; //disable write protect
  REG_ADC_CHER = 0b0111000001111111; // enable the correct channels

  ADC->ADC_ACR = 0x00000100;
  //REG_ADC_MR  = 0x0F00FF80;
  //REG_ADC_MR  = 0x0F000F80; // critique : OK
  //REG_ADC_MR  = 0x02000180; // ok avec 5K
  REG_ADC_MR  = 0x02000380; // ok avec 10K
  
  //adc_set_bias_current (ADC, 1); 
  
  PORTDEFAULT1->PIO_PER  = 1 << DEFAULT1; // default value out
  PORTDEFAULT1->PIO_OER  = 1 << DEFAULT1;
  PORTDEFAULT1->PIO_OWER = 1 << DEFAULT1;
  PORTDEFAULT1->PIO_PUDR = 1 << DEFAULT1;
  PORTDEFAULT2->PIO_PER  = 1 << DEFAULT2;
  PORTDEFAULT2->PIO_OER  = 1 << DEFAULT2;
  PORTDEFAULT2->PIO_OWER = 1 << DEFAULT2;
  PORTDEFAULT2->PIO_PUDR = 1 << DEFAULT2;
  PORTDEFAULT3->PIO_PER  = 1 << DEFAULT3;
  PORTDEFAULT3->PIO_OER  = 1 << DEFAULT3;
  PORTDEFAULT3->PIO_OWER = 1 << DEFAULT3;
  PORTDEFAULT3->PIO_PUDR = 1 << DEFAULT3;
  PORTDEFAULT4->PIO_PER  = 1 << DEFAULT4;
  PORTDEFAULT4->PIO_OER  = 1 << DEFAULT4;
  PORTDEFAULT4->PIO_OWER = 1 << DEFAULT4;
  PORTDEFAULT4->PIO_PUDR = 1 << DEFAULT4;
  PORTDEFAULT5->PIO_PER  = 1 << DEFAULT5;
  PORTDEFAULT5->PIO_OER  = 1 << DEFAULT5;
  PORTDEFAULT5->PIO_OWER = 1 << DEFAULT5;
  PORTDEFAULT5->PIO_PUDR = 1 << DEFAULT5;
  PORTDEFAULT6->PIO_PER  = 1 << DEFAULT6;
  PORTDEFAULT6->PIO_OER  = 1 << DEFAULT6;
  PORTDEFAULT6->PIO_OWER = 1 << DEFAULT6;
  PORTDEFAULT6->PIO_PUDR = 1 << DEFAULT6;

  default1_0V();
  default2_0V();
  default3_0V();
  default4_0V();
  default5_0V();
  default6_0V();

  // ADC init when using a firmware that is not for the correct nozori board
  CV_filter16_out[6]  = 0;      // pot7
  CV_filter16_out[7]  = 0;      // pot8
  CV_filter16_out[8]  = 0x7FFF; // CV1
  CV_filter16_out[9]  = 0x7FFF; // CV2
  CV_filter16_out[10] = 0x7FFF; // CV3
  CV_filter16_out[11] = 0x7FFF; // CV4
}

inline void default1_5V() { 
  PORTDEFAULT1->PIO_SODR =  1 << DEFAULT1;
}
inline void default1_0V() {
  PORTDEFAULT1->PIO_CODR =  1 << DEFAULT1;
}
inline void default2_5V() {
  PORTDEFAULT2->PIO_SODR =  1 << DEFAULT2;
}
inline void default2_0V() {
  PORTDEFAULT2->PIO_CODR =  1 << DEFAULT2;
}
inline void default3_5V() {
  PORTDEFAULT3->PIO_SODR =  1 << DEFAULT3;
}
inline void default3_0V() {
  PORTDEFAULT3->PIO_CODR =  1 << DEFAULT3;
}
inline void default4_5V() {
  PORTDEFAULT4->PIO_SODR =  1 << DEFAULT4;
}
inline void default4_0V() {
  PORTDEFAULT4->PIO_CODR =  1 << DEFAULT4;
}
inline void default5_5V() {
  PORTDEFAULT5->PIO_SODR =  1 << DEFAULT5;
}
inline void default5_0V() {
  PORTDEFAULT5->PIO_CODR =  1 << DEFAULT5;
}
inline void default6_5V() {
  PORTDEFAULT6->PIO_SODR =  1 << DEFAULT6;
}
inline void default6_0V() {
  PORTDEFAULT6->PIO_CODR =  1 << DEFAULT6;
}

inline void default1(uint32_t value) {
  if (value == 0) 
    PORTDEFAULT1->PIO_CODR =  1 << DEFAULT1;
  else
    PORTDEFAULT1->PIO_SODR =  1 << DEFAULT1;
}
inline void default2(uint32_t value) {
  if (value == 0) 
    PORTDEFAULT2->PIO_CODR =  1 << DEFAULT2;
  else
    PORTDEFAULT2->PIO_SODR =  1 << DEFAULT2;
}
inline void default3(uint32_t value) {
  if (value == 0) 
    PORTDEFAULT3->PIO_CODR =  1 << DEFAULT3;
  else
    PORTDEFAULT3->PIO_SODR =  1 << DEFAULT3;
}
inline void default4(uint32_t value) {
  if (value == 0) 
    PORTDEFAULT4->PIO_CODR =  1 << DEFAULT4;
  else
    PORTDEFAULT4->PIO_SODR =  1 << DEFAULT4;
}
inline void default5(uint32_t value) {
  if (value == 0) 
    PORTDEFAULT5->PIO_CODR =  1 << DEFAULT5;
  else
    PORTDEFAULT5->PIO_SODR =  1 << DEFAULT5;
}
inline void default6(uint32_t value) {
  if (value == 0) 
    PORTDEFAULT6->PIO_CODR =  1 << DEFAULT6;
  else
    PORTDEFAULT6->PIO_SODR =  1 << DEFAULT6;
}

// ------------------------------------------------------------------------------------------------------------
// CONFIGURATION SWITCH

#define port_switch0 PIOD
#define port_switch1 PIOD
#define port_switch2 PIOD
#define port_switch3 PIOD
#define port_switch4 PIOA
#define port_switch5 PIOC
#define port_switch6 PIOC
#define port_switch7 PIOC

#define pin_switch0 1 // pin26
#define pin_switch1 0 // pin25
#define pin_switch2 2 // pin27
#define pin_switch3 6 // pin29
#define pin_switch4 7 // pin31
#define pin_switch5 1 // pin33
#define pin_switch6 3 // pin35
#define pin_switch7 5 // pin37

inline void init_switch() {
  port_switch0->PIO_PER   = 1 << pin_switch0;
  port_switch0->PIO_PUER  = 1 << pin_switch0;
  port_switch1->PIO_PER   = 1 << pin_switch1;
  port_switch1->PIO_PUER  = 1 << pin_switch1;
  port_switch2->PIO_PER   = 1 << pin_switch2;
  port_switch2->PIO_PUER  = 1 << pin_switch2;
  port_switch3->PIO_PER   = 1 << pin_switch3;
  port_switch3->PIO_PUER  = 1 << pin_switch3;
  port_switch4->PIO_PER   = 1 << pin_switch4;
  port_switch4->PIO_PUER  = 1 << pin_switch4;
  port_switch5->PIO_PER   = 1 << pin_switch5;
  port_switch5->PIO_PUER  = 1 << pin_switch5;
  port_switch6->PIO_PER   = 1 << pin_switch6;
  port_switch6->PIO_PUER  = 1 << pin_switch6;
  port_switch7->PIO_PER   = 1 << pin_switch7;
  port_switch7->PIO_PUER  = 1 << pin_switch7;
}

inline uint32_t get_switch() {
  uint32_t mode;
 
  mode = 0;
  if (!(port_switch0->PIO_PDSR & (1<<pin_switch0))) mode += 1<<0;
  if (!(port_switch1->PIO_PDSR & (1<<pin_switch1))) mode += 1<<1;
  if (!(port_switch2->PIO_PDSR & (1<<pin_switch2))) mode += 1<<2;
  if (!(port_switch3->PIO_PDSR & (1<<pin_switch3))) mode += 1<<3;
  if (!(port_switch4->PIO_PDSR & (1<<pin_switch4))) mode += 1<<4;
  if (!(port_switch5->PIO_PDSR & (1<<pin_switch5))) mode += 1<<5;
  if (!(port_switch6->PIO_PDSR & (1<<pin_switch6))) mode += 1<<6;
  if (!(port_switch7->PIO_PDSR & (1<<pin_switch7))) mode += 1<<7;
  
  return(mode);
}  

/*
inline bool test_cal() { // test for jumper betweeen pin 7 and 8 for calibration
  bool test;
  test = false;
  if (!(port_switch7->PIO_PDSR & (1<<pin_switch7))) { // connecté a la masse, dc on n'est pas en mode calibration 
    return(false);
  }
  if (!(port_switch6->PIO_PDSR & (1<<pin_switch6))) { // connecté a la masse, dc on n'est pas en mode calibration 
    return(false);
  }
  // switch switch 7 out output , and put it to ground.
  port_switch7->PIO_OER  = 1 << pin_switch7;
  port_switch7->PIO_CODR  = 1 << pin_switch7; // if pin6 is now at 0V, it's because it's connected to pin 7
  if (!(port_switch6->PIO_PDSR & (1<<pin_switch6))) { // connecté a la masse 
    port_switch7->PIO_ODR  = 1 << pin_switch7;  // put back to original state
    return(true);
  }
  // put back to original state
  port_switch7->PIO_ODR  = 1 << pin_switch7;
  return(false);
}
*/

inline uint32_t test_mode() {
  uint32_t value;
  value = 0;
  if (get_switch() != 0)
    return(0); // on as une connection, donc on n'est pas en mode special
    
  // switch switch 7 out output , and put it to ground.
  port_switch7->PIO_OER  = 1 << pin_switch7;
  port_switch7->PIO_CODR  = 1 << pin_switch7; 
  if (!(port_switch6->PIO_PDSR & (1<<pin_switch6))) value = 7; // if pin6 is now at 0V, it's because it's connected to pin 7

  // switch switch 6 out output , and put it to ground.
  port_switch6->PIO_OER  = 1 << pin_switch6;
  port_switch6->PIO_CODR  = 1 << pin_switch6; 
  if (!(port_switch5->PIO_PDSR & (1<<pin_switch5))) value = 6;

  // switch switch 5 out output , and put it to ground.
  port_switch5->PIO_OER  = 1 << pin_switch5;
  port_switch5->PIO_CODR  = 1 << pin_switch5; 
  if (!(port_switch4->PIO_PDSR & (1<<pin_switch4))) value = 5;

  // switch switch 4 out output , and put it to ground.
  port_switch4->PIO_OER  = 1 << pin_switch4;
  port_switch4->PIO_CODR  = 1 << pin_switch4; 
  if (!(port_switch3->PIO_PDSR & (1<<pin_switch3))) value = 4;

  // switch switch 3 out output , and put it to ground.
  port_switch3->PIO_OER  = 1 << pin_switch3;
  port_switch3->PIO_CODR  = 1 << pin_switch3; 
  if (!(port_switch2->PIO_PDSR & (1<<pin_switch2))) value = 3;

  // switch switch 2 out output , and put it to ground.
  port_switch2->PIO_OER  = 1 << pin_switch2;
  port_switch2->PIO_CODR  = 1 << pin_switch2; 
  if (!(port_switch1->PIO_PDSR & (1<<pin_switch1))) value = 2;

  // switch switch 1 out output , and put it to ground.
  port_switch1->PIO_OER  = 1 << pin_switch1;
  port_switch1->PIO_CODR  = 1 << pin_switch1; 
  if (!(port_switch0->PIO_PDSR & (1<<pin_switch0))) value = 1;

  // put back to original state
  port_switch7->PIO_ODR  = 1 << pin_switch7;
  port_switch6->PIO_ODR  = 1 << pin_switch6;
  port_switch5->PIO_ODR  = 1 << pin_switch5;
  port_switch4->PIO_ODR  = 1 << pin_switch4;
  port_switch3->PIO_ODR  = 1 << pin_switch3;
  port_switch2->PIO_ODR  = 1 << pin_switch2;
  port_switch1->PIO_ODR  = 1 << pin_switch1;
 
  return(value); 
}


// ------------------------------------------------------------------------------------------------------------
// toggle

#define port_togle1 PIOA
#define port_togle2 PIOC

#define pin_togle1 20 // pin43
//#define pin_togle2 9 // pin41 (old prototype)
#define pin_togle2 18 // pin45

inline void init_toggle() {
  port_togle1->PIO_PER   = 1 << pin_togle1;
  port_togle1->PIO_PUER  = 1 << pin_togle1;
  port_togle2->PIO_PER   = 1 << pin_togle2;
  port_togle2->PIO_PUER  = 1 << pin_togle2;
}

inline uint32_t get_toggle() {
  if (!(port_togle1->PIO_PDSR & (1<<pin_togle1))) return(2); // 1er contact
  else if (!(port_togle2->PIO_PDSR & (1<<pin_togle2))) return(0); // 2eme contact
  else return(1); // center
}

// ------------------------------------------------------------------------------------------------------------
// serial  
  
// RX MIDI
// PD5 : RXD3, peripheric B, USART3
// Midi is 31.25 Kb/s,  chip is at 84 000 KHz,  CD = 168
// for 115.2 communication, use CD = 46 (1% accurate)

inline void init_serial() {
  // RX3D : PD5, peripherical B  // PIOD
  // TX3D : PD4, peripherical B  
  REG_PMC_PCER0 = 1 << 20; //switch on uart3 clock
  USART3->US_MR = 0x000008C0; // misc configuration
  USART3->US_BRGR = 168; // baude rate
  PIOD->PIO_ABSR |= 1<<5; // peripherical B for serial RX pin (PD5)
  PIOD->PIO_ABSR |= 1<<4; // peripherical B for serial TX pin (PD4)
  USART3->US_CR = 0x00000050; // enable transmettre and receiver
}

inline bool test_serial_in() { // test a RX message, must be pooled faster than a serial message
  if (USART3->US_CSR  & (1<<0)) return(true); else return(false);
}
inline uint32_t get_serial_in() { 
    return((USART3->US_RHR) & 0xFF); // one must check the presence of the message first using test_serial_in();
}
inline uint32_t test_serial_out() { // test if a serial byte can be send
    return(USART3->US_CSR & (1<<1)); // 1 if ready
}
inline void serial_out(uint32_t data) { // send a byte, you need to test if the buffer is empty
  USART3->US_THR = data & 0xFF;
}

// ------------------------------------------------------------------------------------------------------------
// save

inline void init_save(){
  DueFlashStorage();
}

inline void rom_write_int32(uint32_t addresse, uint32_t data){  

  write(FLASH_offset + 4*addresse,      data>>24);
  write(FLASH_offset + 4*addresse + 1, (data>>16) & 0xFF);
  write(FLASH_offset + 4*addresse + 2, (data>>8 ) & 0xFF);
  write(FLASH_offset + 4*addresse + 3, (data>>0 ) & 0xFF);  
}

uint32_t rom_read_int32(uint32_t addresse){
  uint32_t data;
  
  data  = read(FLASH_offset + 4*addresse    ) << 24;
  data += read(FLASH_offset + 4*addresse + 1) << 16;
  data += read(FLASH_offset + 4*addresse + 2) << 8;
  data += read(FLASH_offset + 4*addresse + 3) << 0;
  
  return(data);
  //return(0xFFFFFFFF);
}

// ------------------------------------------------------------------------------------------------------------
// test jack connection 
inline void init_test_connect_loop(){ 
  // not connected 
  CV1_connect = 100;
  CV2_connect = 100;
  CV3_connect = 100;
  CV4_connect = 100;
  IN1_connect = 100;
  IN2_connect = 100;
}

inline void test_connect_loop_68(){ 
  uint32_t tmp;
  if (connect_timer > 0x30) { // on met un certain temps entre la sortie et le test pour compenser la lattence des entrées / sortie de l'audio
    connect_timer = 0;
    tmp = connect_test_value;
    
    if ((REG_CV1 > 0b101000000000) == (tmp > 0)) CV1_connect++; else CV1_connect = 0; // meme valeur entre l'entre et la sortie, si cela arrive tout le temps, c'est que les 2 sont connectés (aucun jack branché)...
    CV1_connect = _min(CV1_connect,0xFFFFFFF0); // pour eviter les overflow 

    if ((REG_CV2 > 0b101000000000) == (tmp > 0)) CV2_connect++; else CV2_connect = 0; 
    CV2_connect = _min(CV2_connect,0xFFFFFFF0); // pour eviter les overflow 

    if ((REG_CV3 > 0b101000000000) == (tmp > 0)) CV3_connect++; else CV3_connect = 0; 
    CV3_connect = _min(CV3_connect,0xFFFFFFF0); // pour eviter les overflow 

    if ((REG_CV4 > 0b101000000000) == (tmp > 0)) CV4_connect++; else CV4_connect = 0; 
    CV4_connect = _min(CV4_connect,0xFFFFFFF0); // pour eviter les overflow 

    if ((audio_inL > 0x98000000) == (tmp > 0)) IN1_connect++; else IN1_connect = 0; 
    IN1_connect = _min(IN1_connect,0xFFFFFFF0); // pour eviter les overflow 

    if ((audio_inR > 0x98000000) == (tmp > 0)) IN2_connect++; else IN2_connect = 0; 
    IN2_connect = _min(IN2_connect,0xFFFFFFF0); // pour eviter les overflow 

    tmp = random32() & 1; // 0 ou 1 : 1 seul bit en sortie pour tester la connection
    default1(tmp); // mise en sortie de ce bit
    default2(tmp);
    default3(tmp);
    default4(tmp);
    default5(tmp);
    default6(tmp);
    connect_test_value = tmp;
  }
}

inline void test_connect_loop_84(){ 
  uint32_t tmp;
  if (connect_timer > 0x30) { // on met un certain temps entre la sortie et le test pour compenser la lattence des entrées / sortie de l'audio
    connect_timer = 0;
    tmp = connect_test_value;

    if ((audio_inL > 0x98000000) == (tmp > 0)) IN1_connect++; else IN1_connect = 0; 
    IN1_connect = _min(IN1_connect,0xFFFFFFF0); // pour eviter les overflow 

    if ((audio_inR > 0x98000000) == (tmp > 0)) IN2_connect++; else IN2_connect = 0; 
    IN2_connect = _min(IN2_connect,0xFFFFFFF0); // pour eviter les overflow 

    tmp = random32() & 1; // 0 ou 1 : 1 seul bit en sortie pour tester la connection
    default5(tmp);
    default6(tmp);
    connect_test_value = tmp;
  }
}


// ------------------------------------------------------------------------------------------------------------
// calibration

inline void init_calibration() {
  // CV1
  CV1_0V = rom_read_int32(rom_address_CV1_0V); // 16 bits
  CV1_1V = rom_read_int32(rom_address_CV1_1V);
  if (CV1_0V == 0xFFFFFFFF) CV1_0V = 1<<15; // valeur moyenne qd il n'y a pas de calibration valable
  if (CV1_1V == 0xFFFFFFFF) CV1_1V = 6192; // CV(0V) - CV(1V) = 1/10.6 du full range
  CV1_1Vminus0V = CV1_1V;
  // la table de CV est lue sur 28 bit, alors qu'elle fait 10 bit, dc interpolation de 18 bit.
  // elle contien une valeur tout les 1/4 note midi, dc il faut 48 valeur par octaves
  // une octave se trouve donc tout les 48 * (1<<18)
  // un CV doit donc etre multiplié par 48<<18 et divisé par (la valeur du CV pour 1V - la valeur du CV a 0V) pour avoir un tracking 1V/Oct.
  CV1_1V = (48<<18) / CV1_1V; 

  // CV2
  CV2_0V = rom_read_int32(rom_address_CV2_0V); // 16 bits
  CV2_1V = rom_read_int32(rom_address_CV2_1V);
  if (CV2_0V == 0xFFFFFFFF) CV2_0V = 1<<15; // valeur moyenne qd il n'y a pas de calibration valable
  if (CV2_1V == 0xFFFFFFFF) CV2_1V = 6192;
  CV2_1Vminus0V = CV2_1V;
  CV2_1V = (48<<18) / CV2_1V; 

  // CV3
  CV3_0V = rom_read_int32(rom_address_CV3_0V); // 16 bits
  CV3_1V = rom_read_int32(rom_address_CV3_1V);
  if (CV3_0V == 0xFFFFFFFF) CV3_0V = 1<<15; // valeur moyenne qd il n'y a pas de calibration valable
  if (CV3_1V == 0xFFFFFFFF) CV3_1V = 6192;
  CV3_1Vminus0V = CV3_1V;
  CV3_1V = (48<<18) / CV3_1V; 

  // CV4
  CV4_0V = rom_read_int32(rom_address_CV4_0V); // 16 bits
  CV4_1V = rom_read_int32(rom_address_CV4_1V);
  if (CV4_0V == 0xFFFFFFFF) CV4_0V = 1<<15; // valeur moyenne qd il n'y a pas de calibration valable
  if (CV4_1V == 0xFFFFFFFF) CV4_1V = 6192;
  CV4_1Vminus0V = CV4_1V;
  CV4_1V = (48<<18) / CV4_1V; 

  // IN1
  IN1_0V = rom_read_int32(rom_address_IN1_0V); // 32 bit
  IN1_1V_value = rom_read_int32(rom_address_IN1_1V);
  if (IN1_0V == 0xFFFFFFFF) IN1_0V = 1<<31; // valeur moyenne qd il n'y a pas de calibration valable
  if (IN1_1V_value == 0xFFFFFFFF) IN1_1V_value = (0xFFFFFF00/14); // idem
  IN1_1V = (48<<18) / (IN1_1V_value>>16); //avec un CV sur 16 bit seulement :  pitch = ((audio_inL-(1<<31)>>16)*IN1_1V)

  // IN2
  IN2_0V = rom_read_int32(rom_address_IN2_0V); // 32 bit
  IN2_1V_value = rom_read_int32(rom_address_IN2_1V);
  if (IN2_0V == 0xFFFFFFFF) IN2_0V = 1<<31; // valeur moyenne qd il n'y a pas de calibration valable
  if (IN2_1V_value == 0xFFFFFFFF) IN2_1V_value = (0xFFFFFF00/14); // idem
  IN2_1V = (48<<18) / (IN2_1V_value>>16); //avec un CV sur 16 bit seulement : ( pitch = (audio_inR-(1<<31)>>16)*IN2_1V)

  OUT1_0V = rom_read_int32(rom_address_OUT1_0V); // 32 bit
  OUT1_1V = rom_read_int32(rom_address_OUT1_1V); // 32 bit
  if (OUT1_0V == 0xFFFFFFFF) OUT1_0V = 1<<31; // valeur moyenne qd il n'y a pas de calibration valable
  if (OUT1_1V == 0xFFFFFFFF) OUT1_1V = (1<<31) + (0xFFFFFF00/14); // idem
  OUT1_1V -= OUT1_0V;

  OUT2_0V = rom_read_int32(rom_address_OUT2_0V); // 32 bit
  OUT2_1V = rom_read_int32(rom_address_OUT2_1V); // 32 bit
  if (OUT2_0V == 0xFFFFFFFF) OUT2_0V = 1<<31; // valeur moyenne qd il n'y a pas de calibration valable
  if (OUT2_1V == 0xFFFFFFFF) OUT2_1V = (1<<31) + (0xFFFFFF00/14); // idem
  OUT2_1V -= OUT2_0V;
}

// ------------------------------------------------------------------------------------------------------------
// chaos for default modulation
volatile int32_t chaos_X, chaos_Y, chaos_Z;
volatile int32_t chaos_dx, chaos_dy, chaos_dz;

inline void init_chaos() {
  chaos_X = random32();
  chaos_Y = random32();
  chaos_Z = random32();
}

inline void chaos(uint32_t dt) { // Thomas chaotic attractor
  // smooth movement is obtained using the derivative of the output, with b= 0
  int32_t chaos_X_local,chaos_Y_local, chaos_Z_local;
  chaos_X_local = chaos_X;
  chaos_Y_local = chaos_Y;
  chaos_Z_local = chaos_Z;
  
  chaos_dx = fast_sin(chaos_Y_local)^0x80000000;
  chaos_dy = fast_sin(chaos_Z_local)^0x80000000;
  chaos_dz = fast_sin(chaos_X_local)^0x80000000;
  
  chaos_X = chaos_X_local + (chaos_dx>>dt);
  chaos_Y = chaos_Y_local + (chaos_dy>>dt);
  chaos_Z = chaos_Z_local + (chaos_dz>>dt);
}

inline void chaos_div(uint32_t diviseur) { // Thomas chaotic attractor
  // smooth movement is obtained using the derivative of the output, with b= 0
  int32_t chaos_X_local,chaos_Y_local, chaos_Z_local;
  chaos_X_local = chaos_X;
  chaos_Y_local = chaos_Y;
  chaos_Z_local = chaos_Z;
  
  chaos_dx = fast_sin(chaos_Y_local)^0x80000000;
  chaos_dy = fast_sin(chaos_Z_local)^0x80000000;
  chaos_dz = fast_sin(chaos_X_local)^0x80000000;
  
  chaos_X = chaos_X_local + (chaos_dx/diviseur);
  chaos_Y = chaos_Y_local + (chaos_dy/diviseur);
  chaos_Z = chaos_Z_local + (chaos_dz/diviseur);
}

/*
inline void chaos() {
  int32_t dx, dy, dz;
  int32_t chaos_X_local,chaos_Y_local, chaos_Z_local;
  // chaos_n : 1 sur 1<<14 environ
  
  chaos_X_local = chaos_X;
  chaos_Y_local = chaos_Y;
  chaos_Z_local = chaos_Z;
  
  dx = fast_sin(chaos_Y_local<<19)^0x80000000;
  dx >>= 20;
  dx -= chaos_X_local>>4;
  dx >>= 7;
  
  dy = fast_sin(chaos_Z_local<<19)^0x80000000;
  dy >>= 20;
  dy -= chaos_Y_local>>4;
  dy >>= 7;
  
  dz = fast_sin(chaos_X_local<<19)^0x80000000;
  dz >>= 20;
  dz -= chaos_Z_local>>4;
  dz >>= 7;

  chaos_X = chaos_X_local + dx;
  chaos_Y = chaos_Y_local + dy;
  chaos_Z = chaos_Z_local + dz;
}
*/


