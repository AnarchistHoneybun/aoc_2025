# coal

C Oriented AOC Loader

minimal orchestration tool for advent of code solutions in c.
automatically fetches inputs, compiles, and runs your daily solutions.

## setup

requires: gcc, bash

1. copy `.env.example` to `.env` and add your AoC session cookie
2. set `AOC_YEAR` in `.env`
3. run `./coal setup <day>` to start

```
usage: coal <command> [day] [options]

commands:
  setup <day>         create source file and fetch input
  run <day>           compile and run with real input
  compile <day>       compile only
  test <day>          run with test input (inputs/dayXX_test.txt)
  fetch <day>         fetch input only
  submit <day> <part> <answer>
                      submit answer for part 1 or 2
  clean [day]         remove binary (all if no day given)
  status              show progress
  help                show this message
```
