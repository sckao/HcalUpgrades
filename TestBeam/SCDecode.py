import os

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-f", "--file", dest="filename",
                  help="read data from <file>.txt ", metavar="FILE")
parser.add_option("-w", "--nbx", dest="nbx",
                  help="number of bx of readout window")
parser.add_option("-i", "--fiber", dest="fiber",
                  help="the fiber link")
parser.add_option("-n", "--nfiber", dest="nfiber", default="2", 
                  help="number of fiber ")


(options, args) = parser.parse_args()


file = open( options.filename + ".txt", 'r' )
file_out = open( options.filename + "_output.txt", 'w' )
nbx = int(options.nbx)
fiber = int(options.fiber) 
nfiber = int(options.nfiber) 
print "filename:", options.filename," nbx =",nbx," fiber =",fiber, " nfiber=", nfiber  

#nfiber =2
#nbx=20
#fiber=4
#file = open( "particleOnHF_quadAnode_32GeVpos_100kCounts_channelSwitch_adjustTDCDAC_001.txt" )
lines = file.readlines()

events = []
formattedEvents = []

def getADC( events = [] ) :

        adcs = []
        tdcs = []
	for i in range( len(events) ) :
                 adcs.append( events[i][1:2] )
                 tdcs.append( events[i][2:] )
                 #print "0==>", events[i][0:1]
                 #print "1==>", events[i][1:2]
 
                 badData = '0'
                 if ( str(events[i][0:1]).find( "Suspicious data format!" ) != -1 ) :
                    badData = '1'

                 capId   = str( str(events[i][0:1])[26:27] )
                 if ( str(events[i][0:1]).find( "BX0" ) != -1 ) :
                    capId   = str( str(events[i][0:1])[29:30] )

                 capBad  = str( str(events[i][0:1])[28:29] )
                 bxWord  = int( str(events[i][1:2])[6:10] )
                 adcWord = str( str(events[i][1:2])[27:50] )
                 tdcWord = str( str(events[i][2:])[27:50] )
                 if ( capBad != 'C' ) : 
                      capBad = '0'
                 if ( capBad == 'C' ) : 
                      capBad = '1'

                 #print (bxWord - 4)/6, capId, capBad, badData, adcWord, tdcWord 
                 file_out.write( badData  )
                 file_out.write( ' '  )
                 file_out.write( capBad  )
                 file_out.write( ' '  )
                 file_out.write( str((bxWord - 4)/6) )
                 file_out.write( ' '  )
                 file_out.write( capId  )
                 file_out.write( ' '  )
                 file_out.write( adcWord  )
                 file_out.write( tdcWord  )
                 file_out.write( '\n'  )
#                 chan = adcWord.split(' ') 
#                 for j in range( len( chan ) ) :
#
#                            if ( chan[j] != " " ) :
#                       		print j,"->",chan[j]  
                 
        return adcs 


for i in range( len(lines) ) :
#for i in range( 1000 ) :
        if ( lines[i].find( "START EVENT" ) != -1 ) :

                for j in range( 4 + (nbx*6 + 2)*(nfiber-1) ) :
#                        print " --> ", j, lines[i+j] 

                        if ( i+j > len(lines) ) :
                           break 

                        if (lines[ i+j ].find( "Fiber:"+str(fiber) ) != -1 ):
                           for k in range(nbx):
                                
                                idx = k*3 + i + j +1;
                                events.append( lines[ idx : idx+3 ] )
#                                print len(events) , lines[ i+j+1 : i+j+13 ]



ADC = getADC(events)
file.close()
file_out.close()
#print "n of ADC", len(ADC)

#for j in range( len(events) ) :
#	print events[j]

