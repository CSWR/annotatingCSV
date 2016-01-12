#!/usr/bin/env python3
"""
Usage: {script_name} INPUT_FILE OUTPUT_FILE

Adds noise to the csv file given as input.
"""
import random
import sys
from collections import deque


def usage():
    print(__doc__.format(script_name=sys.argv[0].split("/")[-1]))
    sys.exit(0)


def empty_lines(line_num):
    empty_count = 500*random.randint(1, 9)
    lines = list(random.randint(0, line_num - 1) for _ in range(empty_count))
    lines.sort()
    return deque(lines)


def process_line(index, line, output, empty_lines):
    # Add random number in the beginning
    add_random_num = random.random() < .05
    # Add extra cell in the beginning
    extra_cell = random.random() < .05

    # Add empty lines
    if empty_lines:
        while empty_lines and empty_lines[0] == index:
            empty_lines.popleft()
            output.write("\n")

    if add_random_num:
        output.write(random.randint(0, 9))

    if extra_cell:
        output.write("{},".format(random.randint(0, 1000)))

    output.write(line)


def main():
    if len(sys.argv) != 3:
        usage()
    input_file = open(sys.argv[1], "r")
    output_file = open(sys.argv[2], "w")

    line_num = sum(1 for _ in input_file)
    input_file.seek(0)
    empty = empty_lines(line_num)

    for index, line in enumerate(input_file):
        process_line(index, line, output_file, empty)

    input_file.close()
    output_file.close()

main()
