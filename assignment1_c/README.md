# Assignment 1 – Song Analyzer (C)

Introductory C implementation of a command-line tool that reads a CSV file of songs, applies a selected query, and writes results to output.csv.

```bash
gcc -Wall -std=c99 -o song_analyzer song_analyzer.c

./song_analyzer --question=1 --data=before_2020s.csv

The program writes its results to output.csv.

