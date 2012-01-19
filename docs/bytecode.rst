.. _bytecode:

.. |S0| replace:: S\ :sub:`0`
.. |S1| replace:: S₀

.. index:: S₀

***********************
Swanson bytecode (|S0|)
***********************

The first stage of executing any Swanson code is to compile it down into
the *bytecode representation*.  Swanson uses a fairly high-level
bytecode; it basically just abstracts away expression trees and adds
explicit typing.  This bytecode language is important enough that we
give it its own name: |S0| (pronounced “ess naught”).

.. note:: One thing to note about |S0| as we learn about it is that the
   language is not really optimized for humans.  There are a lot of
   places where you have to be quite repetitive, and the typing
   information is explicit in the |S0| source.  However, while you'll
   rarely find yourself writing |S0| code directly, it's still useful to
   learn about its design and implementation.


|S0| compiler
=============

An |S0| program consists of a sequence of *instructions*, which is
processed by the |S0| *compiler*.  The job of the compiler is to
translate the |S0| instructions into some representation that is
directly executable.  (The compiler doesn't have to go directly to
low-level machine code; as an example, later on we'll describe the
Lagavulin VM.)

It would also be possible to write an |S0| *interpreter*, which executes
|S0| code directly, by combining the compile-time and runtime phases.
We don't have one of those right now, though.


Entity overview
===============

The various instructions in |S0| manipulate the following entities:

* *types*
* *values*
* *locations*
* *functions*
* *macros*
* *literals*

Of these, only values, locations, and functions are guaranteed to have
some representation at runtime.  Types, macros, and literals are only
guaranteed to exist at compile time.


.. index:: S₀; types

Types
-----

Types in |S0| have a different meaning and interpretation than what you
might be used to.  An |S0| type is simply a collection of *methods*.
Each method has a name, which is an arbitrary string, and will usually
be either functions or macros.  *All* of the operations that can be
performed on a value will be implemented by a method on the
corresponding type.  This even holds for simple things like integer
arithmetic; they are methods of the ``int`` type.  Those methods happen
to be implemented in the C code that implements Swanson itself; but
otherwise, they're no different than any function or macro methods you
define yourself in Swanson code.


.. index:: S₀; values

Values
------

A *value* is an instance of some |S0| type.  Values are also probably
not very much like what you're used to in other programming languages.
In particular, you can't create values directly; they can only be
produced as the result of some method call.  (*Literals*, described
later on, give us a way to define methods that produce constant values
like you'd see in other languages.)

All values are strictly read-only.  (We'll learn about their mutable
cousins, *locations*, in the next section.)  They're also ephemeral: you
have no explicit control over their lifecycle, and there's no way to
distinguish “identical” values from each other.  (There isn't even a
built-in concept of “identical”.)


.. index:: S₀; locations

Locations
---------

A *location* is a memory cell that contains a value of some |S0| type.
To be useful, the location's type should provide an *accessor* method
that can extract the contents of the location into a value.  In
addition, locations will often be *mutable*, in which case there's a
corresponding *mutator* value that can update the contents of the
location with a value passed in as a parameter.  (It's also possible for
a location to be read-only if there isn't a mutator method available.
This isn't quite the same as a value, since the read-only location has a
distinct identity as a particular region of memory.)

Each location is either *local* or *global*.  A local location is one
that can never be used outside of the current function scope; in terms
of a programming language like C, this means that the location can be
allocated on the stack.  A global location is one that can be used in
arbitrary places in the current application; in C terms, these must be
allocated on the heap.  Local locations are allocated using the
``LOCAL`` |S0| instruction.  Global locations, on the other hand, are
allocated using the built-in ``new`` function.


.. index:: S₀; functions

Functions
---------

A *function* is a chunk of executable code that can be called.
Functions in |S0| are first-class objects, and can be passed around just
like values.  A function takes in a list of *parameters* and produces a
list of *results*.  The parameters and results of a function are typed.
Since the values and locations that you can pass in as actual parameters
of the function are also typed, |S0| can statically verify that each
function call is type-safe.

The *body* of a function is a list of |S0| instructions.  The parameters
of the function are available via certain predefined identifiers (which
we'll learn more about later).  Functions can also contain *upvalues*,
which allow the function body to access entities that were declared in
the scope that the function definition appears in.  Any other
identifiers declared in the body of the function are local to that
function.

Functions are called at runtime (unlike *macros*, which we'll describe
next).  When it encounters a function definition, the |S0| compiler
creates some executable representation of that function.  When the
function is called (via the ``CALL`` instruction), the compiler produces
code that will transfer control to the function.  Since functions are
called at runtime, the parameters, results, and upvalues of a function
are limited to those entities which are guaranteed to exist at runtime —
values, locations, and other functions.  (You can't pass in a macro or a
literal, for instance, as a parameter to a function.)


.. index:: S₀; macros

Macros
------

A *macro* is similar to a function, in that it's a chunk of executable
code that can be called.  Like a function, a macro contains a *body* of
|S0| instructions, and is passed in *parameters* and returns *results*.
It also can use *upvalues* to access entities in the scope that the
macro definition appears in.

Macros are different from functions, though, in that they're called at
compile time.  When it encounters a macro definition, the |S0| compiler
does *not* create an executable representation of the macro body.
Instead, the macro body is kept around as a sequence of |S0|
instructions.  When the macro is called (via the ``CALL`` instruction),
the macro's body is interpolated into the current context, as if the
macro's instructions appeared in place of the ``CALL`` instruction.
(It's not quite this simple; macro calls are *hygienic*, which means we
do “the right thing” if there are any identifiers that are defined in
both the caller and the macro body.)

Because macros are called at compile time, their parameters, results,
and upvalues can be any |S0| entity.  (Functions are limited to those
entities that exist at runtime — i.e., values, locations, and
functions.)


.. index:: S₀; literals

Literals
--------

A *literal* is a simple string constant.  They're needed so that we can
easily create constant values of arbitrary types.  Types that support
compile-time constants will define a method that takes in a literal as a
parameter, and returns the value of that type that represents that
literal.  (However, literals only exist at compile time, so note that
this method must be a macro, and not a function.)
