#ifndef UHTRCTRLMEZZINTERFACE_H
#define UHTRCTRLMEZZINTERFACE_H

#include "libsub.h"
#include <readline/readline.h>
#include <readline/history.h>
#include "stdint.h"
#include <ctime>

//#include "uHTRMezzInterface.h"
//#include <vector>
//#include <string>

struct MezzIdStruct {
       uint8_t macid[6];
       uint8_t data_format_version; // this is version 1
       uint8_t mezz_type_code;      // see codes in the sections below
       uint8_t mezz_subtype_code;   // see codes in the sections below
       uint8_t mezz_type[16];       // String version of mezzanine type+sub type
       // (zero -terminated)
       uint8_t serial_number[2];    // construction serial number ([0]*256+[1])
       uint8_t manu_date[11];       // DAY-MON-YEAR (zero-terminated)
       uint8_t manu_site[16];       // Site of manufacture (zero-terminated)
       uint8_t manu_tester[16];     // Name of tester (zero-terminated)
       uint8_t test_release[8];     // Release version of test code used
       uint8_t notes[56];           // String notes field in this version,
                                     // available for future use if needed
};


class uHTRCtrlMezzInterface {

public:

  uHTRCtrlMezzInterface() { } ;
  ~uHTRCtrlMezzInterface() { } ;
  
  void set_sub20_frequence( sub_handle& hd_, int freq = 2000 ) ;
  void scan_i2c_slaves( sub_handle& hd_ ) ;
  void Test( sub_handle& hd_ ) ;
  void spi_test( sub_handle& hd_ ) ;

  // Functions for i2c devices
  void read_ADC( sub_handle& hd_, int i2c_ch ) ;
  void reset_ADC( sub_handle& hd_, int i2c_ch ) ;
  void i2c_read_MAC( sub_handle& hd_, char* macA, int i2c_ch ) ;
  void i2c_write_MAC_EEPROM( sub_handle& hd_, MezzIdStruct& id, int i2c_ch ) ;
  std::vector<char> i2c_read_MAC_EEPROM( sub_handle& hd_, uint16_t addr, int nRead = 1 ) ;
  // GPIO functions
  void write_GPIO( sub_handle& hd_, uint8_t val, bool isFront )  ;
  void status_GPIO( sub_handle& hd_, bool isFront )  ;

  // General SPI functions
  void spi_switch( sub_handle& hd_, int channel, int revision = 1 ) ;
  void spi_config( sub_handle& hd_ ) ;
  void spi_sector_erase( sub_handle& hd_, int addr ) ;
  void spi_read( sub_handle& hd_, std::vector<char> instruct, std::vector<char>& output, int readsize ) ;
  void spi_write( sub_handle& hd_, std::vector<char> address, std::vector<unsigned char>& input, int writesize, int input_i = 0) ;
  bool spi_status( sub_handle& hd_, int bitPos ) ;

  // For CPLD Mezzanine
  void check_EEPROM( sub_handle& hd_ ) ;
  void write_CLK_EEPROM(  sub_handle& hd_, int imageChoice1 , int imageChoice2 ) ;
  void erase_CLK_EEPROM(  sub_handle& hd_, bool allZero = false ) ;
  void spi_read_MAC( sub_handle& hd_, char* macA  ) ;
  void spi_write_MAC_EEPROM( sub_handle& hd_, MezzIdStruct& id ) ;
  std::vector<char> spi_read_EEPROM( sub_handle& hd_, int spi_ch, bool toFile = false ) ;

  // For Flash Mezzanine
  void checkFlashID( sub_handle& hd_ ) ;
  void writeFirmware( sub_handle& hd_, std::string mcsFileName = "" ) ;
  void readFirmwareSector( sub_handle& hd_, std::vector<unsigned char>& inFlash, int iSector, int iPage = -1 ) ;
  bool verifyFirmware( sub_handle& hd_ ) ;
  void readMCSFile( std::vector<std::string>& lines, std::string mcsFileName = "" ) ;
  void readMCSFile( std::vector<unsigned char>& source , std::string mcsFileName = "" ) ;
  void printMCSFile() ;

  void tag_MezzId( sub_handle& hd_, int mezz_type ) ;
  bool load_SN( ) ;
  int log_test( sub_handle& hd_, int mezz_type ) ;

  unsigned char Char2Hex(unsigned char c) ;
  std::string tool_readline( std::string prompt ) ;
  int tool_readline_int( std::string prompt ) ;

//private:

  unsigned int next_sn[3] ;
};

#endif
