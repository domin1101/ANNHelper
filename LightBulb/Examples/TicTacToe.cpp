// Includes
#include "Examples\TicTacToe.hpp"
#include "Examples\TicTacToeKI.hpp"
#include "Learning\EvolutionObjectInterface.hpp"
//Library includes
#include <iostream>

#define RANDOM_KI true


EvolutionObjectInterface* TicTacToe::createNewObject()
{
	return new TicTacToeKI(this);
}

TicTacToe::TicTacToe()
{
	fields.resize(3);
	for (int x = 0; x < fields.size(); x++)
	{
		fields[x].resize(3);
	}
	resetWorld();

	window.create(sf::VideoMode(800, 700), "LightBulb!");
	TicTacToeDrawerOptions options;
	options.ticTacToe = this;
	displayMode = true;
	drawer.reset(new TicTacToeDrawer(options));
}

int TicTacToe::getFieldValue(int x, int y)
{
	return fields[x][y];
}

void TicTacToe::doSimulationStep(EvolutionLearningRule& learningRule)
{	
	int ties = 0;
	int firstWon = 0;
	int secondWon = 0;
	int illegalMoves = 0;
	points.clear();
	for (auto ki = objects.begin(); ki != objects.end(); ki++)
	{	
#ifndef RANDOM_KI
		for (auto otherKI = objects.begin(); otherKI != objects.end(); otherKI++)
		{
			if (*ki != *otherKI)
			{
				
				(*otherKI)->resetNN();
#else
		for (int r = 0; r < 100; r++)
		{
#endif
				(*ki)->resetNN();
				illegalMove = false;
				resetWorld();
				for (int i = 0; i < 9; i++)
				{
					if (i % 2 == 0)
					{
						currentPlayer = 1;
						(*ki)->doNNCalculation(learningRule);
					}
					else
					{
						currentPlayer = -1;
#ifndef RANDOM_KI
						(*otherKI)->doNNCalculation(learningRule);
#else
						int x, y;
						do {
							x = std::min(2, (int)((float)rand() / RAND_MAX * 3));
							y = std::min(2, (int)((float)rand() / RAND_MAX * 3));
						} while (!isFree(x, y));
						setField(x, y);
#endif
					}

					sf::Event event;
					while (window.pollEvent(event))
					{
						if (event.type == sf::Event::Closed)
							window.close();
						else if (event.type == sf::Event::KeyPressed)
							displayMode = !displayMode;
					}

					if (displayMode) {
						window.clear();
						drawer->recalculateAllValues();
						drawer->draw(window);
						window.display();
					}

					if (illegalMove)
						break;
				}
				//if (whoHasWon() == 1)
				//	points[static_cast<TicTacToeKI*>(*ki)]++;
				if (illegalMove)
				{
					if (currentPlayer == 1)
						points[static_cast<TicTacToeKI*>(*ki)]-=1;
#ifndef RANDOM_KI
					else 
						points[static_cast<TicTacToeKI*>(*otherKI)]-=1;
#endif
					illegalMoves++;
				}
				/*else if (whoHasWon() == 0)
				{					
					points[static_cast<TicTacToeKI*>(*ki)]++;
					points[static_cast<TicTacToeKI*>(*otherKI)]++;
					ties++;
				}
				else if (whoHasWon() == 1)
				{
					points[static_cast<TicTacToeKI*>(*ki)] += 1;
					points[static_cast<TicTacToeKI*>(*otherKI)] -= 1;
					firstWon++;
				}
				else if (whoHasWon() == -1)
				{
					points[static_cast<TicTacToeKI*>(*otherKI)] += 1;
					points[static_cast<TicTacToeKI*>(*ki)] -= 1;
					secondWon++;
				}*/
#ifndef RANDOM_KI
			}
#endif
		}
	}
	std::cout << "IM:" << illegalMoves << " T:" << ties << " 1W:" << firstWon << " 2W:" << secondWon << std::endl;
	//rateBestKI(learningRule);
}

void TicTacToe::setIllegalMove(bool illegalMove_)
{
	illegalMove = illegalMove_;
}

void TicTacToe::rateBestKI(EvolutionLearningRule& learningRule)
{
	auto highscoreList = getHighscoreList();
	TicTacToeKI* bestKI = dynamic_cast<TicTacToeKI*>(highscoreList->front().second);

	
	int wins = 0;
	int games = 1000;
	for (int g = 0; g < games; g++)
	{
		resetWorld();
		
		for (int i = 0; i < 9; i++)
		{
			bestKI->resetNN();
			if (i % 2 == 0)
			{
				currentPlayer = 1;
				bestKI->doNNCalculation(learningRule);
			}
			else
			{ 
				currentPlayer = -1;
				int x, y;
				do {
					x = std::min(2, (int)((float)rand() / RAND_MAX * 3));
					y = std::min(2, (int)((float)rand() / RAND_MAX * 3));
				} while (!isFree(x, y));
				setField(x, y);
			}
			if (whoHasWon() != 0)
				break;
		}

		if (whoHasWon() == 1)
			wins++;
	}

	std::cout << "Best KI: " << wins << "/" << games << std::endl;
}




bool TicTacToe::isFree(int x, int y)
{
	return fields[x][y] == 0;
}


int TicTacToe::whoHasWon()
{
	if (fields[0][0] != 0 && fields[0][0] == fields[0][1] && fields[0][1] == fields[0][2])
		return fields[0][0];
	else if (fields[1][0] != 0 && fields[1][0] == fields[1][1] && fields[1][1] == fields[1][2])
		return fields[1][0];
	else if (fields[2][0] != 0 && fields[2][0] == fields[2][1] && fields[2][1] == fields[2][2])
		return fields[2][0];

	else if (fields[0][0] != 0 && fields[0][0] == fields[1][0] && fields[1][0] == fields[2][0])
		return fields[0][0];
	else if (fields[0][1] != 0 && fields[0][1] == fields[1][1] && fields[1][1] == fields[2][1])
		return fields[0][1];
	else if (fields[0][2] != 0 && fields[0][2] == fields[1][2] && fields[1][2] == fields[2][2])
		return fields[0][2];

	else if (fields[0][0] != 0 && fields[0][0] == fields[1][1] && fields[1][1] == fields[2][2])
		return fields[0][0];
	else if (fields[2][0] != 0 && fields[2][0] == fields[1][1] && fields[1][1] == fields[0][2])
		return fields[2][0];
	else
		return 0;
}

void TicTacToe::setField(int x, int y)
{
	if (!isFree(x, y))
		illegalMove = true;
	else
		fields[x][y] = currentPlayer;
}

double TicTacToe::getScore(EvolutionObjectInterface* object)
{
	return points[static_cast<TicTacToeKI*>(object)];
}

void TicTacToe::resetWorld()
{
	for (auto column = fields.begin(); column != fields.end(); column++)
	{
		for (auto field = column->begin(); field != column->end(); field++)
		{
			*field = 0;
		}
	}
}


std::vector<double> TicTacToe::getSight()
{
	std::vector<double> sight;
	for (auto column = fields.begin(); column != fields.end(); column++)
	{
		for (auto field = column->begin(); field != column->end(); field++)
		{
			int fieldValue = currentPlayer * *field;
			if (fieldValue == 1)
				sight.push_back(1);
			else
				sight.push_back(0);
			if (fieldValue == -1)
				sight.push_back(1);
			else
				sight.push_back(0);
		}
	}
	return sight;
}
