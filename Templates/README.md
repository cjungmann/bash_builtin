# BASH BUILTIN TEMPLATES

I create templates to make development of new projects easier.
They compile as they stand, with Makefiles to build and install
the new builtins.

The templates consist of multiple files, so each is in its own
directory.

## TEMPLATES

- **basic**  
  Most sparse template for builtin development, consisting of a source,
  a header, and a Makefile.

- **basic_args**  
  Includes code for handling command line arguments, shell variables,
  some error handling, and stubs for **man** pages to document your
  work.

  The string *TEMPLATE* is used in some files and on some file names
  as a placeholder for a command name if this template is used to
  start a new project.

- **basic_handle**  
  This template is the latest and greatest version.  This template
  continues to get attention because the model it supports (a persistent
  state between calls) is how all my Bash builtin projects work.

  This template includes most of what is in **basic_args**, though
  some things have been updated with new ideas.  At some point it
  may make sense to port useful features back to **basic_args**.

  In addition to what is in **basic args**, this template implements
  a handle action model as a means to communicate intentions to the
  handle.  The template also includes a script to create new and
  register new *actions* to use with a handle.

