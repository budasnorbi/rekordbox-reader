const getPhrase = (file, base) => {
  return {
    index: file.readIntBE(base, 2),
    beat: file.readIntBE(base + 2, 2),
    kind: file.readIntBE(base + 4, 2),
    //empty_: file.readIntBE(base + 6, 1),
    k1: file.readIntBE(base + 7, 1),
    //empty_1: file.readIntBE(base + 8, 1),
    k2: file.readIntBE(base + 9, 1),
    //empty_2: file.readIntBE(base + 10, 1),
    b: file.readIntBE(base + 11, 1),
    beat2: file.readIntBE(base + 12, 2),
    beat3: file.readIntBE(base + 14, 2),
    beat4: file.readIntBE(base + 16, 2),
    //empty_3: file.readIntBE(base + 18, 1),
    k3: file.readIntBE(base + 19, 1),
    //empty_4: file.readIntBE(base + 20, 1),
    fill: file.readIntBE(base + 21, 1),
    beatFill: file.readIntBE(base + 22, 2),
  };
};

const getPhraseDetails = (mood, phrase) => {
  const { kind, k1, k2, k3, beat } = phrase;

  const label = { name: "", beat, barStart: Math.floor(beat / 4) };

  if (mood === 1) {
    if (kind === 1) {
      if (k1 === 1 && k2 === 0 && k3 === 0) {
        label.name = "Intro 1";
      }
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Intro 2";
      }
    }

    if (kind === 2) {
      if (
        (k1 === 0 && k2 === 0 && k3 === 1) ||
        (k1 === 0 && k2 === 0 && k3 === 0)
      ) {
        label.name = "Up 1";
      }

      const { k1: prevK1, k2: prevK2, k3: prevK3 } = phrases[index - 1];
      if (
        k1 === 0 &&
        k2 === 0 &&
        k3 === 1 &&
        k1 === prevK1 &&
        k2 === prevK2 &&
        k3 === prevK3
      ) {
        label.name = "Up 2";
      }

      if (
        (k1 === 0 && k2 === 1 && k3 === 1) ||
        (k1 === 0 && k2 === 1 && k3 === 0)
      ) {
        label.name = "Up 3";
      }
    }
    if (kind === 3) {
      if (
        (k1 === 0 && k2 === 0 && k3 === 1) ||
        (k1 === 0 && k2 === 0 && k3 === 0)
      ) {
        label.name = "Down";
      }
    }
    if (kind === 5) {
      if (
        (k1 === 1 && k2 === 0 && k3 === 1) ||
        (k1 === 1 && k2 === 0 && k3 === 0)
      ) {
        label.name = "Chorus 1";
      }
      if (
        (k1 === 0 && k2 === 0 && k3 === 1) ||
        (k1 === 0 && k2 === 0 && k3 === 0)
      ) {
        label.name = "Chorus 2";
      }
    }
    if (kind === 6) {
      if (
        (k1 === 1 && k2 === 0 && k3 === 1) ||
        (k1 === 1 && k2 === 0 && k3 === 0)
      ) {
        label.name = "Outro 1";
      }
      if (
        (k1 === 0 && k2 === 0 && k3 === 1) ||
        (k1 === 0 && k2 === 0 && k3 === 0)
      ) {
        label.name = "Outro 2";
      }
    }
  }
  if (mood === 2) {
    if (kind === 1) {
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Intro";
      }
    }
    if (kind === 2) {
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Verse 1";
      }
    }
    if (kind === 3) {
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Verse 2";
      }
    }
    if (kind === 4) {
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Verse 3";
      }
    }
    if (kind === 5) {
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Verse 4";
      }
    }
    if (kind === 6) {
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Verse 5";
      }
    }
    if (kind === 7) {
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Verse 6";
      }
    }
    if (kind === 8) {
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Bridge";
      }
    }
    if (kind === 9) {
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Chorus";
      }
    }
    if (kind === 10) {
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Outro";
      }
    }
  }
  if (mood === 3) {
    if (kind === 1) {
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Intro";
      }
    }
    if (kind === 2) {
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Verse 1";
      }
    }
    if (kind === 3) {
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Verse 1";
      }
    }
    if (kind === 4) {
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Verse 1";
      }
    }
    if (kind === 5) {
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Verse 2";
      }
    }
    if (kind === 6) {
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Verse 2";
      }
    }
    if (kind === 7) {
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Verse 1";
      }
    }
    if (kind === 8) {
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Bridge";
      }
    }
    if (kind === 9) {
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Chrorus";
      }
    }
    if (kind === 10) {
      if (k1 === 0 && k2 === 0 && k3 === 0) {
        label.name = "Outro";
      }
    }
  }

  return label;
};

export const phraseParser = (file) => {
  const PSSI_index = file.indexOf("PSSI");
  const len_header = file.readIntBE(PSSI_index + 4, 4);
  //const len_tag = file.readIntBE(PSSI_index + 8, 4);
  const len_entry_bytes = file.readIntBE(PSSI_index + 12, 4);
  const len_entries = file.readIntBE(PSSI_index + 16, 2);
  const mood = file.readIntBE(PSSI_index + 18, 2);
  //const unk = file.readIntBE(PSSI_index + 20, 6);
  //const end = file.readIntBE(PSSI_index + 26, 2);
  //const unk2 = file.readIntBE(PSSI_index + 28, 2);
  //const bank = file.readIntBE(PSSI_index + 30, 1);
  //const unk3 = file.readIntBE(PSSI_index + 31, 1);

  const labels = [];
  const phrasesEntriesStart = PSSI_index + len_header;
  for (let i = 0; i < len_entries; i++) {
    const phrase = getPhrase(file, phrasesEntriesStart + i * len_entry_bytes);
    const label = getPhraseDetails(mood, phrase);
    labels.push(label);
  }

  return labels;
};
