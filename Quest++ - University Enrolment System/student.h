#ifndef _STUDENT_H_     // Do not modify. These preprocessor directives ensure that
#define _STUDENT_H_     // we can include course.h in student.c and vice versa.

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "course.h"

// use as default value for a student's grade in a course as long as grade
//   is not explicitly set
extern const int GRADE_NOT_SET;
// maximum number of courses a student can be registered in
extern const int MAX_COURSES_PER_STUDENT;
// maximum length of a student's name (including zero byte)
extern const int MAX_STUDENT_NAME_LENGTH;

struct student {
  int student_id;
  char student_name[32];
  struct course *courses_registered[32];
  int grades[32];
  int enrolled_course_amount;
};

//
// DO NOT MODIFY THE FUNCTION DECLARATIONS BELOW 
//

// student_init(s, id, name) initializes s with the given student id and
//   student name
// requires: s and name must be valid
//           id must be positive
//           name must be shorter than MAX_STUDENT_NAME_LENGTH
void student_init(struct student *s, const int id, const char name[]);

// student_id(s) returns the student id of s
// requires: s must be valid
int student_id(const struct student *s);

// student_name(s) returns the name of s
// requires: s must be valid
char *student_name(struct student *s);

// student_is_enrolled(s, c) returns true if s is enrolled in c and false
//   otherwise
// requires: s and c must be valid
bool student_is_enrolled(const struct student *s, const struct course *c);

// student_enroll(s, c) adds c to the courses that s is enrolled in
// requires: s and c must be valid
//           s is not already enrolled in c (not asserted)
//           c has space available (not asserted)
//           s is already enrolled in fewer than MAX_COURSES_PER_STUDENT
// effects: modifies *s
void student_enroll(struct student *s, struct course *c);

// student_drop(s, c) removes c from the courses s is enrolled in
// requires: s and c must be valid
//           s is enrolled in c (not asserted)
//           grade of s for c is not yet set (not asserted)
// effects: modifies *s
void student_drop(struct student *s, struct course *c);

// student_set_grade(s, c, grade) sets grade of s for c to grade
// requires: s and c must be valid
//           s is enrolled in c (not asserted)
//           grade is between 0 and 100
// effects: modifies *s
void student_set_grade(struct student *s, struct course *c, int grade);

// student_grade(s c) returns grade of s for c or GRADE_NOT_SET if
//   grade not yet set
// requires: s and c are valid
//           s is enrolled in c (not asserted)
int student_grade(const struct student *s, const struct course *c); 

// student_print(s) prints name and student id of s, their
//   registered courses and grade in a course if set already,
//   see *.expect files for examples
// requires: s must be valid
// effects: creates output
void student_print(const struct student *s);

#endif
