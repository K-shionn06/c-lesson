CC=gcc

parser: cl_getc.o parser.o
	$(CC) -o a.out cl_getc.o parser.o

exec_array: streq.o cl_getc.o parser.o exec_array.o
	$(CC) -o a.out streq.o cl_getc.o parser.o exec_array.o

stack: streq.o cl_getc.o parser.o exec_array.o stack.o
	$(CC) -o a.out streq.o cl_getc.o parser.o exec_array.o stack.o

dict: streq.o dict.o
	$(CC) -o a.out streq.o dict.o

eval: streq.o cl_getc.o parser.o exec_array.o stack.o dict.o eval.o
	$(CC) -o a.out streq.o cl_getc.o parser.o exec_array.o stack.o dict.o eval.o
