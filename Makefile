TARGET=Shisensho

$(TARGET): GenericTimer.o ImageData.o ImageIO.o main.o
	cc -o $(TARGET) GenericTimer.o ImageData.o ImageIO.o main.o -L. -lglut -lGLU -lGL -lXi -lXrandr -lm
.c.o:
	cc -c -I. $< -I. -Wall
cygwin: GenericTimer.o ImageData.o ImageIO.o main.o cygwin.o Shisensho.o
	gcc -c cygwin.c
	gcc -o $(TARGET) GenericTimer.o ImageData.o ImageIO.o cygwin.o Shisensho.o -L. -lfreeglut -lglu32 -lopengl32
run: $(TARGET)
	./$(TARGET)
clean:
	rm -f *.o *.c~ *.h~ $(TARGET) $(TARGET).exe
