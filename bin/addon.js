const process = require('process');
module.exports = require(`./ml64fx-${process.platform}-${process.arch}.node`);
