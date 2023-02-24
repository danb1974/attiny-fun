#### I'm a tiny attiny

Nothing is ready yet...

---

#### 7-segment clock

As a first experiment, we'll put to work a noname oled display (128x64, SH1106-based, I2C bus) which has the first two rows damaged. Wish I knew beforehand how fragile these things are (don't even stare at the two glass corners opposed to the connector or they will break).

Because I want it to look as a clock, we'll draw 7-segment digits.

Because the thing has almost no memory, we cannot use a graphics library like u8g2 to draw pixels and lines, so we'll stick to u8x8 with a custom font (welcome to fony and bdfconv).

For SH1106 there are graphics libraries that do not use a frame buffer (since the chip allows reads) but the one I tried draws artefacts on my display.

Because we need the dots between hour and minute digits, we cannot use the same digits for hours and minutes, they will have different horizontal starting positions.
