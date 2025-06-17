all:

	cat /dev/null>"f1.txt"
	cat /dev/null>"f2.txt"
	cat /dev/null>"f3.txt"
	cat /dev/null>"f4.txt"
	cat /dev/null>"saida.txt"
	
	./gen.py | head -100 | sort | ./split.py f1.txt f2.txt f3.txt f4.txt
	
	clear
	
	gcc ex2.c -o ex2
	./ex2
