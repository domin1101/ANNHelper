#pragma once

#ifndef _TICTACTOE_H_
#define _TICTACTOE_H_

// Library Includes
#include <vector>
#include <map>
#include <array>
#include <SFML/Graphics.hpp>

// Include
#include "Learning/Evolution/AbstractSimpleEvolutionWorld.hpp"
#include "Examples/TicTacToeDrawer.hpp"
#include "NeuralNetwork/NeuralNetworkIO.hpp"

// Forward declarations
class EvolutionLearningRule;
class AbstractEvolutionObject;
class TicTacToeKI;
class AbstractTile;

class TicTacToe : public AbstractSimpleEvolutionWorld
{
protected:
	std::vector<std::vector<int>> fields;	
	std::map<TicTacToeKI*, int> points;
	AbstractEvolutionObject* createNewObject();
	sf::RenderWindow window;
	std::unique_ptr<TicTacToeDrawer> drawer;
	void resetWorld();
	int whoHasWon();
	int currentPlayer;
	bool displayMode;
	bool illegalMove;
	std::vector<TicTacToeKI*> bestAIs;
	int defaultResetGenerationCount;
	int currentResetGenerationCount;
	bool nextDecisionCombination(std::array<int, 4>& decisionNr, int level = 0);
	void simulateGame(TicTacToeKI* ai1, TicTacToeKI* ai2, int startingAI, EvolutionLearningRule& learningRule, int& illegalMoves, int& ties);
public:
	TicTacToe();
	bool doSimulationStep(EvolutionLearningRule& learningRule);
	NeuralNetworkIO<double> getSight();
	void setField(int x, int y);
	double getScore(AbstractEvolutionObject* object);
	bool isFree(int x, int y);
	int getFieldValue(int x, int y);
	void rateBestKI(EvolutionLearningRule& learningRule);
	void setIllegalMove(bool illegalMove_);
	std::vector<TicTacToeKI*>* getBestAIs();
	void startNewGame(int firstPlayer);
	bool hasGameFinished();

};

#endif
