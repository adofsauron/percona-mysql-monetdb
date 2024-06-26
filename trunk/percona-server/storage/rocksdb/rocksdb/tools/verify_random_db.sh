#!/usr/bin/env bash
# Copyright (c) Facebook, Inc. and its affiliates. All Rights Reserved.
#
# A shell script to verify DB generated by generate_random_db.sh cannot opened and read correct data.
# ./ldb needs to be avaible to be executed.
#
# Usage: <SCRIPT> <DB Path>

scriptpath=`dirname $BASH_SOURCE`
if [ "$#" -lt 2 ]; then
  echo "usage: $BASH_SOURCE <db_directory> <compare_base_db_directory> [dump_file_name] [if_try_load_options] [if_ignore_unknown_options]"
  exit 1
fi

db_dir=$1
base_db_dir=$2
dump_file_name=${3:-"dump_file.txt"}
try_load_options=${4:-"1"}
ignore_unknown_options=${5:-"0"}
db_dump=$db_dir"/"$dump_file_name
base_db_dump=$base_db_dir"/"$dump_file_name
extra_params=

if [ "$try_load_options" = "0" ]; then
 extra_params=" --try_load_options=false"
elif [ "$try_load_options" = "1" ]; then
 extra_params=" --try_load_options=true"
fi

if [ "$ignore_unknown_options" = "1" ]; then
 extra_params="$extra_params --ignore_unknown_options"
fi

set -e
echo == Dumping data from $db_dir to $db_dump
./ldb dump --db=$db_dir $extra_params > $db_dump

echo == Dumping data from $base_db_dir to $base_db_dump
./ldb dump --db=$base_db_dir $extra_params > $base_db_dump

diff $db_dump $base_db_dump
