# C-Scheme-Interpreter
a simple Scheme Interpreter writen by C++

## Features

-- tail call optimization, implemented by the machine(virtual) level control
-- garbage collection, supported by some modern C++ features, such as smart pointer(C++11), any container(C++17)

## Compile 

```
make
```

# Test

## lambda

```
>> (define x 2)
<< ()
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
<< ()
>> f
<< (lambda_tag (x y) (+ x y) ...env...)
```

## cons, car, cdr

```
>> (define cons (lambda (x y) 
    (lambda (c)
      (if (= c 0) x y))))
<< ()
>> (define car (lambda (x)
    (x 0)))
<< ()
>> (define cdr (lambda (x)
    (x 1)))
<< ()
>> (define x (cons 100 200))
<< ()
>> (cdr x)
<< 200
>> (car x)
<< 100
```
