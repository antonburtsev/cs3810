### Branch counters

Report the cycles for taken, predicted, and mispredicted branches and provide a short 
explanation for the results you see:

```
branchalways
"PAPI_BR_CN":"XXX",
"PAPI_BR_MSP":"XXX",
"PAPI_BR_PRC":"XXX",

branchhalf
"PAPI_BR_CN":"XXX",
"PAPI_BR_MSP":"XXX",
"PAPI_BR_PRC":"XXX",

Explanation: <text>
```
### Impact of branch counters

Report the cycles per iteration for both predicted and 50% predicted code. Report the numbers of issued and committed instructions. 

```
branchalways
cycles per iteration:"XXX",
"PAPI_TOT_IIS":"XXX",
"PAPI_TOT_INS":"XXX",

branchhalf
cycles per iteration:"XXX",
"PAPI_TOT_IIS":"XXX",
"PAPI_TOT_INS":"XXX",

Explanation: <text>
```


### Bandwidth of seqential test 

Compute bandwidth, i.e., MB/s for the sequential test, explain your answer. 


```
Bandwidth: X

Explanation <text>
```

### TLB misses for both tests

Report the number of TLB misses in sequential and random tests and provide a short explanation for the numbers

```
TLB misses sequential: X
TLB misses random: X

Explanation <text>
```

### Performance of the hash table

Perform an experiment to compute the number of cycles needed to compute the hash function, report your 
findings and explain. 

```
Cycles for key_hash: X

Explanation <text>
```

Perform a simple performance analysis for the lookup function, explain what you see. 

```
Cycles per lookup (fits into L3): X
Cycles per lookup (does not fit into L3): X

Explanation <text>
```


