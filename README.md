# Bash Builtins Project

## Introduction

I wanted to play around with Bash builtins when I realized that they
had special characteristics I could exploit for improving performance
in some of my more ambitious scripts.  Unable to find much in the way
of documentation, I downloaded the Bash source code and referred to
the standard builtins while I started experiments.

I was happy to find that builtins did indeed deliver what I had hoped:
that a C program could run in the same process as Bash script, and
a C program could call back to a function in the script.

There are several details to which one must attend in order to develop
a working Bash builtin.  To avoid missing any of the details, I
created a template with skeletal source files and a Makefile.  The
basic template spawned several more targeted templates.  The last
template, _basic_handle_, continues to get attention as I add tools
and other code.

## Prerequisite

The Bash-Builtin headers must be installed in order to compile the
examples.

~~~sh
sudo apt-get install bash-builtins
~~~

## Development Phases

The project preserves two phases of development, the experimental
phase consisting of several small projects, and a ongoing phase of
developing templates that can jump-start new projects.

### Phase 1: Experimentation

The [README document of the experimental phase](README_discovery.md)
is the original project README file.  It lists the source files
and their learning objectives, and describes how to build and test
them.

The files of the experimentation phase are found in the main project
directory.

### Phase 2: Templates

The experiment phase of the project is mostly complete.
The experiments are found in the main project directory and are
described in the [README file](Templates/README.md).

