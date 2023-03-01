interface Changes {
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
  d1CalculatedFristBeat: number;
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
  d2CalculatedFristBeat: number;
  crossfader: number;
}

declare module "rekordbox-reader" {
  export function trackChanges(callback: (changes: Changes) => void): void;
}
