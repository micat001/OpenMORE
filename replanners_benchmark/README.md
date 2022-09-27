# **replanners_benchmark**

The package **replanners_benchmark** contains a node to benchmark the available replanners.
It uses the environments defined in [*replanners_cells*](https://github.com/JRL-CARI-CNR-UNIBS/replanning_strategies/tree/master/replanners_cells).
To configure your benchmark, look at this [yaml file]() which contains an explanation of all the parameters needed.

Each individual test is executed in the following way:
- Initially, a planner calculates a path from start to goal.
- The robot starts walking the path and in the process several obstacles appear along the path.
- The replanning framework will need to be able to manage the execution of the path and calculate an alternative path when necessary.

Therefore, a pair of start and end configurations, the number of queries and the number of iterations per query must be defined to run the tests. During testing, the start is moved from the start position to the end position in a number of steps equal to the number of queries. The same is done for the goal. For each query (start and goal pair), multiple tests equal to the number of iterations are performed.

The tests are saved in the *.ros/replanners_benchmark* folder as *.bin* files.
To run benchmarks, the replanner manager runs a dedicated thread ([benchmarkThread](https://github.com/JRL-CARI-CNR-UNIBS/replanning_strategies/blob/master/replanners_lib/include/replanners_lib/replanner_managers/replanner_manager_base.h)) that calculates and saves the following data:
- success (true if the robot moved from start to goal without colliding with any object).
- number of objects generated
- number of objects the robot collided with
- length of the path actually traversed by the robot
- start-goal distance
- length of the nominal path, i.e., the one initially calculated
- time taken to move from start to goal
- average time it took the replanner to find a solution
- standard deviation of the time it took the replanner to find a solution
- maximum time taken by the replanner