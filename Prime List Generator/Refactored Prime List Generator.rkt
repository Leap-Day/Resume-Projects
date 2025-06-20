;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname |Refactored Prime List Generator|) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor mixed-fraction #f #t none #f () #t)))
;; (primes n) generates a list of primes from 2 to n

;; Examples:
(check-expect (primes 10) (list 2 3 5 7))

;; primes: Nat -> (listof Nat)
(define (primes n)
  (local [(define candidatelist (cond [(< n 2) empty] ; list of potential primes
                                      [else (build-list (- n 1) (lambda (x) (+ 2 x)))]))
          ;; (prime/filter candlist) produces a list where non primes are filtered out from candlist
          ;; prime/filter: (listof Nat) -> (listof Nat)
          (define (prime/filter candlist)
            (cond [(empty? candlist) empty]
                  [else (cons (first candlist)
                              (prime/filter (filter
                                             (lambda (x) (not (= (remainder x (first candlist)) 0)))
                                             candlist)))]))]
                        
    (prime/filter candidatelist)))


;; Tests
(check-expect (primes 30) (list 2 3 5 7 11 13 17 19 23 29))
(check-expect (primes 33) (list 2 3 5 7 11 13 17 19 23 29 31))
(check-expect (primes 2) (list 2))
(check-expect (primes 7) (list 2 3 5 7))
(check-expect (primes 1) empty)
(check-expect (primes 0) empty)

