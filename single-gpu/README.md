# Artifact 1

This artifact is provided with all the scripts and tools needed for executing the single-gpu experiments.

## Artifact Execution

### Step 1: Profiling
The current flow describes how to profile the application.

1. Run the `build_profiling.sh` (TODO) to build the binaries. The script takes as input two parameters:
    - `--plat`: [`HIP`, `LZ`, `CUDA`]
    - `--arch`: the gpu architecture;

2. Run the `run_profiling.sh` script. The script takes as input  the following parameters:
    - `--arch`: [`HIP`, `LZ`, `CUDA`]
    - `--sampling`: an integers that indicates how many frequencies to skip.

    Alternatively, you can execute the `find-freq.py` (TODO).

### Applications Input
The input sizes of the applications is described in `run_profiling.sh`. The values are alread set for replicating the results obtained in the paper.

We provide in the following an overview of the applications parameters we have used.

**ACE**
``` bash
num_runs=1 # The number of application runs
./ace $num_runs
```
**AOP**
```bash
timesteps=50 # This defines the input size along with `num_paths`
num_paths=128 # This defines the input size along with `timesteps`
num_runs=1
./aop -timesteps $timesteps -paths $num_paths -runs $num_runs
```
**Metropolis**
```bash
L=1024 # Defines the input size
R=1
atrials=1 # This and the following parameters define the number of iterations
ains=1
apts=1
ams=1
./metropolis -l $L $R -a $atrials $ains $apts $ams
```

**MNIST**
```bash
num_iters=1 # The number of iterations
./mnist $num_iters
```

**SRAD**
```bash
num_iters=100 # defines the number of iterations
lambda=1
rows=2048 # defines the X axis dimension
cols=2048 # defines the Y axis dimension
./srad $num_iters $lambda $rows $cols
```

### Step 2: Phase Identification
TODO

### Step 3: Phase-Based Execution
Once you have got the `config` files, you have to rebuild the application in order to disable some profiling features.

1. Run the `build_phase.sh` script. It takes as input:
    - `--plat`: [`HIP`, `LZ`, `CUDA`]
    - `--arch`: the gpu architecture;

2. Run the `run_phase.sh` script: