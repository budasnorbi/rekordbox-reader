# Rekordbox reader for macOS
**Reads rekordbox decks values from memory**

**Supported rekordbox versions (64bit):**
+ 6.6.8

**The following values readed from rekordbox's memory per deck:**
+ Channel Fader 
+ CFX knob (deck 2 WIP)
+ Low filter knob (deck 2 WIP)
+ Mid filter knob (deck 2 WIP)
+ High filter knob (deck 2 WIP)
+ Trim filter knob (deck 1,2 WIP)
+ Tempo (deck 1,2 WIP)
+ Song id (deck 1,2 WIP)
+ Song is playing (deck 1,2 WIP)
+ Current time (deck 2 WIP)

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