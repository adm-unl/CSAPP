This file contains materials for one instance of the attacklab.

Files:

    ctarget

Linux binary with code-injection vulnerability.  To be used for phases
1-3 of the assignment.

    rtarget

Linux binary with return-oriented programming vulnerability.  To be
used for phases 4-5 of the assignment.

     cookie.txt

Text file containing 4-byte signature required for this lab instance.

     farm.c

Source code for gadget farm present in this instance of rtarget.  You
can compile (use flag -Og) and disassemble it to look for gadgets.

     hex2raw

Utility program to generate byte sequences.  See documentation in lab
handout.

Solution: 
Useful commands used:
    objdump -d 'exectuable' - disassembles executable into machine code.
    Can be followed up with grep to only show relevant section.
    e.g., objdump -d ctarget | grep -A30 -B30 'address'

    objdump -t 'executable' - gives symbols useful for finding function
    addresses.

    ./hex2raw < exploitX.txt > exploitX-raw.txt - generates raw binary 
    bytes from hex-formatted string

    g++ -c phaseX.s 
    objdump -d phase.o - compiles machine code and disassembles into 
    byte code for instruction sequences.
    
Part I: Code Injection Attacks
    Level 1: 'exploit.txt' holds exploit string for level 1. Overwrites
    the return address of getbuf function with the address of the 
    touch1 function. 
   
    Level 2: 'exploit2.txt', 'phase2.s' hold the exploit string and 
    injected machine instructions for level 2. Overwrites the return
    address of getbuf function with the address of top of stack where
    code was injected. The injected code sets %rdi (arg1) to cookie
    value, pushes the touch2 function address to the stack and then 
    returns. 

    Level 3: 'exploit3.txt', 'phase3.s' hold the exploit string and 
    injected machine instructions for level 3. Overwrites the return 
    address of getbuf with address where injected code stored as before,
    but also stores the cookie string on the stack above the return 
    address so it is not overwritten when hexmatch is called. Injected 
    code sets %rdi (arg1) to address of this string, pushes the touch3
    function address and then returns. 

Part II: Return-Oriented Programming 
    Level 2: 'exploit4.txt' holds the exploit string for part II, level 2.
    Solution uses two gadgets from the gadget farm. First gadget pops to 
    %rax and returns (byte code 58 90 c3). Hence, in the exploit string, 
    the getbuf return address is overwritten with the address to this 
    gadget and is followed by an 8-byte cookie value that will be popped into 
    %rax. This cookie value is followed by the address to the next gadget 
    that will be returned to, which moves contents of %rax into %rdi and 
    returns (byte code 48 89 c7). The last 8 bytes of the exploit string
    holds the address of the touch2 function which will be returned to by 
    the second gadget.  
