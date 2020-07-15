const main = document.getElementById('main');
const thread = document.getElementById('thread');

main.onclick = function() {
    stop();
    console.log('单线程结束卡顿');
};

thread.onclick = function() {
    // 增加个条件，别多点
    if (this.dataset.running) {
        return;
    }

    this.dataset.running = true;

    const worker = new Worker('./thread.js');
    
    worker.onmessage = (e) => {
        this.dataset.running = false;
        worker.terminate();
        console.log('多线程结束卡顿');
    };
    worker.postMessage('AlloyTeam');
};

function stop() {
    const a = Date.now();
    while (Date.now() - a < 5000) {}
}