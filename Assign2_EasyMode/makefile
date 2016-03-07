all:
	g++ -std=c++11 Assign2.cpp -o Assign2

test:
	g++ -std=c++11 Assign2.cpp -o Assign2Partial

run:
	make all
	./Assign2 mri 256 256
	make clean
	
clean:
	rm Assign2
	rm Assign2Partial
