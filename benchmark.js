function runBenchmark() {
    const data = [];
    for (let i = 0; i < 12000000; i++) {
        data.push({ id: i, value: `item-${i}` });
    }
}

runBenchmark();
