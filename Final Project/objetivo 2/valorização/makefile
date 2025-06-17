all:

	cat /dev/null>"f1.txt"
	cat /dev/null>"f2.txt"
	cat /dev/null>"f3.txt"
	cat /dev/null>"f4.txt"
	cat /dev/null>"saida.txt"
	
	./gen.py | head -100 | sort | ./split.py f1.txt f2.txt f3.txt f4.txt f5.txt f6.txt f7.txt f8.txt
	
	clear
	
	gcc ex2val.c -o ex2val
	./ex2val
