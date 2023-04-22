# PROJECT BASH_BUILTIN

I wonder what you can do with a custom **Bash** builtin function
written in C.  Will it run in the same process as the main **Bash**
thread?  Can the custom builtin access other **Bash** builtins?
And most importantly for my purposes, can the custom **Bash**
builtin access script-based functions?

## Using in Bash

Including a shared-object library for use in **Bash** entails
the use of the **enable** function.  Search the **Bash** manpage
for *^ *enable [*. to jump to the documentation for the
**enable** function.

## Planning For Development

It's not so hard to setup a function to be available as a
builtin, you define the function, an array of strings for a long
description, then populate a **struct builtin** instance.

Reading the parameters is the first real hurdle I found.  As I
work it out, I'm recording my thoughts and discoveries.

### Argument Handling

Assuming there are arguments to the builtin, there are a few
argument types to consider

1. **Flag options** have no values, but only set a flag.
   Take the *-r* **grep** options for recursive search

2. **Value options** are followed by a value, that is, a value
   option usually consists of two arguments.  (It can be a
   single argument if the space between the option and the value
   is omitted.)  Think **-e** *PATTERN* in **grep**.

3. Value arguments that are not options.  For example, the
   filename after **rm** *filename* is not an option, but a
   value.

### internal_getopt Concepts

This is not exhaustive, but is another record of questions and
discoveries.

### internal_getopt Arguments

internal_getopt() collects the command options out of the list
of arguments.  It only returns options, a separate pass through
the **WORD_LIST** can retrieve string arguments.

1. The first argument is a pointer to the **WORD_LIST** struct
   from which the arguments are read.

2. The second argument is a list of recognized option letters.
   If the letter is followed by a colon, the option takes an argument,
   if the letter is followed by another letter instead, the option
   is a flag.

### Collecting String Arguments

Look at walk_args.c for an example collecting string arguments.

## References

I haven't written any code yet, but I am collecting what I hope is
useful information.  The links below are a record of places visited
that I may want to find again.

- This [mirror of Bash source code][bash_mirror] is the best thing
  I've found (so far) from which I can see enough examples to figure
  out what's going on.  I cloned it and looked in the
  `bash/examples/loadables` directory and poked around in the source
  files for some of the standard builtin functions.

  Under the cloned **bash** directory, the following files are of
  particular interest:

  - **bash/builtins/common.h** consolidates the contents of several
    C source files.

  -  **bash/variables.h** and **bash/variables.c** will help the
     developer understand access to script variables.

  - **bash/examples/loadables/asortr.c** is an example of working with
    arrays, both regular and associative, particularly in function
    sort_index.

- **/usr/include/bash** contains several header files that appear to
  be designed for custom **Bash** builtins.

- [Bash GitHub Page][bash_git] may include source code for the
  standard builtins that can demonstrate implementation details we
  might need for our own builtins.

- This [simple example][simple_example] my be the minimalist example
  needed for a boilerplate builtin project.



[bash_git]:      "https://github.com/gitGNU/gnu_bash"
[simple_example] "https://github.com/satoru-takeuchi/bash-builtin-example"
[bash_mirror]:   "https://github.com/bminor/bash.git
