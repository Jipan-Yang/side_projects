CC=gcc
DEBUG = -g -O0
CFLAGS = -Wall $(DEBUG) -I. -DDMALLOC -DDMALLOC_FUNC_CHECK
LDFLAGS = -Wall $(DEBUG) -lreadline -lpthread -ldmallocth

#DEPS = threads_mm.h
DEPS = 
OBJ = threads_mm.o  

$(info $$CFLAGS is [${CFLAGS}])
$(info $$LDFLAGS is [${LDFLAGS}])

default: threads_mm
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

threads_mm: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm $(OBJ) threads_mm
.PHONY: default clean 
