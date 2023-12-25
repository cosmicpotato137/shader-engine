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
    exec(`clang-format -i ${file}`, (err, stdout, stderr) => {
      if (err) {
        console.error(`Error formatting file ${file}: ${err}`);
        return;
      }

      // Read the file
      fs.readFile(file, "utf8", (err, data) => {
        if (err) {
          console.error(`Error reading file ${file}: ${err}`);
          return;
        }

        // Replace comments that start with a lowercase letter
        const newData = data.replace(/\/\/ [a-z]/g, (match) =>
          match.toUpperCase()
        );

        // Check if the file content has been changed
        if (newData === data) {
          console.log(`No changes made to ${file}`);
        } else {
          // Write the new data back to the file
          fs.writeFile(file, newData, "utf8", (err) => {
            if (err) {
              console.error(`Error writing to file ${file}: ${err}`);
            } else {
              console.log(
                `Successfully formatted and updated comments in ${file}`
              );
            }
          });
        }
      });
    });
  });
});
