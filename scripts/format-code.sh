cd ..
find include src test -regex ".*\.[ch]pp$" -exec clang-format -style=Google -i {} \;
