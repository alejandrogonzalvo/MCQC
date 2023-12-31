# MCQC
Openqasm3 compiler for multicore architectures

### Input:

```cpp
OPENQASM 3.0;

/*
 * quantum ripple-carry adder
 * Cuccaro et al, quant-ph/0410184
 */
include "stdgates.inc";

gate majority a, b, c {
    cx c, b;
    cx c, a;
    ccx a, b, c;
}

gate unmaj a, b, c {
    ccx a, b, c;
    cx c, a;
    cx a, b;
}

qubit[1] cin;
qubit[4] a;
qubit[4] b;
qubit[1] cout;
bit[5] ans;
uint[4] a_in = 1;  // a = 0001
uint[4] b_in = 15; // b = 1111
// initialize qubits
reset cin;
reset a;
reset b;
reset cout;

// set input states
for uint i in [0: 3] {
  if(bool(a_in[i])) x a[i];
  if(bool(b_in[i])) x b[i];
}
// add a to b, storing result in b
majority cin[0], b[0], a[0];
for uint i in [0: 2] { majority a[i], b[i + 1], a[i + 1]; }
cx a[3], cout[0];
for uint i in [2: -1: 0] { unmaj a[i],b[i+1],a[i+1]; }
unmaj cin[0], b[0], a[0];
measure b[0:3] -> ans[0:3];
measure cout[0] -> ans[4];
```

### Output:

__*out.qasm*__
```cpp
OPENQASM 3.0 ; 
include "stdgates.inc" ; 
qubit [ 1 ] cin ; 
qubit [ 4 ] a ; 
qubit [ 4 ] b ; 
qubit [ 1 ] cout ; 
bit [ 5 ] ans ; 
uint [ 4 ] a_in = 1 ; 
uint [ 4 ] b_in = 15 ; 
reset cin ; 
reset a ; 
reset b ; 
reset cout ; 
if ( bool ( a_in [ 2 ] ) ) x a [ 2 ] ; 
if ( bool ( a_in [ 1 ] ) ) x a [ 1 ] ; 
if ( bool ( a_in [ 0 ] ) ) x a [ 0 ] ; 
if ( bool ( b_in [ 2 ] ) ) x b [ 2 ] ; 
if ( bool ( b_in [ 1 ] ) ) x b [ 1 ] ; 
if ( bool ( b_in [ 0 ] ) ) x b [ 0 ] ; 
cx a [ 0 ] , b [ 0 ] ; 
cx a [ 0 ] , cin [ 0 ] ; 
ccx cin [ 0 ] , b [ 0 ] , a [ 0 ] ; 
cx a [ 2 ] , b [ 2 ] ; 
cx a [ 1 ] , b [ 1 ] ; 
cx a [ 2 ] , a [ 1 ] ; 
cx a [ 1 ] , a [ 0 ] ; 
ccx a [ 1 ] , b [ 2 ] , a [ 2 ] ; 
ccx a [ 0 ] , b [ 1 ] , a [ 1 ] ; 
cx a [ 3 ] , cout [ 0 ] ; 
ccx a [ 1 ] , b [ 2 ] , a [ 2 ] ; 
ccx a [ 2 ] , b [ 3 ] , a [ 3 ] ; 
cx a [ 2 ] , a [ 1 ] ; 
cx a [ 3 ] , a [ 2 ] ; 
cx a [ 1 ] , b [ 2 ] ; 
cx a [ 2 ] , b [ 3 ] ; 
ccx cin [ 0 ] , b [ 0 ] , a [ 0 ] ; 
cx a [ 0 ] , cin [ 0 ] ; 
cx cin [ 0 ] , b [ 0 ] ; 
measure b [ 0 : 3 ] -> ans [ 0 : 3 ] ; 
measure cout [ 0 ] -> ans [ 4 ] ; 
```

__*shell output*__
```shell
Qubit map:
a [ 0 ] : a [ 1 ]  b [ 0 ]  b [ 1 ]  cin [ 0 ]  
a [ 1 ] : a [ 0 ]  a [ 2 ]  b [ 1 ]  b [ 2 ]  
a [ 2 ] : a [ 1 ]  a [ 3 ]  b [ 2 ]  b [ 3 ]  
a [ 3 ] : a [ 2 ]  b [ 3 ]  cout [ 0 ]  
b [ 0 ] : a [ 0 ]  cin [ 0 ]  
b [ 1 ] : a [ 0 ]  a [ 1 ]  
b [ 2 ] : a [ 1 ]  a [ 2 ]  
b [ 3 ] : a [ 2 ]  a [ 3 ]  
cin [ 0 ] : a [ 0 ]  b [ 0 ]  
cout [ 0 ] : a [ 3 ]  
```