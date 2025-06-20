#ifndef _COURSE_H_         // Do not modify. These preprocessor directives ensure that
#define _COURSE_H_         // we can include student.h in course.c and vice versa.

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "student.h"

// maximum capacity that a course can have, actual capacity may be lower
extern const int MAX_CAPACITY;
// maximum length of a course's title (including zero byte)
extern const int MAX_COURSE_TITLE_LENGTH;
// length of a course subject code (including zero byte), e.g., "CS"
extern const int SUBJECT_LENGTH;

struct course {
  char subject_code[3];
  int course_number;
  char course_title[128];
  int course_capacity;
  int course_current_capacity;
  struct student *enrolled_students[128];
};

//
// DO NOT MODIFY THE FUNCTION DECLARATIONS BELOW 
//

// course_init(c, subject, number, title, capacity) initializes c with the
//   given subject code, course number, title, and capacity
// requires: c, subject, and title must be valid
//           subject must have SUBJECT_LENGTH - 1 letters
//           number must be positive
//           title must be shorter than MAX_COURSE_TITLE_LENGTH
//           capacity cannot be larger than MAX_CAPACITY
// effects: modifies *c
void course_init(struct course *c, const char subject[], const int number,
		 const char title[], const int capacity);

// course_subject(c) returns the subject code of c
// requires: c must be valid
const char* course_subject(const struct course *c);

// course_number(c) returns the course number of c
// requires: c must be valid
int course_number(const struct course *c);

// course_is_full(c) returns true if c is full and false otherwise
// requires: c must be valid
bool course_is_full(const struct course *c);

// course_enroll(c, s) adds s to the students enrolled in c
// requires: c and s must be valid
//           s is not already enrolled in c (not asserted)
//           c has space available (not asserted)
// effects: modifies *c
void course_enroll(struct course *c, struct student *s);

// course_drop(c, s) removes s from the students enrolled in c
// requires: c and s must be valid
//           s is enrolled in c (not asserted)
//           grade of s for c is not yet set (not asserted)
// effects: modifies *c
void course_drop(struct course *c, struct student *s);

// course_print(c) prints information about c, the students
//   enrolled in c, and the average grade of all assigned
//   grades (only if there is at least one assigned grade),
//   see *.expect files for examples
// requires: c must be valid
// effects: creates output
void course_print(const struct course *c);

#endif 
