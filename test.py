from pydbg import *
from pydbg.defines import *
import sys

#print(sys.argv[1])

def detect_overflow(dbg):
        if dbg.dbg.u.Exception.dwFirstChance:
                return DBG_EXCEPTION_NOT_HANDLED
        print("AccessViolation Happened!")
        print("EIP:%0x",dbg.context.Eip)
        return DBG_EXCEPTION_NOT_HANDLED

dbg=gdb()
gdb.attach(int(sys.argv[1]))

dbg.set_calback(EXCEPTION_ACCESS_VIOLATION,detect_overflow)
dbg.run()


