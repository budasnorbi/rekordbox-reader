export enum ChangeIndexes {
  d1ChannelFader = 0,
  d1SongId = 1,
  d1CurrentTime = 2,
  d2ChannelFader = 3,
  d2SongId = 4,
  d2CurrentTime = 5,
}

export type Changes = [
  d1ChannelFader: number,
  d1SongId: number,
  d1CurrentTime: number,

  d2ChannelFader: number,
  d2SongId: number,
  d2CurrentTime: number
];

export interface djmdContent {
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

declare module "rekordbox-reader" {
  export function trackChanges(callback: (changes: Changes) => void): void;
  export function getSongByID(songId: number | string): Promise<djmdContent>;
}
