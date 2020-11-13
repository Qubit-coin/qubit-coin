sudo apt install cmake gcc ninja-build libssl-dev python3-pytest python3-pytest-xdist unzip xsltproc doxygen graphviz

cd src && cd liboqs && mkdir build
cd build && cmake -DBUILD_SHARED_LIBS=ON -GNinja .. && ninja #&& ninja install