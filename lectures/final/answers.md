Q 1.1

 op (6 bits)   target (26 bits)
 |       2 |               256|
 
or in binary 
 | 0000 10  | 00 0000 0000 0000 0001 0000 0000 |


Q 1.2

0b 0001  00 | 01000 00100 1111 1111 1111 0001 |

since opcode is 4 it's a beq (branch on equal instruction) 
it compares two registers 8 and 4 which are t0 and a0 and
if they are equal jumps to 0b1111_1111_1111_000100 (note the 
two 00 at the end). Since the number starts with 1, it's a 
negative number and so we convert it back from two's complement
by inverting the bits and adding 1, or

0b1111_1111_1111_000100 -> 0b111011 + 1 -> -60

So it jumps back 60 bytes from PC+4 


Q1.3

The function adds two integers passed as arguments
in registers $4 and $5 (a0 and a1) and returns the 
result in register $2 (v0). Since the CPU has a branch delay 
slot (i.e., the instruction after the branch is always executed, 
the add instruction is placed into that slot, right after the 
jump via register instruction that implements return. 


Q2

The shift unit shifts the target of the jump instruction by two 
to account for the fact that all jump targets are word-aligned. 


Q3.1

Since the branch is at address 0x00400040 or 0b100000000 0000 0001 0000 00
the predictor can take 9 bits as an index into predictor table. Here
we need 10 bits to identify one of the 1024 entries. Since it doesn't make
sence to take the lowest two bits (the branches are aligned at word boundary
the next 10 bits are a good candidate). Hence the branch can map to the 
entry  0b10000 or entry 16. Note, students are free to choose other mappig 
schemes. 

Q3.2

During the first invocation of add, the branch was taken 9 times (the predictor 
moved to the strongly taken state) and then not taken once (the predictor moved 
to the weakly taken state). So when add() is executed the second time, the 
predictor is in this "weakly taken" state and the branch is correctly predicted. 

Q4

Sequential access pattern brings cache lines into the cache, however first access to 
the cache line is a miss. A cache line contains 16 integer elements, hence, one out 
of 16 iterations will experience two (one for load from b[] and one for store into 
a[]) misses 200 cycles each. 

So the first iteration of the loop will take 407 cycles

```
$L2:
        lw      $3,0($2)   200
        nop                201 
        addiu   $3,$3,2    203
        sw      $3,0($4)   404
        addiu   $2,$2,4    405
        bne     $2,$5,$L2  406
        addiu   $4,$4,4    407
```

Subsequent 15 iterations will take only 7 cycles (we assume that the array is 
cache line aligned in memory) or a total of 15 x 7 = 105 cycles


```
$L2:
        lw      $3,0($2)   408
        nop                409 
        addiu   $3,$3,2    410
        sw      $3,0($4)   411
        addiu   $2,$2,4    412
        bne     $2,$5,$L2  413
        addiu   $4,$4,4    414
```

This pattern will happen 1_000_000/16 = 62500 times, so the total time for the loop is
62500x(105 + 407) = 32000000 cycles. 

Q5.1

Before sending the message, the sender checks if the cache line is `empty` so
it tries to read the cache line with a load instuction. The receiver was the
last to update the message by writing `empty` into it so it is in the `M` state
in the receiver's cache, and in the `I` state on the sender's state. This
generates a `Rd X` transaction on the coherence bus (T1), which brings the
cache line into its cache in a `shared` state. 

Note, that it can happen that receiver hasn't even read the message, or read it
but hasn't updated it. In this case the cache line is either in `M` state
(status is `ready`) or in `S` state (status is `ready`) in the sender's cache
and no transaction is generated. 

If the status is `empty` the sender is ready to write a message into the cache
line, it perorms a store into it, generating an `Upgrade X` request (T2) since
the cache line is already in it's cache in the `S` state. We can assume that
both message data and status are updated with either one store, or two stores
back to back in such manner that cache line is not evicted form the cache
between the stores (this is realistic). After the write the cache line is in
the modified state `M` on the sender's side, and in invalid `I` stat on the
receiver side. 

The receiver tries to receive the message and reads it with a load instruction
that generates a `Rd X` transaction (T3). Again, the cache line is in the
modified state `M` on the sender's side, and in invalid `I` stat on the
receiver side.  Sender's cache responds with the data and performs a write-back
to memory, both caches have the cache line in a shared `S` state. When the
message is processed, receiver acknowledges receive by writing `empty` into the
cache line this generates an `Upgrade X` on the bus (T4) (sender's cache
invalidates the cache line, receiver's cache upgrades cache line's state to
`M`). 

For the next message everything starts again in the same manner. 

Q5.2

The answer to Q5.1 has 4 cache-line transactions (T1, T2, T3 and T4). Each
transaction takes 80 cycles, so the minimal latency of sending a message is
80x4 = 320 cycles. 
