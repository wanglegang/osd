#include <FastSerial.h>
#include <EEPROM.h>
#include <GCS_MAVLink.h>
#include "OSD_Config.h"
#include "ArduCam_Max7456.h"
#include "OSD_Vars.h"
#define TELEMETRY_SPEED  57600  // How fast our MAVLink telemetry is coming to Serial port
FastSerialPort0(Serial);
OSD osd; //OSD object 
void setup() 
{
#ifdef ArduCAM328
    pinMode(10, OUTPUT); // USB ArduCam Only
#endif
    pinMode(MAX7456_SELECT,  OUTPUT); // OSD CS
    Serial.begin(TELEMETRY_SPEED);
    // setup mavlink port
    mavlink_comm_0_port = &Serial;
#ifdef membug
    Serial.println(freeMem());
#endif
    // Prepare OSD for displaying 
    unplugSlaves();
    osd.init();
    // Start 
    startPanels();
    delay(500);
} 
void loop() 
{
}
//--------------------function-----------------------------
void unplugSlaves(){
    //Unplug list of SPI
#ifdef ArduCAM328
    digitalWrite(10,  HIGH); // unplug USB HOST: ArduCam Only
#endif
    digitalWrite(MAX7456_SELECT,  HIGH); // unplug OSD
}
//----------------------panels 面板部分，用于显示-------------------
/******* STARTUP PANEL *******/
void startPanels(){
    osd.clear();
    panLogo(); // Display our logo  
    do_converts(); // load the unit conversion preferences
}
void do_converts()
{
    if (EEPROM.read(measure_ADDR) == 0) {
        converts = 3.6;
        converth = 2.0;
        spe = 0x81;
        high = 0x8D;
    } else {
        converts = 2.23;
        converth = 3.28;
        spe = 0xfb;
        high = 0x66;
    }
}
//------------------ Panel: Startup ArduCam OSD LOGO -------------------------------
void panLogo(){
 //以下优联logo
   osd.setPanel(1,1);
   osd.openPanel();
   osd.printf("%c%c%c%c%c%c",0xa0,0xa1,0xa2,0xa3,0xa4,0xa5);      
   osd.closePanel();
 
 
  //以下车次
    osd.setPanel(11,1);
    osd.openPanel();
    osd.printf("%c%c",0x90,0x91); 
    osd.printf_P(PSTR("u63512"));      
    osd.closePanel();
   
    //以下网络信号
   osd.setPanel(25,1);
   osd.openPanel();
   osd.printf("%c",0xfa); 
   osd.printf("%c",0xb9);  
   osd.printf("%c",0x12);    
   osd.closePanel();
  
  //以下测试
   osd.setPanel(1,6);
   osd.openPanel();
   osd.printf("%c",0x1f);      
   osd.closePanel();
   
  
  //最下面一行
   osd.setPanel(1, 12);
   osd.openPanel();
   
   //以下压力
   osd.printf("%c%c",0x92,0x93); 
   osd.printf_P(PSTR("560kpa  "));   
   
   
   //以下到车距离
   osd.printf("%c%c",0x94,0x95); 
   osd.printf_P(PSTR("0.6m   "));  
  
  //以下铁路里程
   osd.printf("%c%c",0x96,0x97); 
   osd.printf_P(PSTR("58.8km"));  
   
  // osd.printf_P(PSTR("Lc:58.088KM"));    
   osd.closePanel();
}
