require('shelljs/global');
var fs = require('fs');
var _ = require('lodash');
var osc = require('node-osc');
var readDir = require('readdir');
var client = new osc.Client('127.0.0.1', 12345);

var config = JSON.parse(fs.readFileSync('./config.json', 'utf-8'));
var layers = [];

_.each(config, (layerPath, i) => {
    layers.push({
        timer: 0,
        videoIndex: 0,
        oscAddress: `video${i}`,
        raw: readDir.readSync(layerPath, ['**.AVI', '**.avi', '**.mp4'], readDir.ABSOLUTE_PATHS),
        probbed: []
    });
});

_.each(layers, (layer) => {
    _.each(layer.raw, (rawPath) => {
        var o = JSON.parse(exec(`ffprobe -v quiet -of json -show_format -show_streams -count_frames -loglevel fatal -i ${rawPath}`).output);
        layer.probbed.push({ path: rawPath, duration: o.format.duration });
    });
});

setInterval(() => {
    _.each(layers, (layer) => {
        var currentVideo = layer.probbed[layer.videoIndex];
        if (layer.timer > currentVideo.duration) {
            layer.videoIndex++;
            if (layer.videoIndex > layer.probbed.length - 1) {
                layer.videoIndex = 0;
            }
            currentVideo = layer.probbed[layer.videoIndex];
            sendVideo(layer.oscAddress, currentVideo.path);
            layer.timer = 0;
        }
        layer.timer++;
    });
}, 1000);


function sendVideo(address, p) {
	console.log(p);
    client.send(`/${address}`, p, function() {});
}

_.each(layers, (layer) => {
    var currentVideo = layer.probbed[layer.videoIndex];
    sendVideo(layer.oscAddress, currentVideo.path);
});
