const getAppDataPath = require("appdata-path");
const sqlite3 = require("@journeyapps/sqlcipher").verbose();
const masterDBPath = `${getAppDataPath()}\\Pioneer\\rekordbox\\master.db`;
const decryptKey =
  "402fd482c38817c35ffa8ffb8c7d93143b749e7d315df7a81732a1ff43608497";

async function getSongByID(songId) {
  return new Promise((res, rej) => {
    const db = new sqlite3.Database(masterDBPath, (error) => {
      if (error) {
        rej(error);
      }
    });

    db.serialize(function () {
      db.run("PRAGMA cipher_compatibility = 4", (result, error) => {
        if (error) {
          rej(error);
        }
      });

      db.run(`PRAGMA key = '${decryptKey}'`, (result, error) => {
        if (error) {
          rej(error);
        }
      });

      db.get(
        `SELECT * FROM djmdContent WHERE ID = '${songId}'`,
        function (err, row) {
          if (err) {
            rej(err);
          }
          if (row) {
            res(row);
          } else {
            res(null);
          }
        }
      );
    });

    db.close((error) => {
      if (error) {
        rej(error);
      }
    });
  });
}

module.exports = {
  trackChanges: require("./build/Release/addon"),
  getSongByID,
};
