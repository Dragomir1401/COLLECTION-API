from subprocess import call
from pathlib import Path
from os import walk
import hashlib
import argparse
import sys
import logging
import json


def setup_logging(args):
    msg_format = '%(asctime)s:%(levelname)s: %(message)s'
    formatter = logging.Formatter(msg_format, datefmt='%H:%M:%S')
    handler = logging.StreamHandler()
    handler.setFormatter(formatter)
    logging.getLogger().addHandler(handler)
    logging.getLogger().setLevel(logging.DEBUG)


def warn_msg(args, msg, category):
    logging.warning(msg)
    args[category].append((2, msg))


def err_msg(args, msg, category):
    logging.critical(msg)
    args[category].append((1, msg))


def info_msg(args, msg, category):
    logging.info(msg)
    args[category].append((0, msg))


def check_makefile(args):

    options = ["makefile", "./Makefile"]
    rules = ["build", "run-p1", "run-p2", "run-best"]

    for file in options:
        file = Path(file)

        if file.exists():
            logging.info("Found Makefile!")
            with open(file, "r") as f:
                content = f.read()
                for rule in rules:
                    if rule not in content:
                        warn_msg(args, "Rule {} not found in Makefile!".format(rule), "makefile")
            break

    else:
        err_msg(args, "No Makefile found!", "makefile")

    return args


def check_readme(args):

    options = ["README", "readme", "Readme"]
    options = options + [o + ".md" for o in options] + [o + ".txt" for o in options]

    for file in options:

        file = Path(file)
        if file.exists():
            logging.info("Found Readme")
            with open(file, "r", encoding="utf8") as f:
                content = f.read()
                if len(content) < 20:
                    err_msg(args,
                            "Readme file is very short - be sure "
                            "to include all the required information", "readme")
                else:
                    info_msg(args, "Found Readme file!", "readme")
            break
    else:
        err_msg(args, "No README file found!", "readme")

    return args


def check_pdf(args):

    found_doc = False
    for (_, _, files) in walk("./"):

        for file in files:
            if file.endswith(".pdf"):
                file_name = file[:file.find(".")]
                if file_name.isdigit() and 1 <= int(file_name) < 22:
                    found_doc = file_name
                    break
        break

    if not found_doc:
        err_msg(args, "PDF document not found", "pdf")
    else:
        info_msg(args, found_doc, "pdf")

    return args


def check_tests(args):

    input_path  = Path("./in")
    output_path = Path("./out")

    if not input_path.exists() or not input_path.is_dir():
        err_msg(args, "No folder for the input tests found!", "tests")
        return args

    if not output_path.exists() or not output_path.is_dir():
        err_msg(args, "No folder for the output tests found!", "tests")
        return args

    test_names      = set()

    for (_, _, input_tests) in walk(input_path):

        input_tests = [file for file in input_tests if file.endswith(".in")]
        if len(input_tests) < 20:
            err_msg(args, "Only {} input tests generated!".format(len(input_tests)), "tests")
        else:
            info_msg(args, "Found {} tests.".format(len(input_tests)), "tests")

        unique = set()
        for idx, test in enumerate(input_tests):
            with open(input_path / test, "rb") as f:
                contents = f.read()
                checksum = hashlib.md5(contents).hexdigest()
                unique.add(checksum)
                test_names.add(test[:test.find(".")])

        if len(input_tests) != len(unique):
            err_msg(args, "Unique tests: {} / {}!".format(len(unique), len(input_tests)), "tests")
        break

    for (_, _, output_tests) in walk(output_path):

        for file in output_tests:
            if ".out" in file and file[:file.find(".")] in test_names:
                test_names.remove(file[:file.find(".")])

        if len(test_names) > 0:
            err_msg(args, "Number of input tests doesn't match number of output tests!", "tests")
        break

    return args


def main():

    args = {
        "makefile": [],
        "readme": [],
        "tests": [],
        "pdf": []
    }

    args = check_makefile(args)
    args = check_readme(args)
    args = check_tests(args)
    args = check_pdf(args)

    logging.info("Done!")
    return args


if __name__ == "__main__":
    main()