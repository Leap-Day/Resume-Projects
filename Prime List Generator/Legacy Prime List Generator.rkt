;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname hof-hills) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor mixed-fraction #f #t none #f () #t)))
;; Legacy Prime List Generator
(define (primes n)
  (filter (lambda (x) (foldr (lambda (y rror) (and (> (remainder x y) 0) rror)) true
                             (build-list (- x 2) (lambda (i) (+ i 2))))) 
          (build-list (- n 1) (lambda (i) (+ i 2)))))

(check-expect (primes 10) (list 2 3 5 7))
(check-expect (primes 30) (list 2 3 5 7 11 13 17 19 23 29))
(check-expect (primes 33) (list 2 3 5 7 11 13 17 19 23 29 31))
(check-expect (primes 2) (list 2))
(check-expect (primes 7) (list 2 3 5 7))
(check-expect (primes 1) empty)





    