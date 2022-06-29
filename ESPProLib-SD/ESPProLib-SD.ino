#include <SD.h>
#include <Time.h>   

String logFileName = "";
int networks = 0;     

#include "./esppl_functions.h"

#define SD_CS 15 // Changed from D8 as this ins't recognised for generic ESP2866 boards

String source; char srcOctet[2]; // fix string 
String dest;   char destOctet[2];
String ssid;
String packet;

String getFrameTypeName(int frametype){
  String FTName; 
  switch (frametype) {
    case 0:
      FTName = "Management";
      break;
    case 1:
      FTName = "Control";
      break;
    case 2:
      FTName = "Data";
      break;
    case 3:
      FTName = "Extension";
      break;
    default:
      FTName = "Unrecognised";
      break;
  }
  return FTName;
}

String getFrameSubTypeName(int frametype, int framesubtype){
  String FSTName; 
  switch (frametype) {
    case 0:
      switch (framesubtype) {
        case 0:
          FSTName = "Assocaition Req.";
          break;
        case 1:
          FSTName = "Assocaition Resp.";
          break;
        case 2:
          FSTName = "Reassociation Req.";
          break;
        case 3:
          FSTName = "Reassociation Resp.";
          break;
        case 4:
          FSTName = "Probe Req.";
          break;
        case 5:
          FSTName = "Probe Resp.";
          break;
        case 6:
          FSTName = "Timing Advertisement";
          break;
        case 7:
          FSTName = "Reserved";
          break;
        case 8:
          FSTName = "Beacon";
          break;
        case 9:
          FSTName = "ATIM";
          break;
        case 10:
          FSTName = "Disassociation";
          break;
        case 11:
          FSTName = "Authentication";
          break;
        case 12:
          FSTName = "Deauthentication";
          break;
        case 13:
          FSTName = "Action";
          break;
        case 14:
          FSTName = "Action No Ack (NACK)";
          break;
        case 15:
          FSTName = "Reserved";
          break;
        default:
          FSTName = "Unrecognised";
          break;
      }
      break;
    case 1:
      switch (framesubtype) {
        case 0:
          FSTName = "Reserved";
          break;
        case 1:
          FSTName = "Reserved";
          break;
        case 2:
          FSTName = "Trigger";
          break;
        case 3:
          FSTName = "TACK";
          break;
        case 4:
          FSTName = "Beamforming Report Poll";
          break;
        case 5:
          FSTName = "VHT/HE NDP Announcement";
          break;
        case 6:
          FSTName = "Control Frame Extension";
          break;
        case 7:
          FSTName = "Control Wrapper";
          break;
        case 8:
          FSTName = "Block Acknowledge Request (BAR)";
          break;
        case 9:
          FSTName = "Block Ack (BA)";
          break;
        case 10:
          FSTName = "PS-Poll";
          break;
        case 11:
          FSTName = "RTS";
          break;
        case 12:
          FSTName = "CTS";
          break;
        case 13:
          FSTName = "ACK";
          break;
        case 14:
          FSTName = "CF-End";
          break;
        case 15:
          FSTName = "CF-End + CF-ACK";
          break;
        default:
          FSTName = "Unrecognised";
          break;
      }
      break;
    case 2:
      switch (framesubtype) {
        case 0:
          FSTName = "Data";
          break;
        case 1:
          FSTName = "Reserved";
          break;
        case 2:
          FSTName = "Reserved";
          break;
        case 3:
          FSTName = "Reserved";
          break;
        case 4:
          FSTName = "Null (no data)";
          break;
        case 5:
          FSTName = "Reserved";
          break;
        case 6:
          FSTName = "Reserved";
          break;
        case 7:
          FSTName = "Reserved";
          break;
        case 8:
          FSTName = "QoS Data";
          break;
        case 9:
          FSTName = "QoS Data + CF-ACK";
          break;
        case 10:
          FSTName = "QoS Data + CF-Poll";
          break;
        case 11:
          FSTName = "QoS Data + CF-ACK + CF-Poll";
          break;
        case 12:
          FSTName = "QoS Null (no data)";
          break;
        case 13:
          FSTName = "Reserved";
          break;
        case 14:
          FSTName = "QoS CF-Poll (no data)";
          break;
        case 15:
          FSTName = "QoS CF-ACK + CF-Poll (no data)";
          break;
        default:
          FSTName = "Unrecognised";
          break;
      }
      break;
    case 3:
      switch (framesubtype) {
        case 0:
          FSTName = "DMG Beacon";
          break;
        case 1:
          FSTName = "S1G Beacon";
          break;
        case 2:
          FSTName = "Reserved";
          break;
        case 3:
          FSTName = "Reserved";
          break;
        case 4:
          FSTName = "Reserved";
          break;
        case 5:
          FSTName = "Reserved";
          break;
        case 6:
          FSTName = "Reserved";
          break;
        case 7:
          FSTName = "Reserved";
          break;
        case 8:
          FSTName = "Reserved";
          break;
        case 9:
          FSTName = "Reserved";
          break;
        case 10:
          FSTName = "Reserved";
          break;
        case 11:
          FSTName = "Reserved";
          break;
        case 12:
          FSTName = "Reserved";
          break;
        case 13:
          FSTName = "Reserved";
          break;
        case 14:
          FSTName = "Reserved";
          break;
        case 15:
          FSTName = "Reserved";
          break;
        default:
          FSTName = "Unrecognised";
          break;
      }
    default:
      FSTName = "Unrecognised";
      break;
  }
  return FSTName;
}


void cb(esppl_frame_info *info) {
  File logFile = SD.open(logFileName, FILE_WRITE);
  Serial.print("\n");
  logFile.println();

  
  Serial.print("FT: ");  
  //Serial.print((int) info->frametype);
  Serial.print((String) getFrameTypeName(info->frametype));
  logFile.print((String) getFrameTypeName(info->frametype)); logFile.print(",");
  
  Serial.print(" FST: ");  
  Serial.print((String) getFrameSubTypeName(info->frametype, info->framesubtype));
  logFile.print((String) getFrameSubTypeName(info->frametype, info->framesubtype)); logFile.print(",");

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
    Serial.println("Micro SD card not detected");
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
