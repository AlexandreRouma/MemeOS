# Install requirements
echo "Installing requirements..."
sudo apt install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo

# Configuration
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

# Create directories
mkdir $HOME/src

# Download binutils + gcc
echo "Downloading binutils and gcc..."
wget https://ftp.nluug.nl/pub/gnu/binutils/binutils-2.31.tar.xz -O $HOME/src/binutils-2.31.tar.xz
wget https://ftp.gnu.org/gnu/gcc/gcc-8.2.0/gcc-8.2.0.tar.xz -O $HOME/src/gcc-8.2.0.tar.xz

# Decompress
echo "Decompressing..."
cd $HOME/src
tar -xf binutils-2.31.tar.xz
tar -xf gcc-8.2.0.tar.xz

# Build binutils
echo "Building binutils..."
cd $HOME/src
mkdir build-binutils
cd build-binutils
../binutils-2.31/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j4
make install

# Build GCC
echo "Building gcc..."
cd $HOME/src
which -- $TARGET-as || echo $TARGET-as is not in the PATH
mkdir build-gcc
cd build-gcc
../gcc-8.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc -j4
make all-target-libgcc -j4
make install-gcc
make install-target-libgcc

# Add GCC to path
echo "Adding gcc to path..."
echo export PATH=\"\$HOME/opt/cross/bin:\$PATH\" >> ~/.profile

# Install grub + xorriso
echo "Installing grub2 utils and xorriso..."
sudo apt install grub2 xorriso

# Done
echo "Done!"