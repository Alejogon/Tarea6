IC.x : IC.c
	cc IC.c -o IC.x
	./IC.x 0 0 0 0
	mv CondicionesIniciales.txt ci1.txt
	python plots.py ci1.txt
	./IC.x 150 200 0 -100
	cat ci1.txt CondicionesIniciales.txt > ci2.txt
	python plots.py ci2.txt

