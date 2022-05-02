## general:
no header files
statically typed
traits like rust

## basic types

### integers
sign * size
i/u    0-64

### floating point
f * size
    32 or 64

### strings?
string
ptr + len?
or is that just a slice?

## structs
- initial values in structs

## variables
declaration
```
let x = ...
let x: type = ...
```

## optional types
`?u8`

## error types

## functions/lambdas
```
fn foo (a: u32, b: u32, c: f64) -> u32
==
let foo = (a: u32, b: u32, c: f64) -> u32
```

named and default arguments

## Return values
```
let foo = () -> (int, int, int) {

}

let (x, y, z) = foo();
```
