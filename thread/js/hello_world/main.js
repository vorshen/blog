const taskArray = [];
for (let i = 0; i < 5; i++) {
    taskArray.push(new Promise((resolve, reject) => {
        const worker = new Worker('./thread.js');

        worker.onmessage = (e) => {
            resolve();
            worker.terminate();
        };
        worker.postMessage(i * 10);
    }));
}

Promise.all(taskArray).then(() => {
    console.log('Thread End');
}).catch(() => {
    console.error('Thread Error');
});
