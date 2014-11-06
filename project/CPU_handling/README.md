## Part 1 : Measurement Overhead
We use RDTSC and RDTSCP as described in the [Intel Benchmarking
white paper](http://www.intel.com/content/dam/www/public/us/en/documents/white-papers/ia-32-ia-64-benchmark-code-execution-paper.pdf) in order to measure the overhead of timing.

## Part 2 : Loop Overhead
We use RDTSC and RDTSCP as described in the [Intel Benchmarking
white paper](http://www.intel.com/content/dam/www/public/us/en/documents/white-papers/ia-32-ia-64-benchmark-code-execution-paper.pdf) in order to measure the overhead of a loop.

## Part 3 :

## Part 4 : Task Creation Time.
Here we are using the `getnstimeofday(timespec* time)` function because we have evidence of its use by the Linux performance benchmarking community and has an equivalent in user space,  `int clock_gettime(clockid_t clk_id, struct timespec *tp)`.

## Part 5 : Context Switch Time
