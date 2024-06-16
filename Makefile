
CXX = g++
CXXFLAGS = -std=c++14


assignment2_c++: Person.o Creator.o User.o Database.o Main_menu.o Videos.o main.o
	$(CXX) $(CXXFLAGS) Person.o Creator.o User.o Database.o Main_menu.o Videos.o main.o -o assignment2_c++


Person.o: Person.cpp
	$(CXX) $(CXXFLAGS) -c Person.cpp

Creator.o: Creator.cpp
	$(CXX) $(CXXFLAGS) -c Creator.cpp

User.o: User.cpp
	$(CXX) $(CXXFLAGS) -c User.cpp

Database.o: Database.cpp
	$(CXX) $(CXXFLAGS) -c Database.cpp

Videos.o: Videos.cpp
	$(CXX) $(CXXFLAGS) -c Videos.cpp

Main_menu.o: Main_menu.cpp
	$(CXX) $(CXXFLAGS) -c Main_menu.cpp

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp


clean:
	rm *.o assignment2_c++
