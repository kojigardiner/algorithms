// Test client for generic stack data type
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "stack.h"

int main() {
  char entry[256];
  int num1, num2, num3;
  char op;

  printf("Enter a mathematical expression: ");
  fgets(entry, 256, stdin);
  printf("%s\n", entry);

  my_stack_t *ops = stack_init(sizeof(char));
  my_stack_t *nums = stack_init(sizeof(int));

  for (int i = 0; i < strnlen(entry, 256); i++) {
    char c = entry[i];
    switch (c) {
      case '\n':
      case '(':
      break;
      case ')':
        stack_pop(nums, &num1);
        stack_pop(nums, &num2);
        stack_pop(ops, &op);
        switch (op) {
          case '+':
            num3 = num1 + num2;
            break;
          case '-':
            num3 = num1 - num2;
            break;
          case '*':
            num3 = num1 * num2;
            break;
          case '/':
            num3 = num1 / num2;
            break;
          default:
            printf("%c not recognized!\n", op);
            exit(1);
            break;
        }
        stack_push(nums, &num3);
      break;
      case '+':
      case '-':
      case '*':
      case '/':
        stack_push(ops, &c);
        break;
      default: 
        if (isdigit(c)) {
          num1 = atoi(&c);
          stack_push(nums, &num1);
        } else {
          printf("%c not recongized!\n", c);
          exit(1);
        }
    }
  }

  stack_pop(nums, &num1);

  printf("Result is %d\n", num1);

  return 0;
}