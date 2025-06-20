;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-lambda-reader.ss" "lang")((modname n-queens) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor mixed-fraction #f #t none #f () #t)))
;; Position Helper Function

;; A Position is a (list Nat Nat)
;;
;; A Candidate is a (listof Position)
;; Requires: No two Positions are the same

;; (attacking? p1 p2) produces true if a queen on p1 is attacking a queen on p2

;; Examples:
(check-expect (attacking? '(3 2) '(3 1)) true)
(check-expect (attacking? '(3 3) '(2 2)) true)
(check-expect (attacking? '(3 2) '(1 2)) true)
(check-expect (attacking? '(3 3) '(2 1)) false)


;; attacking?: Position Poistion -> Bool
(define (attacking? p1 p2)
  (local [(define inrow? (= (first p1) (first p2)))
          (define incolumn? (= (second p1) (second p2)))
          (define indiagonal? (= (abs (- (first p1) (first p2))) (abs (- (second p1) (second p2)))))]

  (cond [inrow? true]
        [incolumn? true]
        [indiagonal? true]
        [else false])))


;;****************************************************************


;; Candidate helper function

;; (valid-cand? cand n) produces true if the candidate is a valid soliution or partial solution
;; Produces false otherwise

;; Examples:
(check-expect (valid-cand? '((3 3) (1 2)) 4) true)
(check-expect (valid-cand? '((3 2) (2 0) (1 3) (0 1)) 4) true)
(check-expect (valid-cand? '((1 1) (1 1)) 3) false)


;; valid-cand?: Candidate Nat -> Bool
(define (valid-cand? cand n)
  (local [;; (between? a b c) produces true if b is between a and c. Produces false otherwise.
          ;; between?: Num Num Num -> Bool
          (define (between? a b c) (and (<= a b) (<= b c)))

          ;; (outofbounds? cand n) produces true if any queen in cand is outofbounds of the nxn board
          ;; produces false otherwise
          ;; outofbounds?: Candidate Nat -> Bool
          (define (outofbounds? cand n)
            (cond [(empty? cand) false]
                  [(and (between? 0 (first (first cand)) n)
                        (between? 0 (second (first cand)) n)) (outofbounds? (rest cand) n)]
                  [else true]))]

    (cond [(outofbounds? cand n) false]
          [else (local [;; (valid-cand?/attackingrest current others) produces true if the current
                        ;; queen is attacking the other queens in the list
                        ;; valid-cand?/attackingrest: Poisition Candidate -> Bool
                        (define (valid-cand?/attackingrest current others)
                          (cond [(empty? others) false]
                                [(attacking? current (first others)) true]
                                [else (valid-cand?/attackingrest current (rest others))]))

                        ;; (valid-cand?/attacking cand) produces true if none of the queens
                        ;; are attacking each other in the candidate
                        ;; valid-cand?/attacking: Candidate -> Bool
                        (define (valid-cand?/attacking cand)
                          (cond [(empty? cand) true]
                                ((empty? (rest cand)) true)
                                [(valid-cand?/attackingrest (first cand) (rest cand)) false]
                                [else (valid-cand?/attacking (rest cand))]))]

                  (valid-cand?/attacking cand))])))


;; Tests

(check-expect (valid-cand? '((3 2) (2 3) (1 3) (0 1)) 4) false)
(check-expect (valid-cand? '((3 2) (2 3) (1 3) (0 1)) 3) false)
(check-expect (valid-cand? '((0 0) (1 2) (2 4) (3 1) (4 3)) 5) true)
(check-expect (valid-cand? '((0 1) (1 2) (2 4) (3 1) (4 3)) 5) false)
(check-expect (valid-cand? '((0 0) (1 2) (2 4) (3 1) (4 3)) 3) false)
(check-expect (valid-cand? '((0 0) (1 2) (2 4) (3 1) (4 2)) 5) false)
(check-expect (valid-cand? '((0 0) (1 2) (2 3) (3 1) (4 3)) 5) false)
(check-expect (valid-cand? '((1 0) (3 3) (1 2)) 4) false)


;;****************************************************************

;; Helpers for N-Queen Solver Methods

;; (genenerate-cand potentialpoints cand n) produces the possible neighbours for a cand
;; given a list of potentialpoints based on board size n

;; Examples:
(check-expect (generate-cand '((0 0) (0 1)) '() 2) '(((0 0)) ((0 1))))

;; generate-cand: (listof Position) Candidate Nat -> (listof Candidate)
(define (generate-cand potentialpoints cand n)
            (cond [(empty? potentialpoints) empty]
                  [(valid-cand? (cons (first potentialpoints) cand) n)
                   (cons (cons (first potentialpoints) cand)
                         (generate-cand (rest potentialpoints) cand n))]
                  [else (generate-cand (rest potentialpoints) cand n)]))

;; (generate-points-row rownumber n) produces all the points in a row given the rownumber and
;; the size of the board n.

;; Examples:
(check-expect (generate-points-row 0 3) '((0 0) (0 1) (0 2)))

;; generate-points-row: Nat Nat -> (listof Position)
(define (generate-points-row rownumber n)
  (map (lambda (x) (list rownumber x)) (build-list n (lambda (x) x))))


;;****************************************************************


;; N-Queen Solver Naive

;; (neighbours-naive cand n) produces a list of all the possible neighbour candidates to the
;; provided candidates given the board size n

;; Examples:
(check-expect (neighbours-naive '((0 4) (2 3) (4 2) (1 1) (4 0)) 5) empty)
(check-expect (neighbours-naive '() 2) '(((0 0)) ((0 1)) ((1 0)) ((1 1))))

;; neighbours-naive: Candidate Nat -> (listof Candidate)
(define (neighbours-naive cand n)
  (local [(define (generate-board currentrow boardsize acc)
            (cond [(= boardsize currentrow) acc]
                  [else (generate-board (add1 currentrow) boardsize
                                        (append acc (generate-points-row currentrow boardsize)))]))

          (define nboard (generate-board 0 n empty))]

          ;; (neighbours-naive/build currentrow acc) builds the list of all possible neighbour cands
          ;; neighbours-naive/build: Nat Candidate -> (listof Candidate)
;          (define (neighbours-naive/build currentrow acc)
;            (cond [(= n currentrow) acc]
;                  [else (neighbours-naive/build
;                         (add1 currentrow)
;                         (cons (generate-cand (generate-points-row currentrow n) cand n) acc))]))]
   (generate-cand nboard cand n)))



;; Tests
(check-expect (neighbours-naive '((2 2)) 4) '(((0 1) (2 2)) ((0 3) (2 2)) ((1 0) (2 2))
                                              ((3 0) (2 2))))
(check-expect (neighbours-naive '((1 1)) 5) '(((0 3) (1 1)) ((0 4) (1 1)) ((2 3) (1 1))
                                              ((2 4) (1 1)) ((3 0) (1 1)) ((3 2) (1 1))
                                              ((3 4) (1 1)) ((4 0) (1 1)) ((4 2) (1 1))
                                              ((4 3) (1 1))))
(check-expect (neighbours-naive '((0 0)) 4)'(((1 2) (0 0)) ((1 3) (0 0))
                                             ((2 1) (0 0)) ((2 3) (0 0))
                                             ((3 1) (0 0)) ((3 2) (0 0))))
(check-expect (neighbours-naive '((0 0)) 2) empty)


;;****************************************************************


;; N-Queen Solver By Row

;; (neighbours-row cand n) produces the possible neighbour candidates in the unoccupied
;; row of the lowest index

;; Examples:
(check-expect (neighbours-row '((0 2)) 3) '(((1 0) (0 2))))

;; neighbours-row: Candidate Nat -> (listof Candidate)
(define (neighbours-row cand n)
  (cond [(empty? cand) (generate-cand (generate-points-row 0 n) cand n)]
        [else (generate-cand (generate-points-row (+ 1 (first (first cand))) n) cand n)]))


;; Tests
(check-expect (neighbours-row '((3 2) (2 4) (1 1) (0 3)) 5) '(((4 0) (3 2) (2 4) (1 1) (0 3))))
(check-expect (neighbours-row '((4 2) (3 2) (2 0) (1 1) (0 3)) 6) empty)
(check-expect (neighbours-row '((0 0)) 2) empty)
(check-expect (neighbours-row '() 3) '(((0 0)) ((0 1)) ((0 2))))


;;****************************************************************


;; N-Queens Main Function


(define-struct success (path))
;; A Success is a (make-success (listof Candidate))

(define-struct fail (visited))
;; A Fail is a (make-fail (listof Candidate))

;; A Result is (anyof Success Fail)

;; (n-queens n nbr-fun) produces all the possible solutions for n-queens given the board size n
;; and a function nbr-fun that determines the next neighbour candidates

;; Examples:
(check-expect (n-queens 4 neighbours-row) '(((3 1) (2 3) (1 0) (0 2))
                                            ((3 2) (2 0) (1 3) (0 1))))
(check-expect (n-queens 4 neighbours-naive) '(((0 1) (1 3) (2 0) (3 2))
                                              ((0 2) (1 0) (2 3) (3 1))))

;; n-queens: Nat (Candidate Nat -> (listof Candidate)) -> (listof Candidate))
(define (n-queens n nbr-fun)
  (local [(define (samesol?/doublelist loc1 loc2)
            (cond [(empty? loc1) true]
                  [(samesol?/list (first loc1) loc2) (samesol?/doublelist (rest loc1) loc2)]
                  [else false]))

          (define (samesol?/list cand1 loc)
            (cond [(empty? loc) false]
                  [(samesol? cand1 (first loc)) true]
                  [else (samesol?/list cand1 (rest loc))]))

          (define (samesol? cand1 cand2)
            (cond [(empty? cand1) true]
                  [(member? (first cand1) cand2) (samesol? (rest cand1) cand2)]
                  [else false]))

          (define (n-queens/findpaths/listacc cand visited)
             (cond [(= (length visited) n) (make-success (list visited))]
                  [else (n-queens/findpaths/list cand visited)]))

           (define (n-queens/findpaths/list cand visited)
             (cond [(empty? cand) (make-fail (list visited))]
                   [else (local [(define result (n-queens/findpaths/listacc
                                                 (nbr-fun (first cand) n)
                                                 (cons (first (first cand)) visited)))
                                 (define otherresults (n-queens/findpaths/list
                                                       (rest cand)
                                                       visited))]
                           (cond [(and (success? result) (success? otherresults))
                                  (cond
                                    [(samesol?/doublelist (success-path result)
                                                          (success-path otherresults)) otherresults]
                                    [else (make-success (append (success-path otherresults)
                                                                (success-path result)))])]
                                 [(fail? otherresults) result]
                                 [(fail? result) otherresults]
                                 [else (make-fail visited)]))]))

           (define (n-queens/findpaths cand)
             (local [(define result (n-queens/findpaths/listacc (nbr-fun cand n) empty))]
               (cond [(fail? result) empty]
                     [(success? result) (success-path result)])))]

    (n-queens/findpaths '())))
  





;;****************************************************************


;; Unused code, test helpers and random thought process comments


;; destination needs to be when nbr-fun returns empty
;; check if that path is of length n, if its, cons it on. if its not, discard.
;; start point is empty list. Inherent cycle of sorts.
;;

;; (list (list (list 0 0)) (list (list 0 1)) (list (list 0 2)) (list (list 0 3))) ->
;; (neighbours-row (list (list 0 0)) 4) ->
;; (list (list (list 1 2) (list 0 0)) (list (list 1 3) (list 0 0))) >
;; (neighbours-row (list (list 1 3) (list 0 0)) 4) ->

;(samesol?/list '((2 0) (3 2) (0 1) (1 3)) '(((3 2) (2 0) (1 3) (0 1)) ((2 0) (1 3) (0 1) (3 2))))
;(samesol?/list '((2 4) (3 2) (0 1) (1 3)) '(((3 2) (2 0) (1 3) (0 1)) ((2 0) (1 3) (0 1) (3 2))))
;(samesol? '((2 4) (3 2) (0 1) (1 3)) '((3 2) (2 0) (1 3) (0 1)))


 ;[else (local [(define result (n-queens/findpaths/list cand visited solutions))]
                   ;        (cond [(success? result) (make-success (success-path result))]
                   ;              [(fail? result) result]))]))