# non-blocking-cpp
Microbenchmarking of non-blocking concurrent hash tables and queues.
The experiments where executed in Ubuntu 20.04 64 bits. To make sure you have all dependencies needed, run:
```
apt install make cmake libtbb-dev libboost-all-dev git -y
```
Also, download and install LibCDS 2.3.3 from [here](https://github.com/khizmax/libcds.git).

To compile and run the experiments, execute ./queue.sh and ./hashmap.sh.
To plot the results, run ./plot.sh

