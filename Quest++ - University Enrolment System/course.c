#include "course.h"

//
// DO NOT MODIFY THESE CONSTANTS
//
// see course.h for documentation
const int MAX_CAPACITY = 128;
// see course.h for documentation
const int MAX_COURSE_TITLE_LENGTH = 128;
// see course.h for documentation
const int SUBJECT_LENGTH = 3;

// see course.h for documentation
void course_init(struct course *c, const char subject[], const int number,
		 const char title[], const int capacity) {

  assert(c);
  assert(subject);
  assert(title);
  assert(strlen(subject) == SUBJECT_LENGTH - 1);
  assert(number > 0);
  assert(strlen(title) < MAX_COURSE_TITLE_LENGTH);
  assert(capacity <= MAX_CAPACITY);

  strcpy(c->subject_code, subject);
  c->course_number = number;
  strcpy(c->course_title, title);
  c->course_capacity = capacity;
  c->course_current_capacity = 0;

  for (int i = 0; i < MAX_CAPACITY; i++) {
    c->enrolled_students[i] = NULL;
  }
}

// see course.h for documentation
const char* course_subject(const struct course *c) {
  
  assert(c);

  return c->subject_code;
}

// see course.h for documentation
int course_number(const struct course *c) {
  
  assert(c);

  return c->course_number;
}

// see course.h for documentation
bool course_is_full(const struct course *c) {
  
  assert(c);

  return (c->course_capacity == c->course_current_capacity);  
}

// see course.h for documentation
void course_enroll(struct course *c, struct student *s) {
  
  assert(c);
  assert(s);

  int course_capacity = c->course_capacity;

  for (int i = 0; i < course_capacity; i++) {
    if (c->enrolled_students[i] ==  NULL) {
      c->enrolled_students[i] = s;
      break;
    }
  }
  c->course_current_capacity++;
}

// see course.h for documentation
void course_drop(struct course *c, struct student *s) {
  
  assert(c);
  assert(s);

  int course_capacity = c->course_capacity;

  for (int i = 0; i < course_capacity; i++) {
    if (c->enrolled_students[i] ==  s) {
      c->enrolled_students[i] = NULL;
      break;
    }
  }
  c->course_current_capacity--;
}

// see course.h for documentation
void course_print(const struct course *c) {
  
  assert(c);

  int assigned_grade_sum = 0;
  int assigned_grades = 0;
  int course_cap = c->course_capacity;
  

  for (int i = 0; i < course_cap; i++) {
    if (c->enrolled_students[i] == NULL) {
      continue;
    }
    int grade = student_grade(c->enrolled_students[i], c);
    if (grade >= 0) {
      assigned_grade_sum += grade;
      assigned_grades++; 
    }
  }

  printf("%s %d (%s)", c->subject_code, c->course_number, c->course_title);
  
  if (c->course_current_capacity == 0) {
    printf(" has no students enrolled");

  } else {
    printf(" has %d student(s) enrolled:", c->course_current_capacity);
    int printed = 0;
    for (int i = 0; i < c->course_capacity; i++) {
      if (c->enrolled_students[i] != NULL) {
        printf(" %s", student_name(c->enrolled_students[i]));
        printed++;
        if (printed < c->course_current_capacity) {
          printf(",");
        }
      }
    }
  }

  if (assigned_grades > 0) {
    printf(" - mean grade is %d", assigned_grade_sum / assigned_grades);
  }
  printf("\n");
}
