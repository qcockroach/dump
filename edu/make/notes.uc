Source:
http://www.gnu.org/software/make/manual/make.html#Override-Directive

Big Qs:
1) 2.5 Letting make Deduce the Recipes (don't understand)
2) `-rm edit $(objects)' why do we put hypher defore rm command ?
3) What are directives and macros in Makefile
4) Archieve files in Makefile. what is that?
5) Types of Prerequisites (don't understand)
   `targets : normal-prerequisites | order-only-prerequisites'
6) what are `VPATH' and `vpath' used for ?
7) 
8) 
9) 
10) 


TODO:
1) reread Paragraoh 4 (how to write rules)
2) 
3) 
4) 


Terms:
1) 



Quotes:
1) The recompilation must be done if the source file, or any of the header
   files named as prerequisites, is more recent than the object file, or if
   the object file does not exist.
2) Before recompiling an object file, make considers updating its prerequisites,
   the source file and header files.
3) 3.1 What Makefiles Contain (reread)
4) Makefiles contain five kinds of things: explicit rules, implicit rules,
   variable definitions, directives, and comments.
5) The default makefile names GNUmakefile, makefile and Makefile are not checked
   automatically if you specify ‘-f’ or ‘--file’.
6) If you want make to simply ignore a makefile which does not exist or cannot
   be remade, with no error message, use the -include directive instead of include,
   like this: `-include filenames…'
7) However, if a default makefile does not exist but can be created by running
   make rules... (don't understand)
8) The wildcard characters in make are ‘*’, ‘?’ and ‘[…]’, the same as in the
   	Bourne shell
9) The automatic variable ‘$?’ is used to print only those files that have changed;
   see Automatic Variables.
10) 
11) 
12) 
