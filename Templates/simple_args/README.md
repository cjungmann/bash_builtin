# STANDALONE TEMPLATE

Another template to jump-start the development of a new Bash builtin.

This template differs from the **Templates/basic** template in that
it includes facilities that I think are pretty useful for a minimal
Bash buitin.

## FEATURES

- **Name Tokens for Spawning**
  The string *TEMPLATE* in the template can be replaced with a new
  name describing what the new builtin will do.

- **Spawn Script**  
  A simple script that replaces the name token while copying the
  modified template source to a properly named source file.

- **Minimal Includes**  
  To help focus on the task of a project, a few generic functions
  have been moved to _utilities.c_, but the overall goal to be
  a complete and minimal template is preserved.

- **Deferred Error Reporting**  
  Prepares an **ERROR_SINK** process for reporting errors to a
  name-derived shell variable to empower a script to not only decide
  how and when to respond to an error condition, but also to report
  detailed and specific information about the error.

- **Emacs _compile-command_**  
  A **Local Variables** section for compiling in Emacs allows for
  custom build instructions in coresident builtin source files.

## PREREQUISITES

- **Linux system** including the _Bash_ shell

- **GCC build environment** installed with `build-essential`
  if not installed by default

- **bash-builtins** development package

- **[argeaters][argeaters]** argument processing library

## USAGE

Let's assume we are building an amazing new builtin called
*fire*.

1. Use **spawn** script to create a source file (*fire.c*) and a
   Bash script to test the new builtin (*fire_test*).
   ~~~sh
   ./spawn fire
   ~~~

2. Update *fire.c* to provide the new service.
   - Update entry function *fire_builtin* to perform the new
     service.
   - Update the *desc_fire* array of strings that is used for the
     *long_doc* member of *struct builtin fire_struct* that is used
     by Bash to access the builtin.
   - Update the *short_doc* member of *struct builtin fire_struct*
     to reflect the arguments that your builtin will accept.

3. Compile in Emacs.  Alternatively, use the compile instructions
   found in the bottom *Local Variables* section to build the
   command.

4. Update *test_fire* to perform appropriate testing of the new
   `fire` builtin.

5. Test the `fire` builtin

6. If appropriate, deploy new builtin by copying the executable `fire`
   command to */usr/local/lib/*.  This will make it available for
   all Bash scripts with a simple `enable fire` script instruction.

[argeaters]:  https://github.com/cjungmann/libargeater