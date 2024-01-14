const glob = require("glob");
const { exec } = require("child_process");
const fs = require("fs");

glob("{./RenderEngine,./RenderLib}/**/*.{h,cpp}", (err, files) => {
  if (err) {
    console.error(err);
    return;
  }

  // run clang-format on each file
  files.forEach((file) => {
    // Read the file before formatting
    fs.readFile(file, "utf8", (err, originalData) => {
      if (err) {
        console.error(`Error reading file ${file}: ${err}`);
        return;
      }

      exec(`clang-format -i ${file}`, (err, stdout, stderr) => {
        if (err) {
          console.error(`Error formatting file ${file}: ${err}`);
          return;
        }

        // Read the file after formatting
        fs.readFile(file, "utf8", (err, formattedData) => {
          if (err) {
            console.error(`Error reading file ${file}: ${err}`);
            return;
          }

          // Check if the file content has been changed
          if (originalData !== formattedData) {
            console.log(`Changes made to ${file}`);
          } else {
            console.log(`No changes made to ${file}`);
          }
        });
      });
    });
  });
});
