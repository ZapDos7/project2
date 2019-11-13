CC=g++  -std=c++11
CFLAGS=-c -Wpedantic -Ofast
LDFLAGS=
SOURCES=cluster.cpp my_vector.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=cluster


#VECTORS
#LSH
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

#run:
#	./$(EXECUTABLE) -d /home/ioanna/Desktop/sample_datasets/a_vectors/siftsmall/input_small_id_n -q /home/ioanna/Desktop/sample_datasets/a_vectors/XS/query_xs -k 4 -L 3 -o /home/ioanna/Desktop/outputfile.txt

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
