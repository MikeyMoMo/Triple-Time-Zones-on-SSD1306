#if not defined ORIGINAL
/***************************************************************************/
void updateDisplay()
/***************************************************************************/
{
//  time(&UTC);  // Probably not necessary.
  display.clearDisplay();
  display.setCursor(0, displayLine1);
  if ((UTC % 10) > 2)       // Show UTC for 7 seconds
    display.print(localtime(&UTC), "%a %x %R%Z");
//  else if (UTC % 10 > 1)   // Show current Heap size for 1 second
//    display.printf("%i  %i", firstHeapSize,lastHeapSize);
  else {                    // and my name for 2
    display.print("Dual NTP Time "); display.print(myVersion);
  }
  now = UTC + BCOffset;
  display.setCursor( 0, displayLine2); display.print("Benicia");
  display.setCursor(50, displayLine2); display.print(localtime(&now), "%A");
  display.setCursor( 0, displayLine3);
  display.print(localtime(&now), "%x %X "); display.print(BCDST);

  now = UTC + RPOffset;
  //  Serial.printf("PHT: %i\r\n", now);
  //  Serial.println(localtime(&now), "Bangui  %a %d-%m-%Y %T %Z");
  display.setCursor( 0, displayLine4); display.print("Bangui");
  display.setCursor(50, displayLine4); display.print(localtime(&now), "%A");
  display.setCursor( 0, displayLine5);
  display.print(localtime(&now), "%x %X "); display.print(RPDST);
  display.display();
}
/***************************************************************************/
void printDisplayHeader()
/***************************************************************************/
{
  display.clearDisplay();
  display.setCursor(0, displayLine1);
  display.print("Dual NTP Time");
  display.setCursor(0, displayLine2);
  display.print(myVersion);
  display.display();
}
/***************************************************************************/
void initDisplay()
/***************************************************************************/
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }
  display.setTextSize(1);  // 21 characters per line for size 1.
  display.setTextColor(WHITE);  // Should be the default.  Isn't!
}
/***************************************************************************/
void displayW_Header(int x, int y, String what)
/***************************************************************************/
{
  printDisplayHeader();
  display.setCursor(x, y); display.print(what);
  display.display();
}
/***************************************************************************/
void fetchOffsets()
/***************************************************************************/
{
  Serial.println("Fetching new zone offsets.");
  setenv("TZ", BeniciaTZ, 1); tzset();
  Serial.println(localtime(&now), "Benicia initial set %a, %d-%m-%Y %T %z %Z");
  time(&now);
  iYear = 0;
  while (iYear < 2023) {
    delay(1000);
    time(&now);
    strftime (buffer, 80, "%Y", localtime(&now));
    iYear = atoi(buffer);
  }
  Serial.println(localtime(&now), "Benicia waiting %a, %d-%m-%Y %T %Z %z %Z");
  strftime (buffer, 80, "%z", localtime(&now));
  strftime (BCDST, 10, "%Z", localtime(&now));
  tempOffset = atoi(buffer);
  BCOffset = (tempOffset / 100) * 3600 + tempOffset % 100 * 60;
  Serial.printf("BC Offset = %i\r\n", BCOffset);

  setenv("TZ", BanguiTZ, 1); tzset();
  Serial.println(localtime(&now), "Bangui initial set %a, %d-%m-%Y %T %z %Z");
  time(&now);
  iYear = 0;
  while (iYear < 2023) {
    delay(1000);
    time(&now);
    strftime (buffer, 80, "%Y", localtime(&now));
    iYear = atoi(buffer);
  }
  Serial.println(localtime(&now), "Bangui waiting  %a, %d-%m-%Y %T %z %Z");
  strftime (buffer, 80, "%z", localtime(&now));
  strftime (RPDST, 10, "%Z", localtime(&now));
  tempOffset = atoi(buffer);
  RPOffset = (tempOffset / 100) * 3600 + tempOffset % 100 * 60;
  Serial.printf("RP Offset = %i\r\n", RPOffset);

  setenv("TZ", Zulu, 1); tzset();
  Serial.println(localtime(&now), "Zulu initial set %a, %d-%m-%Y %T %z %Z");
  updateDisplay();

  iYear = 0;
  while (iYear < 2023) {
    delay(1000);
    time(&now);
    strftime (buffer, 80, "%Y", localtime(&now));
    iYear = atoi(buffer);
  }
  Serial.println(localtime(&now), "Zulu waiting  %a, %d-%m-%Y %T %z %Z");
}
/***************************************************************************/
void  startWifi()
/***************************************************************************/
{
  WiFi.begin(ssid, wifipw);
  Serial.println("Connecting Wifi");
  int waitCt = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    if (waitCt++ > 20) ESP.restart();
  }
  Serial.println("Connected!");
  Serial.print("Host:\t");       Serial.println(WiFi.SSID());
  Serial.print("IP Address:\t"); Serial.println(WiFi.localIP());
  Serial.print("Wifi RSSI =\t"); Serial.println(WiFi.RSSI());
  String myMACAddress = WiFi.macAddress();
  Serial.print("MAC Address =\t"); Serial.println(myMACAddress);
  //           1111111
  // 01234567890123456
  // 84:CC:A8:47:53:98
  String subS = myMACAddress.substring(   0,  2) + myMACAddress.substring(3,  5)
                + myMACAddress.substring( 6,  8) + myMACAddress.substring(9, 11)
                + myMACAddress.substring(12, 14) + myMACAddress.substring(15);
  Serial.print("Scrubbed MAC:\t");
  Serial.println(subS);  // String of MAC address without the ":" characters.
}
#endif
