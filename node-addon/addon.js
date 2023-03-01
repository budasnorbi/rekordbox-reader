const runForever = require("./build/Release/callback_addon");

runForever((data) => {
  console.clear();
  console.log(data);
});
