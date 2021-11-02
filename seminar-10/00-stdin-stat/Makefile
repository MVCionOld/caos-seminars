file:
	gcc main.c -o main.out
	mkdir tmp && touch tmp/a
	./main.out < tmp/a
	rm -rf tmp main.out
dir:
	gcc main.c -o main.out
	mkdir tmp
	./main.out < tmp
	rm -rf tmp main.out
symlink:
	gcc main.c -o main.out
	mkdir tmp && touch tmp/a && ln tmp/a ./a_link
	./main.out < ./a_link
	rm -rf tmp main.out a_link