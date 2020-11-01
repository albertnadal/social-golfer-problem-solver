# Social Golfer Problem solver
This is a C implementation to solve the Social Golfer Problem.

This implementation performs extensive usage of stack memory. So you need to increase the default application stack size of your operating system with the command bellow:

```sh
$ ulimit -s 65532
```

Compile the source code with the gcc compiler with the command bellow:

```sh
$ gcc main.c -o main
```

Finally run the solver.

```sh
$ ./main
```