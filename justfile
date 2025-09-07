default:
    @just list

build:
    cmake -G Ninja -B build -DCMAKE_INSTALL_PREFIX=install -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
    cmake --build build
    cmake --install build

test:
    cmake --build build --target test

clean:
    rm -rf build
    rm -rf install
