#if defined ORIGINAL
// The full program was developed from this idea from Dave.
// See https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
//  for Timezone codes for your region
// See http://www.cplusplus.com/reference/ctime/strftime/ for strftime functions

#include <WiFi.h>
char time_output[30];
time_t now;
// Oops, we did not include time.h and NTPClient.h.  Whatever will we do?!?!?!
/***************************************************************************/
void setup() 
/***************************************************************************/
{
  Serial.begin(115200); delay(1000);
// >>>>>>>>>>>============= Things to change =============<<<<<<<<<<<
  WiFi.begin("LallaveWifi", "Lallave@Family7");
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  setenv("TZ", "PST8PDT,M3.2.0,M11.1.0", 1); tzset();
  setSyncInterval(86413000);

// >>>>>>>>>>>=============  End of changes  =============<<<<<<<<<<<

  time(&now);
  Serial.println(localtime(&now), "%a %d-%m-%y %T %z");
}
/***************************************************************************/
void loop() 
/***************************************************************************/
{
  //  Serial.println(get_time());
  // ...or...  (leaves out the buffer usage, the strftime call and the 
  //            subroutine call overhead.
  time(&now);
  Serial.println(localtime(&now), "%a %d-%m-%y %T");
  delay(1000);
}

//String get_time() {
//  time(&now);
//  strftime(time_output, 30, "%a %d-%m-%y %T", localtime(&now));
//  return String(time_output); // returns Sat 20-Apr-19 12:31:45
//}
#endif
