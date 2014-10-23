CPPLINT="python third-party/cpplint/cpplint.py"
FLAGS="--filter=-runtime/printf,-runtime/int,-readability/todo,-build/namespace"

cd ..
$CPPLINT $FLAGS $(find include src test -regex ".*\.[hc]pp$")