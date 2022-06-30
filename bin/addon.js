const os = require('os');
module.exports = require(`./addon-${os.platform()}-${os.arch()}.node`);
