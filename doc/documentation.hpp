/***********************************************************************************************************
Copyright (c) 2013, Sebastian Breﬂ, Otto-von-Guericke University of Magdeburg, Germany. All rights reserved.

This program and accompanying materials are made available under the terms of the 
GNU GENERAL PUBLIC LICENSE - Version 3, http://www.gnu.org/licenses/gpl-3.0.txt
 **********************************************************************************************************/

/*!
 *  \file documentation.hpp    
 *  \brief This file contains additional documentation, like the generated web pages in the doxygen documentation.
 *  \author    Andreas Meister
 *  \version   0.1
 *  \date      2016
 *  \copyright GNU GENERAL PUBLIC LICENSE - Version 3, http://www.gnu.org/licenses/gpl-3.0.txt
 */


/*! \mainpage Documentation
 *
 * \section intro_sec Introduction
 *
 * This is the documentation of the programming project for the exercises in the Lecture 'Databases Implementation Techniques' (DB2) in summer term 2016. You can find general information about the lecture <a href="http://wwwiti.cs.uni-magdeburg.de/iti_db/lehre/db2/ss2016/">here</a>.
 *
 * \section tasks_sec Tasks
 *
 * The purpose of the programming tasks is to deepen your knowledge in selected aspects of the lecture. This year, we decided to set this focus on compression techniques in column oriented database management systems. Furthermore, we choose C++ as programming language, because it is the most frequently used programming language for database management systems (except C).
 * The task is to implement compression techniques in our framework. We provide a set of classes as presetting, where you have to include an implementation w.r.t. an interface. You can download the sources <a href="http://wwwiti.cs.uni-magdeburg.de/iti_db/lehre/db2/ss2016/programming_task/db2_programming_project_SoSe2016.zip">here</a>. A set of unit tests will help you during the development process to identify errors. The same unit tests will be used at the end of the term to validate your solution. A working implementation is a necessary prerequisite to participate in the exam!

You may choose between the following compression techniques (you may suggest other compression techniques as well):
 *    - Run Length Encoding
 *    - Delta Coding
 *    - Bit-Vector Encoding
 *    - Dictionary Encoding
 *    - Frequency Partitioning

All compression techniques are explained in the lecture. You can find the slides <a href="http://wwwiti.cs.uni-magdeburg.de/iti_db/lehre/db2/ss2016/slides/teil2.pdf">here</a>.
 *
 * 	\section orga_sec Organization

Students will form teams of two students each. In case the exercise has an uneven number of participants, one team may consist of three students. 
Please register your team until the <b>29.04.2016</b> via email to: andreas.meister@iti.cs.uni-magdeburg.de, subject: DB2-2016 Programmierprojekt Anmeldung.
Content: For each team member, we need to know the name, matrikel number, and email adress.

Solutions are to be submitted via email to: andreas.meister@iti.cs.uni-magdeburg.de, subject: DB2-2016 Programmierprojekt Abgabe. The deadline is the 03.07.2016 at 23:59 o'clock. Note that the deadline is strict, there will be <b>no</b> deadline extension.

Solutions will be presented and discussed by each team in the exercise.

Teams consisting of bachelor students have to implement <b>two</b> compression techniques and will receive 5 credit points when they pass the exam.
Teams consisting of master students have to implement <b>three</b> compression techniques, because they will receive 6 credit points when they pass the exam.

We will check the quality of your submitted solution. It has to pass the unit tests, implement the compression technique it represents, and may not be a copy of a solution submitted by another team or any third party implementation. Solutions who fail to fulfill only one of these requirements will not be able to participate in the exam.

 * 	\section setup_sec Setup and Tools
 *
 * The framework runs on Linux and Windows (cygwin) with common C++ compilers (g++, clang). You need to install the boost libraries (Serialization, Any), which can be installed easily on Linux and Windows (cygwin).

	\subsection ubuntu_subsec Setup in Ubuntu
	Open a terminal and type:
@code
sudo apt-get install build-essential libboost-all-dev doxygen 
@endcode

Then, enter the directory you unpacked the archive with the source code and type the following commands to build the program, the documentation and run the program:
@code
cd db2_programming_project_SoSe2016
make
make documentation
make run
@endcode

	\subsection cygwin_subsec Setup for Windows (Cygwin)

As for Windows (cygwin), you need to install the necessary packages using the GUI of the cygwin setup program, which you can download on the <a href="http://www.cygwin.com/">official website</a>. You should install the latest version of the boost libraries, the compiler you wish to use (e.g., g++, clang), the make program, as well as a tool to unpack the source archive. The build steps are the same as for Ubuntu.

 * 	\section getting_started_sec Getting Started
 * To implement your selected compression technique, you have to inherit from the base class \ref CoGaDB::CompressedColumn and implement it's pure virtual methods (similar to an abstract method in Java). You can test your class by creating an instance and pass a pointer to the \ref unittest function. We prepared an example in the project, the CoGaDB::DictionaryCompressedColumn, which is stored in the file compression \ref dictionary_compressed_column.hpp.


 * 	\section cpp_background_sec C++ Background
 * 	You should familiarize yourself with the following features of the C++ language:
	- pointers, references, and <a href="http://www.informit.com/articles/article.aspx?p=25264">smart pointers</a> (we use the shared_ptr implementation of <a href="http://www.boost.org/doc/libs/1_53_0/libs/smart_ptr/smart_ptr.htm">boost</a>)
	- create objects on the heap with new
	- call by value and call by reference
	- public inheritance
	- basic STL containers, such as std::vector and std::list
	- basic templates and how to use them

	You can find a lot of useful examples in the framework code, e.g., the unit tests.

	Recommended (selected) sources of information about C++ are:
	- </a href="http://www.cplusplus.com/">http://www.cplusplus.com/</a>
	- Bjarne Stroustrup. The C++ Programming Language. Addison-Wesley, 4th edition, 2013
	- Scott Meyers. Effective C++: 55 Specific Ways to Improve Your Programs and Designs, Addison-Wesley, 3rd edition, 2005

*/
