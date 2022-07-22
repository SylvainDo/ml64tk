const process = require('process');
module.exports = require(`./ml64tk-${process.platform}-${process.arch}.node`);
