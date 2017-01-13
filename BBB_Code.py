import Adafruit_BBIO.UART as UART
import Adafruit_BBIO.GPIO as GPIO
GPIO.setup("P9_42",GPIO.OUT)
import serial
import time
import math
UART.setup("UART1")
global gps_string
global i
global lat_digit
global lat_decimal
global lon_digit
global lon_decimal
j=0
index = 0
PI = math.pi
R = 63298393536
lat_c=0
lon_c=0
char1=char2=char3=char4=[0,0,0,0,'N']
database = [char1,char2,char3,char4]
float(lat_c)
float(lon_c)
ser = serial.Serial(port = "/dev/ttyO1", baudrate=9600)
ser.close()
ser.open()
if ser.isOpen():
    while(1):
        gps_string = ser.read(51)
        if gps_string[0]=='R':
            i=4
            print gps_string
            while(gps_string[i]!=','):
                i+=1
            i+=1;
            if gps_string[i] == 'V':
                GPIO.output("P9_42",GPIO.HIGH)
            elif gps_string[i] == 'A':
                lat_c=0
                lon_c=0
                GPIO.output("P9_42",GPIO.LOW)
                i = i+2
                #start extraction
                
                while(gps_string[i]!='.'):
                    lat_digit = ord(gps_string[i]) - 48
                    lat_c = lat_c*10 + lat_digit
                    i+=1
                n=1
                i+=1
                while(gps_string[i]!=','):
                    lat_decimal = ord(gps_string[i]) - 48
                    lat_decimal/(n*10.0)
                    lat_c = lat_c + lat_decimal
                    i+=1
                    n+=1
                i+=1
                lat_c_dir=gps_string[i]
                i+=2
                a1=int(lat_c)
                a1=a1/100
                a1=int(a1)
                a1=a1*100
                a2=lat_c-a1
                a2=a2/60
                lat_c=a2+(a1/100)
                #lat and lat_dir extracted 

                while(gps_string[i]!='.'):
                    lon_digit = ord(gps_string[i]) - 48
                    lon_c = lon_c*10 + lon_digit
                    i+=1
                n=1
                i+=1
                while(gps_string[i]!=','):
                    lon_decimal = ord(gps_string[i]) - 48
                    lon_decimal/(n*10.0)
                    lon_c = lon_c + lon_decimal
                    i+=1
                    n+=1
                i+=1
                lon_c_dir = gps_string[i]
                i+=2
                a1=int(lon_c)
                a1=a1/100
                a1=int(a1)
                a1=a1*100
                a2=lon_c-a1
                a2=a2/60
                lon_c=a2+(a1/100)
                i+=2
                inconsistency = gps_string[i]
                #lon and lon_dir extracted
                
                print "Latitude & Latitude direction\t"
                print lat_c,lat_dir
                print "Longitude & Longitude direction\t"
                print lon_c,lon_dir
                
                lat_c = lat_c*PI/180.0
                lon_c = lon_c*PI/180.0
                
                if inconsistency!='N':
                    lat_i = lat_c;
                    lat_i_dir = lat_c_dir;
                    lon_i = lon_c;
                    lon_i_dir = lon_c_dir;
                    for c in range(0,4):
                        distance = math.acos((math.sin(lat_c))*(math.sin(database[c][0])) + (math.cos(lat_c))*(math.cos(database[c][0]))*(math.cos(lon_c - database[c][2])))
                        if c == 0:
                            min_distance = distance
                        elif min_distance > distance:
                            min_distance = distance
                        if min_distance < 15:
                            exchange = c
                    if min_distance > 15:
                        database[index] == [lat_i,lat_i_dir,lon_i,lon_i_dir,inconsistency]
                    else:
                        temp = database[exchange]
                        database[exchange] = database[3]
                        database[3] = temp
                elif inconsistency == 'N':
                    ser.write("N")
                for d in range(0,index-1):
                    distance = math.acos((math.sin(lat_c))*(math.sin(database[d][0])) + (math.cos(lat_c))*(math.cos(database[d][0]))*(math.cos(lon_c - database[d][2])))
                    if d == 0:
                        min_distance = distance
                    elif min_distance > distance:
                        min_distance = distance
                    if min_distance < 15:
                        warning_char = database[e][4]
                        ser.write(warning_char)
                                        
ser.close()
