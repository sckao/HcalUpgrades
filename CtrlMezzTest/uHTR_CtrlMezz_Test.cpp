#include <cstdlib>
#include <cstdio>
#include <unistd.h> 
#include <getopt.h>
#include <time.h>
#include <map>
#include <string>
#include <vector>
//#include "uHTR_CtrlMezz_Test.h"
#include "uHTRCtrlMezzInterface.h"
#include <readline/readline.h>
#include <readline/history.h>
//#include <boost/thread.hpp>
//#include <boost/bind.hpp>

using namespace std ;

int spi_channel = 2 ;

void help() ;
bool InitializeSub20( sub_device& dev_ , sub_handle& hd_ ) ;

void help()
{
    printf("Usage: ./uHTR_CtrlMezz_Test.exe modeFlags [options]\n");
    printf("Mode Flags - These can be used in conjunction but will always evaluate in the order listed below.\n");
    printf(" --flash,           -f   Flash test\n");
    printf(" --clkset,          -k   Write CPLD EEPROM\n");
    printf(" --jtag,            -j   Tag JTAG Mezzanine\n");
    printf(" --mmc,             -m   Test MMC\n");
    printf(" --readflash,       -r   Read Flash\n");
    printf(" --writeflash,      -w   Write Flash\n");
    printf(" --cpld,            -c   Test CPLD\n");
    printf(" --initial,         -i   Initialize sub20\n");
    printf(" --scan,            -s   Scan i2c slaves\n");
    printf(" --printMCS,        -p   Print MCS file\n");
    printf(" --test,            -t   Test \n");
    printf(" --help,            -h   Showing this help menu\n");
    printf(" ==================================================== \n") ;
    printf(" == SPI Targets 0: BCK_CS0,   1: BCK_CS1, 2: BCK_CS  \n") ;
    printf(" ==             3: FNT_CS0,   4: FNT_CS1, 5: FNT_CS  \n") ;
    printf(" ==             8: CS_EEPROM, 9: CS_MAC, 10: CS_CPLD \n") ;
    printf(" ==================================================== \n") ;

}

int main(int argc, char* argv[])
{

    int opt;
    int option_index = 0;
    int rev, freq_;
    static struct option long_options[] = {
        {"flash",               no_argument, 0, 'f'},
        {"initial",             no_argument, 0, 'i'},
        {"scan",                no_argument, 0, 's'},
        {"test",                no_argument, 0, 't'},
        {"mmc",                 no_argument, 0, 'm'},
        {"clkset",              no_argument, 0, 'k'},
        {"jtag",                no_argument, 0, 'j'},
        {"cpld",          required_argument, 0, 'c'},
        {"readflash",     required_argument, 0, 'r'},
        {"writeflash",    required_argument, 0, 'w'},
        {"printMCS",            no_argument, 0, 'p'},
        {"help",                no_argument, 0, 'h'}
    };

    if(argc == 1) {
        help();
        return 0;
    }

    uHTRCtrlMezzInterface* ctrl = new uHTRCtrlMezzInterface() ;
    sub_device dev = 0 ;
    sub_handle hdl = 0 ;

    InitializeSub20( dev, hdl );
    ctrl->set_sub20_frequence( hdl, 2000 ) ;
    ctrl->set_spi_clock( 1000 ) ;
    bool validMac = true ;

    while((opt = getopt_long(argc, argv, "ftipkjmsh:c:w:r:", long_options, &option_index)) != -1)
    {
        switch (opt)
        {
            case 'i':
                //InitializeSub20( dev, hdl );
                ctrl->Test2( hdl ) ;          
                break;
            case 't':
                //spi_channel = int(atoi(optarg));
                //freq_ = ctrl->tool_readline_int(" Set I2C Frequence (489Hz ~ 444444 Hz) : " ) ;
                //ctrl->set_sub20_frequence( hdl, freq_ ) ;
                ctrl->Test( hdl ) ;          
                //ctrl->log_SN( hdl, 4 ) ;          
                break;
            case 'c':
                spi_channel = int(atoi(optarg));
                ctrl->spi_switch( hdl, spi_channel ) ;          
                ctrl->check_EEPROM( hdl ) ;
                ctrl->spi_read_EEPROM( hdl, spi_channel, true ) ;
                break;
            case 'j':
                ctrl->start_log_test() ;
                validMac = ctrl->tag_MezzId( hdl, 4 ) ;
                if ( validMac ) {
                   ctrl->reset_ADC( hdl, 1 ) ;
                   ctrl->read_ADC( hdl, 1 ) ;
                   ctrl->stop_log_test( "/home/daq/hcal/TestLog/JTag_test_log.txt" ) ;
                }
                break;

            case 'm':
                ctrl->start_log_test() ;
                ctrl->reset_ADC( hdl, 0 ) ;
                ctrl->read_ADC( hdl, 0 ) ;
                ctrl->stop_log_test( "/home/daq/hcal/TestLog/MMC_test_log.txt" ) ;
                break;
            case 'k':
                ctrl->start_log_test() ;
                validMac = ctrl->tag_MezzId( hdl, 5 ) ;
                if ( validMac ) {
                   ctrl->spi_switch( hdl, 8 ) ;          
		   ctrl->check_EEPROM( hdl ) ;
		   ctrl->erase_CLK_EEPROM( hdl ) ;
		   ctrl->write_CLK_EEPROM( hdl, 320, 240 ) ;
		   ctrl->stop_log_test( "/home/daq/hcal/TestLog/Clock_test_log.txt" ) ;
                }
                break;
            case 'f':
                ctrl->start_log_test() ;
                validMac = ctrl->tag_MezzId( hdl, 3 ) ;
                if ( validMac ) {
		   ctrl->spi_switch( hdl, 2, 1  ) ;          
		   ctrl->writeFirmware( hdl, "/home/daq/firmware/uhtr_back_HF1600_0_E_10.mcs" ) ;
                   ctrl->spi_switch( hdl, 5, 1  ) ;          
		   ctrl->writeFirmware( hdl, "/home/daq/firmware/uhtr_front_HF1600_0_C_0.mcs" ) ;
		   ctrl->spi_switch( hdl, 2, 0  ) ;          
		   ctrl->writeFirmware( hdl, "/home/daq/firmware/uhtr_back_HF4800_0_E_10.mcs" ) ;
		   ctrl->spi_switch( hdl, 5, 0  ) ;          
		   ctrl->writeFirmware( hdl, "/home/daq/firmware/uhtr_front_HF4800_0_C_0.mcs" ) ;
		   ctrl->stop_log_test( "/home/daq/hcal/TestLog/Flash_test_log.txt" ) ;
                }
                break;
            case 'w':
                ctrl->tag_MezzId( hdl, 3 ) ;
                spi_channel = int(atoi(optarg));
                rev = ctrl->tool_readline_int(" Which Revision ? (0 or 1) : " ) ;
                ctrl->spi_switch( hdl, spi_channel, rev ) ;          
                ctrl->writeFirmware( hdl ) ;
                break;
            case 'r':
                spi_channel = int(atoi(optarg));

                rev = ctrl->tool_readline_int(" Which Revision ? (0 or 1) : " ) ;
                ctrl->spi_switch( hdl , spi_channel, rev ) ;          
                ctrl->verifyFirmware( hdl ) ;
                break;
            case 'p':
                ctrl->printMCSFile() ;
                break;
            case 's':
                InitializeSub20( dev, hdl );
                ctrl->scan_i2c_slaves( hdl ) ;
                break;
            case 'h':
                help() ;
                break ;
            default:
                help() ;
                return 0;

        }
    }


   return 0 ;

}

bool InitializeSub20( sub_device& dev_ , sub_handle& hd_ ) {

    bool found_s20 = false ;
    int k = 0 ;
    while ( !found_s20 && k < 10 ) {

           dev_ = sub_find_devices( dev_ ) ;
  
            if ( dev_ !=0 ) {
        
               printf(" found dev %d \n", k ) ; 
	       hd_ = sub_open( dev_ );
	       if ( hd_ == 0) { 
                  printf("DEVICE FAILED TO OPEN\n");
                  unsigned int errval_ = sub_errno;
		  unsigned int i2c_status_ = sub_i2c_status;
		  printf(" Dev(%d) error : %u  i2c_stat: %u \n\n", k, errval_ , i2c_status_ ) ;
               } 
	       else {
                  found_s20 = true ;
               }
            }

            k++ ;
    }

    // check the sub20 sn 
    char buf[100] ;
    int sn = sub_get_serial_number( hd_ , buf, sizeof( buf ) ) ;
    printf(" sn: %d = %s \n", sn, buf );
 
    return found_s20 ;

}

