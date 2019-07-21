CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -O2

all: unionFind.o trie.o particle.o commands.o main.o
	$(CC) $(CFLAGS) $^ -o quantization

unionFind.o: UnionFind/unionFind.c
	$(CC) $(CFLAGS) -c UnionFind/unionFind.c

trie.o: Trie/trie.c
	$(CC) $(CFLAGS) -c Trie/trie.c

particle.o: Particle/particle.c
	$(CC) $(CFLAGS) -c Particle/particle.c

commands.o: Commands/commands.c
	$(CC) $(CFLAGS) -c Commands/commands.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -rf *.o quantization
