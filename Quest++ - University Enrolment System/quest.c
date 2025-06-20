#include "quest.h"

//
// DO NOT MODIFY THESE CONSTANTS
//
// see quest.h for documentation
const int MAX_STUDENTS = 512;   
// see quest.h for documentation
const int MAX_COURSES = 128;
// id of first registered student; compute further ids by increasing by one
//   for each additional registration
static const int BASE_ID = 20000000;

// see quest.h for documentation
void quest_init(struct quest *q) {
  
  assert(q);
  q->nof_courses = 0;
  q->nof_students = 0;
}

// see quest.h for documentation
int quest_register_student(struct quest *q, const char *name) {
  
  assert(q);
  assert(name);
  assert(q->nof_students < MAX_STUDENTS);

  int i = q->nof_students;
  int id = BASE_ID + q->nof_students;

  student_init(&(q->students[i]), id, name);
  q->nof_students++;
  return id;
}

// see quest.h for documentation
bool quest_create_course(struct quest *q, const char subject[], 
			 const int number, const char title[],
			 const int capacity) {

  assert(q);
  assert(subject);
  assert(title);
  assert(q->nof_courses < MAX_COURSES);

  int i = 0;
  for (; i < q->nof_courses; i++) {
    int crs_num = course_number(&(q->courses[i]));
    if (crs_num == number && 
      strcmp(course_subject(&(q->courses[i])), subject) == 0) {
        printf("CREATE FAILURE: course already exists\n");
        return false;
    }
  }

  course_init(&(q->courses[i]), subject, number, title, capacity);
  q->nof_courses++;
  return true;
}

// see quest.h for documentation
bool quest_enroll(struct quest *q, const int id,
		  const char subject[], const int crs_number) {
  
  assert(q);
  assert(subject);

  bool student_exists = false;
  int i = 0;

  for (; i < MAX_STUDENTS; i++) {
    if (student_id(&(q->students[i])) == id) {
      student_exists = true;
      break;
    }
  }
  
  if (!student_exists) {
    printf("ENROLLMENT FAILURE: student does not exist\n");
    return false;
  }

  bool course_exists = false;
  int j = 0;

  for (; j < MAX_COURSES; j++) {
    int crs_num = course_number(&(q->courses[j]));
    if (crs_num == crs_number && 
      strcmp(course_subject(&(q->courses[j])), subject) == 0) {
        course_exists = true;
        break;
      }
  }

  if (!course_exists) {
    printf("ENROLLMENT FAILURE: course does not exist\n");
    return false;
  } 

  if (course_is_full(&(q->courses[j]))) {
    printf("ENROLLMENT FAILURE: course is full\n");
    return false;
  }

  if (student_is_enrolled(&(q->students[i]), &(q->courses[j]))) {
    printf("ENROLLMENT FAILURE: student is already enrolled in course\n");
    return false;
  }
  
  student_enroll(&(q->students[i]), &(q->courses[j]));
  course_enroll(&(q->courses[j]), &(q->students[i]));
  return true;

  //
  // ADD CODE
  // Use matching error message in case of error:
  //   "ENROLLMENT FAILURE: student does not exist\n"
  //   "ENROLLMENT FAILURE: course does not exist\n"
  //   "ENROLLMENT FAILURE: course is full\n"
  //   "ENROLLMENT FAILURE: student is already enrolled in course\n"
  //
 
}

// see quest.h for documentation
bool quest_drop(struct quest *q, const int id,
		const char subject[], const int crs_number) {

  assert(q);
  assert(subject);

  bool student_exists = false;
  int i = 0;

  for (; i < MAX_STUDENTS; i++) {
    if (student_id(&(q->students[i])) == id) {
      student_exists = true;
      break;
    }
  }
  
  if (!student_exists) {
    printf("DROP FAILURE: student does not exist\n");
    return false;
  }

  bool course_exists = false;
  int j = 0;

  for (; j < MAX_COURSES; j++) {
    int crs_num = course_number(&(q->courses[j]));
    if (crs_num == crs_number && 
      strcmp(course_subject(&(q->courses[j])), subject) == 0) {
        course_exists = true;
        break;
      }
  }

  if (!course_exists) {
    printf("DROP FAILURE: course does not exist\n");
    return false;
  }

  if (!student_is_enrolled(&(q->students[i]), &(q->courses[j]))) {
    printf("DROP FAILURE: student is not enrolled in course\n");
    return false;
  }

  if (student_grade(&(q->students[i]), &(q->courses[j])) != GRADE_NOT_SET) {
    printf("DROP FAILURE: student's grade is already set\n");
    return false;
  }

  student_drop(&(q->students[i]), &(q->courses[j]));
  course_drop(&(q->courses[j]), &(q->students[i]));
  return true;
  //
  // ADD CODE
  // Use matching error message in case of error:
  //   "DROP FAILURE: student does not exist\n"
  //   "DROP FAILURE: course does not exist\n"
  //   "DROP FAILURE: student is not enrolled in course\n"
  //   "DROP FAILURE: student's grade is already set\n"
  //
}

// see quest.h for documentation
bool quest_submit_grades(struct quest *q, const char subject[],
			 const int crs_number, const int student_ids[],
			 const int grades[], const int len) {

  assert(q);
  assert(subject);
  assert(student_ids);
  assert(grades);


  bool course_exists = false;
  int j = 0;

  for (; j < MAX_COURSES; j++) {
    int crs_num = course_number(&(q->courses[j]));
    if (crs_num == crs_number && 
      strcmp(course_subject(&(q->courses[j])), subject) == 0) {
        course_exists = true;
        break;
      }
  }

  if (!course_exists) {
    printf("SUBMIT FAILURE: course does not exist\n");
    return false;
  }

  for (int i = 0; i < len; i++) {

    bool student_exists = false;
    int k = 0;

    for (; k < MAX_STUDENTS; k++) {
      if (student_id(&(q->students[k])) == student_ids[i]) {
        student_exists = true;
        break;
      }
    }

    if (!student_exists) {
      printf("SUBMIT FAILURE: student does not exist\n");
      return false;
    } 

    if (!student_is_enrolled(&(q->students[k]), &(q->courses[j]))) {
      printf("SUBMIT FAILURE: student is not enrolled in course\n");
      return false;
    }

    int grade = grades[i];

    if (!(grade >= 0 && grade <= 100)) {
      printf("SUBMIT FAILURE: invalid grade\n");
      return false;
    }
  }

  for (int i = 0; i < len; i++) {

    int k = 0;

    for (; k < MAX_STUDENTS; k++) {
      if (student_id(&(q->students[k])) == student_ids[i]) {
        break;
      }
    }

    student_set_grade(&(q->students[k]), &(q->courses[j]), grades[i]);
  }

  return true;
  //
  // ADD CODE
  // Use matching error message in case of error:
  //   "SUBMIT FAILURE: course does not exist\n"
  //   "SUBMIT FAILURE: student does not exist\n"
  //   "SUBMIT FAILURE: student is not enrolled in course\n"
  //   "SUBMIT FAILURE: invalid grade\n"
  //
}

bool quest_print_student(struct quest *q, const int id) {

  assert(q);

  bool student_exists = false;
  int i = 0;

  for (; i < MAX_STUDENTS; i++) {
    if (student_id(&(q->students[i])) == id) {
      student_exists = true;
      break;
    }
  }
  
  if (!student_exists) {
    printf("PRINT STUDENT FAILURE: student does not exist\n");
    return false;
  }

  student_print(&(q->students[i]));
  return true;
  //
  // ADD CODE
  // Use this error message in case of error:
  //   "PRINT STUDENT FAILURE: student does not exist\n"
  
}

bool quest_print_course(struct quest *q, const char subject[],
			const int number) {
  
  assert(q);
  assert(subject);

  bool course_exists = false;
  int j = 0;

  for (; j < MAX_COURSES; j++) {
    int crs_num = course_number(&(q->courses[j]));
    if (crs_num == number && 
      strcmp(course_subject(&(q->courses[j])), subject) == 0) {
        course_exists = true;
        break;
      }
  }

  if (!course_exists) {
    printf("PRINT COURSE FAILURE: course does not exist\n");
    return false;
  }

  course_print(&(q->courses[j]));
  return true;
  //
  // ADD CODE
  // Use this error message in case of error:
  //   "PRINT COURSE FAILURE: course does not exist\n"
  //
}
