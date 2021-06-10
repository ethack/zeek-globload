# @TEST-EXEC: zeek -NN Corelight::GlobLoad |sed -e 's/version.*)/version)/g' >output
# @TEST-EXEC: btest-diff output
