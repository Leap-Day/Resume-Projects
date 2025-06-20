#include "student.h"

//
// DO NOT MODIFY THESE CONSTANTS
//
// see student.h for documentation
const int GRADE_NOT_SET = -1;
// see student.h for documentation
const int MAX_COURSES_PER_STUDENT = 32;
// see student.h for documentation
const int MAX_STUDENT_NAME_LENGTH = 32;

// see student.h for documentation
void student_init(struct student *s, const int id, const char name[]) {
  
  assert(s);
  assert(name);
  assert(id > 0);
  assert(strlen(name) < MAX_STUDENT_NAME_LENGTH);

  s->student_id = id;
  strcpy(s->student_name, name);
  s->enrolled_course_amount = 0;

  for (int i = 0; i < MAX_COURSES_PER_STUDENT; i++) {
    s->courses_registered[i] = NULL;
    s->grades[i] = GRADE_NOT_SET;
  }
}

// see student.h for documentation
int student_id(const struct student *s) {

  assert(s);

  return s->student_id;
}

// see student.h for documentation
char *student_name(struct student *s) {
  
  assert(s);

  return s->student_name;
}

// see student.h for documentation
bool student_is_enrolled(const struct student *s, const struct course *c) {
  
  assert(s);
  assert(c);

  for (int i = 0; i < MAX_COURSES_PER_STUDENT; i++) {
    if (s->courses_registered[i] == c) {
      return true;
    }
  }

  return false;
}

// see student.h for documentation
void student_enroll(struct student *s, struct course *c) {

  assert(s);
  assert(c);
  assert(s->enrolled_course_amount < MAX_COURSES_PER_STUDENT);

  for (int i = 0; i < MAX_COURSES_PER_STUDENT; i++) {
    if (s->courses_registered[i] == NULL) {
      s->courses_registered[i] = c;
      break;
    }
  }
  s->enrolled_course_amount++;
}

// see student.h for documentation
void student_drop(struct student *s, struct course *c) {
  
  assert(s);
  assert(c);

  int i = 0;

  for (; i < MAX_COURSES_PER_STUDENT; i++) {
    if (s->courses_registered[i] == c) {
      s->courses_registered[i] = NULL;
      break;
    }
  }
  s->enrolled_course_amount--;
  s->grades[i] = GRADE_NOT_SET;
}

// see student.h for documentation
void student_set_grade(struct student *s, struct course *c, int grade) {
  
  assert(s);
  assert(c);
  assert(grade >= 0);
  assert(grade <= 100);

  // Set the grade in student struct
  for (int i = 0; i < MAX_COURSES_PER_STUDENT; i++) {
    if (s->courses_registered[i] == c) {
      s->grades[i] = grade;
    }
  }
}

// see student.h for documentation
int student_grade(const struct student *s, const struct course *c) {
  
  assert(s);
  assert(c);

  int grade = GRADE_NOT_SET;

  for (int i = 0; i < MAX_COURSES_PER_STUDENT; i++) {
    if (s->courses_registered[i] == c) {
      grade = s->grades[i];
    }
  }
  return grade;
}

// see student.h for documentation
void student_print(const struct student *s) {
  
  assert(s);

  printf("%s (id: %d) is registered in", s->student_name, s->student_id);
  
  if (s->enrolled_course_amount == 0) {
    printf(" no courses");

  } else {
    int printed = 0;
    
    for (int i = 0; i < MAX_COURSES_PER_STUDENT; i++) {
      if (s->courses_registered[i]) {
        printf(" %s %d", course_subject(s->courses_registered[i]), 
          course_number(s->courses_registered[i]));
        if (s->grades[i] != GRADE_NOT_SET) {
          printf(" (%d)", s->grades[i]);
        }
        printed++;
        if (printed < s->enrolled_course_amount) {
          printf(",");
        }
      }
    }
  }
  printf("\n");
}
