 1)  Usually, when you use a single variable for different types, the result is 
      messy code.
 2)  You can assign nil to a global variable to delete it. Lua uses nil as a kind 
      of non-value, to represent the absence of a useful value.
 3)  Strings in Lua are immutable values. You cannot change a character inside
      a string, as you may in C.
 4)  Strings in Lua are subject to automatic memory management, like all Lua
      objects.
 5)  Lua provides automatic conversions between numbers and strings at run time.
-6)  The table type implements associative arrays.
-7)  Tables are the main (in fact, the only) data structuring mechanism in Lua.
-8)  There are no hidden copies or creation of new tables behind the scenes.
      Moreover, you do not have to declare a table in Lua; in fact, there is no way
      to declare one.
 9)   One of the design goals of Lua is to have a tiny core.
 10) 