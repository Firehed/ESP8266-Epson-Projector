# ESP8266-Epson-Projector
Control an Epson projector over wifi with an ESP8266

## Goals

Expose basic RS-232 control commands for the projector over HTTP.

## Configuration

* Copy `wifi.example.h` to `wifi.h`
* Fill in the values to connect to your wifi network

## Usage

### `GET /`

Parameters: none

Response: `text/plain`

Example:

`curl epson-projector.local`

```
OKa
```

Emits the raw output from the "power state" command

Per the manual:
```
a: power state
  0 = off
  1 = on
```
### `POST /power`

Parameters:

* `state`: string "on" or "off"

Response: `text/plain`

Example:

`curl -d state=on epson-projector.local`

```
P
```

Emits the raw output from the power on/off command.
`P` is "pass" and `F` is "fail".

## Wiring

This was built and tested using an ESP-12e dev board and a [MAX3232 UART-to-RS232 adapter](https://www.amazon.com/gp/product/B00OPU2QJ4).
This adapter (or an equivalent) is NOT OPTIONAL, much to my dismay.
Connect as follows:

| Adapter | ESP8266 |
|---------|---------|
| GND     | G       |
| OUT     | RX      |
| IN      | TX      |
| VCC     | 3v      |

Note that OUT refers to the arrow pointing *away* from the chip, and IN refers to the arrow pointing *towards* it.
The four bottom-most pins on the right side (top view) on the dev board should be the ones you want.

## Notes
In the Arduino IDE, I have selected a NodeMCU ESP-12e board, 115200 baud connection.
It would always error out during uploading unless the wiring was disconnected.
No idea if that's normal or something specific to serial output, but disconnecting the wires fixed it.
Pressing the flash button before/during programming was not necessary with the wiring disconnected, and only worked about 2% of the time with it connected.
Follow [this guide](https://learn.adafruit.com/adafruit-huzzah-esp8266-breakout/using-arduino-ide) to set up Arduino IDE to speak to the ESP8266 boards (but select NodeMCU instead of the Adafruit board).


Upon setup, it will attempt to broadcast itself at `epson-projector.local`.
Depending on your network and DNS configuration, this may not resolve.
To find the device's IP, the easiest approach is to probably just check your router's DHCP leases.
It should print to the serial console as well, but that will be inaccessible when connected to the switch (so make note of it when programming via the Arduino IDE).

There is a lot of room for improvement, which is welcome.

There is ABSOLUTELY NO SECURITY here - this is a plaintext HTTP server with no authentication.
As such, it is very strongly advised that you do not expose it beyond your local network.
Typically this is automatic in a home network with most routers, but it is worth confirming (especially if your network supports IPv6).

## License
MIT
