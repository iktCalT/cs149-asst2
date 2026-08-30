# Test results

## MacOS + M5 + Topological sorting

```zsh
python3 ../tests/run_test_harness.py -n 10 -a
runtasks_ref
Darwin arm64
================================================================================
Running task system grading harness... (22 total tests)
  - Detected CPU with 10 execution contexts
  - Task system configured to use at most 10 threads
================================================================================
================================================================================
Executing test: super_super_light...
Reference binary: ./runtasks_ref_osx_arm
Results for: super_super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                3.36      3.362       1.00  (OK)
[Parallel + Always Spawn]               3.357     25.91       0.13  (OK)
[Parallel + Thread Pool + Spin]         3.337     34.446      0.10  (OK)
[Parallel + Thread Pool + Sleep]        12.392    14.356      0.86  (OK)
================================================================================
Executing test: super_super_light_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: super_super_light_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                3.479     3.414       1.02  (OK)
[Parallel + Always Spawn]               3.439     26.423      0.13  (OK)
[Parallel + Thread Pool + Spin]         3.44      25.619      0.13  (OK)
[Parallel + Thread Pool + Sleep]        11.996    15.56       0.77  (OK)
================================================================================
Executing test: super_light...
Reference binary: ./runtasks_ref_osx_arm
Results for: super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                13.555    25.464      0.53  (OK)
[Parallel + Always Spawn]               13.522    34.463      0.39  (OK)
[Parallel + Thread Pool + Spin]         13.452    41.975      0.32  (OK)
[Parallel + Thread Pool + Sleep]        18.956    21.177      0.90  (OK)
================================================================================
Executing test: super_light_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: super_light_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                13.59     18.103      0.75  (OK)
[Parallel + Always Spawn]               13.581    34.86       0.39  (OK)
[Parallel + Thread Pool + Spin]         13.577    33.209      0.41  (OK)
[Parallel + Thread Pool + Sleep]        17.879    16.768      1.07  (OK)
================================================================================
Executing test: ping_pong_equal...
Reference binary: ./runtasks_ref_osx_arm
Results for: ping_pong_equal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                218.287   379.929     0.57  (OK)
[Parallel + Always Spawn]               218.162   86.782      2.51  (NOT OK)
[Parallel + Thread Pool + Spin]         218.096   80.766      2.70  (NOT OK)
[Parallel + Thread Pool + Sleep]        57.398    72.445      0.79  (OK)
================================================================================
Executing test: ping_pong_equal_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: ping_pong_equal_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                218.95    387.267     0.57  (OK)
[Parallel + Always Spawn]               218.923   87.077      2.51  (NOT OK)
[Parallel + Thread Pool + Spin]         218.581   73.129      2.99  (NOT OK)
[Parallel + Thread Pool + Sleep]        57.03     70.237      0.81  (OK)
================================================================================
Executing test: ping_pong_unequal...
Reference binary: ./runtasks_ref_osx_arm
Results for: ping_pong_unequal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                395.077   468.834     0.84  (OK)
[Parallel + Always Spawn]               393.892   113.053     3.48  (NOT OK)
[Parallel + Thread Pool + Spin]         392.981   111.554     3.52  (NOT OK)
[Parallel + Thread Pool + Sleep]        89.739    100.442     0.89  (OK)
================================================================================
Executing test: ping_pong_unequal_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: ping_pong_unequal_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                394.192   468.155     0.84  (OK)
[Parallel + Always Spawn]               394.707   112.452     3.51  (NOT OK)
[Parallel + Thread Pool + Spin]         395.122   102.642     3.85  (NOT OK)
[Parallel + Thread Pool + Sleep]        89.52     99.878      0.90  (OK)
================================================================================
Executing test: recursive_fibonacci...
Reference binary: ./runtasks_ref_osx_arm
Results for: recursive_fibonacci
                                        STUDENT   REFERENCE   PERF?
[Serial]                                823.768   817.296     1.01  (OK)
[Parallel + Always Spawn]               823.686   124.748     6.60  (NOT OK)
[Parallel + Thread Pool + Spin]         822.814   127.943     6.43  (NOT OK)
[Parallel + Thread Pool + Sleep]        123.901   123.859     1.00  (OK)
================================================================================
Executing test: recursive_fibonacci_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: recursive_fibonacci_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                824.366   823.829     1.00  (OK)
[Parallel + Always Spawn]               823.92    125.358     6.57  (NOT OK)
[Parallel + Thread Pool + Spin]         823.525   122.818     6.71  (NOT OK)
[Parallel + Thread Pool + Sleep]        121.559   123.505     0.98  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop
                                        STUDENT   REFERENCE   PERF?
[Serial]                                182.859   184.015     0.99  (OK)
[Parallel + Always Spawn]               182.947   146.843     1.25  (OK)
[Parallel + Thread Pool + Spin]         182.993   159.309     1.15  (OK)
[Parallel + Thread Pool + Sleep]        97.874    97.526      1.00  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                185.826   184.13      1.01  (OK)
[Parallel + Always Spawn]               183.026   149.236     1.23  (OK)
[Parallel + Thread Pool + Spin]         182.922   114.802     1.59  (NOT OK)
[Parallel + Thread Pool + Sleep]        95.782    87.944      1.09  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fewer_tasks...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_fewer_tasks
                                        STUDENT   REFERENCE   PERF?
[Serial]                                184.035   184.868     1.00  (OK)
[Parallel + Always Spawn]               182.957   147.67      1.24  (OK)
[Parallel + Thread Pool + Spin]         183.117   160.607     1.14  (OK)
[Parallel + Thread Pool + Sleep]        99.973    96.027      1.04  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fewer_tasks_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_fewer_tasks_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                194.217   185.73      1.05  (OK)
[Parallel + Always Spawn]               183.614   155.388     1.18  (OK)
[Parallel + Thread Pool + Spin]         183.225   34.769      5.27  (NOT OK)
[Parallel + Thread Pool + Sleep]        32.763    34.007      0.96  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fan_in...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_fan_in
                                        STUDENT   REFERENCE   PERF?
[Serial]                                103.127   103.298     1.00  (OK)
[Parallel + Always Spawn]               94.438    35.528      2.66  (NOT OK)
[Parallel + Thread Pool + Spin]         94.299    36.155      2.61  (NOT OK)
[Parallel + Thread Pool + Sleep]        28.456    30.575      0.93  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fan_in_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_fan_in_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                102.334   103.862     0.99  (OK)
[Parallel + Always Spawn]               94.317    37.318      2.53  (NOT OK)
[Parallel + Thread Pool + Spin]         94.336    19.646      4.80  (NOT OK)
[Parallel + Thread Pool + Sleep]        17.378    18.089      0.96  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_reduction_tree...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_reduction_tree
                                        STUDENT   REFERENCE   PERF?
[Serial]                                102.117   103.259     0.99  (OK)
[Parallel + Always Spawn]               94.227    21.627      4.36  (NOT OK)
[Parallel + Thread Pool + Spin]         93.829    22.064      4.25  (NOT OK)
[Parallel + Thread Pool + Sleep]        19.136    18.856      1.01  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_reduction_tree_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_reduction_tree_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                101.959   102.934     0.99  (OK)
[Parallel + Always Spawn]               93.838    21.375      4.39  (NOT OK)
[Parallel + Thread Pool + Spin]         93.765    16.726      5.61  (NOT OK)
[Parallel + Thread Pool + Sleep]        16.585    16.563      1.00  (OK)
================================================================================
Executing test: spin_between_run_calls...
Reference binary: ./runtasks_ref_osx_arm
Results for: spin_between_run_calls
                                        STUDENT   REFERENCE   PERF?
[Serial]                                292.87    293.02      1.00  (OK)
[Parallel + Always Spawn]               292.923   148.669     1.97  (NOT OK)
[Parallel + Thread Pool + Spin]         292.995   173.846     1.69  (NOT OK)
[Parallel + Thread Pool + Sleep]        148.442   148.322     1.00  (OK)
================================================================================
Executing test: spin_between_run_calls_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: spin_between_run_calls_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                292.654   292.783     1.00  (OK)
[Parallel + Always Spawn]               292.991   148.68      1.97  (NOT OK)
[Parallel + Thread Pool + Spin]         292.748   157.008     1.86  (NOT OK)
[Parallel + Thread Pool + Sleep]        148.463   148.439     1.00  (OK)
================================================================================
Executing test: mandelbrot_chunked...
Reference binary: ./runtasks_ref_osx_arm
Results for: mandelbrot_chunked
                                        STUDENT   REFERENCE   PERF?
[Serial]                                224.841   227.904     0.99  (OK)
[Parallel + Always Spawn]               223.892   32.446      6.90  (NOT OK)
[Parallel + Thread Pool + Spin]         223.822   33.524      6.68  (NOT OK)
[Parallel + Thread Pool + Sleep]        31.973    32.283      0.99  (OK)
================================================================================
Executing test: mandelbrot_chunked_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: mandelbrot_chunked_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                223.884   226.83      0.99  (OK)
[Parallel + Always Spawn]               223.43    32.561      6.86  (NOT OK)
[Parallel + Thread Pool + Spin]         223.13    33.425      6.68  (NOT OK)
[Parallel + Thread Pool + Sleep]        32.097    32.406      0.99  (OK)
================================================================================
Overall performance results
[Serial]                                : All passed Perf
[Parallel + Always Spawn]               : Perf did not pass all tests
[Parallel + Thread Pool + Spin]         : Perf did not pass all tests
[Parallel + Thread Pool + Sleep]        : All passed Perf
```

## Linux + AMD 5700G

```bash
python3 ../tests/run_test_harness.py -n 16 -a
runtasks_ref
Linux x86_64
================================================================================
Running task system grading harness... (22 total tests)
  - Detected CPU with 16 execution contexts
  - Task system configured to use at most 16 threads
================================================================================
================================================================================
Executing test: super_super_light...
Reference binary: ./runtasks_ref_linux
Results for: super_super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                9.226     9.107       1.01  (OK)
[Parallel + Always Spawn]               9.218     480.772     0.02  (OK)
[Parallel + Thread Pool + Spin]         8.983     29.231      0.31  (OK)
[Parallel + Thread Pool + Sleep]        83.457    82.711      1.01  (OK)
================================================================================
Executing test: super_super_light_async...
Reference binary: ./runtasks_ref_linux
Results for: super_super_light_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                9.168     9.206       1.00  (OK)
[Parallel + Always Spawn]               9.236     483.807     0.02  (OK)
[Parallel + Thread Pool + Spin]         9.079     21.532      0.42  (OK)
[Parallel + Thread Pool + Sleep]        82.602    82.967      1.00  (OK)
================================================================================
Executing test: super_light...
Reference binary: ./runtasks_ref_linux
Results for: super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                41.888    56.892      0.74  (OK)
[Parallel + Always Spawn]               41.726    486.404     0.09  (OK)
[Parallel + Thread Pool + Spin]         41.942    27.731      1.51  (NOT OK)
[Parallel + Thread Pool + Sleep]        84.02     85.073      0.99  (OK)
================================================================================
Executing test: super_light_async...
Reference binary: ./runtasks_ref_linux
Results for: super_light_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                42.27     57.205      0.74  (OK)
[Parallel + Always Spawn]               42.031    491.028     0.09  (OK)
[Parallel + Thread Pool + Spin]         41.944    24.101      1.74  (NOT OK)
[Parallel + Thread Pool + Sleep]        82.39     84.857      0.97  (OK)
================================================================================
Executing test: ping_pong_equal...
Reference binary: ./runtasks_ref_linux
Results for: ping_pong_equal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                677.07    920.322     0.74  (OK)
[Parallel + Always Spawn]               677.459   524.132     1.29  (OK)
[Parallel + Thread Pool + Spin]         676.858   194.162     3.49  (NOT OK)
[Parallel + Thread Pool + Sleep]        173.117   211.123     0.82  (OK)
================================================================================
Executing test: ping_pong_equal_async...
Reference binary: ./runtasks_ref_linux
Results for: ping_pong_equal_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                678.074   919.743     0.74  (OK)
[Parallel + Always Spawn]               679.262   526.068     1.29  (OK)
[Parallel + Thread Pool + Spin]         678.476   187.028     3.63  (NOT OK)
[Parallel + Thread Pool + Sleep]        172.822   197.597     0.87  (OK)
================================================================================
Executing test: ping_pong_unequal...
Reference binary: ./runtasks_ref_linux
Results for: ping_pong_unequal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                1274.353  1293.644    0.99  (OK)
[Parallel + Always Spawn]               1274.469  537.199     2.37  (NOT OK)
[Parallel + Thread Pool + Spin]         1274.757  203.837     6.25  (NOT OK)
[Parallel + Thread Pool + Sleep]        233.42    224.406     1.04  (OK)
================================================================================
Executing test: ping_pong_unequal_async...
Reference binary: ./runtasks_ref_linux
Results for: ping_pong_unequal_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                1273.072  1293.957    0.98  (OK)
[Parallel + Always Spawn]               1270.829  539.376     2.36  (NOT OK)
[Parallel + Thread Pool + Spin]         1269.349  197.602     6.42  (NOT OK)
[Parallel + Thread Pool + Sleep]        233.34    209.466     1.11  (OK)
================================================================================
Executing test: recursive_fibonacci...
Reference binary: ./runtasks_ref_linux
Results for: recursive_fibonacci
                                        STUDENT   REFERENCE   PERF?
[Serial]                                727.648   1284.442    0.57  (OK)
[Parallel + Always Spawn]               726.483   163.792     4.44  (NOT OK)
[Parallel + Thread Pool + Spin]         724.546   152.636     4.75  (NOT OK)
[Parallel + Thread Pool + Sleep]        101.466   150.071     0.68  (OK)
================================================================================
Executing test: recursive_fibonacci_async...
Reference binary: ./runtasks_ref_linux
Results for: recursive_fibonacci_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                726.267   1284.75     0.57  (OK)
[Parallel + Always Spawn]               726.822   163.017     4.46  (NOT OK)
[Parallel + Thread Pool + Spin]         724.846   145.276     4.99  (NOT OK)
[Parallel + Thread Pool + Sleep]        95.478    138.576     0.69  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop
                                        STUDENT   REFERENCE   PERF?
[Serial]                                449.608   455.965     0.99  (OK)
[Parallel + Always Spawn]               449.632   2494.585    0.18  (OK)
[Parallel + Thread Pool + Spin]         449.488   177.14      2.54  (NOT OK)
[Parallel + Thread Pool + Sleep]        433.876   435.462     1.00  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_async...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                451.659   460.129     0.98  (OK)
[Parallel + Always Spawn]               450.229   2522.595    0.18  (OK)
[Parallel + Thread Pool + Spin]         448.906   153.612     2.92  (NOT OK)
[Parallel + Thread Pool + Sleep]        450.415   200.604     2.25  (NOT OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fewer_tasks...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop_fewer_tasks
                                        STUDENT   REFERENCE   PERF?
[Serial]                                451.982   460.022     0.98  (OK)
[Parallel + Always Spawn]               451.562   2522.09     0.18  (OK)
[Parallel + Thread Pool + Spin]         450.34    174.579     2.58  (NOT OK)
[Parallel + Thread Pool + Sleep]        429.791   427.786     1.00  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fewer_tasks_async...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop_fewer_tasks_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                450.584   458.068     0.98  (OK)
[Parallel + Always Spawn]               450.348   2508.355    0.18  (OK)
[Parallel + Thread Pool + Spin]         450.679   71.549      6.30  (NOT OK)
[Parallel + Thread Pool + Sleep]        66.16     424.882     0.16  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fan_in...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop_fan_in
                                        STUDENT   REFERENCE   PERF?
[Serial]                                232.757   234.926     0.99  (OK)
[Parallel + Always Spawn]               231.216   321.593     0.72  (OK)
[Parallel + Thread Pool + Spin]         231.176   53.56       4.32  (NOT OK)
[Parallel + Thread Pool + Sleep]        64.354    64.953      0.99  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fan_in_async...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop_fan_in_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                278.584   239.061     1.17  (OK)
[Parallel + Always Spawn]               230.365   327.377     0.70  (OK)
[Parallel + Thread Pool + Spin]         230.104   38.293      6.01  (NOT OK)
[Parallel + Thread Pool + Sleep]        34.507    35.396      0.97  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_reduction_tree...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop_reduction_tree
                                        STUDENT   REFERENCE   PERF?
[Serial]                                247.254   238.751     1.04  (OK)
[Parallel + Always Spawn]               229.942   90.541      2.54  (NOT OK)
[Parallel + Thread Pool + Spin]         229.627   43.876      5.23  (NOT OK)
[Parallel + Thread Pool + Sleep]        46.398    45.498      1.02  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_reduction_tree_async...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop_reduction_tree_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                273.486   238.205     1.15  (OK)
[Parallel + Always Spawn]               229.106   90.628      2.53  (NOT OK)
[Parallel + Thread Pool + Spin]         228.672   36.284      6.30  (NOT OK)
[Parallel + Thread Pool + Sleep]        34.292    31.559      1.09  (OK)
================================================================================
Executing test: spin_between_run_calls...
Reference binary: ./runtasks_ref_linux
Results for: spin_between_run_calls
                                        STUDENT   REFERENCE   PERF?
[Serial]                                261.326   457.96      0.57  (OK)
[Parallel + Always Spawn]               259.927   234.187     1.11  (OK)
[Parallel + Thread Pool + Spin]         259.213   311.144     0.83  (OK)
[Parallel + Thread Pool + Sleep]        132.712   237.628     0.56  (OK)
================================================================================
Executing test: spin_between_run_calls_async...
Reference binary: ./runtasks_ref_linux
Results for: spin_between_run_calls_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                260.546   457.126     0.57  (OK)
[Parallel + Always Spawn]               260.392   235.563     1.11  (OK)
[Parallel + Thread Pool + Spin]         260.099   324.294     0.80  (OK)
[Parallel + Thread Pool + Sleep]        131.797   234.375     0.56  (OK)
================================================================================
Executing test: mandelbrot_chunked...
Reference binary: ./runtasks_ref_linux
Results for: mandelbrot_chunked
                                        STUDENT   REFERENCE   PERF?
[Serial]                                296.632   296.768     1.00  (OK)
[Parallel + Always Spawn]               296.539   22.025      13.46  (NOT OK)
[Parallel + Thread Pool + Spin]         296.57    22.745      13.04  (NOT OK)
[Parallel + Thread Pool + Sleep]        21.19     22.126      0.96  (OK)
================================================================================
Executing test: mandelbrot_chunked_async...
Reference binary: ./runtasks_ref_linux
Results for: mandelbrot_chunked_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                295.904   296.4       1.00  (OK)
[Parallel + Always Spawn]               296.759   21.941      13.53  (NOT OK)
[Parallel + Thread Pool + Spin]         295.664   22.917      12.90  (NOT OK)
[Parallel + Thread Pool + Sleep]        21.104    21.604      0.98  (OK)
================================================================================
Overall performance results
[Serial]                                : All passed Perf
[Parallel + Always Spawn]               : Perf did not pass all tests
[Parallel + Thread Pool + Spin]         : Perf did not pass all tests
[Parallel + Thread Pool + Sleep]        : Perf did not pass all tests
```

## MacOS + M5

```zsh
python3 ../tests/run_test_harness.py -n 10 -a runtasks_ref
Darwin arm64
================================================================================
Running task system grading harness... (22 total tests)
  - Detected CPU with 10 execution contexts
  - Task system configured to use at most 10 threads
================================================================================
================================================================================
Executing test: super_super_light...
Reference binary: ./runtasks_ref_osx_arm
Results for: super_super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                3.395     3.337       1.02  (OK)
[Parallel + Always Spawn]               3.331     25.473      0.13  (OK)
[Parallel + Thread Pool + Spin]         3.326     34.052      0.10  (OK)
[Parallel + Thread Pool + Sleep]        12.498    16.81       0.74  (OK)
================================================================================
Executing test: super_super_light_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: super_super_light_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                3.448     3.406       1.01  (OK)
[Parallel + Always Spawn]               3.435     26.391      0.13  (OK)
[Parallel + Thread Pool + Spin]         3.46      24.783      0.14  (OK)
[Parallel + Thread Pool + Sleep]        11.793    12.86       0.92  (OK)
================================================================================
Executing test: super_light...
Reference binary: ./runtasks_ref_osx_arm
Results for: super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                13.585    18.192      0.75  (OK)
[Parallel + Always Spawn]               13.518    33.554      0.40  (OK)
[Parallel + Thread Pool + Spin]         13.567    42.516      0.32  (OK)
[Parallel + Thread Pool + Sleep]        17.038    19.364      0.88  (OK)
================================================================================
Executing test: super_light_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: super_light_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                13.631    23.067      0.59  (OK)
[Parallel + Always Spawn]               13.596    33.951      0.40  (OK)
[Parallel + Thread Pool + Spin]         13.618    32.621      0.42  (OK)
[Parallel + Thread Pool + Sleep]        17.026    17.856      0.95  (OK)
================================================================================
Executing test: ping_pong_equal...
Reference binary: ./runtasks_ref_osx_arm
Results for: ping_pong_equal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                218.462   373.075     0.59  (OK)
[Parallel + Always Spawn]               218.479   87.516      2.50  (NOT OK)
[Parallel + Thread Pool + Spin]         218.228   82.638      2.64  (NOT OK)
[Parallel + Thread Pool + Sleep]        55.701    73.707      0.76  (OK)
================================================================================
Executing test: ping_pong_equal_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: ping_pong_equal_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                218.785   369.286     0.59  (OK)
[Parallel + Always Spawn]               218.813   87.864      2.49  (NOT OK)
[Parallel + Thread Pool + Spin]         218.574   73.562      2.97  (NOT OK)
[Parallel + Thread Pool + Sleep]        56.108    73.16       0.77  (OK)
================================================================================
Executing test: ping_pong_unequal...
Reference binary: ./runtasks_ref_osx_arm
Results for: ping_pong_unequal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                394.979   472.428     0.84  (OK)
[Parallel + Always Spawn]               395.23    115.426     3.42  (NOT OK)
[Parallel + Thread Pool + Spin]         394.734   113.601     3.47  (NOT OK)
[Parallel + Thread Pool + Sleep]        91.191    102.654     0.89  (OK)
================================================================================
Executing test: ping_pong_unequal_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: ping_pong_unequal_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                393.753   470.474     0.84  (OK)
[Parallel + Always Spawn]               395.129   115.735     3.41  (NOT OK)
[Parallel + Thread Pool + Spin]         395.753   104.415     3.79  (NOT OK)
[Parallel + Thread Pool + Sleep]        92.86     101.321     0.92  (OK)
================================================================================
Executing test: recursive_fibonacci...
Reference binary: ./runtasks_ref_osx_arm
Results for: recursive_fibonacci
                                        STUDENT   REFERENCE   PERF?
[Serial]                                826.127   825.168     1.00  (OK)
[Parallel + Always Spawn]               825.554   127.716     6.46  (NOT OK)
[Parallel + Thread Pool + Spin]         825.859   131.539     6.28  (NOT OK)
[Parallel + Thread Pool + Sleep]        126.748   127.301     1.00  (OK)
================================================================================
Executing test: recursive_fibonacci_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: recursive_fibonacci_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                824.993   825.191     1.00  (OK)
[Parallel + Always Spawn]               825.215   126.92      6.50  (NOT OK)
[Parallel + Thread Pool + Spin]         825.001   126.339     6.53  (NOT OK)
[Parallel + Thread Pool + Sleep]        123.626   126.76      0.98  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop
                                        STUDENT   REFERENCE   PERF?
[Serial]                                185.698   184.337     1.01  (OK)
[Parallel + Always Spawn]               183.324   149.671     1.22  (OK)
[Parallel + Thread Pool + Spin]         183.31    163.45      1.12  (OK)
[Parallel + Thread Pool + Sleep]        94.708    97.536      0.97  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                185.243   184.138     1.01  (OK)
[Parallel + Always Spawn]               183.185   150.512     1.22  (OK)
[Parallel + Thread Pool + Spin]         183.586   114.565     1.60  (NOT OK)
[Parallel + Thread Pool + Sleep]        99.417    88.499      1.12  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fewer_tasks...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_fewer_tasks
                                        STUDENT   REFERENCE   PERF?
[Serial]                                185.131   184.297     1.00  (OK)
[Parallel + Always Spawn]               183.197   148.81      1.23  (OK)
[Parallel + Thread Pool + Spin]         183.333   159.336     1.15  (OK)
[Parallel + Thread Pool + Sleep]        96.332    96.211      1.00  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fewer_tasks_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_fewer_tasks_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                192.925   186.253     1.04  (OK)
[Parallel + Always Spawn]               183.005   149.246     1.23  (OK)
[Parallel + Thread Pool + Spin]         183.101   34.822      5.26  (NOT OK)
[Parallel + Thread Pool + Sleep]        33.272    33.772      0.99  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fan_in...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_fan_in
                                        STUDENT   REFERENCE   PERF?
[Serial]                                103.202   96.177      1.07  (OK)
[Parallel + Always Spawn]               94.436    36.024      2.62  (NOT OK)
[Parallel + Thread Pool + Spin]         94.275    36.164      2.61  (NOT OK)
[Parallel + Thread Pool + Sleep]        26.337    30.701      0.86  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fan_in_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_fan_in_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                102.888   98.351      1.05  (OK)
[Parallel + Always Spawn]               94.241    35.959      2.62  (NOT OK)
[Parallel + Thread Pool + Spin]         94.133    20.126      4.68  (NOT OK)
[Parallel + Thread Pool + Sleep]        17.396    18.439      0.94  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_reduction_tree...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_reduction_tree
                                        STUDENT   REFERENCE   PERF?
[Serial]                                93.994    102.893     0.91  (OK)
[Parallel + Always Spawn]               93.838    22.162      4.23  (NOT OK)
[Parallel + Thread Pool + Spin]         93.639    21.934      4.27  (NOT OK)
[Parallel + Thread Pool + Sleep]        19.329    19.054      1.01  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_reduction_tree_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_reduction_tree_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                103.218   95.935      1.08  (OK)
[Parallel + Always Spawn]               93.816    22.353      4.20  (NOT OK)
[Parallel + Thread Pool + Spin]         93.801    16.866      5.56  (NOT OK)
[Parallel + Thread Pool + Sleep]        16.871    16.702      1.01  (OK)
================================================================================
Executing test: spin_between_run_calls...
Reference binary: ./runtasks_ref_osx_arm
Results for: spin_between_run_calls
                                        STUDENT   REFERENCE   PERF?
[Serial]                                292.986   293.37      1.00  (OK)
[Parallel + Always Spawn]               293.209   148.767     1.97  (NOT OK)
[Parallel + Thread Pool + Spin]         293.242   156.682     1.87  (NOT OK)
[Parallel + Thread Pool + Sleep]        148.627   148.532     1.00  (OK)
================================================================================
Executing test: spin_between_run_calls_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: spin_between_run_calls_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                293.136   293.08      1.00  (OK)
[Parallel + Always Spawn]               293.252   148.722     1.97  (NOT OK)
[Parallel + Thread Pool + Spin]         293.281   174.724     1.68  (NOT OK)
[Parallel + Thread Pool + Sleep]        148.529   148.425     1.00  (OK)
================================================================================
Executing test: mandelbrot_chunked...
Reference binary: ./runtasks_ref_osx_arm
Results for: mandelbrot_chunked
                                        STUDENT   REFERENCE   PERF?
[Serial]                                225.987   228.376     0.99  (OK)
[Parallel + Always Spawn]               225.019   32.821      6.86  (NOT OK)
[Parallel + Thread Pool + Spin]         224.401   33.238      6.75  (NOT OK)
[Parallel + Thread Pool + Sleep]        32.411    32.873      0.99  (OK)
================================================================================
Executing test: mandelbrot_chunked_async...
Reference binary: ./runtasks_ref_osx_arm
Results for: mandelbrot_chunked_async
                                        STUDENT   REFERENCE   PERF?
[Serial]                                223.647   226.781     0.99  (OK)
[Parallel + Always Spawn]               223.18    32.464      6.87  (NOT OK)
[Parallel + Thread Pool + Spin]         222.996   33.408      6.67  (NOT OK)
[Parallel + Thread Pool + Sleep]        31.896    32.407      0.98  (OK)
================================================================================
Overall performance results
[Serial]                                : All passed Perf
[Parallel + Always Spawn]               : Perf did not pass all tests
[Parallel + Thread Pool + Spin]         : Perf did not pass all tests
[Parallel + Thread Pool + Sleep]        : All passed Perf
```

## Linux + M5 (Lima virtual machine)

```bash
python3 ../tests/run_test_harness.py -n 10 runtasks_ref
Linux aarch64
================================================================================
Running task system grading harness... (11 total tests)
  - Detected CPU with 4 execution contexts
  - Task system configured to use at most 10 threads
================================================================================
================================================================================
Executing test: super_super_light...
Reference binary: ./runtasks_ref_linux_arm
Results for: super_super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                3.677     3.563       1.03  (OK)
[Parallel + Always Spawn]               3.628     49.795      0.07  (OK)
[Parallel + Thread Pool + Spin]         3.592     37.963      0.09  (OK)
[Parallel + Thread Pool + Sleep]        17.279    14.706      1.17  (OK)
================================================================================
Executing test: super_light...
Reference binary: ./runtasks_ref_linux_arm
Results for: super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                33.566    32.004      1.05  (OK)
[Parallel + Always Spawn]               32.591    49.255      0.66  (OK)
[Parallel + Thread Pool + Spin]         33.208    34.415      0.96  (OK)
[Parallel + Thread Pool + Sleep]        23.052    21.315      1.08  (OK)
================================================================================
Executing test: ping_pong_equal...
Reference binary: ./runtasks_ref_linux_arm
Results for: ping_pong_equal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                559.608   532.49      1.05  (OK)
[Parallel + Always Spawn]               567.349   198.253     2.86  (NOT OK)
[Parallel + Thread Pool + Spin]         572.69    266.411     2.15  (NOT OK)
[Parallel + Thread Pool + Sleep]        173.857   173.213     1.00  (OK)
================================================================================
Executing test: ping_pong_unequal...
Reference binary: ./runtasks_ref_linux_arm
Results for: ping_pong_unequal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                595.766   590.011     1.01  (OK)
[Parallel + Always Spawn]               593.567   210.747     2.82  (NOT OK)
[Parallel + Thread Pool + Spin]         592.988   286.421     2.07  (NOT OK)
[Parallel + Thread Pool + Sleep]        187.208   182.689     1.02  (OK)
================================================================================
Executing test: recursive_fibonacci...
Reference binary: ./runtasks_ref_linux_arm
Results for: recursive_fibonacci
                                        STUDENT   REFERENCE   PERF?
[Serial]                                414.412   477.035     0.87  (OK)
[Parallel + Always Spawn]               413.755   136.209     3.04  (NOT OK)
[Parallel + Thread Pool + Spin]         413.842   216.878     1.91  (NOT OK)
[Parallel + Thread Pool + Sleep]        118.359   135.55      0.87  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop...
Reference binary: ./runtasks_ref_linux_arm
Results for: math_operations_in_tight_for_loop
                                        STUDENT   REFERENCE   PERF?
[Serial]                                233.444   230.061     1.01  (OK)
[Parallel + Always Spawn]               242.251   260.078     0.93  (OK)
[Parallel + Thread Pool + Spin]         241.976   217.004     1.12  (OK)
[Parallel + Thread Pool + Sleep]        136.518   147.226     0.93  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fewer_tasks...
Reference binary: ./runtasks_ref_linux_arm
Results for: math_operations_in_tight_for_loop_fewer_tasks
                                        STUDENT   REFERENCE   PERF?
[Serial]                                234.19    210.212     1.11  (OK)
[Parallel + Always Spawn]               234.866   264.695     0.89  (OK)
[Parallel + Thread Pool + Spin]         234.799   206.308     1.14  (OK)
[Parallel + Thread Pool + Sleep]        134.24    144.562     0.93  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fan_in...
Reference binary: ./runtasks_ref_linux_arm
Results for: math_operations_in_tight_for_loop_fan_in
                                        STUDENT   REFERENCE   PERF?
[Serial]                                104.054   101.519     1.02  (OK)
[Parallel + Always Spawn]               103.259   58.131      1.78  (NOT OK)
[Parallel + Thread Pool + Spin]         103.223   62.54       1.65  (NOT OK)
[Parallel + Thread Pool + Sleep]        41.19     43.297      0.95  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_reduction_tree...
Reference binary: ./runtasks_ref_linux_arm
Results for: math_operations_in_tight_for_loop_reduction_tree
                                        STUDENT   REFERENCE   PERF?
[Serial]                                106.794   103.915     1.03  (OK)
[Parallel + Always Spawn]               136.834   44.904      3.05  (NOT OK)
[Parallel + Thread Pool + Spin]         136.877   65.743      2.08  (NOT OK)
[Parallel + Thread Pool + Sleep]        39.79     39.374      1.01  (OK)
================================================================================
Executing test: spin_between_run_calls...
Reference binary: ./runtasks_ref_linux_arm
Results for: spin_between_run_calls
                                        STUDENT   REFERENCE   PERF?
[Serial]                                144.832   175.012     0.83  (OK)
[Parallel + Always Spawn]               143.941   88.99       1.62  (NOT OK)
[Parallel + Thread Pool + Spin]         143.889   116.682     1.23  (NOT OK)
[Parallel + Thread Pool + Sleep]        73.243    88.764      0.83  (OK)
================================================================================
Executing test: mandelbrot_chunked...
Reference binary: ./runtasks_ref_linux_arm
Results for: mandelbrot_chunked
                                        STUDENT   REFERENCE   PERF?
[Serial]                                216.868   213.358     1.02  (OK)
[Parallel + Always Spawn]               216.227   56.051      3.86  (NOT OK)
[Parallel + Thread Pool + Spin]         215.646   64.42       3.35  (NOT OK)
[Parallel + Thread Pool + Sleep]        56.898    55.992      1.02  (OK)
================================================================================
Overall performance results
[Serial]                                : All passed Perf
[Parallel + Always Spawn]               : Perf did not pass all tests
[Parallel + Thread Pool + Spin]         : Perf did not pass all tests
[Parallel + Thread Pool + Sleep]        : All passed Perf
```
