#  $<表示server.cpp $@表示ser

all: ser cli pre
LPTH = -lpthread
ser: server.cpp chat.cpp chat.h
	g++ -g $< -o $@ $(LPTH)

cli: client.cpp chat.cpp chat.h
	g++ -g $< -o $@ $(LPTH)

pre: pressure_test.cpp chat.cpp chat.h
	g++ -g $< -o $@ $(LPTH)

clean:
	rm ser cli pre


