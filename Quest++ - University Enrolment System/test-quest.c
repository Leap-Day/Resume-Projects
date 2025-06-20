#include "quest.h"

// The following commands are available:
//   
// c <subject code> <number> <capacity> <title>
// Create a course with given subject code, number, title, and capacity
// Example: c CS 136 32 Elementary Algorithm Design and Data Abstraction
// 
// s <student name>
// Register student with given name. Id of first registered student will be 20000000.
// Example: s Alice Smith
//
// p <subject code> <number>
// Print information about course with given subject code and number
// Example: p CS 136
//
// P <student id>
// Print information about student with given id.
// Example: P 20000000
//
// e <student id> <subject code> <number>
// Enroll student with given id in given course
// Example: e 20000000 CS 136
//
// d <student_id> <subject code> <number>
// Drop student with given id from given course
// Example: d 20000000 CS 136
//
// g <subject code> <number> <student id 1> <grade 1> <student id 2> <grade 2> ... <student id n> <grade n> end
// Submit the grades of n students with given ids for given course
// Example: g CS 136 20000000 99 end

int main(void) {
  struct quest q = { 0 };
  quest_init(&q);  
  char cmd;
  while(1) {
    char subject[3] = "";
    int number = 0;
    char title[128] = "";
    int capacity = 0;
    char name[32] = "";
    int id = 0;
    int res = scanf(" %c", &cmd);
    if (cmd == 'q' || res != 1) {
      break;
    }
    if (cmd == 'c') {
      if (scanf("%s %d %d %127[^\n]\n", subject, &number, &capacity, title) != 4) {
        break;
      }
      quest_create_course(&q, subject, number, title, capacity);
    } else if (cmd == 'p') {
      if (scanf("%s %d", subject, &number) != 2) {
        break;
      }
      quest_print_course(&q, subject, number);
    } else if (cmd == 's') {
      if (scanf(" %31[^\n]\n", name) != 1) {
        break;
      }
      quest_register_student(&q, name);
    } else if (cmd == 'P') {
      if (scanf("%d", &id) != 1) {
        break;
      }
      quest_print_student(&q, id);
    } else if (cmd == 'e') {
      if (scanf("%d %s %d", &id, subject, &number) != 3) {
        break;
      }
      quest_enroll(&q, id, subject, number);
    } else if (cmd == 'd') {
      if (scanf("%d %s %d", &id, subject, &number) != 3) {
        break;
      }
      quest_drop(&q, id, subject, number);
    } else if (cmd == 'g') {
      if (scanf("%s %d", subject, &number) != 2) {
        break;
      }
      int ids[32] = { 0 };
      int grades[32] = { 0 };
      int i = 0;
      for (; i < 32; i++) {
        char num[1024];
        if (scanf("%s", num) != 1) {
          break;
        }
        if (strcmp(num, "end") == 0) {
          break;
        }
        if (sscanf(num, "%d", &ids[i]) != 1) {
          i = -1;
          break;
        }
        if (scanf("%d", &grades[i]) != 1) {
          i = -1;
          break;
        }
      }
      if (i == -1) {
        break;
      }
      quest_submit_grades(&q, subject, number, ids, grades, i);
    }
    else {
      break;
    }
  }
}

