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

interface djmdContentRow {
  ID: string;
  ContentID: string;
  Cues: string;
  rb_cue_count: number;
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

declare module "rekordbox-reader" {
  export function trackChanges(callback: (changes: Changes) => void): void;
  export function getSongByID(songId: number | string): Promise<any>;
}
