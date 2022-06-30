const os = require('os');
module.exports = require(`./ml64fx-${os.platform()}-${os.arch()}.node`);
