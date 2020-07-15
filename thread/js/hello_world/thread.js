function calc(i) {
    if (i < 3) {
		return 1;
	}

	return calc(i-1) + calc(i-2);
}

function fibonacci(i) {
    let result = calc(i.data);

    console.log("Result: %d", result);

    return result;
}

onmessage = function(i) {
    fibonacci(i);
    postMessage(true);
}