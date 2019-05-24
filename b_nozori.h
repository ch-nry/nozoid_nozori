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

// variuous definition and modul list

union uint64 {
  uint64_t U64;
  uint32_t U32[2];
};

union uint32 {
  uint32_t U32;
  byte U8[4] ;
};

#define hysteresis_size 10

// sound caractere.  
#define DAC_clear 1 // 1 for "clear", 0 for "fat"

#define pot1 13 // A11   // PB20
#define pot2 12 // A10   // PB19
#define pot3 14 // PIN52 (AD14) // PB21
#define pot4 1 // A6    // PA3
#define pot5 0 // A7    // PA2
#define pot6 2 // A5    // PA4
#define CV1 4  // A3    // PA22
#define CV2 3  // A4    // PA6
#define CV3 5  // A2    // PA23
#define CV4 6  // A1    // PA24

#define REG_pot5   (*(RoReg*)0x400C0050U) // 0
#define REG_pot4   (*(RoReg*)0x400C0054U) // 1
#define REG_pot6   (*(RoReg*)0x400C0058U) // 2
#define REG_CV2    (*(RoReg*)0x400C005CU) // 3
#define REG_CV1    (*(RoReg*)0x400C0060U) // 4
#define REG_CV3    (*(RoReg*)0x400C0064U) // 5
#define REG_CV4    (*(RoReg*)0x400C0068U) // 6
//#define REG_     (*(RoReg*)0x400C006CU) // 7
//#define REG_     (*(RoReg*)0x400C0070U) // 8
//#define REG_     (*(RoReg*)0x400C0074U) // 9
//#define REG_     (*(RoReg*)0x400C0078U) // 10
//#define REG_     (*(RoReg*)0x400C007CU) // 11
#define REG_pot2   (*(RoReg*)0x400C0080U) // 12
#define REG_pot1   (*(RoReg*)0x400C0084U) // 13
#define REG_pot3   (*(RoReg*)0x400C0088U) // 14
//#define REG_     (*(RoReg*)0x400C008CU) // 15 // temperature sensor
#define REG_pot7    REG_CV2
#define REG_pot8    REG_CV4

const uint32_t table_adc[] = { 13, 12, 14, 1, 0, 2, 4, 3, 5, 6 };

#define index_filter_pot1 0
#define index_filter_pot2 1
#define index_filter_pot3 2
#define index_filter_pot4 3
#define index_filter_pot5 4
#define index_filter_pot6 5
#define index_filter_pot7 6
#define index_filter_pot8 7
#define index_filter_cv1  8
#define index_filter_cv2  9
#define index_filter_cv3  10
#define index_filter_cv4  11
#define index_filter_CV1  8
#define index_filter_CV2  9
#define index_filter_CV3  10
#define index_filter_CV4  11

#define FLASH_offset IFLASH1_SIZE - 256


#define rom_address_CV1_0V 0
#define rom_address_CV2_0V 1
#define rom_address_CV3_0V 2
#define rom_address_CV4_0V 3
#define rom_address_IN1_0V 4
#define rom_address_IN2_0V 5
#define rom_address_CV1_1V 6
#define rom_address_CV2_1V 7
#define rom_address_CV3_1V 8
#define rom_address_CV4_1V 9
#define rom_address_IN1_1V 10
#define rom_address_IN2_1V 11
#define rom_address_OUT1_0V 12
#define rom_address_OUT2_0V 12
#define rom_address_OUT1_1V 13
#define rom_address_OUT2_1V 14

// macro to facilitate the "case" in init / data loop and audio loop
#define module(nb, type, nom)                               \
  case nb :                                                 \
    pointeurSurFonction_ ## type =  nom ## _ ## type ## _;  \
  break;

#ifdef NOZORI_68
  #define switch_module_type(type)        \
  module( 0,   type, tune68)              \
  module( 1,   type, rien)                \
  module( 2,   type, rien)                \
  module( 3,   type, rien)                \
  module( 4,   type, rien)                \
  module( 5,   type, rien)                \
  module( 6,   type, rien)                \
  module( 7,   type, rien)                \
  module( 8,   type, VCO)                 \
  module( 9,   type, VCO_Q)               \
  module( 10,  type, VCO_WS)              \
  module( 11,  type, KS)                  \
  module( 12,  type, VCO_Param)           \
  module( 13,  type, VCO_SIN_MOD)         \
  module( 14,  type, VCO_LOOP)            \
  module( 15,  type, noise)               \
  module( 16,  type, rien)                \
  module( 17,  type, rien)                \
  module( 18,  type, rien)                \
  module( 19,  type, rien)                \
  module( 20,  type, rien)                \
  module( 21,  type, rien)                \
  module( 22,  type, rien)                \
  module( 23,  type, rien)                \
  module( 24,  type, VCF_Param)           \
  module( 25,  type, VCF)                 \ 
  module( 26,  type, rien)                \
  module( 27,  type, rien)                \
  module( 28,  type, rien)                \
  module( 29,  type, rien)                \
  module( 30,  type, rien)                \
  module( 31,  type, rien)                \
  module( 32,  type, rien)                \
  module( 33,  type, rien)                \
  module( 34,  type, rien)                \
  module( 35,  type, rien)                \
  module( 36,  type, rien)                \
  module( 37,  type, rien)                \
  module( 38,  type, rien)                \
  module( 39,  type, rien)                \
  module( 40,  type, Pan)                 \
  module( 41,  type, rien)                \
  module( 42,  type, rien)                \
  module( 43,  type, rien)                \
  module( 44,  type, rien)                \
  module( 45,  type, rien)                \
  module( 46,  type, rien)                \
  module( 47,  type, rien)                \
  module( 48,  type, rien)                \
  module( 49,  type, rien)                \
  module( 50,  type, rien)                \
  module( 51,  type, rien)                \
  module( 52,  type, rien)                \
  module( 53,  type, rien)                \
  module( 54,  type, rien)                \
  module( 55,  type, rien)                \
  module( 56,  type, ADSR_VCA)            \ 
  module( 57,  type, AR)                  \
  module( 58,  type, rien)                \
  module( 59,  type, rien)                \
  module( 60,  type, rien)                \
  module( 61,  type, rien)                \
  module( 62,  type, rien)                \
  module( 63,  type, rien)                \
  module( 64,  type, rien)                \
  module( 65,  type, rien)                \
  module( 66,  type, rien)                \
  module( 67,  type, rien)                \
  module( 68,  type, rien)                \
  module( 69,  type, rien)                \
  module( 70,  type, rien)                \
  module( 71,  type, rien)                \
  module( 72,  type, rien)                \
  module( 73,  type, LFO_Dual)            \
  module( 74,  type, CV_REC)              \
  module( 75,  type, RND_LOOP)            \
  module( 76,  type, LFO_MOD)             \
  module( 77,  type, rien)                \
  module( 78,  type, rien)                \
  module( 79,  type, rien)                \
  module( 80,  type, rien)                \
  module( 81,  type, rien)                \
  module( 82,  type, rien)                \
  module( 83,  type, rien)                \
  module( 84,  type, rien)                \
  module( 85,  type, rien)                \
  module( 86,  type, rien)                \
  module( 87,  type, rien)                \
  module( 88,  type, BIT_Crush)           \
  module( 89,  type, Delay)               \
  module( 90,  type, Gran)                \
  module( 91,  type, PShift)              \
  module( 92,  type, WS)                  \
  module( 93,  type, EQ)                  \
  module( 94,  type, rien)                \
  module( 95,  type, rien)                \
  module( 96,  type, rien)                \
  module( 97,  type, rien)                \
  module( 98,  type, rien)                \
  module( 99,  type, rien)                \
  module( 100, type, rien)                \
  module( 101, type, rien)                \
  module( 102, type, rien)                \
  module( 103, type, rien)                \
  module( 104, type, rien)                \
  module( 105, type, rien)                \
  module( 106, type, rien)                \
  module( 107, type, rien)                \
  module( 108, type, rien)                \
  module( 109, type, rien)                \
  module( 110, type, rien)                \
  module( 111, type, rien)                \
  module( 112, type, rien)                \
  module( 113, type, rien)                \
  module( 114, type, rien)                \
  module( 115, type, rien)                \
  module( 116, type, rien)                \
  module( 117, type, rien)                \
  module( 118, type, rien)                \
  module( 119, type, rien)                \
  module( 120, type, rien)                \
  module( 121, type, rien)                \
  module( 122, type, rien)                \
  module( 123, type, rien)                \
  module( 124, type, rien)                \
  module( 125, type, rien)                \
  module( 126, type, rien)                \
  module( 127, type, rien)                \
  module( 128, type, rien)                \
  module( 129, type, rien)                \
  module( 130, type, rien)                \
  module( 131, type, rien)                \
  module( 132, type, rien)                \
  module( 133, type, rien)                \
  module( 134, type, rien)                \
  module( 135, type, rien)                \
  module( 136, type, rien)                \
  module( 137, type, rien)                \
  module( 138, type, rien)                \
  module( 139, type, rien)                \
  module( 140, type, rien)                \
  module( 141, type, rien)                \
  module( 142, type, rien)                \
  module( 143, type, rien)                \
  module( 144, type, rien)                \
  module( 145, type, rien)                \
  module( 146, type, rien)                \
  module( 147, type, rien)                \
  module( 148, type, rien)                \
  module( 149, type, rien)                \
  module( 150, type, rien)                \
  module( 151, type, rien)                \
  module( 152, type, rien)                \
  module( 153, type, rien)                \
  module( 154, type, rien)                \
  module( 155, type, rien)                \
  module( 156, type, rien)                \
  module( 157, type, rien)                \
  module( 158, type, rien)                \
  module( 159, type, rien)                \
  module( 160, type, rien)                \
  module( 161, type, rien)                \
  module( 162, type, rien)                \
  module( 163, type, rien)                \
  module( 164, type, rien)                \
  module( 165, type, rien)                \
  module( 166, type, rien)                \
  module( 167, type, rien)                \
  module( 168, type, rien)                \
  module( 169, type, rien)                \
  module( 170, type, rien)                \
  module( 171, type, rien)                \
  module( 172, type, rien)                \
  module( 173, type, rien)                \
  module( 174, type, rien)                \
  module( 175, type, rien)                \
  module( 176, type, rien)                \
  module( 177, type, rien)                \
  module( 178, type, rien)                \
  module( 179, type, rien)                \
  module( 180, type, rien)                \
  module( 181, type, rien)                \
  module( 182, type, rien)                \
  module( 183, type, rien)                \
  module( 184, type, rien)                \
  module( 185, type, rien)                \
  module( 186, type, rien)                \
  module( 187, type, rien)                \
  module( 188, type, rien)                \
  module( 189, type, rien)                \
  module( 190, type, rien)                \
  module( 191, type, rien)                \
  module( 192, type, rien)                \
  module( 193, type, rien)                \
  module( 194, type, rien)                \
  module( 195, type, rien)                \
  module( 196, type, rien)                \
  module( 197, type, rien)                \
  module( 198, type, rien)                \
  module( 199, type, rien)                \
  module( 200, type, rien)                \
  module( 201, type, rien)                \
  module( 202, type, rien)                \
  module( 203, type, rien)                \
  module( 204, type, rien)                \
  module( 205, type, rien)                \
  module( 206, type, rien)                \
  module( 207, type, rien)                \
  module( 208, type, rien)                \
  module( 209, type, rien)                \
  module( 210, type, rien)                \
  module( 211, type, rien)                \
  module( 212, type, rien)                \
  module( 213, type, rien)                \
  module( 214, type, rien)                \
  module( 215, type, rien)                \
  module( 216, type, rien)                \
  module( 217, type, rien)                \
  module( 218, type, rien)                \
  module( 219, type, rien)                \
  module( 220, type, rien)                \
  module( 221, type, rien)                \
  module( 222, type, rien)                \
  module( 223, type, rien)                \
  module( 224, type, rien)                \
  module( 225, type, rien)                \
  module( 226, type, rien)                \
  module( 227, type, rien)                \
  module( 228, type, rien)                \
  module( 229, type, rien)                \
  module( 230, type, rien)                \
  module( 231, type, rien)                \
  module( 232, type, rien)                \
  module( 233, type, rien)                \
  module( 234, type, rien)                \
  module( 235, type, rien)                \
  module( 236, type, rien)                \
  module( 237, type, rien)                \
  module( 238, type, rien)                \
  module( 239, type, rien)                \
  module( 240, type, rien)                \
  module( 241, type, rien)                \
  module( 242, type, rien)                \
  module( 243, type, rien)                \
  module( 244, type, rien)                \
  module( 245, type, rien)                \
  module( 246, type, rien)                \
  module( 247, type, rien)                \
  module( 248, type, rien)                \
  module( 249, type, rien)                \
  module( 250, type, rien)                \
  module( 251, type, rien)                \
  module( 252, type, rien)                \
  module( 253, type, rien)                \
  module( 254, type, rien)                \
  module( 255, type, rien)                \
  module( 256, type, calibrate_pd)        \
  module( 257, type, calibrate_serial)    \
  module( 258, type, calibrate_manual_1)  \
  module( 259, type, calibrate_manual_2)  \
  module( 260, type, test_1)              \
  module( 261, type, test_2)              \
  module( 262, type, template)            
#endif

#ifdef NOZORI_84
  #define switch_module_type(type)        \
  module( 0,   type, tune84)              \
  module( 1,   type, rien)                \
  module( 2,   type, rien)                \
  module( 3,   type, rien)                \
  module( 4,   type, rien)                \
  module( 5,   type, rien)                \
  module( 6,   type, rien)                \
  module( 7,   type, rien)                \
  module( 8,   type, rien)                \
  module( 9,   type, rien)                \
  module( 10,  type, rien)                \
  module( 11,  type, rien)                \
  module( 12,  type, rien)                \
  module( 13,  type, rien)                \
  module( 14,  type, rien)                \
  module( 15,  type, rien)                \
  module( 16,  type, rien)                \
  module( 17,  type, rien)                \
  module( 18,  type, rien)                \
  module( 19,  type, rien)                \
  module( 20,  type, rien)                \
  module( 21,  type, rien)                \
  module( 22,  type, rien)                \
  module( 23,  type, rien)                \
  module( 24,  type, rien)                \
  module( 25,  type, rien)                \
  module( 26,  type, rien)                \
  module( 27,  type, rien)                \
  module( 28,  type, rien)                \
  module( 29,  type, rien)                \
  module( 30,  type, rien)                \
  module( 31,  type, rien)                \
  module( 32,  type, rien)                \
  module( 33,  type, rien)                \
  module( 34,  type, rien)                \
  module( 35,  type, rien)                \
  module( 36,  type, rien)                \
  module( 37,  type, rien)                \
  module( 38,  type, rien)                \
  module( 39,  type, rien)                \
  module( 40,  type, rien)                \
  module( 41,  type, rien)                \
  module( 42,  type, rien)                \
  module( 43,  type, rien)                \
  module( 44,  type, rien)                \
  module( 45,  type, rien)                \
  module( 46,  type, rien)                \
  module( 47,  type, rien)                \
  module( 48,  type, rien)                \
  module( 49,  type, rien)                \
  module( 50,  type, rien)                \
  module( 51,  type, rien)                \
  module( 52,  type, rien)                \
  module( 53,  type, rien)                \
  module( 54,  type, rien)                \
  module( 55,  type, rien)                \
  module( 56,  type, rien)                \
  module( 57,  type, rien)                \
  module( 58,  type, rien)                \
  module( 59,  type, rien)                \
  module( 60,  type, rien)                \
  module( 61,  type, rien)                \
  module( 62,  type, rien)                \
  module( 63,  type, rien)                \
  module( 64,  type, rien)                \
  module( 65,  type, rien)                \
  module( 66,  type, rien)                \
  module( 67,  type, rien)                \
  module( 68,  type, rien)                \
  module( 69,  type, rien)                \
  module( 70,  type, rien)                \
  module( 71,  type, rien)                \
  module( 72,  type, rien)                \
  module( 73,  type, rien)                \
  module( 74,  type, rien)                \
  module( 75,  type, rien)                \
  module( 76,  type, rien)                \
  module( 77,  type, rien)                \
  module( 78,  type, rien)                \
  module( 79,  type, rien)                \
  module( 80,  type, rien)                \
  module( 81,  type, rien)                \
  module( 82,  type, rien)                \
  module( 83,  type, rien)                \
  module( 84,  type, rien)                \
  module( 85,  type, rien)                \
  module( 86,  type, rien)                \
  module( 87,  type, rien)                \
  module( 88,  type, rien)                \
  module( 89,  type, rien)                \
  module( 90,  type, rien)                \
  module( 91,  type, rien)                \
  module( 92,  type, rien)                \
  module( 93,  type, rien)                \
  module( 94,  type, rien)                \
  module( 95,  type, rien)                \
  module( 96,  type, rien)                \
  module( 97,  type, rien)                \
  module( 98,  type, rien)                \
  module( 99,  type, rien)                \
  module( 100, type, rien)                \
  module( 101, type, rien)                \
  module( 102, type, rien)                \
  module( 103, type, rien)                \
  module( 104, type, rien)                \
  module( 105, type, rien)                \
  module( 106, type, rien)                \
  module( 107, type, rien)                \
  module( 108, type, rien)                \
  module( 109, type, rien)                \
  module( 110, type, rien)                \
  module( 111, type, rien)                \
  module( 112, type, rien)                \
  module( 113, type, rien)                \
  module( 114, type, rien)                \
  module( 115, type, rien)                \
  module( 116, type, rien)                \
  module( 117, type, rien)                \
  module( 118, type, rien)                \
  module( 119, type, rien)                \
  module( 120, type, rien)                \
  module( 121, type, rien)                \
  module( 122, type, rien)                \
  module( 123, type, rien)                \
  module( 124, type, rien)                \
  module( 125, type, rien)                \
  module( 126, type, rien)                \
  module( 127, type, rien)                \
  module( 128, type, tune84)              \
  module( 129, type, rien)                \
  module( 130, type, rien)                \
  module( 131, type, rien)                \
  module( 132, type, rien)                \
  module( 133, type, rien)                \
  module( 134, type, rien)                \
  module( 135, type, rien)                \
  module( 136, type, VCO_Add)             \
  module( 137, type, VCO_Harmo)           \
  module( 138, type, sin_AM)              \
  module( 139, type, sin_FM)              \
  module( 140, type, sin_FM_lin)          \
  module( 141, type, sin_PM)              \
  module( 142, type, VCO_Tropical)        \
  module( 143, type, VCO_8bit)            \
  module( 144, type, OSC_Param)           \
  module( 145, type, rien)                \
  module( 146, type, DEJONG_AUDIO)        \
  module( 147, type, SIN_WS)              \
  module( 148, type, FM)                  \
  module( 149, type, rien)                \
  module( 150, type, rien)                \
  module( 151, type, rien)                \
  module( 152, type, rien)                \
  module( 153, type, rien)                \
  module( 154, type, rien)                \
  module( 155, type, rien)                \
  module( 156, type, rien)                \
  module( 157, type, rien)                \
  module( 158, type, rien)                \
  module( 159, type, rien)                \
  module( 160, type, VCF_Param84)         \
  module( 161, type, Stereo_WS)           \
  module( 162, type, modulation)          \
  module( 163, type, Doppler)             \
  module( 164, type, rien)                \
  module( 165, type, rien)                \
  module( 166, type, rien)                \
  module( 167, type, rien)                \
  module( 168, type, rien)                \
  module( 169, type, rien)                \
  module( 170, type, rien)                \
  module( 171, type, rien)                \
  module( 172, type, rien)                \
  module( 173, type, rien)                \
  module( 174, type, rien)                \
  module( 175, type, rien)                \
  module( 176, type, rien)                \
  module( 177, type, rien)                \
  module( 178, type, rien)                \
  module( 179, type, rien)                \
  module( 180, type, rien)                \
  module( 181, type, rien)                \
  module( 182, type, rien)                \
  module( 183, type, rien)                \
  module( 184, type, ADSR_Loop)           \ 
  module( 185, type, ADSR_Dual)           \
  module( 186, type, rien)                \
  module( 187, type, rien)                \
  module( 188, type, rien)                \
  module( 189, type, rien)                \
  module( 190, type, rien)                \
  module( 191, type, rien)                \
  module( 192, type, rien)                \
  module( 193, type, rien)                \
  module( 194, type, rien)                \
  module( 195, type, rien)                \
  module( 196, type, rien)                \
  module( 197, type, rien)                \
  module( 198, type, rien)                \
  module( 199, type, rien)                \
  module( 200, type, LFO_Mod)             \
  module( 201, type, LFO_SEQ)             \
  module( 202, type, CLK_ADSR)            \
  module( 203, type, rien)                \
  module( 204, type, THOMAS)              \
  module( 205, type, SEQ8)                \
  module( 206, type, SEQ4)                \
  module( 207, type, rien)                \
  module( 208, type, rien)                \
  module( 209, type, rien)                \
  module( 210, type, DEJONG_LFO)          \
  module( 211, type, rien)                \
  module( 212, type, rien)                \
  module( 213, type, rien)                \
  module( 214, type, rien)                \
  module( 215, type, rien)                \
  module( 216, type, rien)                \
  module( 217, type, rien)                \
  module( 218, type, rien)                \
  module( 219, type, rien)                \
  module( 220, type, rien)                \
  module( 221, type, rien)                \
  module( 222, type, rien)                \
  module( 223, type, rien)                \
  module( 224, type, rien)                \
  module( 225, type, rien)                \
  module( 226, type, rien)                \
  module( 227, type, rien)                \
  module( 228, type, rien)                \
  module( 229, type, rien)                \
  module( 230, type, rien)                \
  module( 231, type, rien)                \
  module( 232, type, rien)                \
  module( 233, type, rien)                \
  module( 234, type, rien)                \
  module( 235, type, rien)                \
  module( 236, type, rien)                \
  module( 237, type, rien)                \
  module( 238, type, rien)                \
  module( 239, type, rien)                \
  module( 240, type, rien)                \
  module( 241, type, rien)                \
  module( 242, type, rien)                \
  module( 243, type, rien)                \
  module( 244, type, rien)                \
  module( 245, type, rien)                \
  module( 246, type, rien)                \
  module( 247, type, rien)                \
  module( 248, type, rien)                \
  module( 249, type, rien)                \
  module( 250, type, rien)                \
  module( 251, type, rien)                \
  module( 252, type, rien)                \
  module( 253, type, rien)                \
  module( 254, type, rien)                \
  module( 255, type, rien)                \
  module( 256, type, calibrate_pd)        \
  module( 257, type, calibrate_serial)    \
  module( 258, type, calibrate_manual_1)  \
  module( 259, type, calibrate_manual_2)  \
  module( 260, type, test_1)              \
  module( 261, type, test_2)              \
  module( 262, type, template)            
#endif

