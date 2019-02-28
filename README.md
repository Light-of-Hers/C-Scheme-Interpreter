# C-Scheme-Interpreter
a simple Scheme Interpreter writen by C++

## Features

+ tail call optimization, implemented by the machine(virtual) level control
+ garbage collection, supported by some modern C++ features, such as smart pointer(C++11), any container(C++17)

## Not support

+ some builtin procedure, especially 'apply' (to implement 'map' and many other procedure)
+ number system (only supporting 64-bit float now)
+ ......

## Compile & Run 

```
make && ./a
```

# Test

## lambda

```
>> (define x 2)
<< ok
>> ((lambda (f)
           ((lambda (x)     
                    (f 3))
            4))
   (lambda (y) (+ x y)))
<< 5
```

The representation of `lambda` and `environment` is shown below.

```
>> (define f (lambda (x y) (+ x y)))
<< ok
>> f
<< (**procedure** (x y) ((+ x y)) (**Frame** (+) (-) (*) (/) (%) (cons) (car) (cdr) (set-car!) (set-cdr!) (=) (<) (>) (<=) (>=) (not) (eq?) (null) (string?) (boolean?) (number?) (symbol?) (pair?) (x) (f)))
```

## cons, car, cdr

```
>> (define x (cons 100 200))
<< ok
>> (cdr x)
<< 200
>> (car x)
<< 100
>> (define lst '(1 2 3 4))
<< ok
>> lst
<< (1 2 3 4)
>> (car lst)
<< 1
>> (cdr lst)
<< (2 3 4)
```
## List Argument
```
>> (define (f first . rest) rest)
<< ok
>> (f 1 2 3 4)
<< (2 3 4)
```
