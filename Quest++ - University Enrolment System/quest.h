#include <stdlib.h>
#include "student.h"
#include "course.h"

//
// DO NOT MODIFY THIS FILE
//

// maximum number of students than can be registered
extern const int MAX_STUDENTS;
// maximum number of courses that can be created
extern const int MAX_COURSES;

struct quest {
  struct student students[512];   // registered students
  struct course courses[128];     // created courses
  int nof_students;               // number of registered students
  int nof_courses;                // number of created courses
};

// quest_init(q) initializes q
// effects: modifies *q
void quest_init(struct quest *q);

// quest_register_student(q, name) registers student with given name
//   and returns their student id
// requires: q and name must be valid
//           number of already registered students must be smaller than
//             MAX_STUDENTS
// effects: modifies *q
int quest_register_student(struct quest *q, const char *name);

// quest_create_course(q, subject, number, title, capacity) creates
//   a course with given subject code, course number, title, and capacity,
//   returns false if there is already a course with given subject code
//   and course number and true otherwise
// requires: q, subject, and title must be valid
//           number of already created courses must be smaller than MAX_COURSES
// effects: modifies *q, may create output
bool quest_create_course(struct quest *q, const char subject[],
			 const int number, const char title[],
			 const int capacity);

// quest_enroll(q, student_id, course_subject, course_number) enrolls
//   student with given student id in course with given subject code and number;
//   returns false if student does not exist, if course does not exist, if
//   course is full, or if student is already enrolled in the course; returns
//   true otherwise
// requires: q and course_subject must be valid
// effects: may modify *q, may create output
bool quest_enroll(struct quest *q, const int student_id,
		  const char course_subject[], const int course_number);

// quest_drop(q, student_id, course_subject, course_number) drops student
//   with given student id from course with given subject code and number;
//   returns false if student does not exist, if course does not exist, if
//   student is not enrolled in the course, or if the student's grade for
//   the course is already set; returns true otherwise
// requires: q and course_subject must be valid
// effects: may modify *q, may create output
bool quest_drop(struct quest *q, const int student_id,
		const char course_subject[], const int course_number);

// quest_submit_grades(q, course_subject, course_number, student_ids,
//   grades, len) sets the grades of the len students with the given student
//   ids to the given grades for the course with the given subject code
//   and course number; returns false if the course does not exist, if
//   at least one of the students does not exist, if at least one of the
//   students is not enrolled in the course, or if at least one of the
//   listed grades is not within 0 and 100; returns true otherwise; none
//   of the given grades must be set if false is returned
// requires: q, course_subject, student_ids, and grades must be valid
// effects: may modify *q, may create output
bool quest_submit_grades(struct quest *q, const char course_subject[],
			 const int course_number, const int student_ids[],
			 const int grades[], const int len);

// quest_print_student(q, id) prints information about student id by
//   calling the corresponding function in the student module;
//   returns false if student does not exist and true otherwise;
//   see *.expect files for examples
// requires: q must be valid
// effects: may create output
bool quest_print_student(struct quest *q, const int id);

// quest_print_course(q, subject, number) prints information about
//   the course with the given subject code and course number by
//   calling the corresponding function in the course module;
//   returns false if course does not exist and true otherwise;
//   see *.expect files for examples
// requires: q and subject must be valid
// effects: may create output
bool quest_print_course(struct quest *q, const char subject[],
			const int number);
