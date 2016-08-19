
# SimpleShell

## Synopsis

 SimpleShell is a creation based off of my own needs through the years.  I have found that as
 I have developed more complex designs, I need to often test the chunk of code under development with either
 a harness or some other framework around it.  When doing development on a system that doesn't have
 a debugger or an in system debugger, printf debugging can be quite tough.  Can't dump a register
 like you can through an ICDB?  Next best thing is to have a function that can do that for you.
 It's still printf debugging, but it's at least given you the ability to interact with it.

 I made several versions of this for the PIC series of microcontrollers, but when I became serious
 about some of the AVR and ARM development, I realized I needed to make this into a library that would
 work across many environments.  While I did not borrow code directly from the ChibiOS project, I did
 make observations about what they did, and used some of those ideas in designing this code.

## Using it

 To set up, SimpleShell requires a table of "string/function pointer" combos.  These strings are
 the command names you would type at the console.  The function pointer component should point to
 the function to invoke for the command.  executeCommand is desi

 It tokenizes it, pulls off the first token, looks to see if it is in the table of strings,
 and executes it if it is, handing the remainder of the tokens into it.  If it cannot find the
 string in the table, it returns the NOTFOUND enum value.

 Each function needs to return the `ExecStatus` enum type, either `SUCCESS` or `FAILED`.  It must take
 one argument, of the type `CommandLine`, as a pointer.  Like this:

```CPP
 ExecStatus fooFunction( CommandLine* clp )
 {
      Serial.println("Foo called");
      return SUCCESS;
 }
```

 Now to create a table to pass into the shell object:

```CPP
 Command cmdlist[] = {
    {"foo",fooFunction},
    {"bar",bar},
    {"failme",failme},
    {NULL,NULL}
 };
```

 Finally to instantiate a shell and hand it the command list:

```CPP
 SimpleShell myShell(cmdlist);
```

 Each item is an instance of `Command` type, which can be found in SimpleShell.h.  It contains
 a string and a function pointer.  Should the command typed in match any of the strings in
 this table, the corresponding function pointer will be invoked.  The `{NULL,NULL}` at the end
 of the list is required, as just like with null terminated strings, that's how the system
 knows not to traverse beyond that memory location.

 Now let's look at what your function will receive when it is invoked:

```CPP
 typedef struct CommandLine_t{
    char cmd[SIMPLESHELL_COMMAND_SIZE];
    int argc;
    char argv[SIMPLESHELL_MAX_ARGS][SIMPLESHELL_ARG_SIZE];
 } CommandLine;
```

 There's the command itself, then the count of the args handed to it, finally the string
 array of the arguments handed to the command.  If any of your arguments are longer than
 `SIMPLESHELL_ARG_SIZE`, it truncates them and only copies in `SIMPLESHELL_ARG_SIZE - 1` chars
 (the final char being the null).

 Some notes on this design:  since this can operate on many platforms, I have fixed the memory
 sizes, and have not used dynamic allocation of memory.  This is twofold: small systems require
 more resources to do dynamic allocation, and memory leaks are a pain to track down in regular
 computers, let alone an embedded system.  This compromise means that you will only ever have
 one command line in memory at a time, so if your system is multi-threaded or if you wish to
 keep around a command line, you will need to be responsible for copying off the command line
 in your called function.

 To reduce platform dependencies, it has been designed to not require the ability to read an input,
 or print to an output stream.  This means that it takes a string, and uses that for the command
 input. 

## What this doesn't do well:

 It can't handle arguments with spaces in them.  You may want to make your function translate
 between _ and spaces so that you can use it with spaces.  It sucks, but maybe in a future
 version I can incorporate the ability to escape spaces using either backslash, or quotes.
 Then again, it _**is**_ called SimpleShell for a reason.

 Multi-threaded-ness.  While I use `strtok_r()`, I have not vetted this out for other multithreadded
 issues.   As well, since this code is meant to be more generic and not system dependent, locking
 is usually something that is platform dependent, so it's not used.   Maybe in the future, I could
 add an abstract class that makes you override a "lock" member function, where you would put your
 OS/Platform dependent locking.

## Testing

 This code should work with the example provided, and should build and load on any Arduino out there.
 Most of the code is standard C/C++, so please email me if you find an environment where it doesn't work.

 You can also use `build.sh` to build a linux version of this code, and test.src is the C++ wrapper that
 is the linux version of the example code.  It's named test.src, because the arduino environment
 slurps it in if it's a .cpp file, which isn't something we want.

 