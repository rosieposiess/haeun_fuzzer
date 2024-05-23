from pwn import *
import sys

# Define the function to detect overflow (access violation)
def detect_overflow():
    print("AccessViolation Happened!")
    # EIP is the instruction pointer in x86 architecture.
    print("EIP: 0x{:x}".format(gdb.selected_frame().pc()))


# Ensure the user provides a PID as a command line argument
'''
if len(sys.argv) != 2:
    print("Usage: python3 script.py <PID>")
    sys.exit(1)
'''
# Attach to the process with the given PID
#pid = int(sys.argv[1])
p=process("./bof")
gdb.attach(p)

# Set the callback for the access violation exception
gdb.events.except_hook.append(detect_overflow)

# Continue the execution of the process
gdb.continue_now()

# Keep the script running to handle the events
gdb.interact()
