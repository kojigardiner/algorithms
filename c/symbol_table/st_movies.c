// A symbol table client that reads in a text file consisting of film and actor
// names, then prompts the user to enter an actor name and outputs the films
// that actor was in.
//
// Example data: https://algs4.cs.princeton.edu/41graph/movies.txt
//
// Inspired by Algorithms, Fourth Edition (Sedgewick & Wayne).

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "../queue/queue.h"

#define MAX_LINE_LENGTH 1024

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Not enough arguments\nUsage: ./st_movies <delimiter> <filename>");
    exit(EXIT_FAILURE);
  }

  char line[MAX_LINE_LENGTH];
  char *delim = argv[1];
  char *movie;
  char *actor;
  char *token;

  FILE *fp = fopen(argv[2], "r");
  if (!fp){
    perror("Failed to open file\n");
    exit(EXIT_FAILURE);
  }

  queue_t *movie_q;
  queue_t *actor_q;
  st_t *movie_to_actor = st_init(sizeof(char *), sizeof(queue_t *), compare_str, HASH_TABLE_PROBING);
  st_t *actor_to_movie = st_init(sizeof(char *), sizeof(queue_t *), compare_str, HASH_TABLE_PROBING);

  // Read each line from the file
  while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
    line[strcspn(line, "\n")] = 0;  // replace \n with null-terminator
    // printf("---\n");

    // Read each delimited token from the line
    token = strtok(line, delim);
    if (!token) {
      continue;
    }

    movie = strdup(token);
    if (!movie) {
      perror("Failed to strdup\n");
      exit(EXIT_FAILURE);
    }
    if (!st_get(movie_to_actor, &movie, &actor_q)) {
      actor_q = queue_init(sizeof(char *));
    }

    token = strtok(NULL, delim);
    while (token != NULL) {
      // printf("%s\n", token);
      actor = strdup(token);
      if (!actor) {
        perror("Failed to strdup\n");
        exit(EXIT_FAILURE);
      }
      queue_enqueue(actor_q, &actor);
      st_put(movie_to_actor, &movie, &actor_q);

      if (!st_get(actor_to_movie, &actor, &movie_q)) {
        movie_q = queue_init(sizeof(char *));
      }
      queue_enqueue(movie_q, &movie);
      // printf("%s: %s\n", movie, actor);
      st_put(actor_to_movie, &actor, &movie_q);
      token = strtok(NULL, delim);
    }
  }
  
  fclose(fp);

  char input[MAX_LINE_LENGTH];
  char *actor_name;
  char *movie_name;
  queue_t *movies;
  printf("Enter an actor's name (Last Name, First Name): ");
  while (fgets(input, MAX_LINE_LENGTH, stdin) != NULL) {
    input[strcspn(input, "\n")] = 0;  // replace \n with null-terminator
    actor_name = strdup(input);

    if (st_get(actor_to_movie, &actor_name, &movies)) {
      queue_iter_init(movies);
      while (queue_iter_has_next(movies)) {
        queue_iter_next(movies, &movie_name);
        printf("%s\n", movie_name);
      }
      printf("\n");
    } else {
      printf("Not found\n\n");
    }
    free(actor_name);
  }

  return 0;
}