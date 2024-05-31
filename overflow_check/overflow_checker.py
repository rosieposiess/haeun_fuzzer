#!/usr/bin/python3
"""This program finds the minimun number of bytes needed to overflow the stack of a simple binary.
Use:
./overflow_checker.py <file_path>
"""

import struct, subprocess, sys, argparse
from subprocess import PIPE, TimeoutExpired
from pwn import *

#context.log_level='debug'
max = 99999
program = ""


def overflow(n):
        """Tries to overflow the given program with n bytes.

        Returns True if the program overflows, false if it doesn't.
        It also returns false if the program excedes the timeout.
        This is to prevent never-ending binaries to stall this program
        """
        command = "./"+program
        payload = "A"*n
        try:
                process = subprocess.run(command.split(" "),
                                         stdout=PIPE,
                                         stderr=PIPE,
                                         input= payload,
                                         encoding='ascii',
                                         timeout=0.1)

                if(process.returncode == -11):
#                        print("PC:",get_pc_address(n))
                        return True
                return False

        except TimeoutExpired:
                return False


def get_pc_address(n):
    """Tries to overflow the program and get the RIP address."""

    global program
    command = "./"+program
    payload = "A"*n
    # Using gdb to run the program and get the RIP address
    gdb_command = [
        "gdb", "--batch",
        "-ex", f"run {payload}",
	"-ex","bt",
        "-ex", "info registers eip",
#	"-ex", "bt",
        command
    ]
    try:
        result = subprocess.run(gdb_command, input=payload, encoding='ascii', stdout=subprocess.PIPE, stderr=PIPE, timeout=5)
        output = result.stdout
#        print("output:",output)
        # Extract the EIP address from the output
        for line in output.split('\n'):
            print(line)
            if 'eip' in line:
                pc_address = line.split()[1]
                return pc_address
        return None

    except TimeoutExpired:
        return None


def insaneOverflow():
        """Calls the overflow function with the stablished maximun value"""
        return overflow(max)


def findMinPaddingInInterval(start, end):
        """Recursively looks for the minimun value needed to overflow the program.
        """
        if (start == end):
                print(start)
                return start
        mid = (int)((start+end)/2)
        if (overflow(mid)):
                return findMinPaddingInInterval(start, mid)
        return findMinPaddingInInterval(mid+1, end)



def findMinPadding():
        """Starts the search for minimun padding"""
        return findMinPaddingInInterval(1,max)

def initProgram():
        """Parses the program arguments and stores them accordingly"""
        global program, max

        parser = argparse.ArgumentParser(description='Options',
                                         formatter_class=argparse.ArgumentDefaultsHelpFormatter)

        parser.add_argument("-m","--max",
                            help="maximum number of bytes to test",
                            type=int,
                            default=max)

        parser.add_argument("program",
                            help="program to perform the check on")

        args= parser.parse_args()



        program = args.program
        max = args.max


def main():

        initProgram()

        if (insaneOverflow()):
                minimunPadding = findMinPadding()
                print("Program needs at least %d bytes to break." % minimunPadding)
                print("That means your padding should be %d bytes long" % (minimunPadding-4))

                pc=get_pc_address(minimunPadding)
                if(pc):	print("PC:",pc)
                else: print("Can't get RIP")
        else:
                print("%s is not vulnerable to buffer oveflow. Better luck next time" % program)
        return



main()

