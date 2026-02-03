# BASH BUILTIN DYNAMICS

Innocently browsing the Bash source module `variables.h`, one might
notice the **assign_func** and the **dynamic_value** members of
**SHELL_VAR** (aka _put_ and _get_ methods) and be tempted to
experiment with implementing these methods.  Be advised that they
are not as useful as they seem.

Note that I am making these conclusions based on my work with
builtins that implement a persistent "handle" that is referenced
through the **value** member of the SHELL_VAR.

## DESCRIBING THE PROBLEM

The main problem is the inability to set the exit_value in the
case of an error, so extraordinary error-checking would be required
in the script code using the put or get overrides, perhaps saving
the error state in the SHELL_VAR somehow for reference after the
put or get operation.

## IGNORING THE PROBLEM

If you can live with the extraordinary error-checking, the following
advice may be helpful.

### Function Prototypes

I have given my own names for the prototypes to reinforce my use
of *put* and *get* to describe them.

```c
SHELL_VAR* get_value_func(SHELL_VAR *sv);
SHELL_VAR* put_value_func(SHELL_VAR  *sv,
                          const char *new_value,
                          int        array_index,
                          const char *assoc_key);
```

- **Return value of the C functions**  
  Both the *put* and *get* methods are expected to return the same
  SHELL_VAR that they received in the arguments.

- **Shell return value**  
  The return value must be found at the beginning of the **value**
  member of the SHELL_VAR.  Assuming that SHELL_VAR::value points
  to a custom struct, the struct could start with a 0-terminated
  string, beyond which the formatted data could begin.  Changing
  the value would involve   making a new memory block with the
  changed value heading up the  memory block.
