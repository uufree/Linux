#  $<表示server.cpp $@表示ser

all: ser cli
LPTH = -lpthread
ser: server.cpp chat.cpp chat.h
	g++ -g $< -o $@ $(LPTH)

cli: client.cpp chat.cpp chat.h
	g++ -g $< -o $@ $(LPTH)

clean:
	rm ser cli


