# Bash Builtins Project

This project was started as an archive of coding experiments.  The
first experiments were exercises in understanding how to setup and
build Bash builtins.  More recent programs are about transferring
control back-and-forth between Bash script code and the C-language
builtin code.

## Exploring the Project

Simply clone the project and run `make`.  The Makefile will
generate several _shared object_ libraries (ie compiled object
files with an `.so` extension).

Each Bash builtin library has a corresponding Bash script whose
name is derived from the C source file, adding a `test_` prefix
and discarding the `.c` extension.

There is a Bash script, `test_tests`, that cycles through all of
the individual test scripts.  The order of the test of tests is
roughly the order of development, so the more basic tests come
first, and more advanced or experimental things come later.

## What I Hoped to Accomplish

- Design commands that run in the same process as the script.
  Returning values in nameref variables is 10x faster than
  starting a child process and reading stdout.

- Deliver a more _black-box_ utility compared with some
  Bash frameworks, especially like my work in [bash_patterns][bash_patterns].

- To be able to create C language objects whose lifetimes can
  endure and can be managed through Bash script functions.

## List of Builtins

- **hellow.c**  
  My first builtin simply says "hello" and, crucially for starting
  out: confirms that the builtin is in the same process as the
  script that calls it, otherwise my potential builtins would forfeit
  and C performance benefits to the sub process penalty.

- **walk_opts.c** and **walk_args.c**  
  contain my first efforts at reading the command line arguments
  passed to the builtin command.

- **nameref_arg.c**  
  Simple confirmation that a variable can be accessed through
  its name, and that changes to the name-accessed variable are
  visible to the calling script when the builtin terminates.

- **demo_collections.c**  
  Practice filling arrays and associative arrays with content
  that the calling script can access upon return. This source
  file also includes some code for printing an integer in any
  number base from 2 to 63.

- **read_array.c**  
  Practice conventions for processing elements of a Bash array.

- **write_array.c**  
  Confirming that a command can change elements of an array
  passed to the builtin, and the new values will be visible
  in the Bash script when the builtin finishes.

- **identify_name.c**  
  This is not so useful.  It was my first attempt at a builtin
  reporting the details of a named variable.  See the
  `show_attributes` builtin below.

- **func_callback.c**  
  Confirmation that a builtin command can invoke a Bash script
  function.

- **func_callout.c**
  Another exercise in invoking a callback script function.
  This early attempt is more complicated and verbose than necessary,
  as later efforts will show.

- **show_attributes.c**  
  Given the name of a variable, this builtin prints a human-readable
  accounting of the variable's attributes.  Created primarily as a
  debugging tool for builtin development.

- **func_dissect.c**  
  This builtin prints a not-very-sophisticated command tree of
  the function whose name is passed to the builtin.  I wrote this
  program to dig a bit into the anatomy of Bash functions so I
  can better understand how I might create a function I could
  return to a script for that script to call back into the
  builtin.

- **func_iface.c**  
  The name is a bit misleading.  This builtin implements a handle
  that can be returned to the calling script to be used to call
  back into the builtin from Bash functions.  As a proof of concept,
  the handle is very simple, with just barely enough utility to
  prove that the Bash script can manipulate a suspended builtin
  object.  The handle object is created by exploiting the rarely if
  ever used `att_special` attribute and casting the interface to
  `char*` and saving it to the SHELL_VAR::value member.  Since Bash
  expects the `value` member to be a simple memory string, this
  builtin disposes of the interface and sets `value` to NULL so Bash
  leaves it alone.

## Other Interesting Files

There are a few other files that may be useful:

- **utilities.c**  
  A non-builtin C source file that includes a few functions that
  were useful to more than one other builtin.  As I gain experience
  developing builtins, I wanted to isolate the builtin basics from
  implementation details, thus this file was created.

- **word_list_stack.h**  
  What a lame name.  With object lifetimes limited to the life of
  the builtin function, I thought it would be interesting to use
  stack memory to build argument WORD_LISTs.  Without reliable
  inline functions, this requires some tricky macro work, but it
  was fun to do and useful.

- **FNANE.c.template**  
  This template file is a boilerplate source file to start a
  new builtin command.  It's not perfect, I will probably update
  it sometime.  Use your editor's search-and-replace utility to
  change `<FNAME>` to your builtin function name.

- **bash_breakpoints.7**, **bash_builtin.7**, and **bash_variables.7**
  These rudimentary man files are mostly aborted attempts to
  document what I'm learning with this project.  I got impatient
  with trying to write everything I was learning, especially as what
  I was doing got more complicated.

[bash_patterns]:   "https://github.com/cjungmann/bash_patterns"
[bash_source]:     "https://git.savannah.gnu.org/git/bash"