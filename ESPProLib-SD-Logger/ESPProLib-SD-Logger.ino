#include <SD.h>
#include <Time.h>   

String logFileName = "";
int networks = 0;     

#include "./esppl_functions.h"

#define SD_CS D8

String source; char srcOctet[2]; // fix string 
String dest;   char destOctet[2];
String ssid;
String packet;

void cb(esppl_frame_info *info) {
  File logFile = SD.open(logFileName, FILE_WRITE);
  Serial.print("\n");
  logFile.println();

  
  Serial.print("FT: ");  
  Serial.print((int) info->frametype);
  logFile.print((int) info->frametype); logFile.print(",");
  
  Serial.print(" FST: ");  
  Serial.print((int) info->framesubtype);
  logFile.print((int) info->framesubtype); logFile.print(",");

  source = "";
    dest = "";
    ssid = "";
    
    for (int i= 0; i< 6; i++) { sprintf(srcOctet, "%02x", info->sourceaddr[i]);    source+=srcOctet; if (i!=5) source+=":";} // source MAC
    for (int i= 0; i< 6; i++) { sprintf(destOctet, "%02x", info->receiveraddr[i]); dest+=destOctet;  if (i!=5) dest+=":";}   // destination MAC    

    if (info->ssid_length > 0) {for (int i= 0; i< info->ssid_length; i++) { ssid+= (char) info->ssid[i]; }}

    source.toUpperCase(); dest.toUpperCase();
    
    logFile.print(source); logFile.print(",");
    logFile.print(dest); logFile.print(",");
        Serial.print(" SRC: "); Serial.print(source);
    Serial.print(" DST: "); Serial.print(dest);

    
  
  Serial.print(" RSSI: ");
  Serial.print(info->rssi);
  logFile.print((int) info->rssi); logFile.print(",");
  
  Serial.print(" SEQ: ");
  Serial.print(info->seq_num);
  logFile.print(info->seq_num); logFile.print(",");
  
  Serial.print(" CHNL: ");
  Serial.print(info->channel);
  logFile.print(info->channel); logFile.print(",");

      logFile.print(ssid); logFile.print(",");
  Serial.print(" SSID: "); Serial.println(ssid);
  
  logFile.close();
}

void setup() {
  delay(500);
  Serial.begin(115200);
  
  if (!SD.begin(SD_CS)) {
    Serial.println("Waiting on SD Card...");
    while (!SD.begin(SD_CS));
  }
  
  Serial.println("SD Card found!");
  int i = 0; logFileName = "log0.csv";
  
  while (SD.exists(logFileName)) { i++; logFileName = "log" + String(i) + ".csv"; }
  File logFile = SD.open(logFileName, FILE_WRITE);
  if (logFile) {
    logFile.println("Frame Type,Subframe Type,Source MAC, Destination MAC, RSSI, Sequence, Channel, SSID");
  }
  logFile.close();
  esppl_init(cb);
}

void loop() {
  esppl_sniffing_start();
  while (true) {
    for (int i = 1; i < 15; i++ ) {
      esppl_set_channel(i);
      while (esppl_process_frames()) {
        //
      }
    }
  }  
}
