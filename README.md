#### I'm a tiny attiny

Lying in a drawer

Waiting for my day to come

---

#### 7-segment clock

As a first experiment, we'll put to work a noname oled display (128x64, SH1106-based, I2C bus) which has the first two rows damaged. Wish I knew beforehand how fragile these things are (don't even stare at the two glass corners opposed to the connector or they will break).

Quite a rollercoaster - TinyWireM not working well, U8x8 consumes too much memory and has it's own wire code... so, wrote another version that draws pixels and lines and has a minimal oled driver - [attiny85-sh1106-clock-using-drawline](attiny85-sh1106-clock-using-drawline)

![](images/IMG_20230227_183553.jpg?raw=true)
