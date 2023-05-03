import * as fs from "fs";
import { resolver } from "./resolver.mjs";

const dir = fs.readdirSync(
  "/Users/norbi/Library/Pioneer/rekordbox/share/PIONEER/USBANLZ"
);
for (const folder of dir) {
  if (folder === ".DS_Store") {
    continue;
  }
  const [uuidDir] = fs
    .readdirSync(
      `/Users/norbi/Library/Pioneer/rekordbox/share/PIONEER/USBANLZ/${folder}`
    )
    .filter((f) => f !== ".DS_Store");

  if (!uuidDir) {
    continue;
  }

  const file = fs.readFileSync(
    `/Users/norbi/Library/Pioneer/rekordbox/share/PIONEER/USBANLZ/${folder}/${uuidDir}/ANLZ0000.EXT`
  );
  resolver(file, uuidDir);
}
