//#define ORIGINAL  // This was the almost original code from Dave.

#if not defined ORIGINAL
// See https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
//  for Timezone codes for your region
// See http://www.cplusplus.com/reference/ctime/strftime/ for strftime functions

#define myVersion "v2.04"

// >>>>>>>>>>>============= Things to change =============<<<<<<<<<<<
// There is a known and, apparently, intentional Heap cancer in the setenv TZ library code.
//  If you use different lengths for these, it does not release the previous memory.  
//  The explanation makes no sense and author will not fix it.  Go figure...
const char * Zulu      = "UTC0                  ";  // The lengths must all be equal to the longest one
const char * BeniciaTZ = "PST8PDT,M3.2.0,M11.1.0";  // The lengths must all be equal to the longest one
const char * BanguiTZ  = "PHT-8                 ";  // The lengths must all be equal to the longest one

const char * ssid      = "LallaveWifi";
const char * wifipw    = "Lallave@Family7";
// >>>>>>>>>>>=============  End of changes  =============<<<<<<<<<<<

// Only update the TZ hourly to get the current offset and DST setting.  
// That's really more than necessary but not such a big deal.  I was doing it every second.
// It totally saves me from having to figure out DST for myself.  I did it once and it was
// NOT pretty!

#include <WiFi.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int displayLine1 =  0;
const int displayLine2 = 16;
const int displayLine3 = 29;
const int displayLine4 = 42;
const int displayLine5 = 56;

time_t now, UTC;
char buffer [80];
struct tm timeinfo;
int iYear, BCOffset, RPOffset, tempOffset;
char BCDST[10];
char RPDST[10];

int  prevHour = -1, prevSecond = -1;
long int firstHeapSize=0, lastHeapSize;
// Oops, we did not include time.h and NTPClient.h.  Whatever will we do?!?!?!
/***************************************************************************/
void setup()
/***************************************************************************/
{
  Serial.begin(115200); delay(1000); Serial.println("Starting");
  Serial.println("Dual NTP Time");
  Serial.println("Running from: ");
  Serial.println(__FILE__);

  initDisplay();

  displayW_Header(0, displayLine3, "Start WiFi");  // Display What after putting up header
  startWifi();

  displayW_Header(0, displayLine3, "Fetch NTP Time");  // Display What after putting up header
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  setenv("TZ", Zulu, 1); tzset();
  Serial.println("Waiting for correct time...");

  strftime (buffer, 80, "%Y", localtime(&now));
  iYear = atoi(buffer);
  int iLooper = 0;
  while (iYear < 2023) {
    time_t UTC; time(&UTC);
    delay(1000);
    strftime (buffer, 80, "%Y", localtime(&UTC));
    iYear = atoi(buffer);
    Serial.println(localtime(&UTC), "UTC %a %m-%d-%Y %T");
    if (iLooper++ > 20) {
      Serial.println("Cannot get time set. Rebooting.");
      ESP.restart();
    }
  }
  strftime (buffer, 80, "%H", localtime(&now));
  if (prevHour != atoi(buffer)) {
    prevHour = atoi(buffer);
    fetchOffsets();
  }
  lastHeapSize = esp_get_free_heap_size();
  firstHeapSize = lastHeapSize;
  Serial.printf("Starting Heap Size: %i\r\n",  lastHeapSize);
}
/***************************************************************************/
void loop()
/***************************************************************************/
{
  time(&UTC);
  strftime (buffer, 80, "%S", localtime(&UTC));
  if (prevSecond == atoi(buffer)) return;
  prevSecond = atoi(buffer);
  if (lastHeapSize > esp_get_free_heap_size()) {
    lastHeapSize = esp_get_free_heap_size();
    Serial.println(localtime(&UTC), "UTC %a %m-%d-%Y %T");
    Serial.printf("New Heap Size: %i\r\n", lastHeapSize);
    if (lastHeapSize < 50000) ESP.restart();
  }
  updateDisplay();
  delay(50);

  //  Serial.println(UTC % 60);  // Current second, worldwide
  //  byte second = UTC % 60; UTC /= 60;
  //  byte minute = UTC % 60; UTC /= 60;
  //  byte hour   = UTC % 24; UTC /= 24;
  //  Serial.printf("% 2i:%02i:%02i\r\n", hour, minute, second);
}
#endif
