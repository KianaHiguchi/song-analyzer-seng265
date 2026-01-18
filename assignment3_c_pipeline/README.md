# Assignment 3 – Song Analyzer (C)

Advanced C implementation of the Song Analyzer using multiple source files, a linked list, merge sort, and a safe malloc wrapper. The program reads data.csv, applies filtering and sorting based on command-line arguments, optionally limits results, and writes output to output.csv.

```bash
make

./song_analyzer --data=data.csv --filter=ARTIST --value="Dua Lipa" --order_by=STREAMS --order=ASC --limit=6

The program writes its results to output.csv.

make clean
