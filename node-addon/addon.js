const { runForever } = require("./build/Release/callback_addon");

runForever((data) => {
  console.log(data);
});
