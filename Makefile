all: quiz.cgi submit.cgi

quiz.cgi: src/quiz.cpp src/quiz_logic.cpp
	mkdir -p cgi-bin
	g++ -std=c++17 -Iinclude src/quiz.cpp src/quiz_logic.cpp -o cgi-bin/quiz.cgi

submit.cgi: src/submit.cpp src/quiz_logic.cpp
	mkdir -p cgi-bin
	g++ -std=c++17 -Iinclude src/submit.cpp src/quiz_logic.cpp -o cgi-bin/submit.cgi
