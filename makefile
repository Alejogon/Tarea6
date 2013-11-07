IC.x : IC.c
	cc IC.c -lm -o IC.x
	./IC.x 0 0 0 0
	mv CondicionesIniciales.txt ci1.txt
	python plots.py ci1.txt
	./IC.x 150 200 0 -100
	cat ci1.txt CondicionesIniciales.txt > ci2.txt
	python plots.py ci2.txt
	cc evolve.c -lm -lgsl -lgslcblas -o evolve.x
	./evolve.x ci2.txt
	python plots.py Estado de galaxia_1 
	python plots.py Estado de galaxia_2 
	python plots.py Estado de galaxia_3 
	python plots.py Estado de galaxia_4 
	python plots.py Estado de galaxia_5
