#include <cstdlib>
#include <cstdio>
#include <unistd.h> 
#include <getopt.h>
#include <time.h>
#include <map>
#include <string>
#include <vector>
//#include <iostream>
#include <stdio.h>
//#include <boost/thread.hpp>
//#include <boost/bind.hpp>
#include <readline/readline.h>
#include <readline/history.h>
#include "uHTRCtrlMezzInterface.h"

using namespace std ;

#define I2C_ADDR_BASE_MUX    0x70  // I2C [PCA9548A]
#define I2C_ADDR_GPIO        0x20  // GPIO [PCA9534] 
#define I2C_MAC_EEPROM       0x50  // 24AA02E48/24AA025E48 series 
#define I2C_ADC              0x6F  // MPC3426 12 bit ADC

/*
string tool_readline() ;
*/
//        MAC       ,           < MezzType       SN >
std::map<std::string, std::pair<unsigned int, unsigned int> > snMap;

uHTRCtrlMezzInterface::uHTRCtrlMezzInterface() {

   doLog = false ;
   spi_clk = 250 ;
}

string uHTRCtrlMezzInterface::tool_readline( string prompt ) {

    string retval;

    char* input = readline( prompt.c_str() );
    if ( input && *input ) add_history( input ) ;

    retval   = string( input );
    free( input );

    for ( size_t i = retval.size()-1 ; i >=0; i-- ) {
        if ( isspace( retval[i] ) )  retval.erase( i, 1 ) ;
        else break ;
    }
    //if (retval.empty()) retval=defval;
    return retval;
}

int uHTRCtrlMezzInterface::tool_readline_int( std::string prompt) {
  return strtol(tool_readline(prompt).c_str(),0,0);
}


// set operation frequence of i2c communication of sub20 (489Hz ~ 444444 Hz)
void uHTRCtrlMezzInterface::set_sub20_frequence( sub_handle& hd_, int freq ) {

   if ( hd_ == 0 ) {
      printf(" Sub20 is not initialized, Please initialize sub20 first !!! " ) ;
      return ;
   }

   int f_ =   sub_i2c_freq( hd_ , &freq ) ;
   if ( f_ == 0 ) printf(" Sub20 frequency is set to %d Hz ! \n", freq ) ;
   else           printf(" freq set fail ! \n") ;

}


// In order to find the i2c slave , which is the top level mux(PCA9548A) 
void uHTRCtrlMezzInterface::scan_i2c_slaves( sub_handle& hd_ ) {

   if ( hd_ == 0 ) {
      printf(" Sub20 is not initialized, Please initialize sub20 first !!! " ) ;
      return ;
   }

   // target slave - i2c MUX
   int nSlave = 0 ;
   char sAddr[128];
   int s_ = sub_i2c_scan( hd_, &nSlave, sAddr ) ;
   printf(" ==> i2c(%d) scan : %d  \n", s_ , nSlave  ) ;

   for ( int j =0 ; j < nSlave ; j++ )
       printf("    --> I2C Addr: %x \n", sAddr[j] ) ;
}

void uHTRCtrlMezzInterface::Test( sub_handle& hd_ ) {

   if ( hd_ == 0 ) {
      printf(" Sub20 is not initialized, Please initialize sub20 first !!! " ) ;
      return ;
   }

   printf(" ================================================ \n" );
   printf(" | 0: MMC, 1: JTAG, 3:GPIO-1, 4:FLASH, 5:GPIO-2 | \n" );
   printf(" ================================================ \n" );
  
   int i2c_ch = tool_readline_int(" Which i2c channel ? (0,1,3,4,5) : " ) ;

   // switch to different channel
   char wbuff[8] ;
   char rbuff[8] ;

   wbuff[0] = (char)(1 << i2c_ch);
   printf(" =========================== \n");
   printf(" Set to channel %d \n", i2c_ch ) ;
   
   int w_ = sub_i2c_write( hd_, I2C_ADDR_BASE_MUX, 0, 0, wbuff, 1);
   if ( w_ != 0 )  printf(" switch to channel %d failed !! \n", i2c_ch ) ;
   scan_i2c_slaves( hd_ ) ;

   //read_ADC( hd_, i2c_ch ) ;

   
   i2c_read_MAC( hd_, rbuff, i2c_ch ) ; 
   printf("   -->  [%x][%x][%x][%x][%x][%x] \n", 
           rbuff[0]&0xFF,rbuff[1]&0xFF,rbuff[2]&0xFF,rbuff[3]&0xFF,rbuff[4]&0xFF,rbuff[5]&0xFF ) ;
   //tag_MezzId( hd_, 4 ) ;
   
   vector<char> rV= i2c_read_MAC_EEPROM( hd_, 0x0, 128 ) ; 
   //printf(" sz  of read : %d \n", rV.size() ) ;

   // switch to channel 0 - MMC 
   /*
   wbuff[0] = (char)(1 << 0);
   printf(" =========================== \n");
   printf(" Set to channel 0 - MMC \n" ) ;
   w_ = sub_i2c_write( hd_, I2C_ADDR_BASE_MUX, 0, 0, wbuff, 1);
   if ( w_ != 0 )  printf(" switch to MMC failed !! \n") ;

   scan_i2c_slaves( hd_ ) ;
   */

}

void uHTRCtrlMezzInterface::Test2( sub_handle& hd_ ){

  if ( hd_ == 0 ) {
     printf(" Sub20 is not initialized, Please initialize sub20 first !!! " ) ;
     return ;
  }

  char macA[6] ;
  bool validMAC = spi_read_MAC( hd_, macA ) ;
  if ( !validMAC ) { 
     printf(" Invalid MAC Address \n") ;
  }

  char hmac[20];
  sprintf(hmac, "%02x:%02x:%02x:%02x:%02x:%02x", 0xff&macA[0], 0xff&macA[1], 0xff&macA[2], 0xff&macA[3], 0xff&macA[4], 0xff&macA[5]);

}

// MPC3426 ADC i2c address - 0x6f 
// Connect with a current amplifier (with gain 20) with resistor 0.02 ohm
// Conversion formula : V_out = (Readout) * (1mV) / (0.02*20) = 2.5*(Readout) mA
// Only MMC (ch0) and JTAG (ch1) have ADC connected
void uHTRCtrlMezzInterface::read_ADC( sub_handle& hd_, int i2c_ch ) {

   char wbuff[8], rbuff[8] ;

   wbuff[0] = (char)(1 << i2c_ch);
   
   int w_ = sub_i2c_write( hd_, I2C_ADDR_BASE_MUX, 0, 0, wbuff, 1);
   if ( w_ != 0 )  printf(" switch to channel %d failed !! \n", i2c_ch ) ;

   // 0x90 (10010000) : Initial new conversation in continuous 12-bit mode for channel 1
   wbuff[0] =  0x90 ;
   w_ = sub_i2c_write( hd_, I2C_ADC, 0, 0, wbuff, 1);
   if ( w_ != 0 )  printf(" configure error \n");
  
   for ( int k=0; k < 10 ; k++ ) {
   
     int r_ = sub_i2c_read( hd_, I2C_ADC, 0, 0, rbuff, 3);
     if ( r_ != 0 ) { 
        printf(" read error \n") ;
     }  else {
        if ( ((rbuff[2] >> 7) & 0x1) == 0 ) { 
           uint8_t retval = uint8_t((0xf & rbuff[0]) << 8) | (uint8_t)rbuff[1];
           double Is = (double)retval*2.5*1 ;
           //printf(" read adc : [%x][%x][%x] = %u -> %.2f mA \n",  rbuff[0], rbuff[1], rbuff[2],retval, Is  )  ;
           printf(" read current  : %.2f mA \n", Is  )  ;
           if ( doLog && k%3 == 1 ) {
              char theI[8] ;
              sprintf( theI, "%03.3f", Is ) ;
              theLog.result.push_back( string(theI) ) ;
           }
        } else {
           printf(" No vailid ADC yet !\n") ;
           if ( doLog && k%3 == 1 ) theLog.result.push_back( "N/A" ) ;
        }
     }
     sleep(1) ;  
   }

}

// Only MMC (ch0) and JTAG (ch1) have ADC connected
void uHTRCtrlMezzInterface::reset_ADC( sub_handle& hd_, int i2c_ch ) {

   char wbuff[8] ;

   wbuff[0] = (char)(1 << i2c_ch);
   
   int w_ = sub_i2c_write( hd_, I2C_ADDR_BASE_MUX, 0, 0, wbuff, 1);
   if ( w_ != 0 )  printf(" switch to channel %d failed !! \n", i2c_ch ) ;

   // 0x90 (10010000) : Initial new conversation in continuous 12-bit mode for channel 1
   wbuff[0] =  0x00 ;
   wbuff[1] =  0x06 ;
   w_ = sub_i2c_write( hd_, I2C_ADC, 0, 0, wbuff, 2);
   if ( w_ != 0 )  printf(" ADC reset error \n");
   else            printf(" ADC Reset ! \n") ;
}  

// Read the MAC address from the EEPROM (24AA02E48/24AA025E48 series )
bool uHTRCtrlMezzInterface::i2c_read_MAC( sub_handle& hd_, char* macA, int i2c_ch ) {

   char wbuff[8] ;
   char rbuff[8] ;

   wbuff[0] = (char)(1 << i2c_ch);
   int w_ = sub_i2c_write( hd_, I2C_ADDR_BASE_MUX, 0, 0, wbuff, 1);
   if ( w_ != 0 )  printf(" switch to i2c channel %d failed !! \n", i2c_ch ) ;

   wbuff[0] = 0xFA;
   w_ = sub_i2c_write( hd_, I2C_MAC_EEPROM, 0, 0, wbuff, 1);
   if ( w_ != 0 )  printf(" Can not reach MAC eeprom \n");
   //int w_ = sub_i2c_read( hd_, maddr, 0xfa, 1, rbuff, 6);
   w_ = sub_i2c_read( hd_, I2C_MAC_EEPROM, 0, 0, rbuff, 6);
   if ( w_ != 0 )  printf(" Can not read MAC eeprom \n");


   bool valid = true ;
   int ntry = 0 ;
   do {
      int nff = 0; 
      for (int i=0; i< 6; i++) {
           macA[i] = rbuff[i] ;
           if ( int(macA[i]&0xff) == 0xff ) nff++ ;
      }
      printf(" MAC:  [%x][%x][%x][%x][%x][%x] \n", 
            rbuff[0]&0xFF,rbuff[1]&0xFF,rbuff[2]&0xFF,rbuff[3]&0xFF,rbuff[4]&0xFF,rbuff[5]&0xFF ) ;
      if ( nff > 2 ) valid = false ; 
      if ( ntry > 3 ) break ;
   } while ( !valid ) ;
   return valid ;
}

// Only work for JTAG(1), FLASH(4) 
void uHTRCtrlMezzInterface::i2c_read_MezzId( sub_handle& hd_ , int i2c_ch ) {

   if ( hd_ == 0 ) {
      printf(" Sub20 is not initialized, Please initialize sub20 first !!! " ) ;
      return ;
   }


   // switch to different channel
   char wbuff[8] ;
   char rbuff[8] ;

   wbuff[0] = (char)(1 << i2c_ch);
   printf(" =========================== \n");
   printf(" Set to channel %d \n", i2c_ch ) ;
   
   int w_ = sub_i2c_write( hd_, I2C_ADDR_BASE_MUX, 0, 0, wbuff, 1);
   if ( w_ != 0 )  printf(" switch to channel %d failed !! \n", i2c_ch ) ;

   // read MAC address 
   i2c_read_MAC( hd_, rbuff, i2c_ch ) ; 
   //printf("   -->  [%x][%x][%x][%x][%x][%x] \n", 
   //        rbuff[0]&0xFF,rbuff[1]&0xFF,rbuff[2]&0xFF,rbuff[3]&0xFF,rbuff[4]&0xFF,rbuff[5]&0xFF ) ;
   
   // read the MezzId
   vector<char> rV= i2c_read_MAC_EEPROM( hd_, 0x0, 128 ) ; 

   //byte length :   1,1,1,16, 2,11,16,16, 8, 56
   int bt_len[]  = { 1,1,1,16, 2,11,16,16, 8, 56 } ;  // length of the byte stream
   int pos_id[]  = { 0,1,2, 3,19,21,32,48,64, 72 } ;  // starting position of the byte stream
   //int id_type[] = { 0,0,0, 1, 0, 1, 1, 1, 1,  1 } ;  // 0: integer 1: character

   int format_version = (int)rV[0] ; 
   int type_code      = (int)rV[1] ;
   int subtype_code   = (int)rV[2] ;
   int sn             =  (int)( (rV[20] << 8) | rV[19] ) ;
   printf(" formate :%d , type: %d, subtype: %d, SN: %d \n", format_version, type_code, subtype_code, sn ) ;

   char typeStr[16] ;
   for ( int i=3; i <19; i++ )  typeStr[ i-3 ] = rV[i]  ;
   printf( " mezz type: %s \n", typeStr ) ;

   char dateStr[11] ;         
   for ( int i=21; i<32; i++ )  dateStr[i-21] = rV[i]  ;
   printf( " date : %s \n", dateStr ) ;

   char siteStr[16] ;         
   for ( int i=32; i<48; i++ )  siteStr[i-32] = rV[i]  ;
   printf( " Site -> %s \n", siteStr ) ;

   char nameStr[16] ;         
   for ( int i=48; i<64; i++ )  nameStr[i-48] = rV[i]  ;
   printf( " Name -> %s \n", nameStr ) ;

   char relStr[8] ;         
   for ( int i=64; i<72; i++ )  relStr[i-64] = rV[i]  ;
   printf( " Release : %s \n", relStr ) ;

   char noteStr[56] ;         
   for ( int i=72; i<128; i++ )  noteStr[i-72] = rV[i]  ;
   printf( " Note : %s \n", noteStr ) ;

 
}

// Read the content from the MAC EEPROM (24AA02E48/24AA025E48 series )
vector<char> uHTRCtrlMezzInterface::i2c_read_MAC_EEPROM( sub_handle& hd_, uint16_t maddr, int nRead ) {

   char wbuff[8] ;
   char rbuff[256] ;
   wbuff[0] = maddr ;
   vector<char> output ;
   int w_ = sub_i2c_write( hd_, I2C_MAC_EEPROM, 0, 0, wbuff, 1);
   w_ = sub_i2c_read( hd_, I2C_MAC_EEPROM, 0, 0, rbuff, nRead);
   //usleep( 1000*(nRead+3) ) ;
   if ( w_ != 0 ) {
      printf(" Can not read MAC eeprom \n");
   } else { 
      for ( int i=0; i< nRead; i++ ) {
          output.push_back( rbuff[i] ) ;
          //if ( i < 8 ) printf(" [%2x]",  rbuff[i]&0xFF ) ;
          //else
          //printf(" [%c]",  rbuff[i] ) ;
          //if ( i%8 == 7 ) printf("\n") ;
      }
   }
   //printf("\n") ;
   return output ;    
}

// Write the content from the MAC EEPROM (24AA02E48/24AA025E48 series )
// Only Flash (ch4) and JTAG (ch1) mezzanine are accessible
void uHTRCtrlMezzInterface::i2c_write_MAC_EEPROM( sub_handle& hd_, MezzIdStruct& id, int i2c_ch ) {

  char wbuff[9] ;
  char* data_buff = (char*)(&id) ; 

  wbuff[0] = (char)(1 << i2c_ch ) ;  
  int w_ = sub_i2c_write( hd_, I2C_ADDR_BASE_MUX, 0, 0, wbuff, 1);

  w_ = 0 ;
  for ( unsigned int dpos = 6; dpos < sizeof(MezzIdStruct); dpos += 0x8 ) {
      wbuff[0] = 0x0 + dpos -6 ;  // Data must be written in 8 byte pages 
      for ( unsigned int k=0 ; k <8 ; k++ ) {
          wbuff[k+1] = data_buff[ k+dpos ] ;
      }
      //printf(" (%d) %c %c %c %c %c %c %c %c \n", 
      //      wbuff[0], wbuff[1], wbuff[2], wbuff[3], wbuff[4], wbuff[5], wbuff[6], wbuff[7], wbuff[8] ) ;
      w_ = sub_i2c_write( hd_, I2C_MAC_EEPROM, 0, 0, wbuff, 9);
      if ( w_ != 0 ) {
         printf(" Can not reach MAC eeprom .. try again \n");
	 w_ = sub_i2c_write( hd_, I2C_MAC_EEPROM, 0, 0, wbuff, 9);
	 if ( w_ != 0 ) printf(" .... still fail \n") ;
      } 
      usleep(40000) ;
      //printf(" writing page%d \n", (dpos-6)/8);
  }

}

void uHTRCtrlMezzInterface::spi_test( sub_handle& hd_ ) {

   if ( hd_ == 0 ) {
      printf(" Sub20 is not initialized, Please initialize sub20 first !!! " ) ;
      return ;
   }

   /* Read current SPI configuration */
   printf(" ========================== \n") ;
   int spi_cfg = 0 ;

   /* Configure SPI */
   printf(" SPI_CFG = %d \n", SPI_ENABLE|SPI_CPOL_RISE|SPI_SMPL_SETUP|SPI_MSB_FIRST|SPI_CLK_500KHZ ) ;
   int rc = sub_spi_config( hd_, SPI_ENABLE|SPI_CPOL_RISE|SPI_SMPL_SETUP|SPI_MSB_FIRST|SPI_CLK_500KHZ, 0 ) ;
   sub_spi_config( hd_, 0, &spi_cfg );
   //int rc = sub_spi_config( hd_,  64, &spi_cfg ) ;
   if ( rc ==0 ) printf(" SPI configured (%d) \n", spi_cfg ) ;

   // Select spi targets - Always SS0 (because using DB9 connector )  
   char wbuff[128] ;
   char rbuff[256] ;

   // Read the Flash ID
   checkFlashID( hd_ ) ;
   /*
   wbuff[0] = 0x9F ;
   rc = sub_spi_transfer( hd_, wbuff, 0, 1 , SS_CONF(0,SS_L) ) ;
   if ( rc !=0 ) printf(" SPI Write fail \n" ) ;

   rc = sub_spi_transfer( hd_, 0, rbuff, 3 , SS_CONF(0,SS_LO) ) ;
   if ( rc !=0 ) printf(" SPI READ fail - error code :%d \n", rc ) ;
   if ( rc ==0 ) { 
          printf(" Flash ID :        [%x] [%x] [%x] \n", rbuff[0], rbuff[1], rbuff[2]) ;
   }*/

   // Read status register
   wbuff[0] = 0x5 ;
   rc = sub_spi_transfer( hd_, wbuff, 0, 1 , SS_CONF(0,SS_LO) ) ;
   if ( rc !=0 ) printf(" SPI Write fail \n" ) ;
   rc = sub_spi_transfer( hd_, 0, rbuff, 3 , SS_CONF(0,SS_LO) ) ;
   if ( rc !=0 ) printf(" SPI READ fail - error code :%d \n", rc ) ;
   if ( rc ==0 ) { 
          printf(" Status Register : [%x] [%x] [%x] \n\n", rbuff[0], rbuff[1], rbuff[2]) ;
   }

   // Read Flash Content
   int addr = 1*(0x40000) ;
   wbuff[0] = 0x3 ;
   wbuff[1] = (addr>>16)&0xFF ;
   wbuff[2] = (addr>>8)&0xFF ;
   wbuff[3] = addr&0xFF ;
   rc = sub_spi_transfer( hd_, wbuff, 0, 4 , SS_CONF(0,SS_LO) ) ;
   if ( rc !=0 ) printf(" SPI Write fail \n" ) ;

   rc = sub_spi_transfer( hd_, 0, rbuff, 256 , SS_CONF(0,SS_LO) ) ;
   if ( rc !=0 ) printf(" SPI READ fail - error code :%d \n", rc ) ;
   if ( rc ==0 ) { 
      for ( int i=0 ; i < 16; i++ )
          printf(" SPI read : [%x] \n", rbuff[i] ) ;
   }
}

// Two GPIO [PCA9534] - Device address: 0x20 , MUX channel 3 and 5
void uHTRCtrlMezzInterface::write_GPIO( sub_handle& hd_, uint8_t val, bool isFront ) {

   char wbuff[8] ;

   // Switch to channle 5 - Set up revision for firmware
   if ( !isFront ) wbuff[0] = (char)(1 << 5); // for back
   else            wbuff[0] = (char)(1 << 3); // for front 
   printf(" =========================== \n");
   if ( !isFront ) printf("  Set to mux ch5 GPIO \n" ) ;
   else            printf("  Set to mux ch3 GPIO \n" ) ;

   int w_ = sub_i2c_write( hd_, I2C_ADDR_BASE_MUX, 0, 0, wbuff, 1);
   if ( w_ != 0 )  printf(" switch to SPI-MUX failed !! \n");

   // Disable polarity-inverse for all pins
   wbuff[0] = 0x2 ; 
   wbuff[1] = 0x0 ; 
   w_ = sub_i2c_write( hd_, I2C_ADDR_GPIO, 0, 0, wbuff, 2);

   // Set pins to output ( for writing )
   wbuff[0] = 0x3 ; // set I/O as write 
   wbuff[1] = 0x0 ; // set I/O as write 
   w_ = sub_i2c_write( hd_, I2C_ADDR_GPIO, 0, 0, wbuff, 2);

   // write value to the port
   printf("  Writing %X to the I/O \n", (val&0xFF) ) ;
   wbuff[0] = 0x1 ;  // command byte to write
   wbuff[1] = val ; //
   w_ = sub_i2c_write( hd_, I2C_ADDR_GPIO, 0, 0, wbuff, 2);
   if ( w_ != 0 )  printf(" configur error \n") ;

}

void uHTRCtrlMezzInterface::status_GPIO( sub_handle& hd_, bool isFront ) {

   printf(" =========================== \n");
   printf("   GPIO STATUS \n" ) ;
   // Switch to the desire GPIO
   char wbuff[8] ;
   char rbuff[8] ;
   if ( !isFront ) wbuff[0] = (char)(1 << 5); // for back
   else            wbuff[0] = (char)(1 << 3); // for front 
   printf(" =========================== \n");
   if ( !isFront ) printf("  Set to mux ch5 GPIO \n" ) ;
   else            printf("  Set to mux ch3 GPIO \n" ) ;

   int w_ = sub_i2c_write( hd_, I2C_ADDR_BASE_MUX, 0, 0, wbuff, 1);

   // check status register
   // Read Register (0x0) 
   int r_ = sub_i2c_read( hd_, I2C_ADDR_GPIO, 0x0, 1, rbuff, 1);
   if ( r_ != 0 ) printf("  read error \n");
   else           printf("  read status register value: [%2X] \n", rbuff[0]&0xFF )  ;

   // Write Register (0x1) 
   w_ = sub_i2c_read( hd_, I2C_ADDR_GPIO, 0x1, 1, rbuff, 1);
   if ( w_ != 0 ) printf("  read error \n") ;
   else           printf("  write status register value: [%2X] \n", rbuff[0]&0xFF )  ;

}

// CPLD at mux channel 3 and 5 for two i2c GPIO
// SPI target select - 8 bits , only TargetSelect[0:3] match to GPIO_MUX3 I/O[0:3]
// 0: BCK_CS0 , 1: BCK_CS1, 2: BCK_CS, 3: FNT_CS0 , 4: FNT_CS1, 5: FNT_CS, 
// 8: CS_EEPROM , 9: CS_MAC, 10: CS_CPLD 
void uHTRCtrlMezzInterface::spi_switch( sub_handle& hd_, int channel, int revision ) {

   if ( hd_ == 0 ) {
      printf(" Sub20 is not initialized, Please initialize sub20 first !!! " ) ;
      return ;
   }

   // switch to different channel
   //char wbuff[8] ;
   //char rbuff[8] ;

   //printf(" select revision : %d \n", revision ) ;
   // 1st step, set up revision 
   uint8_t val = channel ;
   if ( channel == 2 ) {
      val = ( revision == 0 ) ? (1<<6) : (1<<5) ;
      write_GPIO( hd_, val, false ) ;     // setup revision 
      write_GPIO( hd_, channel, true ) ;  // setup channel
      val |= channel ;
   } else if ( channel == 5 ) {
      val = ( revision == 0 ) ? (1<<7) : (1<<6) ;
      val |= channel ;
      write_GPIO( hd_, val, true ) ;
   }
   else {
      write_GPIO( hd_, val, true ) ;
   }

   /*
   printf("\n ==================================================== \n") ;
   printf(" ==== SPI Targets 0: BCK_CS0,   1: BCK_CS1, 2: BCK_CS \n") ;
   printf(" ====             3: FNT_CS0,   4: FNT_CS1, 5: FNT_CS \n") ;
   printf(" ====             8: CS_EEPROM, 9: CS_MAC, 10: CS_CPLD \n") ;
   printf(" ==== SPI Selection : %X ====\n", val ) ;
   wbuff[0] = 0x1 ;  // command byte to write
   wbuff[1] = targetId ; // FNT_CS
   w_ = sub_i2c_write( hd_, I2C_ADDR_GPIO, 0, 0, wbuff, 2);
   if ( w_ != 0 )  printf(" configur error \n") ;
   */
   spi_config( hd_ ) ; 
}


void uHTRCtrlMezzInterface::spi_config( sub_handle& hd_ ) {

   if ( hd_ == 0 ) {
      printf(" Sub20 is not initialized, Please initialize sub20 first !!! " ) ;
      return ;
   }

   /* Read current SPI configuration */
   printf(" ========================== \n") ;
   int spi_cfg = 0 ;
   int rc = 0 ;
   /* Configure SPI */
   if ( spi_clk == 500 ) {
      printf(" SPI_CFG = %d \n", SPI_ENABLE|SPI_CPOL_RISE|SPI_SMPL_SETUP|SPI_MSB_FIRST|SPI_CLK_500KHZ ) ;
      rc = sub_spi_config( hd_, SPI_ENABLE|SPI_CPOL_RISE|SPI_SMPL_SETUP|SPI_MSB_FIRST|SPI_CLK_500KHZ, 0 ) ;
   } 
   else if ( spi_clk == 1000 ) {
      printf(" SPI_CFG = %d \n", SPI_ENABLE|SPI_CPOL_RISE|SPI_SMPL_SETUP|SPI_MSB_FIRST|SPI_CLK_1MHZ ) ;
      rc = sub_spi_config( hd_, SPI_ENABLE|SPI_CPOL_RISE|SPI_SMPL_SETUP|SPI_MSB_FIRST|SPI_CLK_1MHZ, 0 ) ;
   } else {
      printf(" SPI_CFG = %d \n", SPI_ENABLE|SPI_CPOL_RISE|SPI_SMPL_SETUP|SPI_MSB_FIRST|SPI_CLK_250KHZ ) ;
      rc = sub_spi_config( hd_, SPI_ENABLE|SPI_CPOL_RISE|SPI_SMPL_SETUP|SPI_MSB_FIRST|SPI_CLK_250KHZ, 0 ) ;
   }
   sub_spi_config( hd_, 0, &spi_cfg );
   if ( rc ==0 ) printf(" SPI configured (%d) \n", spi_cfg ) ;

}

// Read the register of EEPROM (RDSR)
void uHTRCtrlMezzInterface::check_EEPROM( sub_handle& hd_ ){

   // Select spi targets - Always SS0 (because using DB9 connector )  
   char wbuff[8] ;
   char rbuff[8] ;

   wbuff[0] = 0x05 ; 
   int rc = sub_spi_transfer( hd_, wbuff, 0, 1 , SS_CONF(0,SS_L) )  ;
   if ( rc !=0 ) printf(" SPI Write fail \n" ) ;

   rc = sub_spi_transfer( hd_, 0, rbuff, 1 , SS_CONF(0,SS_LO) ) ;
   if ( rc !=0 ) printf(" SPI READ fail - error code :%d \n", rc ) ;
   if ( rc ==0 ) { 
          printf(" EEPROM Register:        [%X] \n", rbuff[0] ) ;
   }
}

void uHTRCtrlMezzInterface::spi_read_MezzId( sub_handle& hd_ ) {

   // Read MAC address 
   char macA[6] ;
   spi_read_MAC(hd_, macA ) ;
   // spi channel 9 is the MAC EEPROM for clock mezzanine
   int spi_ch = 9 ;
   spi_switch( hd_, spi_ch ) ;
   check_EEPROM( hd_ ) ;
   vector<char> rV = spi_read_EEPROM( hd_, spi_ch ) ;

   //  byte length :   1,1,1,16, 2,11,16,16, 8, 56
   //int pos_id[]  = { 0,1,2, 3,19,21,32,48,64, 72 } ;  // starting position of the byte stream
   //int id_type[] = { 0,0,0, 1, 0, 1, 1, 1, 1,  1 } ;  // 0: integer 1: character
   
   int format_version = (int)rV[0] ;
   int type_code      = (int)rV[1] ;
   int subtype_code   = (int)rV[2] ;
   int sn             =  (int)( (rV[20] << 8) | rV[19] ) ;
   printf(" formate :%d , type: %d, subtype: %d, SN: %d \n", format_version, type_code, subtype_code, sn ) ;
   
   char typeStr[16] ;
   for ( int i=3; i <19; i++ )  typeStr[ i-3 ] = rV[i]  ;
   printf( " mezz type: %s \n", typeStr ) ;
   
   char dateStr[11] ;
   for ( int i=21; i<32; i++ )  dateStr[i-21] = rV[i]  ;
   printf( " date : %s \n", dateStr ) ;
   
   char siteStr[16] ;
   for ( int i=32; i<48; i++ )  siteStr[i-32] = rV[i]  ;
   printf( " Site -> %s \n", siteStr ) ;
   
   char nameStr[16] ;
   for ( int i=48; i<64; i++ )  nameStr[i-48] = rV[i]  ;
   printf( " Name -> %s \n", nameStr ) ;
   
   char relStr[8] ;
   for ( int i=64; i<72; i++ )  relStr[i-64] = rV[i]  ;
   printf( " Release : %s \n", relStr ) ;

   char noteStr[56] ;
   for ( int i=72; i<128; i++ )  noteStr[i-72] = rV[i]  ;
   printf( " Note : %s \n", noteStr ) ;


}

// 128K bit EEPROM ,256 pages,   
// Each page contains 64 bytes , Each Read gives 1 byte
vector<char> uHTRCtrlMezzInterface::spi_read_EEPROM( sub_handle& hd_, int spi_ch, bool toFile ) {

     vector<char> output ;
     vector<char> instruct ;
     int readSize   = 64 ;
 
     if ( spi_ch == 8 ) {
        for ( int j = 0; j < 0x3FFF; j+=readSize ) {
            instruct.push_back( 0x3 );
   	    instruct.push_back( (j >>8)&0xFF );
	    instruct.push_back( (j >>0)&0xFF );
	    spi_read( hd_ , instruct, output, readSize ) ;
            instruct.clear() ;
        }

     } else if ( spi_ch == 9 ) {
        for ( int j = 0; j < 0xFF; j+=readSize ) {
            instruct.push_back( 0x3 );
	    instruct.push_back( j & 0xFF ) ;
            spi_read( hd_ , instruct, output, readSize ) ;
            instruct.clear() ;
        }
     } else {
        printf(" No SPI target selected ! \n" ) ;
        return output;
     }

     if ( toFile ) {
        string imageFile = tool_readline(" EEPROM Readout Filename : " ) ;
	FILE* pfile = fopen( imageFile.c_str(), "w" ) ; 

	for ( int i=0; i< (int)output.size(); i++ ) {
            if ( spi_ch == 8 ) {
               unsigned char byte1 = ( output[i] >> 4 ) & 0xF ;
	       unsigned char byte2 = output[i] & 0xF ;

	       fprintf(pfile, "%X%X ", byte1, byte2 );
	       //fprintf(pfile, "%2X", output[i] );
	       if ( i%4 == 3 ) fprintf(pfile, "\n") ;
            }
	    if ( spi_ch == 9 ) {
	        if ( i > output.size()-7 ) fprintf(pfile, " %2x", output[i]&0xFF );
		else                       fprintf(pfile, " %c", output[i] );
		if ( i%8 == 7 ) fprintf(pfile, "\n") ;
	    }
        }
        fclose( pfile ) ;
     }

     return output ;
}

bool uHTRCtrlMezzInterface::spi_read_MAC( sub_handle& hd_, char* macA ) {

     // switch to the SPI MAC EEPROM
     spi_switch( hd_ ,  9 ) ;
     usleep(1000) ;
     vector<char> output ;

     vector<char> instruct(2) ;
     instruct[0] = 0x3 ;
     instruct[1] = 0xFA ;

     bool valid = true ;

     int ntry = 0; 
     do {
        output.clear() ;
        spi_read( hd_ , instruct, output, 6 ) ;
        usleep(1000) ;

        printf(" MAC:  [%x][%x][%x][%x][%x][%x] \n", 
            output[0]&0xFF,output[1]&0xFF,output[2]&0xFF,output[3]&0xFF,output[4]&0xFF,output[5]&0xFF ) ;
        int nff = 0 ;
        for ( int i=0; i<6 ; i++) {
            macA[i] = output[i] ;
            if ( int(macA[i]&0xff) == 0xff ) nff++ ;
        }
        if ( nff > 2 ) valid = false ; 
        if ( !valid ) printf( " Bad MAC address reading !!! " ) ;
        ntry++ ; 
        if ( ntry > 3 ) break ;
     } while ( !valid ) ;

     return valid ;    

}

const uint8_t clock_image_240[] = { 45,
      0,0x14,1,0xE4,2,0xA2,3,0x15,4,0x12,5,0xED,6,0x3F,7,0x2A,8,0x00,
      9,0xC0,10,0x00,11,0x42,16,0x00,17,0x80,18,0x00,19,0x2C,20,0x3E,
      21,0x7E,22,0xDF,23,0x1F,24,0x3F,25,0xE0,31,0x00,32,0x00,33,0x01,
      34,0x00,35,0x00,36,0x01,40,0x60,41,0x01,42,0x8B,43,0x00,44,0x00,
      45,0x14,46,0x00,47,0x00,48,0x14,55,0x00,131,0x1F,132,0x02,
      138,0x0F,139,0xFF,142,0x00,143,0x00,136,0x40
};

const uint8_t clock_image_320[] = { 45,
      0,0x14,1,0xE4,2,0xA2,3,0x15,4,0x12,5,0xED,6,0x3F,7,0x2A,8,0x00,
      9,0xC0,10,0x00,11,0x42,16,0x00,17,0x80,18,0x00,19,0x2C,20,0x3E,
      21,0xFE,22,0xDF,23,0x1F,24,0x3F,25,0x00,31,0x00,32,0x00,33,0x03,
      34,0x00,35,0x00,36,0x03,40,0x80,41,0x01,42,0x4F,43,0x00,44,0x00,
      45,0x14,46,0x00,47,0x00,48,0x14,55,0x00,131,0x1F,132,0x02,
      138,0x0F,139,0xFF,142,0x00,143,0x00,136,0x40
};

const uint8_t clock_image_fr[] = { 45,
      0,0x54,1,0xE4,2,0x24,3,0x15,4,0x92,5,0xED,6,0x3F,7,0x2A,8,0x00,
      9,0xC0,10,0x00,11,0x40,16,0x00,17,0x80,18,0x00,19,0x2C,20,0x3E,
      21,0x7E,22,0xDF,23,0x1F,24,0x3F,25,0xA0,31,0x00,32,0x00,33,0x0D,
      34,0x00,35,0x00,36,0x0D,40,0xA0,41,0x85,42,0x37,43,0x00,44,0x09,
      45,0x83,46,0x00,47,0x1B,48,0x2F,55,0x00,131,0x1F,132,0x02,
      138,0x0F,139,0xFF,142,0x00,143,0x00,136,0x40
};

// Write all 0 or 1 to the EEPROM
void uHTRCtrlMezzInterface::erase_CLK_EEPROM( sub_handle& hd_, bool allZero ) {

     char input[128] ;
     int writeSize = 64 ;
 
     printf("\n Erasing the EEPROM \n") ;
     // Write data - maximum 64 bytes
     for ( int j = 0; j < 0x3FFF; j+=writeSize ) {
         
         // Write enable latch
         input[0] = 0x6 ;
	 int rc = sub_spi_transfer( hd_, input, 0, 1 , SS_CONF(0,SS_LO) ) ;
	 if ( rc !=0 ) printf(" CPLD EEPROM WREN ERROR (%d) \n", rc ) ;
         // Check status
         //check_EEPROM( hd_ ) ;

         // Write page
         input[0] = 0x2 ;
	 input[1] = (j >> 8)&0xFF ;
	 input[2] = (j >> 0)&0xFF ;
         //printf(" Erase page %d (%X%X)  \n", j/writeSize + 1, input[1] , input[2]  ) ;
         for ( size_t k = j ; (k-j) < 64 ; k++ ) {
             if ( allZero ) input[3 + k-j] = 0x0 ;
             else           input[3 + k-j] = 0xFF ;
         }
         rc = sub_spi_transfer( hd_, input, 0, writeSize+3 , SS_CONF(0,SS_LO) ) ;
         if ( rc !=0 ) printf(" CPLD EEPROM Page Write Fail (%d) \n", rc ) ;

         //check_EEPROM( hd_ ) ;
         // (period of clk = 4 micro-sec) * (8bits/byte) * (size of write data + address + command + 100) 
         int tt = (1000/spi_clk)*8*(writeSize+2+1+100) ; 
         usleep( tt ) ;
     }

}

void uHTRCtrlMezzInterface::write_CLK_EEPROM( sub_handle& hd_, int imageChoice1 , int imageChoice2 ) {

     static const uint8_t SILABS_CMD_SETADDRESS=(0x00);
     static const uint8_t SILABS_CMD_WRITE=(0x40);

     char input[128] ;
     int writeSize = 64 ;

     // Choose images
     const uint8_t* theImage1 = 0 ;
     const uint8_t* theImage2 = 0 ;
     vector<unsigned char> cmdlist1 ;
     vector<unsigned char> cmdlist2 ;
     if ( imageChoice1 == 240 ) theImage1 = clock_image_240 ;
     if ( imageChoice1 == 320 ) theImage1 = clock_image_320 ;
     if ( imageChoice2 == 240 ) theImage2 = clock_image_240 ;
     if ( imageChoice2 == 320 ) theImage2 = clock_image_320 ;
 
     // Construct the command list 
     for ( uint8_t i=0; i< theImage1[0]; i++ ) {
         cmdlist1.push_back( SILABS_CMD_SETADDRESS ) ;
         cmdlist1.push_back( theImage1[2*i+1] ) ;
         cmdlist1.push_back( SILABS_CMD_WRITE );
         cmdlist1.push_back( theImage1[2*i+2] );

         cmdlist2.push_back( SILABS_CMD_SETADDRESS ) ;
         cmdlist2.push_back( theImage2[2*i+1] ) ;
         cmdlist2.push_back( SILABS_CMD_WRITE );
         cmdlist2.push_back( theImage2[2*i+2] );
      
     }
     for ( uint8_t i=0; i< 12; i++ ) {
         cmdlist1.push_back( 0 );
         cmdlist2.push_back( 0 );
     }
     /*
     for ( size_t i=0; i< cmdlist1.size() ; i+=4 ) {
         printf("%2X %2X %2X %2X    %2X %2X %2X %2X \n", 
                cmdlist1[i], cmdlist1[i+1], cmdlist1[i+2], cmdlist1[i+3],  
                cmdlist2[i], cmdlist2[i+1], cmdlist2[i+2], cmdlist2[i+3] );
     }
     printf(" img1: %d , img2: %d \n", (int)cmdlist1.size() , (int)cmdlist2.size() ) ;
     */

     // Write data - maximum 64 bytes
     printf(" Programming Clock Mezzanine %d %d  \n", imageChoice1 , imageChoice2   ) ;
     bool complete = true ;
     for ( int j = 0; j < 0x3FFF; j+=writeSize ) {
         
         // Write enable latch
         input[0] = 0x6 ;
	 int rc = sub_spi_transfer( hd_, input, 0, 1 , SS_CONF(0,SS_LO) ) ;
	 if ( rc !=0 ) printf(" CPLD EEPROM WREN ERROR (%d) \n", rc ) ;
         // Check status
         //check_EEPROM( hd_ ) ;

         // Write page
         input[0] = 0x2 ;
	 input[1] = (j >> 8)&0xFF ;
	 input[2] = (j >> 0)&0xFF ;
         //printf(" Programming page %d %X%X  \n", j/writeSize + 1, input[1] , input[2]  ) ;
         bool emptyPage = false ;
         for ( size_t k = j ; (k-j) < 64 ; k++ ) {
             // write image1 to the first half of the EEPROM
             if ( k < 8191 && k-0 < cmdlist1.size() )  {
                input[3 + k-j] = cmdlist1[k] ;
                //printf( "%02X ", cmdlist1[k] );
                //if ( (k-j)%4 == 3 )  printf( "\n" );
                emptyPage = false ;
             }
             // write image2 to the second half of the EEPROM
             else if ( k > 8191 && k-8192 < cmdlist2.size() )  {
                input[3 + k-j] = cmdlist2[k-8192] ;
                //printf( "%02X ", cmdlist2[k-8192] ) ;
                //if ( (k-j)%4 == 3 )  printf( "\n" );
                emptyPage = false ;
             } else {
                //printf( "%02X ", 0xFF );
                //if ( (k-j)%4 == 3 )  printf( "\n" );
                emptyPage = true ;
             }
         }
          
         if ( !emptyPage ) 
            rc = sub_spi_transfer( hd_, input, 0, writeSize+3 , SS_CONF(0,SS_LO) ) ;
         if ( rc !=0 ) {
            printf(" CPLD EEPROM Page Write Fail (%d) \n", rc ) ;
            complete = false ;
         }
         //check_EEPROM( hd_ ) ;
         // (period of clk = 4 micro-sec) * (8bits/byte) * (size of write data + address + command + 10) 
         int tt = (1000/spi_clk)*8*(writeSize+2+1+100) ; 
         usleep( tt ) ;
     }

     if ( doLog ) {
        char images[8];
	sprintf( images, "%d-%d", imageChoice1 , imageChoice2 );
	theLog.result.push_back( string(images) );
        if ( complete ) theLog.result.push_back(" pass ") ;
        else            theLog.result.push_back(" fail ") ;
     }
}

// Write Mezzanine ID to spi EEPROM (25AA02E48)
void uHTRCtrlMezzInterface::spi_write_MAC_EEPROM( sub_handle& hd_, MezzIdStruct& id ) {

     //printf(" sz of id : %d \n", sizeof(id) ) ;

     char input[32] ;
     char* data_buff = (char*)(&id) ; 
     int writeSize = 16 ;

     int rc = 0 ;
     for ( size_t j = 6; j < sizeof(MezzIdStruct) ; j+=writeSize ) {
         
         // Write enable latch
         input[0] = 0x6 ;
	 rc = sub_spi_transfer( hd_, input, 0, 1 , SS_CONF(0,SS_LO) ) ;
	 if ( rc !=0 ) printf(" MAC EEPROM WREN ERROR (%d) \n", rc ) ;
         // Check status
         //check_EEPROM( hd_ ) ;

         // Write page
         input[0] = 0x2 ;              // write command 
	 input[1] = (j-6) & 0xFF ;     // write address
         //printf(" Programming page %d , %X %X  \n", (j-6)/writeSize , input[1] , input[2]  ) ;
         for ( size_t k = 0 ; k < writeSize  ; k++ ) {
             input[2 + k] = data_buff[k+j] ;
             //printf("([%x]: %c , %c) ", k+j, data_buff[k+j], input[2 + k+j] )  ;
         }
         //printf("\n") ;
         //printf("\n [%x](%x) %c %c %c %c %c %c %c %c \n", j+16, 
         //    input[1], input[2], input[4], input[6], input[8], input[10], input[12], input[14], input[16] ) ;
          
         rc = sub_spi_transfer( hd_, input, 0, writeSize+2 , SS_CONF(0,SS_LO) ) ;
         if ( rc !=0 ) printf(" MAC EEPROM Page Write Fail (%d) \n", rc ) ;

         // (period of clk = 4 micro-sec) * (8bits/byte) * (size of write data + address + command + 100) 
         int tt = (1000/spi_clk)*8*(writeSize+1+1+100) ; 
         usleep( tt ) ;
         //check_EEPROM( hd_ ) ;
     }
}

// Read the Flash memory ID - a cross-check for spi operation using FLASH mezzanine
void uHTRCtrlMezzInterface::checkFlashID( sub_handle& hd_ ){

   // Select spi targets - Always SS0 (because using DB9 connector )  
   char wbuff[16] ;
   char rbuff[16] ;

   // Read the Flash ID , RDID = 0x9F
   wbuff[0] = 0x9F ; 
   int rc = sub_spi_transfer( hd_, wbuff, 0, 1 , SS_CONF(0,SS_L) ) ;
   if ( rc !=0 ) printf(" SPI Write fail \n" ) ;

   rc = sub_spi_transfer( hd_, 0, rbuff, 3 , SS_CONF(0,SS_LO) ) ;
   if ( rc !=0 ) printf(" SPI READ fail - error code :%d \n", rc ) ;
   if ( rc ==0 ) { 
          printf(" Flash ID :        [%x] [%x] [%x] \n", rbuff[0], rbuff[1], rbuff[2]) ;
   }
}

void uHTRCtrlMezzInterface::writeFirmware( sub_handle& hd_, string mcsFileName ){

     // It should be 20 20 18
     checkFlashID( hd_ ) ;

     // Read from MCS file - 4 bits character
     vector<unsigned char> source ;
     readMCSFile( source, mcsFileName ) ;
     // Log writing firmware 
     if ( doLog ) {
        int pos = 1 + mcsFileName.rfind("/") ;
	string mcsNameNoPath = mcsFileName.substr( pos ) ;
	theLog.result.push_back( mcsNameNoPath );
     }
     // Form the input format  - 8 bits words
     //string imageFile = tool_readline(" mcs write Filename : " ) ;
     //FILE* pfile = fopen( imageFile.c_str(), "w" ) ; 
     vector<unsigned char> input ;
     for ( size_t i=0; i< source.size() ; i+= 2 ) {
         unsigned char theByte = source[i] << 4 ;
         theByte += source[i+1] ; 
         input.push_back( theByte ) ; 

         //fprintf(pfile, "%02X", input[ input.size()-1 ] ) ;
         //if ( i%32  == 30 ) fprintf(pfile, "\n") ; 
     }
     printf(" source size: %d , input size: %d \n", (int)source.size() , (int)input.size() ) ;
     //fclose( pfile ) ;

     //int doErase = 0 ;
     //if ( source.size() < 1 ) 
     //   doErase = tool_readline_int(" Erase Firmware ? (Yes : 1 /No : 0 ) : " ) ;

     // Write into flash
     int nsectors = ( input.size() / 0x40000 ) + 1;
     int sectorSize = (0x40000) ;
     int writeSize = 256 ;  // this is page size
     printf(" start writing to flash (%d sectors) \n\n", nsectors) ;
     int k = 0 ;
     vector<unsigned char> justIn ;
     vector<unsigned char> fillIn ;
     vector<char> addr(3) ;

     bool complete = true ;
     for ( int i = 0; i < nsectors; i++) {
         spi_sector_erase( hd_ , i ) ;
         printf(" Erased and writing to sector[%d] -> starting addr[%x] ", i, i*sectorSize ) ;

         bool sectorDone = true ;
         int nTrial = 0 ;

         do {
            sectorDone = true ;
            justIn.clear() ;
            fillIn.clear() ;
            // write to each page (256 words)
            k = i*sectorSize ; 
            printf("  -> writing ..." ) ;
            for ( int j = i*sectorSize; j <(i+1)*sectorSize; j+=writeSize ) {
                //printf(" writing to sector[%d]_addr[%x] ", i, j ) ;
		addr[0] = ( j >>16 )&0xFF ;
		addr[1] = ( j >>8 )&0xFF ;
		addr[2] =   j &0xFF ;
		spi_write( hd_ , addr, input, writeSize, k ) ;

                for ( int h=0; h < 256; h++ ) {
                    unsigned char bt1 = (input[k+h] >> 4) & 0xF ;
                    unsigned char bt2 = input[k+h]  & 0xF ;
                    fillIn.push_back( bt1 ) ;
                    fillIn.push_back( bt2 ) ;
                }
                k += writeSize ;
            }
            printf("  -> reading back ..." ) ;
            readFirmwareSector( hd_ , justIn, i ) ;

            // verify the write of each sector
            printf("  -> verifying ..." ) ;
            //printf("  -> fill in %d and read out %d \n", (int)fillIn.size() , (int)justIn.size() ) ;
            int nCorr(0), nBad(0) ;

            for ( size_t m=0; m < justIn.size() ; m++ ) {
                if ( justIn[m] != fillIn[m] ) {
                   sectorDone = false ;
                   nBad++ ;
                   //printf("   .... flash writing fail at sector [%d] !! \n", i  ) ;
                   break ;
                }  
                else {
                   nCorr++ ;
                } 
            }

            if ( sectorDone ) {
               printf(" sector done  \n") ;
               break ;
            } else {
               //printf(" %d good and %d bad from total %d \n",  nCorr, nBad, (nCorr+ nBad) ) ;
               nTrial++ ;
               if ( nTrial > 1 ) printf("\n Another attempt to write to the sector ... \n" ) ;
            }
           
            //if ( nTrial > 0 ) printf(" Another attempt(%d) to write to the sector ... \n", nTrial+1 ) ;
            if ( nTrial > 2 ) break ;

         } while ( !sectorDone ) ;

         if ( !sectorDone ) {
            printf(" Firmware programming failed !!!! \n" ) ;
            complete = false ;
            break ;
         }
     }
     if ( doLog ) {
        if ( complete )  theLog.result.push_back("pass") ;
        else            theLog.result.push_back("fail") ;
     }
     printf("  ... write cycle finished \n" ) ;

}

// Read flash content using spi 
void uHTRCtrlMezzInterface::readFirmwareSector( sub_handle& hd_, vector<unsigned char>& inFlash, int iSector, int iPage ){

     // read firmware from flash
     vector<char> output ;
     int sectorSize = (0x40000) ;
     int readSize   = 256 ;
     int i = iSector ;
     int k = 0 ;
     vector<char> instruct(4) ;
     instruct[0] = 0x3 ;
     for ( int j = i*sectorSize; j <(i+1)*sectorSize; j+=readSize ) {
         if ( iPage >= 0 && k != iPage ) continue ;
         
	 instruct[1] = ( j >>16 )&0xFF ;
	 instruct[2] = ( j >>8 )&0xFF ;
	 instruct[3] =   j &0xFF ;
         spi_read( hd_ , instruct, output, readSize ) ;
         k++ ;
     }
     //printf(" size of image from flash : %d \n", (int)output.size() ) ;

     inFlash.clear() ;
     for ( size_t j=0 ; j < output.size() ; j++ ) { 
         unsigned char byte1 = ( output[j] >> 4 ) & 0xF ;
         unsigned char byte2 = output[j] & 0xF ;
         inFlash.push_back( byte1 ) ;
         inFlash.push_back( byte2 ) ;
     }
}

bool uHTRCtrlMezzInterface::verifyFirmware( sub_handle& hd_ ){

     checkFlashID( hd_ ) ;

     // read from MCS file
     vector<unsigned char> source ;
     readMCSFile( source ) ;

     string imageFile1 = tool_readline(" mcs readout Filename : " ) ;
     FILE* pfile1 = fopen( imageFile1.c_str(), "w" ) ; 
     for ( size_t i=0; i< source.size() ; i++) {
         fprintf(pfile1, "%X", source[i] ) ;
         if ( i%32  == 31 ) fprintf(pfile1, "\n") ;
     }
     fclose( pfile1 ) ;


     int nSector = source.size()/(0x80000) + 1 ;
     printf(" N of sector = %d \n\n", nSector ) ;

     // read from Flash
     string imageFile = tool_readline(" Flash Readout Filename : " ) ;
     FILE* pfile = fopen( imageFile.c_str(), "w" ) ; 

     vector<unsigned char> inFlash ;
     vector<unsigned char> eachSector ;
     for ( int i=0; i< nSector; i++ ) {
         printf(" reading sector[%d] \n", i ) ;
         readFirmwareSector( hd_ , eachSector, i ) ;
         for ( size_t j=0 ; j < eachSector.size() ; j++ ) { 
             fprintf(pfile, "%X", eachSector[j] );
             inFlash.push_back( eachSector[j] ) ;
             if ( j%32 == 31 ) fprintf(pfile, "\n") ;
          }
     }
     fclose( pfile ) ;

     // verify
     string imageFile2 = tool_readline(" MisMatch output : " ) ;
     FILE* pfile2 = fopen( imageFile2.c_str(), "w" ) ; 
     int ln = 0 ;
     bool allMatch = true ;
     bool corrline = true ;
     //for ( size_t i=0 ; i < inFlash.size(); i++ ) {
     for ( size_t i=0 ; i < source.size(); i++ ) {
         if ( i%16 == 0 ) {
            ln++ ;
            corrline = true ;
         }

         if ( inFlash[i] != source[i] ) {
            corrline = false ;
            //fprintf(pfile2, " Mismatch at Line : [%d] , pos [%u] -> ( %X : %X )\n", ln, i%16, inFlash[i], source[i] ) ;
            allMatch = false ;
         }
         if ( i%16 == 15 && !corrline ) fprintf(pfile2, " Mismatch at Line : [%d] \n", ln) ;      
     }
     fclose( pfile2 ) ;

     return allMatch ;
} 

// cast the none-data information - only keep firmware information 
void uHTRCtrlMezzInterface::readMCSFile( vector<unsigned char>& source, string mcsFileName ) {

     vector<string> lines ;
     readMCSFile( lines, mcsFileName ) ;
     source.clear() ;
     for ( size_t i=1; i< lines.size() ; i++) {
         string theLine = lines[i] ;
         if ( (uint8_t)Char2Hex( theLine[1]) != 1 ) continue ;  // exclude non-data-recording content
         for ( size_t j=9; j < theLine.length() -2 ; j++ ) {
             source.push_back( Char2Hex( theLine[j]) ) ;
         }
     }
}

// Get every character from the MCS file
void uHTRCtrlMezzInterface::readMCSFile( vector<string>& lines , string mcsFileName ) {

     lines.clear() ;
     string mcsFile = mcsFileName ;
     // read firmware from mcs file
     if ( mcsFileName.size() <  1 ) {  
        mcsFile = tool_readline(" MCS File : " ) ;
     }
     printf(" Open MCS File : %s \n", mcsFile.c_str() ) ;
     FILE* pfile = fopen(mcsFile.c_str(), "r" ) ; 

     if (pfile==0) {
        printf( "  \nError opening '%s'\n",mcsFile.c_str() );
     } else { 

       char buffer[1024];
       while ( !feof(pfile) ) {
  	     buffer[0]=0;

             fgets(buffer,1000,pfile);

             if (buffer[0]!=0) {
                for (int i=strlen(buffer)-1; i>=0; i--) {
                    if (isspace(buffer[i])) buffer[i]=0;
                    else break;
                }
             }
             if (strlen(buffer)>4)   lines.push_back(buffer);

       }
     }
     fclose( pfile );

}

void uHTRCtrlMezzInterface::printMCSFile( ) {
 
     vector<string> lines ;
     readMCSFile( lines ) ;
     printf("size of lines : %d \n", (int)lines.size() ) ;

     for ( size_t i=0; i< lines.size() ; i++) {
     
         string theLine = lines[i] ;
         for ( size_t j=0; j < theLine.length() ; j++ ) {
             printf("%c ", theLine[j] ) ;
         }
         printf("\n") ;
         /*
         for ( size_t j=0; j < theLine.length() ; j++ ) {
             unsigned char hexChar = Char2Hex( theLine[j] ) ;
             printf("%2x", (uint8_t) hexChar ) ;
         }
         printf("\n") ;
         */
     }

}


void uHTRCtrlMezzInterface::spi_read( sub_handle& hd_ , vector<char> instruct, vector<char>& output , int readsize  ) {

   if ( hd_ == 0 ) {
      printf(" Sub20 is not initialized, Please initialize sub20 first !!! " ) ;
      return ;
   }

   char wbuff[128] ;
   char rbuff[256] ;

   // Read command (0x3) + 24 bit address
   /*
   wbuff[0] = 0x3 ;
   wbuff[1] = (addr_base >>16)&0xFF ;
   wbuff[2] = (addr_base >>8)&0xFF ;
   wbuff[3] =  addr_base &0xFF ;
   */
   int sz_i = (int) instruct.size() ; 
   for ( int i=0; i < sz_i ; i++ )
       wbuff[i] = instruct[i] ;
   

   int rc = sub_spi_transfer( hd_, wbuff, 0, sz_i , SS_CONF(0,SS_L) ) ;
   if ( rc !=0 ) printf(" SPI Write fail \n" ) ;

   rc = sub_spi_transfer( hd_, 0, rbuff, readsize , SS_CONF(0,SS_LO) ) ;
   if ( rc !=0 ) printf(" SPI READ fail - error code :%d \n", rc ) ;
   if ( rc ==0 ) { 
        
      for ( int i=0 ; i < readsize ; i++ ) {
          output.push_back( (unsigned char)rbuff[i] ) ;
          //printf("%02X ", (unsigned char)rbuff[i] ) ;
          //if ( i%16 ==15  ) printf("\n") ;
      }
      //printf("\n") ;
   }

}

// execute sector_erase (SE) , size of sector is 0x40000 bytes
void uHTRCtrlMezzInterface::spi_sector_erase( sub_handle& hd_, int iSector ) {

   if ( hd_ == 0 ) {
      printf(" Sub20 is not initialized, Please initialize sub20 first !!! " ) ;
      return ;
   }

   // Write Sequence : 
   // (1) Write Enable : WREN 
   // (2) Erase the memory(all bits) to 1s (0xFF) : SE 
   // (3) Page program : PP + 3bytes address + data(>=1 bytes , <= 256 bytes )
   static const uint8_t CMD_WREN = 0x06;   // write enable
   static const uint8_t CMD_SE   = 0xD8;   // sector erase

   char wbuff[260] = { 0 };

   bool running = false ;
   bool isWEL   = false ;  // Write Enable Latched

   // (1) write enable 
   wbuff[0] =  CMD_WREN ; 
   int rc = sub_spi_transfer( hd_, wbuff, 0, 1 , SS_CONF(0,SS_LO) ) ;
   usleep(50) ;
   if ( rc !=0 ) printf(" SPI Write Enable fail \n" ) ;
   isWEL = spi_status( hd_ , 1 ) ;
   //if ( !isWEL )  printf(" WEL fail before writing \n" ) ;

   // (2) sector erase
   wbuff[0] =  CMD_SE ; 
   wbuff[1] = 3 + (iSector*4) ;
   wbuff[2] = 0xFF ;
   wbuff[3] = 0xFF ;
   rc = sub_spi_transfer( hd_, wbuff, 0, 4 , SS_CONF(0,SS_LO) ) ;
   if ( rc !=0 ) printf(" SPI Sector Erase fail \n" ) ;

   // Check the status register 
   do {
      running = spi_status( hd_ , 0 ) ;
      isWEL   = spi_status( hd_ , 1 ) ;
   } while( running ) ;
   usleep(100000) ;

}

// write a page (256 bytes )
// write size = 256, input_i : indicate the position of each sector
void uHTRCtrlMezzInterface::spi_write( sub_handle& hd_, vector<char> addr , vector<unsigned char>& input, int writeSize, int input_i ) {

   if ( hd_ == 0 ) {
      printf(" Sub20 is not initialized, Please initialize sub20 first !!! " ) ;
      return ;
   }

   // Write Sequence : 
   // (1) Write Enable : WREN 
   // (2) Erase the memory(all bits) to 1s (0xFF) : SE 
   // (3) Page program : PP + 3bytes address + data(>=1 bytes , <= 256 bytes )
   static const uint8_t CMD_WREN = 0x06;   // write enable
   static const uint8_t CMD_PP   = 0x02;   // page program
   //static const uint8_t CMD_WRDI = 0x04;   // write disable

   bool running = false ;
   //bool isWEL   = false ;
   char wbuff[512] = { 0 };

   // (3) Page programming 
   //printf(" begin to write \n") ;
   wbuff[0] =  CMD_WREN ; 
   int rc = sub_spi_transfer( hd_, wbuff, 0, 1 , SS_CONF(0,SS_LO) ) ;
   if ( rc !=0 ) printf(" SPI Write Enable fail \n" ) ;
   spi_status( hd_ , 1 ) ;
   
   // specify address
   wbuff[0] =  CMD_PP ;  
   int sz_a = (int)addr.size() ;
   for ( int i=0 ; i < sz_a ; i++ ) 
       wbuff[i+1] = addr[i] ;

   //printf(" input size = %d \n", input.size() ) ;
   for ( int i=0; i < writeSize ; i++ ) {
       wbuff[i+sz_a+1] = 0 ;
       if ( (i + input_i) < input.size() ) wbuff[i+sz_a+1] = input[i + input_i] ;
       //printf("  ... wbuff[%d] = %2X \n", i, input[i+input_i] ) ;
   }
   rc = sub_spi_transfer( hd_, wbuff, 0, writeSize+sz_a+1 , SS_CONF(0,SS_LO) ) ;
   if ( rc !=0 ) printf(" SPI Page programming fail - error code :%d \n", rc ) ;

   // (period of clk = 4 micro-sec, 250kHz ) * ( 8bits/byte) * ( size of write data + address + command + 10 ) 
   int tt = (1000/spi_clk)*8*(writeSize+sz_a+1+100) ; 
   usleep( tt ) ;

   // Check the status register 
   do {
      running = spi_status( hd_, 0 ) ;
      usleep( 10 ) ;
   } while( running ) ;


}

bool uHTRCtrlMezzInterface::spi_status( sub_handle& hd_, int bitPos ) {

   static const uint8_t CMD_RDSR = 0x05;  // read status register 
   char wbuff[16] = { 0 };
   char rbuff[16] = { 0 };

   bool running = false ; 
   wbuff[0] = CMD_RDSR ;
   //int rc = sub_spi_transfer( hd_, wbuff, rbuff, 1 , SS_CONF(0,SS_LO) ) ;
   int rc = sub_spi_transfer( hd_, wbuff, 0, 1 , SS_CONF(0,SS_LO) ) ;
   usleep(1100);
   rc = sub_spi_transfer( hd_, 0, rbuff, 1 , SS_CONF(0,SS_LO) ) ;
   if ( rc !=0 ) printf(" SPI status check fail !\n" ) ;

   if ( rbuff[0] !=0 ) printf(" SPI Status(%d)  = %2X \n", bitPos , (unsigned char)rbuff[0]&0xFF ) ;
   if ( ( rbuff[0] >> bitPos ) & 0x1 ) { 
      if ( bitPos == 0 )  printf(" ... Busy ... \n") ;
      if ( bitPos == 1 )  printf(" ... Write Enable Latch ...\n") ;
      if ( bitPos == 7 )  printf(" ... Write Protection ...\n") ;
      running = true ;
   }
   //   printf("\n") ;

   return running ;
}

// Tranform Char(0 ~ F/f) to Hex value , other charactor will be set to 0 
unsigned char uHTRCtrlMezzInterface::Char2Hex(unsigned char c) {

      char val5 = (c-'a'+0xa) ;

      char val3 = (c-'A'+0xa) ;
      char val4 =  (c>='a' && c<='f') ? val5 : 0 ;
    
      char val1 = (c-'0') ;
      char val2 = (c>='A' && c<='F') ? val3 : val4 ;

      return ( (c>='0' && c<='9')? val1 : val2 );
}


// mezz_type_code  3: Flash , 4: JTag , 5: CPLD 
void uHTRCtrlMezzInterface::check_MezzId( sub_handle& hd_ ){

  printf(" ==============================\n") ;
  printf(" | Mezzanine ID code          |\n") ;
  printf(" | 3: Flash, 4: JTag, 5 CPLD  |\n") ;
  printf(" ==============================\n") ;
  
  int mezz_type = tool_readline_int("(3)Flash, (4)JTag, (5)CPLD ? " ) ;

  if ( mezz_type == 3 )  i2c_read_MezzId( hd_ , 4 ) ;
  if ( mezz_type == 4 )  i2c_read_MezzId( hd_ , 1 ) ;
  if ( mezz_type == 5 )  spi_read_MezzId( hd_ ) ;

}

// mezz_type_code  3: Flash , 4: JTag , 5: CPLD 
bool uHTRCtrlMezzInterface::tag_MezzId( sub_handle& hd_, int mezz_type ){

  printf(" ==============================\n") ;
  printf(" | Mezzanine ID code          |\n") ;
  printf(" | 3: Flash, 4: JTag, 5 CPLD  |\n") ;
  printf(" ==============================\n") ;

  MezzIdStruct id ;
  memset(&id,0,sizeof(id));
  //printf(" sz of id : %d \n", sizeof(id) ) ;

  char macA[6] ;
  bool validMAC ;
  if ( mezz_type == 3 )  validMAC = i2c_read_MAC( hd_, macA, 4 ) ;
  if ( mezz_type == 4 )  validMAC = i2c_read_MAC( hd_, macA, 1 ) ;
  if ( mezz_type == 5 )  validMAC = spi_read_MAC( hd_, macA ) ;
  if ( !validMAC ) { 
     printf(" Invalid MAC Address \n") ;
     return false ;
  }

  char hmac[20];
  sprintf(hmac, "%02x:%02x:%02x:%02x:%02x:%02x", 0xff&macA[0], 0xff&macA[1], 0xff&macA[2], 0xff&macA[3], 0xff&macA[4], 0xff&macA[5]);
  if (doLog) theLog.mac       = string( hmac ) ;
  if (doLog) theLog.mezz_type = mezz_type ;


  id.data_format_version = 1 ; // this is version 1
  id.mezz_subtype_code   = 1 ; // see codes in the sections below
  id.mezz_type_code      = mezz_type ; // String version of mezzanine type+sub type

  //id.mezz_type[16]       = 0 ; // String version of mezzanine type+sub type
  if ( mezz_type == 3 ) snprintf((char*)id.mezz_type,15,"FLASH");
  if ( mezz_type == 4 ) snprintf((char*)id.mezz_type,15,"JTAG");
  if ( mezz_type == 5 ) snprintf((char*)id.mezz_type,15,"CPLD");

  // Gather Information for mezzanine

  // Serial Number
  // log the record in file and determine SN for the mezzanine
  int sn = log_SN( hd_, mezz_type ) ;
  if ( sn <  0 ) {
     printf(" Fail to find a SN !!!\n") ;
     return false ;
  }

  id.serial_number[0]=sn&0xFF;
  id.serial_number[1]=(sn>>8)&0xFF;
  if (doLog) theLog.sn = sn ;

  // Testing Date
  time_t tt ;
  time(&tt ) ;
  char dateStr[11] ;
  struct tm* now = localtime( &tt );

  sprintf ( dateStr, "%d-%d-%d", now->tm_year+1900 , now->tm_mon+1 , now->tm_mday  ); 
  //printf(" date2: %s \n", dateStr ) ;
  strncpy( (char*)id.manu_date, dateStr, 11 );
  id.manu_date[10]=0;
  if (doLog) theLog.date = string(dateStr) ;

  // Testing Site
  std::string site  = "Site: "; 
  std::string site_ = tool_readline("Site: ") ;
  site += site_ ;
  strncpy((char*)id.manu_site,site.c_str(),16);
  id.manu_site[15]=0;
  if (doLog) theLog.site = site_ ;

  // Tester Name
  std::string tester  = "Tester: "; 
  std::string tester_ = tool_readline("Tester: " );
  tester += tester_ ;
  strncpy((char*)id.manu_tester,tester.c_str(),16);
  id.manu_tester[15]=0;
  if (doLog) theLog.tester = tester_ ;

  // Testing release 
  std::string rel = "ctr_v01";
  strncpy((char*)id.test_release,rel.c_str(),8);
  id.test_release[7]=0;

  // Note
  std::string note_ = tool_readline("Note: " );
  strncpy((char*)id.notes, note_.c_str(), 56 );
  id.notes[55]=0;
  if (doLog) theLog.note = note_ ;

  // Write record into MAC_EEPROM
  if ( mezz_type == 3 ) i2c_write_MAC_EEPROM( hd_ , id, 4 ) ;
  if ( mezz_type == 4 ) i2c_write_MAC_EEPROM( hd_ , id, 1 ) ;
  if ( mezz_type == 5 ) {
     spi_switch( hd_ ,  9 ) ;
     spi_write_MAC_EEPROM( hd_ , id ) ;
  }
  return true ;

}

// Load the SN from the log file
// mezz_type_code  3: Flash , 4: JTag , 5: CPLD 
bool uHTRCtrlMezzInterface::load_SN( ) {

  for (int i=0; i<3; i++) next_sn[i] = 1 ;
  FILE* snfile = fopen( "ctrlTestLog.txt", "r" ) ; 
  if ( snfile == NULL ) {
     printf(" Can not open SN log file !!!\n") ;
     return false ;
  }

  char line[256] ;
  while ( !feof(snfile) ) {

        if ( line[0] == '#' ) continue ;

        char* c_ = fgets(line, 256, snfile ) ;
        if ( c_ == NULL ) break ;

        char mac[17];
        unsigned int sn, mezzType;
        sscanf( line, "%s %d %d\n", mac, &sn, &mezzType );
        //printf(" %s, %d, %d \n", mac, mezzType, sn) ;
        snMap[ string(mac) ] = std::make_pair(mezzType, sn);
        if ( sn >= next_sn[mezzType-3] ) next_sn[mezzType-3] = sn+1  ;
  }
  fclose( snfile );
  return true ;

}

// mezz_type_code  3: Flash , 4: JTag , 5: CPLD 
int uHTRCtrlMezzInterface::log_SN( sub_handle& hd_, int mezz_type ) {

  // Load the database of sn list from the sn file
  bool isloaded = load_SN() ;
  if ( !isloaded ) return -1 ;

  // Read the MAC address
  char macA[6] ;
  bool validMAC = true ; 
  if ( mezz_type == 3 )  validMAC = i2c_read_MAC( hd_, macA, 4 ) ;
  if ( mezz_type == 4 )  validMAC = i2c_read_MAC( hd_, macA, 1 ) ;
  if ( mezz_type == 5 )  validMAC = spi_read_MAC( hd_, macA ) ;

  char hmac[20];
  sprintf(hmac, "%02x:%02x:%02x:%02x:%02x:%02x", 0xff&macA[0], 0xff&macA[1], 0xff&macA[2], 0xff&macA[3], 0xff&macA[4], 0xff&macA[5]);
  string mac(hmac) ;
  if ( !validMAC ) {
     printf(" Invalid MAC address !!!\n" ) ;
     return -1 ;
  }

  // determine the SN for the mezzine
  int sn = 0 ;
  if ( snMap.find( mac ) == snMap.end() ) {

     FILE* snfile = fopen( "ctrlTestLog.txt", "a+" ) ; 
     if ( snfile == NULL )  printf(" file opened error ! \n") ;

     fprintf( snfile, "%s %d %d\n", hmac, next_sn[mezz_type-3], mezz_type );
     sn = next_sn[mezz_type-3] ;
     next_sn[mezz_type-3] +=1 ;
     fclose( snfile );
  } else {

     pair<unsigned int, unsigned int> val =  snMap.find( mac )->second ;
     printf( " Found mezzanine ==> %s type: %d  SN: %d \n", (snMap.find( mac )->first).c_str() , val.first , val.second ) ;
     sn = val.second ;
  }
  return sn ;

}


void uHTRCtrlMezzInterface::start_log_test() {

   theLog.sn = -1 ;
   theLog.mac = "00:00:00:00:00:00" ;
   theLog.date = "" ;
   doLog = true ;
}

void uHTRCtrlMezzInterface::stop_log_test( string logfileName ) {

    if ( logfileName.size() < 1 ) logfileName = "Test_log.txt" ;
    FILE* logfile = fopen( logfileName.c_str() , "a+" ) ; 
 
    // The case for MMC 
    if ( theLog.sn == -1 && theLog.mac == "00:00:00:00:00:00" ) {
  
       int mmc_sn = tool_readline_int(" Please enter the SN for MMC :" ) ; 
       theLog.sn = mmc_sn ;

       theLog.mezz_type = 6 ;
       time_t tt ;
       time(&tt ) ;
       char dateStr[11] ;
       struct tm* now = localtime( &tt );
       sprintf ( dateStr, "%d-%d-%d", now->tm_year+1900 , now->tm_mon+1 , now->tm_mday  ); 
       theLog.date = string(dateStr) ;

       string site_ = tool_readline(" Testing Site: " ) ; 
       theLog.site = site_ ;
       string tester_ = tool_readline(" Tester Name: " ) ; 
       theLog.tester  = tester_ ;
       string note_ = tool_readline(" Testing Note: " ) ; 
       theLog.note  = note_ ;
    }
   
    //fprintf( logfile, " sn        mac         type   date    site  name   result  note\n" );
    fprintf( logfile, " %d  %s   %d   %s  %s  %s ",  theLog.sn, theLog.mac.c_str(), theLog.mezz_type, theLog.date.c_str(), theLog.site.c_str(), theLog.tester.c_str()  );
    for ( size_t i=0; i< theLog.result.size() ; i++ ) {
        fprintf( logfile, " %s", theLog.result[i].c_str() ) ;
    }
    fprintf(logfile, " %s ", theLog.note.c_str() ) ;
    fprintf( logfile, "\n" ) ;
    fclose( logfile ) ;
}
