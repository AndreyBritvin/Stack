CC = g++

HEADERS_INCLUDE = -Iinclude

#CFLAGS = $(HEADERS_INCLUDE) -lm -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ 						\
							-Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations 			\
							-Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported 			\
							-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral \
							-Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op 			\
							-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith  \
							-Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wswitch-default 		\
							-Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn 		\
							-Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wsign-promo   \
							-Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast	\
							-Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers 			\
							-Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new 	\
							-fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging	\
							-fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 					\
							-pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,			\
							float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,			\
							nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,		\
							signed-integer-overflow,undefined,unreachable,vla-bound,vptr

CFLAGS = $(HEADERS_INCLUDE) -lm -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr


all: a.out

a.out: build/main.o build/my_stack.o build/my_stack_protection.o build/my_stack_print.o
	@$(CC) $(CFLAGS) build/main.o build/my_stack.o build/my_stack_protection.o build/my_stack_print.o

build/main.o: src/main.cpp
	@$(CC) $(CFLAGS) -c src/main.cpp -o build/main.o

build/my_stack.o: src/my_stack.cpp include/my_stack.h
	@$(CC) $(CFLAGS) -c src/my_stack.cpp -o build/my_stack.o

build/my_stack_protection.o: src/my_stack_protection.cpp include/my_stack_protection.h
	@$(CC) $(CFLAGS) -c src/my_stack_protection.cpp -o build/my_stack_protection.o

build/my_stack_print.o: src/my_stack_print.cpp include/my_stack.h
	@$(CC) $(CFLAGS) -c src/my_stack_print.cpp -o build/my_stack_print.o

run:
	./a.out

clean:
	rm a.out

