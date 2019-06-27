# Introduction
Chip-8 is an interpreter designed by Joseph Weisbecker in the late 1970s for use on the RCA COSMAC VIP computer to program simple games.

<table>
<tr>
  <td><img src="https://media.giphy.com/media/j2Gfd6Dpa7Oos7gTah/giphy.gif" width=256px></td>
  <td><img src="https://media.giphy.com/media/IdUzWuiM2gbmNdlkwH/giphy.gif" width=256px></td>
  <td><img src="https://media.giphy.com/media/LkwObUfvuuPhsXaxvt/giphy.gif" width=256px></td>
</tr>
</table>

# Dependencies
The emulator requires the SDL2 library to be installed on your machine.

# Build
To launch the emulator, execute the below command from the `src` directory:

`make && ./chip8 path_to_rom [clock_speed]`

Public domain ROMs are included in the `roms` directory. The clock speed is an optional argument and defaults to 500 Hz.

# Controls
The hex keypad maps to the standard keyboard as per below:

<table>
<tr><th colspan=4>Hex Keypad</th><td></td><th colspan=4>Keyboard</th></tr>
<tr><td><kbd>1</kbd></td><td><kbd>2</kbd></td><td><kbd>3</kbd></td><td><kbd>C</kbd></td><td></td><td><kbd>1</kbd></td><td><kbd>2</kbd></td> <td><kbd>3</kbd></td><td><kbd>4</kbd></td></tr>
<tr><td><kbd>4</kbd></td><td><kbd>5</kbd></td><td><kbd>6</kbd></td><td><kbd>D</kbd></td><td></td><td><kbd>Q</kbd></td><td><kbd>W</kbd></td> <td><kbd>E</kbd></td><td><kbd>R</kbd></td></tr>
<tr><td><kbd>7</kbd></td><td><kbd>8</kbd></td><td><kbd>9</kbd></td><td><kbd>E</kbd></td><td></td><td><kbd>A</kbd></td><td><kbd>S</kbd></td> <td><kbd>D</kbd></td><td><kbd>F</kbd></td></tr>
<tr><td><kbd>A</kbd></td><td><kbd>0</kbd></td><td><kbd>B</kbd></td><td><kbd>F</kbd></td><td></td><td><kbd>Z</kbd></td><td><kbd>X</kbd></td> <td><kbd>C</kbd></td><td><kbd>V</kbd></td></tr>
</table>

# Implementation Notes
 - The emulator does not currently support sound.

# References
 - [Cowgod's Chip-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
 - [Emulator 101 - CHIP-8 Emulator](http://www.emulator101.com/chip-8-emulator.html)
 - [How to write an emulator by Laurence Muller](http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/)
 - [Mastering CHIP-8 by Matthew Mikolay](http://mattmik.com/files/chip8/mastering/chip8.html)

