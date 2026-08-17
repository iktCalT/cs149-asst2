# Test results

## Linux + AMD 5700G

```bash
python3 ../tests/run_test_harness.py -n 16
runtasks_ref
Linux x86_64
================================================================================
Running task system grading harness... (11 total tests)
  - Detected CPU with 16 execution contexts
  - Task system configured to use at most 16 threads
================================================================================
================================================================================
Executing test: super_super_light...
Reference binary: ./runtasks_ref_linux
Results for: super_super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                9.017     8.948       1.01  (OK)
[Parallel + Always Spawn]               448.536   482.213     0.93  (OK)
[Parallel + Thread Pool + Spin]         5.555     20.092      0.28  (OK)
[Parallel + Thread Pool + Sleep]        82.247    81.189      1.01  (OK)
================================================================================
Executing test: super_light...
Reference binary: ./runtasks_ref_linux
Results for: super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                41.79     55.869      0.75  (OK)
[Parallel + Always Spawn]               450.692   484.355     0.93  (OK)
[Parallel + Thread Pool + Spin]         10.245    22.46       0.46  (OK)
[Parallel + Thread Pool + Sleep]        82.594    82.248      1.00  (OK)
================================================================================
Executing test: ping_pong_equal...
Reference binary: ./runtasks_ref_linux
Results for: ping_pong_equal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                667.7     905.651     0.74  (OK)
[Parallel + Always Spawn]               481.12    522.167     0.92  (OK)
[Parallel + Thread Pool + Spin]         125.514   186.275     0.67  (OK)
[Parallel + Thread Pool + Sleep]        195.892   207.833     0.94  (OK)
================================================================================
Executing test: ping_pong_unequal...
Reference binary: ./runtasks_ref_linux
Results for: ping_pong_unequal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                1262.371  1284.403    0.98  (OK)
[Parallel + Always Spawn]               502.422   534.095     0.94  (OK)
[Parallel + Thread Pool + Spin]         183.001   193.101     0.95  (OK)
[Parallel + Thread Pool + Sleep]        253.645   221.963     1.14  (OK)
================================================================================
Executing test: recursive_fibonacci...
Reference binary: ./runtasks_ref_linux
Results for: recursive_fibonacci
                                        STUDENT   REFERENCE   PERF?
[Serial]                                720.537   1280.183    0.56  (OK)
[Parallel + Always Spawn]               113.724   162.529     0.70  (OK)
[Parallel + Thread Pool + Spin]         101.031   154.914     0.65  (OK)
[Parallel + Thread Pool + Sleep]        103.612   148.561     0.70  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop
                                        STUDENT   REFERENCE   PERF?
[Serial]                                443.706   452.44      0.98  (OK)
[Parallel + Always Spawn]               2298.129  2480.889    0.93  (OK)
[Parallel + Thread Pool + Spin]         95.47     164.463     0.58  (OK)
[Parallel + Thread Pool + Sleep]        427.461   430.13      0.99  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fewer_tasks...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop_fewer_tasks
                                        STUDENT   REFERENCE   PERF?
[Serial]                                443.898   452.412     0.98  (OK)
[Parallel + Always Spawn]               2324.239  2483.155    0.94  (OK)
[Parallel + Thread Pool + Spin]         128.544   168.101     0.76  (OK)
[Parallel + Thread Pool + Sleep]        421.372   420.152     1.00  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fan_in...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop_fan_in
                                        STUDENT   REFERENCE   PERF?
[Serial]                                228.369   233.607     0.98  (OK)
[Parallel + Always Spawn]               296.95    316.735     0.94  (OK)
[Parallel + Thread Pool + Spin]         42.154    50.804      0.83  (OK)
[Parallel + Thread Pool + Sleep]        64.948    62.646      1.04  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_reduction_tree...
Reference binary: ./runtasks_ref_linux
Results for: math_operations_in_tight_for_loop_reduction_tree
                                        STUDENT   REFERENCE   PERF?
[Serial]                                243.217   232.765     1.04  (OK)
[Parallel + Always Spawn]               85.489    90.497      0.94  (OK)
[Parallel + Thread Pool + Spin]         41.94     43.006      0.98  (OK)
[Parallel + Thread Pool + Sleep]        48.047    45.38       1.06  (OK)
================================================================================
Executing test: spin_between_run_calls...
Reference binary: ./runtasks_ref_linux
Results for: spin_between_run_calls
                                        STUDENT   REFERENCE   PERF?
[Serial]                                256.95    452.517     0.57  (OK)
[Parallel + Always Spawn]               133.803   232.509     0.58  (OK)
[Parallel + Thread Pool + Spin]         251.124   332.252     0.76  (OK)
[Parallel + Thread Pool + Sleep]        133.437   231.215     0.58  (OK)
================================================================================
Executing test: mandelbrot_chunked...
Reference binary: ./runtasks_ref_linux
Results for: mandelbrot_chunked
                                        STUDENT   REFERENCE   PERF?
[Serial]                                293.773   293.954     1.00  (OK)
[Parallel + Always Spawn]               21.846    21.955      1.00  (OK)
[Parallel + Thread Pool + Spin]         23.529    22.669      1.04  (OK)
[Parallel + Thread Pool + Sleep]        23.199    21.625      1.07  (OK)
================================================================================
Overall performance results
[Serial]                                : All passed Perf
[Parallel + Always Spawn]               : All passed Perf
[Parallel + Thread Pool + Spin]         : All passed Perf
[Parallel + Thread Pool + Sleep]        : All passed Perf
```

## MacOS + M5

```zsh
runtasks_ref
Darwin arm64
================================================================================
Running task system grading harness... (11 total tests)
  - Detected CPU with 10 execution contexts
  - Task system configured to use at most 16 threads
================================================================================
================================================================================
Executing test: super_super_light...
Reference binary: ./runtasks_ref_osx_arm
Results for: super_super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                3.427     3.414       1.00  (OK)
[Parallel + Always Spawn]               35.188    34.943      1.01  (OK)
[Parallel + Thread Pool + Spin]         14.242    51.197      0.28  (OK)
[Parallel + Thread Pool + Sleep]        21.677    19.737      1.10  (OK)
================================================================================
Executing test: super_light...
Reference binary: ./runtasks_ref_osx_arm
Results for: super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                13.646    24.732      0.55  (OK)
[Parallel + Always Spawn]               41.367    39.852      1.04  (OK)
[Parallel + Thread Pool + Spin]         16.749    61.617      0.27  (OK)
[Parallel + Thread Pool + Sleep]        26.005    23.185      1.12  (OK)
================================================================================
Executing test: ping_pong_equal...
Reference binary: ./runtasks_ref_osx_arm
Results for: ping_pong_equal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                218.814   402.29      0.54  (OK)
[Parallel + Always Spawn]               78.325    92.265      0.85  (OK)
[Parallel + Thread Pool + Spin]         47.575    94.492      0.50  (OK)
[Parallel + Thread Pool + Sleep]        63.129    77.832      0.81  (OK)
================================================================================
Executing test: ping_pong_unequal...
Reference binary: ./runtasks_ref_osx_arm
Results for: ping_pong_unequal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                396.719   467.549     0.85  (OK)
[Parallel + Always Spawn]               110.047   119.589     0.92  (OK)
[Parallel + Thread Pool + Spin]         82.248    125.193     0.66  (OK)
[Parallel + Thread Pool + Sleep]        93.412    104.399     0.89  (OK)
================================================================================
Executing test: recursive_fibonacci...
Reference binary: ./runtasks_ref_osx_arm
Results for: recursive_fibonacci
                                        STUDENT   REFERENCE   PERF?
[Serial]                                824.692   824.167     1.00  (OK)
[Parallel + Always Spawn]               125.431   125.491     1.00  (OK)
[Parallel + Thread Pool + Spin]         125.019   154.027     0.81  (OK)
[Parallel + Thread Pool + Sleep]        126.686   124.299     1.02  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop
                                        STUDENT   REFERENCE   PERF?
[Serial]                                184.941   184.39      1.00  (OK)
[Parallel + Always Spawn]               183.204   191.516     0.96  (OK)
[Parallel + Thread Pool + Spin]         63.276    231.492     0.27  (OK)
[Parallel + Thread Pool + Sleep]        107.186   102.971     1.04  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fewer_tasks...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_fewer_tasks
                                        STUDENT   REFERENCE   PERF?
[Serial]                                185.759   185.731     1.00  (OK)
[Parallel + Always Spawn]               184.158   203.414     0.91  (OK)
[Parallel + Thread Pool + Spin]         72.749    214.457     0.34  (OK)
[Parallel + Thread Pool + Sleep]        115.259   101.435     1.14  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fan_in...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_fan_in
                                        STUDENT   REFERENCE   PERF?
[Serial]                                106.321   105.842     1.00  (OK)
[Parallel + Always Spawn]               43.651    43.516      1.00  (OK)
[Parallel + Thread Pool + Spin]         24.325    51.418      0.47  (OK)
[Parallel + Thread Pool + Sleep]        35.024    34.127      1.03  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_reduction_tree...
Reference binary: ./runtasks_ref_osx_arm
Results for: math_operations_in_tight_for_loop_reduction_tree
                                        STUDENT   REFERENCE   PERF?
[Serial]                                104.564   101.326     1.03  (OK)
[Parallel + Always Spawn]               22.597    22.815      0.99  (OK)
[Parallel + Thread Pool + Spin]         19.871    22.924      0.87  (OK)
[Parallel + Thread Pool + Sleep]        19.625    18.994      1.03  (OK)
================================================================================
Executing test: spin_between_run_calls...
Reference binary: ./runtasks_ref_osx_arm
Results for: spin_between_run_calls
                                        STUDENT   REFERENCE   PERF?
[Serial]                                292.611   292.755     1.00  (OK)
[Parallel + Always Spawn]               148.729   148.7       1.00  (OK)
[Parallel + Thread Pool + Spin]         150.175   340.765     0.44  (OK)
[Parallel + Thread Pool + Sleep]        148.632   148.48      1.00  (OK)
================================================================================
Executing test: mandelbrot_chunked...
Reference binary: ./runtasks_ref_osx_arm
Results for: mandelbrot_chunked
                                        STUDENT   REFERENCE   PERF?
[Serial]                                229.322   232.832     0.98  (OK)
[Parallel + Always Spawn]               31.498    31.942      0.99  (OK)
[Parallel + Thread Pool + Spin]         31.696    32.416      0.98  (OK)
[Parallel + Thread Pool + Sleep]        31.451    31.853      0.99  (OK)
================================================================================
Overall performance results
[Serial]                                : All passed Perf
[Parallel + Always Spawn]               : All passed Perf
[Parallel + Thread Pool + Spin]         : All passed Perf
[Parallel + Thread Pool + Sleep]        : All passed Perf
```

## Linux + M5 (lima virtual machine)

It didn't pass all tests here.

```bash
python3 ../tests/run_test_harness.py -n 16
runtasks_ref
Linux aarch64
================================================================================
Running task system grading harness... (11 total tests)
  - Detected CPU with 4 execution contexts
  - Task system configured to use at most 16 threads
================================================================================
================================================================================
Executing test: super_super_light...
Reference binary: ./runtasks_ref_linux_arm
Results for: super_super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                3.696     3.645       1.01  (OK)
[Parallel + Always Spawn]               73.178    76.736      0.95  (OK)
[Parallel + Thread Pool + Spin]         4.522     63.26       0.07  (OK)
[Parallel + Thread Pool + Sleep]        295.026   11.524      25.60  (NOT OK)
================================================================================
Executing test: super_light...
Reference binary: ./runtasks_ref_linux_arm
Results for: super_light
                                        STUDENT   REFERENCE   PERF?
[Serial]                                35.069    33.238      1.06  (OK)
[Parallel + Always Spawn]               64.552    67.009      0.96  (OK)
[Parallel + Thread Pool + Spin]         12.466    48.636      0.26  (OK)
[Parallel + Thread Pool + Sleep]        91.663    16.789      5.46  (NOT OK)
================================================================================
Executing test: ping_pong_equal...
Reference binary: ./runtasks_ref_linux_arm
Results for: ping_pong_equal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                576.447   543.38      1.06  (OK)
[Parallel + Always Spawn]               218.798   213.926     1.02  (OK)
[Parallel + Thread Pool + Spin]         157.324   519.704     0.30  (OK)
[Parallel + Thread Pool + Sleep]        179.19    174.347     1.03  (OK)
================================================================================
Executing test: ping_pong_unequal...
Reference binary: ./runtasks_ref_linux_arm
Results for: ping_pong_unequal
                                        STUDENT   REFERENCE   PERF?
[Serial]                                601.69    586.553     1.03  (OK)
[Parallel + Always Spawn]               238.872   225.904     1.06  (OK)
[Parallel + Thread Pool + Spin]         177.681   733.977     0.24  (OK)
[Parallel + Thread Pool + Sleep]        188.665   181.894     1.04  (OK)
================================================================================
Executing test: recursive_fibonacci...
Reference binary: ./runtasks_ref_linux_arm
Results for: recursive_fibonacci
                                        STUDENT   REFERENCE   PERF?
[Serial]                                420.809   476.51      0.88  (OK)
[Parallel + Always Spawn]               124.108   139.156     0.89  (OK)
[Parallel + Thread Pool + Spin]         122.269   347.296     0.35  (OK)
[Parallel + Thread Pool + Sleep]        125.758   135.481     0.93  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop...
Reference binary: ./runtasks_ref_linux_arm
Results for: math_operations_in_tight_for_loop
                                        STUDENT   REFERENCE   PERF?
[Serial]                                224.084   202.518     1.11  (OK)
[Parallel + Always Spawn]               341.869   353.212     0.97  (OK)
[Parallel + Thread Pool + Spin]         101.765   435.943     0.23  (OK)
[Parallel + Thread Pool + Sleep]        155.43    144.683     1.07  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fewer_tasks...
Reference binary: ./runtasks_ref_linux_arm
Results for: math_operations_in_tight_for_loop_fewer_tasks
                                        STUDENT   REFERENCE   PERF?
[Serial]                                240.973   234.299     1.03  (OK)
[Parallel + Always Spawn]               340.186   357.347     0.95  (OK)
[Parallel + Thread Pool + Spin]         123.201   492.141     0.25  (OK)
[Parallel + Thread Pool + Sleep]        140.505   149.04      0.94  (OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_fan_in...
Reference binary: ./runtasks_ref_linux_arm
Results for: math_operations_in_tight_for_loop_fan_in
                                        STUDENT   REFERENCE   PERF?
[Serial]                                112.065   109.261     1.03  (OK)
[Parallel + Always Spawn]               65.399    65.985      0.99  (OK)
[Parallel + Thread Pool + Spin]         36.257    118.284     0.31  (OK)
[Parallel + Thread Pool + Sleep]        49.417    40.371      1.22  (NOT OK)
================================================================================
Executing test: math_operations_in_tight_for_loop_reduction_tree...
Reference binary: ./runtasks_ref_linux_arm
Results for: math_operations_in_tight_for_loop_reduction_tree
                                        STUDENT   REFERENCE   PERF?
[Serial]                                112.524   109.274     1.03  (OK)
[Parallel + Always Spawn]               49.305    49.014      1.01  (OK)
[Parallel + Thread Pool + Spin]         41.069    150.136     0.27  (OK)
[Parallel + Thread Pool + Sleep]        42.41     40.127      1.06  (OK)
================================================================================
Executing test: spin_between_run_calls...
Reference binary: ./runtasks_ref_linux_arm
Results for: spin_between_run_calls
                                        STUDENT   REFERENCE   PERF?
[Serial]                                146.103   174.81      0.84  (OK)
[Parallel + Always Spawn]               74.66     89.381      0.84  (OK)
[Parallel + Thread Pool + Spin]         98.389    129.459     0.76  (OK)
[Parallel + Thread Pool + Sleep]        78.134    93.246      0.84  (OK)
================================================================================
Executing test: mandelbrot_chunked...
Reference binary: ./runtasks_ref_linux_arm
Results for: mandelbrot_chunked
                                        STUDENT   REFERENCE   PERF?
[Serial]                                222.433   218.8       1.02  (OK)
[Parallel + Always Spawn]               57.053    56.102      1.02  (OK)
[Parallel + Thread Pool + Spin]         59.396    62.204      0.95  (OK)
[Parallel + Thread Pool + Sleep]        57.409    55.918      1.03  (OK)
================================================================================
Overall performance results
[Serial]                                : All passed Perf
[Parallel + Always Spawn]               : All passed Perf
[Parallel + Thread Pool + Spin]         : All passed Perf
[Parallel + Thread Pool + Sleep]        : Perf did not pass all tests
```
