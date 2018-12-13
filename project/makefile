src = $(wildcard ./src/*.c)
src1 = $(wildcard ./test/*.c)



obj = $(patsubst ./src/%.c, ./obj/%.o, $(src))
obj1 = ./test/test.c

target = app 
target2 = mytest
ALL:$(target) $(target2)
inc_path= ./inc
lib_name=./lib/libkeyMsg.so

$(target):$(obj)
	gcc -shared -o $(lib_name) $^

$(obj):./obj/%.o : ./src/%.c
	gcc -c $< -o $@ -I $(inc_path) -fPIC

$(target2):$(obj) $(obj1)
	gcc $^ -o $@

clean:
	rm -rf $(obj) $(target)

.PHONY: clean ALL



