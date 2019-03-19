# Esp8266EdgeSoftwareSerial
Software serial for the ESP8266 that uses edge triggered interrupts to improve performance.
Incoming characters do not block programs for the time it takes them to arrive, leading to signficant performance improvements when used on serial data ports running at speeds of 9600 baud or less. Note that the slower the data rate, the more useful this driver becomes. For high speeds it doesn't confer as much of an advantage. 


You can drop the files into your Arduino sketch folder and use them as straight replacements for the SoftwareSerial library files. Simply replace the < and > characters with double quote characters in the include statement to use the new library:

```c++
#include <SoftwareSerial.h>
```
with 

```c++
#include "SoftwareSerial.h"
```

To use the edge triggered operation add an extra parameter to the constructor for your software serial object:


```c++
SoftwareSerial gpsSerial = new  SoftwareSerial(12, -1, false, 128, true);
```

The above statement creates a gps receiver that listens on pin 12 but does not transmit anything (that's what the -1 means in the constructor).
The data is not inverted (that's what false means) and it is using a 128 byte buffer for incoming characters. 

The final parameter (which is true in the above code) selects edge operation. 

If you leave the final parameter off your constructor the SoftwareSerial library works in exaclty the same way as the original library. In other words it defaults to non-edge operation.

# Late charcters

There is one issue with this implmenentation. It is not guaranteed that the last character of a sequence of characters will be detected when it arrives. This happens because the driver needs to detect a signal change to detect data and some values don't have a signal change at their end. 

The "late" character will be registered when the next character arrives. This is not a problem for devices such as GPS sensors and Air Quality sensors as these transmit data continuously, but it would be an issue if you used this mechanism on a user terminal connection. Having said that, you would not need to use edge triggering on such an interface, as your user will not type so quickly and continuously as to cause a problem. 

Rob Miles
