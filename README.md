# Thread

This project, developed by Chuanwise, is for the course `Analysing and Application of Linux Kernel Source Codes` in the XiDian University. 

## Build

Run following commands in the repository directory:

```shell
cmake .
make
```

## Test

After build, run following commands to execute the demo test `test_producer_consumer` located in `tests/test_producer_consumer`:

```shell
make test
```

And you'll see outputs:

```
Running tests...
Test project /home/chuanwise/develop/projects/thread
    Start 1: test_producer_consumer
1/1 Test #1: test_producer_consumer ...........   Passed    1.00 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   1.01 sec
```

You can find the corresponding test report in `Testing/Temporary/LastTest.log`:

```
Start testing: Dec 31 16:03 CST
----------------------------------------------------------
1/1 Testing: test_producer_consumer
1/1 Test: test_producer_consumer
Command: "/home/chuanwise/develop/projects/thread/tests/test_producer_consumer"
Directory: /home/chuanwise/develop/projects/thread/tests
"test_producer_consumer" start time: Dec 31 16:03 CST
Output:
----------------------------------------------------------
[M] waiting 
[P] procuding [C] consuming 0
[P] procuding 1
[P] procuding 2
[P] procuding 3
[P] procuding 4
[P] procuding 5
[P] procuding 6
[P] procuding 7
[C] consuming 6
[P] procuding 8
[P] procuding 9
[C] consuming 9
[C] consuming 8
[C] consuming 7
[C] consuming 5
[C] consuming 4
[C] consuming 3
[C] consuming 2
[C] consuming 1
[P] procuding 10
[P] procuding 11
[C] consuming 10
[P] procuding [C
 consuming 12
[P] procuding 13
[C] consuming [P] procuding 14
[C] consuming 14
[P] procuding 15
[C] consuming 15
[P] procuding 16
[C] consuming 16
[P] procuding 17
[C] consuming 17
[P] procuding 18
[C] consuming 18
[P] procuding 19[C] consuming 19
[P] procuding 20
[C] consuming 20
[P] procuding 21
[C] consuming 21
[P] procuding 22
[C] consuming 22
[P] procuding [C] consuming 23
[P] procuding 24
[C] consuming [P] procuding 25
[C] consuming 25
[P] procuding 26
[C] consuming 26
[P] procuding 27
[C] consuming 27
[P] procuding [C] consuming 28
[P] procuding 29
[C] consuming [P] producer exited! 
[C] consuming 11
[C] consumer exited! 
<end of output>
Test time =   1.00 sec
----------------------------------------------------------
Test Passed.
"test_producer_consumer" end time: Dec 31 16:03 CST
"test_producer_consumer" time elapsed: 00:00:01
----------------------------------------------------------

End testing: Dec 31 16:03 CST
```

The problem with the output format is caused by multi-threading calling `printf` without locking. 

However, I don't want to fix it because I'm using a non-blocking queue to improve performance, and `printf` is just used to check if the program is running correctly. If I add locking here just for testing, then I might as well use a lock to implement the queue.

## Todo

1. Remove the signal handler of main thread to prevent automatical calling of `waitpid`. 