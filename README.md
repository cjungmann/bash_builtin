# BASH_BUILTIN

An effort to provide guidance for the development of Bash builtins

## INTRODUCTION

Bash builtins are an integral part of the Bash shell environment.
Standard builtins are widely-used in scripts and on the command
line.  Bash buitlins have several advantages over standalone
commands, especially when the commands are called from a script.

Bash includes hooks and a development package by which custom builtin
commands can extend Bash, but documentation and examples of custom
builtins are sparse.

This project aspires to provide additional guidance for people who
want to experiment with develop custom Bash builtins.

### The Case for Builtins

Bash provides low-level hooks that expose a shell's runtime
environment to specially-configured and pre-loaded external
commands referred to as **builtins**.

A standard set of builtins provide much of Bash's utility.
Control flow statements **case**, **for**, **if**, **until**,
and **while** are builtins, as are important commands like
**echo**, **printf**, **read**, and many more.

There are several reasons to consider using a Bash builtin:

- **Running Speed**  
  Builtin commands are typically written in C, a compiled
  language that is much faster than the interpretive Bash language.

- **Invocation Speed**  
  Bash builtins run in the same process as the caller, avoiding
  the performance penalty of opening a child process.

- **Privileged Access**  
  Builtins can directly read and change shell variables and
  execute functions defined in a Bash script.

## DEVELOPING BUILTINS

The hooks that enable the standard Bash builtins can also be
exploited to extend Bash with custom commands.

### Remedy for Missing Documentation

Unfortunately, documentation about creating builtins is sparse.

Among the experiments, small projects, documentation, and templates
in this project, there should be useful information to help others
explore building Bash builtins.

### Prerequisite

There are online resources available for developing new Bash builtins.

#### Required Download

The Bash-Builtin headers must be installed in order to compile the
examples.

~~~sh
sudo apt-get install bash-builtins
~~~

#### Recommended (Strongly) Download

What little official documentation that exists is found in the
directory of the source code of the Bash shell.  Download the latest
Bash version from:

```sh
git clone git://git.sv.gnu.org/bash.git
```

Besides the Bash source code, the study of which aids understanding
of the context in which builtins run, the source code directory also
includes sub-directory `bash/builtins` containing the source code of
the standard builtins, as well as `bash/examples/loadables` where one
can find some instruction in the source files.

## RESOURCES FOR DEVELOPMENT

The project include various learning resources:

- **Simple Builtin Examples**
  With the exception of **utilities.c**, the main directory files
  with the `.c` extension create simple Bash builtins.  **hellow.c**
  was my first Bash builtin, and most of the others each explore a
  simple assumed capablility of the Bash environment.

- **Templates Directory**
  I created templates to help me complete the various requirements
  of compiling a Bash builtin.  Most have not been touched in a
  while.  However, with my primary interest being a builtin with
  a persistent state to be manipulated, the template under
  `Templates/basic_handle` continues to be modified as my experience
  and needs evolve.

- **Man Pages**
  Since Bash is a TUI (text user-interface) shell, I am writing
  TUI-focused `man` pages for my benefit as I continue to develop
  certain Bash builtins.  Some man pages remain from older efforts,
  but I am starting over using **bashbi**(7) as an entry point,
  with topics using **bashbi_** as a prefix.  The new version will
  be less for learning and more for a reference during development.

  At some point, I will set up *github actions* to generate and make
  links to HTML pages generated from the `groff` documents that make
  up the man pages.
