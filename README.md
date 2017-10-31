# xeraph

Xeraph is a self-modifying language, where the source code is the data. It also has a "memory head", used for comparisons. It is also a 2D language, executing commands on a plane.

## Commands


    +   increment the current character
    -   decrement the current character
    l   move left in the memory
    r   move right in the memory
    .   output the current character in memory
    #   output the current memory
    ,   input character
    @   input number
    n   set the memory head to -1 (eof)
    ;   set the head to the current character in memory
    :   set the head to the next character
    D   debug the memory head and cell width
    |   vertical if; go down if the memory head is equal to the pointed cell, up otherwise
    0   set the memory head to 0x00 (null)
    1..9 set to 0x0<A>
    _   horizontal if; go left if the memory head is equal to the pointed cell, right otherwise
    !   skip over the next cell
    ?   skip if the head does not equal the cell
     ...execute next if head and cell are equal