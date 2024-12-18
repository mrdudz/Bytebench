##############################################################################################
# cc65 bytebench makefile
##############################################################################################

#ASMLIB=$(C64DEV)/lib/c64
#ASMINC=$(C64DEV)/include/c64
#INCPATH=$(C64DEV)/include/c64
#LIBPATH=$(C64DEV)/lib/c64/c

HZ=1000000

CC=cc65
CCFLAGS=-t c64
C1541=c1541
AS=ca65
ASFLAGS=-t c64
LD=ld65
LDFLAGS=-t c64
CL=cl65
CLFLAGS=-t c64
#START=c64.o
#STDLIBS=c64.lib

##############################################################################################

all: libs main disc

%.o: %.s
		$(AS) -o $*.o $(ASFLAGS) $*.s
%.prg: %.o
#		$(LD) $(LDFLAGS) -o $*.prg $(START) $*.o $(OVLDOBJS) $(OBJS) $(LIBS) $(STDLIBS)
		$(CL) $(CLFLAGS) -o $*.prg $*.o $(OVLDOBJS) $(OBJS) $(LIBS)


libs:$(LIBS)

main: arithoh.prg register.prg short.prg int.prg long.prg \
      hanoi.prg \
      dhry2.prg dhry2reg.prg

#fstime.prg syscall.prg context1.prg pipe.prg spawn.prg execl.prg looper.prg
#float.prg double.prg

arithoh.s: arith.c
		$(CC) -o arithoh.s $(CCFLAGS) -Darithoh arith.c
register.s: arith.c
		$(CC) -o register.s $(CCFLAGS) -Ddatum=register arith.c
short.s: arith.c
		$(CC) -o short.s $(CCFLAGS) -Ddatum=short arith.c
int.s: arith.c
		$(CC) -o int.s $(CCFLAGS) -Ddatum=int arith.c
long.s: arith.c
		$(CC) -o long.s $(CCFLAGS) -Ddatum=long arith.c
#float.s: arith.c
#        $(CC) -o float.s $(CCFLAGS) -Ddatum=float arith.c
#double.s: arith.c
#        $(CC) -o double.s $(CCFLAGS) -Ddatum=double arith.c



arithoh.prg: arithoh.s
register.prg: register.s
short.prg: short.s
int.prg: int.s
long.prg: long.s
#float.prg: float.s
#double.prg: double.s

hanoi.s: hanoi.c
	$(CC) -o hanoi.s $(CCFLAGS) hanoi.c
fstime.s: fstime.c
	$(CC) -o fstime.s $(CCFLAGS) -Dawk=1 fstime.c
syscall.s: syscall.c
	$(CC) -o syscall.s $(CCFLAGS) syscall.c
context1.s: context1.c
	$(CC) -o context1.s $(CCFLAGS) context1.c
pipe.s: pipe.c
	$(CC) -o pipe.s $(CCFLAGS) pipe.c
spawn.s: spawn.c
	$(CC) -o spawn.s $(CCFLAGS) spawn.c
execl.s: execl.c big.c
	$(CC) -o execl.s $(CCFLAGS) execl.c
dhry1.s: dhry_1.c dhry_2.c dhry.h
#       $(CC) -o dhry1.s  $(CCFLAGS) -DHZ=$(HZ) dhry_1.c
dhry2.prg: dhry_1.c dhry_2.c dhry.h
#       $(CC) -o dhry2.s  $(CCFLAGS) -DHZ=$(HZ) dhry_2.c
		cl65  -c $(CCFLAGS) -DHZ=$(HZ) dhry_1.c
		cl65  -c $(CCFLAGS) -DHZ=$(HZ) dhry_2.c
		cl65  -o dhry2.prg $(CCFLAGS)  dhry_1.o dhry_2.o
		$(RM) -f dhry_1.o dhry_2.o
dhry2reg.prg: dhry_1.c dhry_2.c dhry.h
	cl65  -c $(CCFLAGS) -DREG=register -DHZ=$(HZ) dhry_1.c
	cl65  -c $(CCFLAGS) -DREG=register -DHZ=$(HZ) dhry_2.c
	cl65 -o dhry2reg.prg $(CCFLAGS)  dhry_1.o dhry_2.o
	$(RM) -f dhry_1.o dhry_2.o
looper.s: looper.c
	$(CC) -o looper.s $(CCFLAGS)  looper.c

fstime.prg:fstime.s
hanoi.prg:hanoi.s
syscall.prg:syscall.s
context1.prg:context1.s
pipe.prg:pipe.s
spawn.prg:spawn.s
execl.prg:execl.s
#dhry2.prg:dhry1.s dhry2.s
#dhry2reg.prg:dhry2reg.s
looper.prg:looper.s

disc:
	$(C1541) -format bytebench,00 d64 bytebench.d64 \
 	-write arithoh.prg arithoh \
 	-write register.prg register \
 	-write short.prg short \
 	-write int.prg int \
	-write long.prg long  \
	-write hanoi.prg hanoi \
	-write dhry2.prg dhrystone  \
	-write dhry2reg.prg dhrystonereg \


##############################################################################################
clean:
	$(RM) *.prg *.s *.o *.map *.lbl *.log *.lst *.d64

