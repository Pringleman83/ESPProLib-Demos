// ESP Pro Library
#include "./esppl_functions.h"
 
String source; char srcOctet[2];
String dest;   char destOctet[2];
String ssid;
String packet;

void cb(esppl_frame_info *info) {
    source = "";
    dest = "";
    ssid = "";
    
    for (int i= 0; i< 6; i++) { sprintf(srcOctet, "%02x", info->sourceaddr[i]);    source+=srcOctet; if (i!=5) source+=":";} // source MAC
    for (int i= 0; i< 6; i++) { sprintf(destOctet, "%02x", info->receiveraddr[i]); dest+=destOctet;  if (i!=5) dest+=":";}   // destination MAC
    source.toUpperCase(); dest.toUpperCase();
    // ESSID
    if (info->ssid_length > 0) {for (int i= 0; i< info->ssid_length; i++) { ssid+= (char) info->ssid[i]; }}
      packet = "{\"dev\": \""+ WiFi.macAddress()+"\" , \"src\": \""+ source +"\" , \"dst\": \""+ dest +"\", \"rss\":\""+(String) info->rssi+"\", \"ess\":\""+ssid+"\", \"ch\":\""+info->channel+"\", \"ft\":\""+(String) info->frametype+"\", \"fst\":\""+(String) info->framesubtype+"}";
      int rando = random(300,500);
      
      for (int i = 0; i < packet.length(); i++) {
        Serial.write(packet[i]);   // Push each char 1 by 1 on each loop pass
      }
      Serial.println();
//    Serial.println(ESP.getFreeHeap());

}

void setup() {
  delay(500); 
  Serial.begin(115200);
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
