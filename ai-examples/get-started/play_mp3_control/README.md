# Play mp3 file with playback control

Functionality extension of [play_mp3](../play_mp3) example by adding possibility to start, stop, pause and resume playback, as well as adjust volume. 

The playback control is done using ESP32 touch pad functionality.

## Usage

:

- Connect speakers or headphones to the board. 

Configure the example:

- Select compatible audio board in `menuconfig` > `Audio HAL`.

Load and run the example. Use buttons to control how audio is played:

- Use [key4] to start, pause and resume playback.
- Adjust sound volume with [Key6] or [Key5].
- To stop the pipeline press [Key3].
