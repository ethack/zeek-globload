# Various glob-matching/non-matching scenarios to test the plugin.
#
# @TEST-EXEC: bash %INPUT
# @TEST-EXEC: zeek glob-1.zeek >output1
# @TEST-EXEC: zeek glob-2.zeek >output2
# @TEST-EXEC: zeek glob-3.zeek >output3
# @TEST-EXEC: zeek glob-absolute.zeek >output4
# @TEST-EXEC: btest-diff output1
# @TEST-EXEC: btest-diff output2
# @TEST-EXEC: btest-diff output3
# @TEST-EXEC: btest-diff output4

mkdir dir1 dir2 dir3

# Populate dir1 with a few regular files, some matching the glob
cat >dir1/test1.zeek <<EOF
event zeek_init() &priority=3 { print("test1.zeek"); }
EOF

cat >dir1/test2.zeek <<EOF
event zeek_init() &priority=2 { print("test2.zeek"); }
EOF

cat >dir1/test3.txt <<EOF
event zeek_init() &priority=1 { print("test3.zeek"); }
EOF

# Symlink test3.zeek to test3.txt, so we should see it run once
( cd dir1 && ln -s test3.txt test3.zeek )

# Populate dir2 with a normal __load__.zeek scenario
cp dir1/test1.zeek dir2

cat >dir2/__load__.zeek <<EOF
@load test1.zeek
EOF

# Populate dir3 with a symlink "adir" linking to dir2. The symlink's
# name matches the glob, so the pointed-to directory should load. Also
# add a file that shouldn't match.

( cd dir3 && ln -s ../dir2 adir )

cat >dir3/notadir <<EOF
event zeek_init() { print("uh-oh"); }
EOF

@TEST-START-FILE glob-1.zeek
@load dir1/*.zeek
@TEST-END-FILE

@TEST-START-FILE glob-2.zeek
@load dir3/a*
@TEST-END-FILE

# Verify that matching nothing is okay too
@TEST-START-FILE glob-3.zeek
@load dir3/nothing*
@TEST-END-FILE

# Create a file that loads via absolute path. This verifies that we
# don't try to resolve this based on the local folder.
cat >glob-absolute.zeek<<EOF
@load $(pwd)/glob-1.zeek
EOF
