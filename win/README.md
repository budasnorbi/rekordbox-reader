# Rekordbox reader for Windows
**Reads rekordbox decks values from memory**

**Supported rekordbox versions (64bit):**
+ 6.6.8

**Currently the following values readed from rekordbox's memory per deck:**
+ Channel Fader
+ CFX knob
+ Low filter knob
+ Mid filter knob
+ High filter knob
+ Trim filter knob
+ Tempo
+ Song id
+ Song is playing
+ Current time

**Following things sended through udp socket on the default port 54000:**
+ Channel Fader
+ CFX knob
+ Low filter knob
+ Mid filter knob
+ High filter knob
+ Trim filter knob
+ Tempo
+ Current time

**Following things sended through tcp socket on the default port 54001:**
+ Song id
+ Song is playing