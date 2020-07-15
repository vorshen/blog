function stop() {
    const a = Date.now();
    while (Date.now() - a < 5000) {}
}

onmessage = function() {
    stop();
    postMessage(true);
}