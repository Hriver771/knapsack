# knapsack
Algorithm Analysis Homework 3
Due by 4/26(Fri.)
You are to write a program to solve 0-1 knapsack problem with following approach. (100 points)
a. Greedy
b. Dynamic Programming c. Branch and bound
As you know, Greedy approach does not provide 0-1 knapsack problem solution. Thus, assume it is fractional knapsack problem when you implement greedy approach.
Generate benefit value and weight – that are all integer - using random number generator function. The range of benefit value is between $1 and $300 and that of weight is between 1 and 100. And maximum capacity W is
W = (number of items) X 40
If CPU time exceeds approximately 15 minutes for specific input size, stop running the program. Therefore, you should check if the running time is over 15 minutes occasionally and go to next case if time is over.
For an output, build a table as follows.

1) Write program in C, not C++ nor Java.
2) If the program does not compile, you will get no point. Make sure
that your program runs in ‘gcc 6.3.0’.
3) Submit one program only, not multiple programs.
4) Output your result on file named ‘output.txt’.
5) Follow the output format suggested above.
6) Total running time of program should be within approximately one
hour (no more than 75 minutes). If your program does not produce any output in time, you will get no point.
7) Note that unit is mili-seconds, not seconds. Calculate down to three places of decimals, i.e. round your answer to the nearest thousandth.
8) At the first part of program, include comment about which algorithms were implemented. For ex) if your program provide solution only for greedy and DP, include comment such as “my program works for greedy and DP solution only.”
