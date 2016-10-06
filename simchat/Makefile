all: ser cli
ser: server.cpp chat.cpp chat.h
    g++ -g $< -o $@ -lpthread
cli: client.cpp chat.cpp chat.h
    g++ -g $< -o $@ -lpthread
 
clean:
    rm cli ser
