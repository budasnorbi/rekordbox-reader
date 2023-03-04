# Rekordbox reader for Windows

**Reads rekordbox decks values from memory**
The project goal is to solve those ones problems, who bought a beginner pionner's dj controller like the DDJ-FLX4 and still want to get the time codes of the current played song like on the expensive ones or track more actions like changing the crossfader or the deck knobs.
This repository contains a dll project and a node package project.

#### It currently support rekordbox 6.6.8

## How it works

In the dll file there is a memory reading function that read the rekordbox process based on the fetched cheat engine offsets.

## Installation

> To use this package you should install yalc(https://github.com/wclr/yalc) to be able to use this node addon in other projects.
> The package is only contains one function which is called trackChanges.

```
yalc publish // use this command in the node-addon folder to publish the package locally on your system
yalc add rekordbox-reader // use this command in your target project directory
```

## Usage

> It has built in typescript support, so you can see what properties is avaible in the changes object.

```js
const { trackChanges } = require("rekordbox-reader");

trackChanges((changes) => {
  console.log(changes);
});
```

The first time when the callback is fired it gives back all the changed properties:

```js
{
  d1ChannelFader: 0.8250015259720442,
  d1CfxKnob: 0.500030517578125,
  d1LowFilter: 0.5000305194408838,
  d1MidFilter: 0.5000305194408838,
  d1HighFilter: 0.5000305194408838,
  d1TrimKnob: 0.4717084783006775,
  d1SongId: 2645840387,
  d1CurrentTime: 0.04598639455782313,
  d1CalculatedTempo: 124,
  d1CalculatedFristBeat: 0.046,
  d2ChannelFader: 1,
  d2CfxKnob: 0.500030517578125,
  d2LowFilter: 0.5000305194408838,
  d2MidFilter: 0.5000305194408838,
  d2HighFilter: 0.5000305194408838,
  d2TrimKnob: 0.5005188304950253,
  d2SongId: 2796361984,
  d2CurrentTime: 0.6586621315192743,
  d2CalculatedTempo: 128,
  d2CalculatedFristBeat: 0.12,
  crossfader: 0.7423548800585973
}
```

But after that, only those properties will be in the changes object that are changed.

```js
{ d1IsPlaying: true, d1CurrentTime: 0.056643990929705214 }
```

## Features

#### getSongByID

| It retrieves the song details from rekordbox internal database

```js
const { trackChanges, getSongByID } = require("rekordbox-reader");

trackChanges((changes) => {
  if (changes.d1SongId) {
    getSongByID(changes.d1SongId).then((data) => {
      console.log(data);
    });
  }
});
```
