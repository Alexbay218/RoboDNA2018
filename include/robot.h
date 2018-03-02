#ifndef ROBOT_H
#define ROBOT_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>

class robot {
	public:
		std::string sourcePriority;
		/*
			e - exchange (return)
			z - zone
			s - self back zone
			o - opponent back zone
			p - portal
		*/
		std::string autoStrat;
		/* 3 char of 5 sec
			e - exchange
			a - auto line
			w - switch
			s - scale
		*/
		std::string telopNoEG;
		/* 1 char of 15 sec + 9 char of 10 sec
			e - exchange
			w - switch
			x - enemy switch
			s - scale
		*/
		std::string telopEG;
		/* 3 char of 10 sec
			e - exchange
			w - switch
			x - enemy switch
			s - scale
			c - climb
		*/
		int maxVel;

		int curMaxVel;
		int posX, posY;
		int tarX, tarY;
		int timeOut;
		bool hasCube;
		bool hasClimb;
		bool hasAuto;
		bool hasPark;
		sf::CircleShape drawShape;
		sf::Text drawText;
		sf::RectangleShape drawLine;

		robot();
		robot(std::string dna, std::string sP);
		void init();
		virtual ~robot();
		char targetAtTime(int t);
};

#endif //ROBOT_H
