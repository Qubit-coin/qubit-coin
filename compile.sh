#g++ -o src/quantum_interface.o src/quantum_interface.cpp -L./src/liboqs/build/lib -loqs

make -j8
./run.sh
