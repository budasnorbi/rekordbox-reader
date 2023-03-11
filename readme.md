# Rekordbox reader for Windows

**Reads rekordbox decks values from memory.**

The project goal is to solve those ones problems, who bought a beginner pionner's dj controller like the DDJ-FLX4 and still want to get the time codes of the current played song like on the expensive ones or track more actions like changing the crossfader the channel faders or the deck knobs.
This repository contains a dll project and a node package project too.

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

```ts
{
  d1ChannelFader: number;
  d1CfxKnob: number;
  d1LowFilter: number;
  d1MidFilter: number;
  d1HighFilter: number;
  d1TrimKnob: number;
  d1Tempo: number;
  d1SongId: number;
  d1IsPlaying: boolean;
  d1CurrentTime: number;
  d1CalculatedTempo: number;
  d1CalculatedFirstBeat: number;
  d2ChannelFader: number;
  d2CfxKnob: number;
  d2LowFilter: number;
  d2MidFilter: number;
  d2HighFilter: number;
  d2TrimKnob: number;
  d2Tempo: number;
  d2SongId: number;
  d2IsPlaying: boolean;
  d2CurrentTime: number;
  d2CalculatedTempo: number;
  d2CalculatedFirstBeat: number;
  crossfader: number;
}
```

But after that, only those properties will be in the changes object that are changed.

```js
{ d1IsPlaying: true, d1CurrentTime: 0.056643990929705214 }
```

## Features

#### getSongByID

> It retrieves the song details from rekordbox internal database

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

The getSongByID is an async function and it resolves with the following interface

```ts
{
  ID: string;
  FolderPath: string;
  FileNameL: string;
  FileNameS: string;
  Title: string;
  ArtistID: string;
  AlbumID: string;
  GenreID: string;
  BPM: number;
  Length: number;
  TrackNo: number;
  BitRate: number;
  BitDepth: number;
  Commnt: string;
  FileType: number;
  Rating: number;
  ReleaseYear: number;
  RemixerID: string;
  LabelID: string;
  OrgArtistID: string;
  KeyID: string;
  StockDate: string;
  ColorID: string;
  DJPlayCount: number;
  ImagePath: string;
  MasterDBID: string;
  MasterSongID: string;
  AnalysisDataPath: string;
  SearchStr: string;
  FileSize: number;
  DiscNo: number;
  ComposerID: string;
  Subtitle: string;
  SampleRate: number;
  DisableQuantize: number;
  Analysed: number;
  ReleaseDate: string;
  DateCreated: string;
  ContentLink: number;
  Tag: string;
  ModifiedByRBM: string;
  HotCueAutoLoad: string;
  DeliveryControl: string;
  DeliveryComment: string;
  CueUpdated: string;
  AnalysisUpdated: string;
  TrackInfoUpdated: string;
  Lyricist: string;
  ISRC: string;
  SamplerTrackInfo: number;
  SamplerPlayOffset: number;
  SamplerGain: number;
  VideoAssociate: string;
  LyricStatus: number;
  ServiceID: number;
  OrgFolderPath: string;
  Reserved1: string;
  Reserved2: string;
  Reserved3: string;
  Reserved4: string;
  ExtInfo: string;
  rb_file_id: string;
  DeviceID: string;
  rb_LocalFolderPath: string;
  SrcID: string;
  SrcTitle: string;
  SrcArtistName: string;
  SrcAlbumName: string;
  SrcLength: number;
  UUID: string;
  rb_data_status: number;
  rb_local_data_status: number;
  rb_local_deleted: number;
  rb_local_synced: number;
  usn: number;
  rb_local_usn: number;
  created_at: number;
  updated_at: number;
}
```

Rekordbox versions that i saved

- 6.6.11(lastest)

  - (win) https://cdn.rekordbox.com/files/20230306171653/Install_rekordbox_x64_6_6_11.zip
  - (mac) https://cdn.rekordbox.com/files/20230306173735/Install_rekordbox_6_6_11.pkg_.zip

- 6.6.10
  - (win) https://cdn.rekordbox.com/files/20230201181231/Install_rekordbox_x64_6_6_10.zip
