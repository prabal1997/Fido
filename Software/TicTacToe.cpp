#include "TicTacToe.h"

net::NeuralNet TicTacToe::getBestPlayer(int numberOfIterations) {
	gen::GeneticAlgo geneticAlgo(50, 0.3, 0.7, getPlayerFitnesses);

	net::NeuralNet modelNet(9, 2, 2, 6, net::NeuralNet::integer);

	return geneticAlgo.getBestNeuralNetwork(numberOfIterations, modelNet);
}

std::vector<double> TicTacToe::getPlayerFitnesses(std::vector<net::NeuralNet> players) {
	std::vector<double> wins;
	for(int a = 0; a < players.size(); a++) {
		wins.push_back(0);
	}

	for(int a = 0; a < players.size(); a++) {
		for(int b = a+1; b < players.size(); b++) {
            net::NeuralNet player1 = players[a], player2 = players[b];
			int outcome = getOutcomeOfGame(&player1, &player2);
            if(outcome == 1) {
                wins[a]++;
                /*
                std::cout << "\n\n";
                std::vector<double> firstWeights, secondWeights, thirdWeights;
                firstWeights = player1.getWeights();
                std::cout << "outcome " << outcome << "\n";
                
                outcome = getOutcomeOfGame(&player1, &player2);
                secondWeights = player1.getWeights();
                std::cout << "outcome " << outcome << "\n";
                
                outcome = getOutcomeOfGame(&player1, &player2);
                thirdWeights = player1.getWeights();
                std::cout << "outcome " << outcome << "\n";
                
                bool firstEqualToSecond = true;
                bool firstEqualToThird = true;
                for(int a = 0; a < firstWeights.size(); a++) {
                    if(firstWeights[a] != secondWeights[a]) firstEqualToSecond = false;
                    if(firstWeights[a] != thirdWeights[a]) firstEqualToSecond = false;
                }
                
                if(firstEqualToSecond == true) {
                    std::cout << "First equal to second\n";
                } else {
                    std::cout << "First NOT EQUAL to second\n";
                }
                
                if(firstEqualToThird == true) {
                    std::cout << "First equal to third\n";
                } else {
                    std::cout << "First NOT EQUAL to third\n";
                }*/
                
                std::cout << "win: " << wins[a] << "\n";
            } else if(outcome == 2){
                wins[b]++;
                std::cout << "win: " << wins[a] << "\n";
            }
		}
	}
	return wins;
}

int TicTacToe::getOutcomeOfGame(net::NeuralNet *player1, net::NeuralNet *player2) {
    /*std::vector<double> weights1 = player1->getWeights(), weights2 = player2->getWeights();
    std::cout << "Weights 1: ";
    for (int a = 0; a < weights1.size(); a++) {
        std::cout << weights1[a] << ", ";
    }
    std::cout << "\n";
    
    std::cout << "Weights 2: ";
    for (int a = 0; a < weights2.size(); a++) {
        std::cout << weights2[a];
    }
    std::cout << "\n";*/
    
    std::vector< std::vector<int> > board;
	for(int a = 0; a < 3; a++) {
		std::vector<int> row;
		for(int b = 0; b < 3; b++) {
			row.push_back(0);
		}
		board.push_back(row);
	}
    
    bool didOneMakeMove = true;
    
    std::vector<double> oneDBoard;
    for(int a = 0; a < board.size(); a++) for(int b = 0; b < board[a].size(); b++) oneDBoard.push_back(board[a][b]);
    
	while(hasGameEnded(board) == false && didOneMakeMove == true) {
        didOneMakeMove = false;
        //std::cout << "Board: ";
        //for(int a = 0; a < oneDBoard.size(); a++) std::cout << oneDBoard[a];
        //std::cout << "\n";
        
		std::vector<double> coordinates1 = player1->getOutput(oneDBoard);
		long p1X = ((long)coordinates1[0]) % 3, p1Y = ((long)coordinates1[1]) % 3;
        //std::cout << "x, y: " << p1X << ", " << p1Y << "\n";
        if(board[p1X][p1Y] == 0) {
            didOneMakeMove = true;
            board[p1X][p1Y] = 1;
            oneDBoard[((p1X*3)+p1Y)] = 1;
        }

		std::vector<double> coordinates2 = player2->getOutput(oneDBoard);
		long p2X = ((long)coordinates2[0]) % 3, p2Y = ((long)coordinates2[1]) % 3;
        //std::cout << "x, y: " << p2X << ", " << p2Y << "\n";
        if(board[p2X][p2Y] == 0) {
            didOneMakeMove = true;
            board[p2X][p2Y] = 2;
            oneDBoard[((p2X*3)+p2Y)] = 1;
        }
	}
    

	/// Check if someone filled up a row
	for(int a = 0; a < board.size(); a++) {
		int initialValue = board[a][0];
		bool isRowOneValue = true;
		for(int b = 1; b < board[a].size(); b++) {
			if(initialValue != board[a][b]) isRowOneValue = false;
		}
		if(isRowOneValue == true) {
			if(initialValue == 1) return 1;
			else if(initialValue == 2) return 2;
		}
	}
	
	/// Check if someone filled up a column
	for(int a = 0; a < board[0].size(); a++) {
		int initialValue = board[0][a];
		bool isColumOneValue = true;
		for(int b = 1; b < board.size(); b++) {
			if(initialValue != board[b][a]) isColumOneValue = false;
		}
		if(isColumOneValue == true) {
			if(initialValue == 1) return 1;
			else if(initialValue == 2) return 2;
		}
	}

	/// Check if someone filled a diagonal
	int initialValue = board[0][0];
	bool isDiagonalOneValue = true;
	for(int a = 1; a < board.size(); a++) if(board[a][a] != initialValue) isDiagonalOneValue = false;
	if(isDiagonalOneValue == true) {
		if(initialValue == 1) return 1;
		else if(initialValue == 2) return 2;
	}

	initialValue = board[0][board.size()-1];
	isDiagonalOneValue = true;
	for(int a = 1; a < board.size(); a++) if(board[a][(board.size() - 1) - a] != initialValue) isDiagonalOneValue = false;
	if(isDiagonalOneValue == true) {
		if(initialValue == 1) return 1;
		else if(initialValue == 2) return 2;
	}
    
    std::cout << "Reached end\n";
	return 0;
}

bool TicTacToe::hasGameEnded(std::vector< std::vector<int> > board) {
	bool foundEmptySpace = false;
    for(int a = 0; a < board.size(); a++) for(int b = 0; b < board[a].size(); b++) if(board[a][b] == 0) foundEmptySpace = true;
	if(foundEmptySpace == false) return true;
	
	/// Check if someone filled up a row
	for(int a = 0; a < board.size(); a++) {
		int initialValue = board[a][0];
        if(initialValue != 0) {
            bool isRowOneValue = true;
            for(int b = 1; b < board[a].size(); b++) {
                if(initialValue != board[a][b]) isRowOneValue = false;
            }
            if(isRowOneValue == true) return true;
        }
	}
	
	/// Check if someone filled up a column
	for(int a = 0; a < board[0].size(); a++) {
		int initialValue = board[0][a];
        if(initialValue != 0) {
            bool isColumOneValue = true;
            for(int b = 1; b < board.size(); b++) {
                if(initialValue != board[b][a]) isColumOneValue = false;
            }
            if(isColumOneValue == true) return true;
        }
	}
    

	/// Check if someone filled a diagonal
	int initialValue = board[0][0];
    if(initialValue != 0) {
        bool isDiagonalOneValue = true;
        for(int a = 1; a < board.size(); a++) if(board[a][a] != initialValue) isDiagonalOneValue = false;
        if(isDiagonalOneValue == true) return true;
    }
    
	initialValue = board[0][board.size()-1];
	if(initialValue != 0) {
        bool isDiagonalOneValue = true;
        for(int a = 1; a < board.size(); a++) if(board[a][(board.size() - 1) - a] != initialValue) isDiagonalOneValue = false;
        if(isDiagonalOneValue == true) return true;
    }
	
	return false;
}

void TicTacToe::printBoard(std::vector< std::vector<int> > board) {
	for(int a = 0; a < board.size(); a++) {
		for(int b = 0; b < board[a].size(); b++) {
			if(board[a][b] == 0) std::cout << "| ";
			else if(board[a][b] == 1) std::cout << "|X";
			else if(board[a][b] == 2) std::cout << "|O";
		}
		std::cout << "|\n";
	}
}
