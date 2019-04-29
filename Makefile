
HEADER_FILES := $(wildcard */*.hpp)

all: main

main: base_column.cpp  main.cpp  ${HEADER_FILES}
	g++ -Werror -I. main.cpp base_column.cpp -o main -lboost_serialization

run:
	./main

documentation:
	cd doc; doxygen doxygen.conf

view_documentation:
	chrome doc/documentation/index.htm

release:
	rm -rf Release db2_programming_project_SoSe2015 db2_programming_project_SoSe2015.tgz
	svn export . Release/
	rm -rf Release/.gitignore
	mv Release db2_programming_project_SoSe2015
	tar -cvzf db2_programming_project_SoSe2015.tgz db2_programming_project_SoSe2015


