#!/bin/bash

# To be run from the project root as ./scripts/buildCoverage.sh, will generate code coverage to /build/coverage.  Project must be compiled with coverage.
# Based of documentation here: https://wiki.documentfoundation.org/Development/Lcov

rm -rf ./build/coverage/
mkdir ./build/coverage/
lcov --no-external --capture --initial --directory . --output-file ./build/coverage/output_base.info
./build/test/test
lcov --no-external --capture --directory . --output-file ./build/coverage/output_test.info
lcov --add-tracefile ./build/coverage/output_base.info --add-tracefile ./build/coverage/output_test.info --output-file ./build/coverage/output_total.info
lcov --remove ./build/coverage/output_total.info '*/test/*' '*/build/*' -o ./build/coverage/output_filtered.info
genhtml --prefix . --ignore-errors source ./build/coverage/output_filtered.info --legend --output-directory=./build/coverage/