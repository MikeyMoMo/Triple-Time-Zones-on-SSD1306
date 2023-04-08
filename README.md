# Triple-Time-Zones-on-SSD1306
This avoids ezTime and gets 3 times zones information including DST and displays on SSD1306.

You can easily change the time zones used.  They are listed starting at line 14.  Up to 3 are allowed and required.  Just be sure than they are all the same length and the length of the longest character string or there will be a memory leak.  This is a wel known bug that the author of the library has chosen to call a feature and leave it in.  Really dumb idea!  But, by having all of the strings the same length overcomes this bug and does not eat up the heap.  DST is handled and the time should be corrected per your local requirement and the suffix on the time will show DST or standard time.

![20230409_004956](https://user-images.githubusercontent.com/15792417/230733560-b31ba985-3dd1-4529-8cd0-fa06ffc90c03.jpg)
