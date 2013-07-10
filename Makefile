CC = gcc
NAME = test_agent_router
BUFFER_LIST = list.o buffer.o
TEST_BUFFER_LIST = suite_test_list_of_string.o suite_test_buffer_of_int.o
AGENT_ROUTER = agent.o router.o
TEST_AGENT_ROUTER = suite_test_list_of_agent.o suite_test_agent.o suite_test_router.o
LAB = lab.o
TEST_LAB = suite_test_lab.o
CFLAGS = -g -W -Wall -ansi -pedantic `pkg-config --cflags plplotd` `pkg-config --cflags gsl`
LIBS = -lpthread -lcunit `pkg-config --libs plplotd` `pkg-config --libs gsl`

test_buffer_list: $(BUFFER_LIST) $(TEST_BUFFER_LIST) test_buffer_list.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

test_agent_router: $(BUFFER_LIST) $(AGENT_ROUTER) $(TEST_AGENT_ROUTER) test_agent_router.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

test_lab: $(BUFFER_LIST) $(AGENT_ROUTER) $(LAB) $(TEST_LAB) test_lab.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

start_agent: $(BUFFER_LIST) $(AGENT_ROUTER) start_agent.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

start_router: $(BUFFER_LIST) $(AGENT_ROUTER) start_router.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

all: clean test_buffer_list test_agent_router test_lab start_agent start_router

debug: all
	gdb ./$(NAME)
	
run: all
	./$(NAME)

clean:
	-rm *.o *.log test_buffer_list test_agent_router test_lab start_agent start_router

