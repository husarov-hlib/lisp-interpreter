# lisp-interpreter

A small Scheme-like Lisp interpreter written from scratch in C.

## Examples

### Fibonacci

```scheme
(define fib
  (lambda (n)
    (if (< n 2)
        n
        (+ (fib (- n 1)) (fib (- n 2))))))

(display (fib 20))
(newline)
```

Output:

```text
6765
```

### Square root with Newton's method

```scheme
(define newton
  (lambda (x n count)
    (if (= count 0)
        x
        (newton (/ (+ x (/ n x)) 2) n (- count 1)))))

(define sqrt
  (lambda (x)
    (newton 1 x 10)))

(display (sqrt 2))
(newline)
```

Output:

```text
1.41421356237309
```

## Error messages

### Accessing an undefined variable

```scheme
(display (+ 1 undefined-variable))
```

Output:

```text
error: symbol not defined
  1 | (display (+ 1 undefined-variable))
    |               ^^^^^^^^^^^^^^^^^^
```

### Passing the wrong number of arguments

```scheme
(define square
  (lambda (x) (* x x)))

(display (square 1 2))
```

Output:

```text
error: expected 1 operands, got 2
  4 | (display (square 1 2))
    |          ^^^^^^^^^^^^
```
