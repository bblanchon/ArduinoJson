FROM ubuntu:20.04

RUN apt-get update && \
  apt-get install -y software-properties-common curl && \
  apt-get upgrade -y
RUN apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 15CF4D18AF4F7421
RUN add-apt-repository -yn 'deb http://archive.ubuntu.com/ubuntu/ trusty main' && \
  add-apt-repository -yn 'deb http://archive.ubuntu.com/ubuntu/ trusty universe' && \
  add-apt-repository -yn 'deb http://archive.ubuntu.com/ubuntu/ xenial main' && \
  add-apt-repository -yn 'deb http://archive.ubuntu.com/ubuntu/ xenial universe' && \
  add-apt-repository -yn 'deb http://archive.ubuntu.com/ubuntu/ bionic main' && \
  add-apt-repository -yn 'deb http://archive.ubuntu.com/ubuntu/ bionic universe' && \
  add-apt-repository -yn 'deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-6.0 main' && \
  add-apt-repository -yn 'deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-7 main' && \
  add-apt-repository -yn 'deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-8 main' && \
  add-apt-repository -yn 'deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-9 main' && \
  add-apt-repository -yn 'deb http://mirrors.kernel.org/ubuntu hirsute main universe'

RUN apt-get update && apt-get -y install \
  cmake \
  ninja-build \
  git \
  clang-format \
  g++-4.4 \
  g++-4.6 \
  g++-4.7 \
  g++-4.8 \
  g++-4.9 \
  g++-5 \
  g++-7 \
  g++-8 \
  g++-9 \
  g++-10 \
  g++-11 \
  clang-3.5 \
  clang-3.6 \
  clang-3.7 \
  clang-3.8 \
  clang-3.9 \
  clang-6.0 \
  clang-7 \
  clang-8 \
  clang-9 \
  clang-10 \
  clang-11
