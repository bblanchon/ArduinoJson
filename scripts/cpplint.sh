CPPLINT="python third-party/cpplint/cpplint.py"
FLAGS=""

cd ..
$CPPLINT $FLAGS $(find include src test -regex ".*\.[hc]pp$")