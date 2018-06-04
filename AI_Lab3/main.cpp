// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Piece.h"
#include "PieceGUI.h"
#include "Board.h"
#include "BoardGUI.h"
#include "Agent.h"

using namespace sf;

// Define constraints 
constexpr int aiDepth{ 2 }; // depth of the tree

//void playerMove(RenderWindow* window, Board &board);
//void AIMove(sf::RenderWindow* window, Board &board);
//void updateDisplay(RenderWindow* window, Board &board);

void playerMove(RenderWindow* window, Board &board, BoardGUI &boardGui);
void AIMove(sf::RenderWindow* window, Board &board, BoardGUI &boardGui, Piece::Color agentColor);
void updateDisplay(RenderWindow* window, Board &board, BoardGUI &boardGui);


int main()
{
	RenderWindow window(VideoMode(504, 504), "AI Draughts");
	
	Board *board = new Board();
	BoardGUI *boardGui = new BoardGUI();

	board->loadPosition();
	boardGui->loadPosition(board->getPieces());

	//updateDisplay(&window, *board, *boardGui);

	while (true && window.isOpen())
	{
		//calculateAllMovesForColor(*board, Piece::Color::WHITE);
		board->calculateAllMovesForColor(Piece::Color::WHITE);
		if (!board->isGameOver(Piece::Color::WHITE))
		{
			std::cout << "White's turn" << std::endl;

			//playerMove(&window, *board);
			//playerMove(&window, *board, *boardGui);
			AIMove(&window, *board, *boardGui, Piece::Color::WHITE);
		}
		else
		{
			std::cout << "Black won!" << std::endl;
			break;
		}
		//clearAllMovesForColor(*board, Piece::Color::WHITE);
		board->clearAllMovesForColor(Piece::Color::WHITE);

		updateDisplay(&window, *board, *boardGui);

		system("pause");

		//calculateAllMovesForColor(*board, Piece::Color::BLACK);
		board->calculateAllMovesForColor(Piece::Color::BLACK);
		if (!board->isGameOver(Piece::Color::BLACK))
		{
			std::cout << "Black's turn" << std::endl;
			//AIMove(&window, *board);
			AIMove(&window, *board, *boardGui, Piece::Color::BLACK);
		}
		else
		{
			std::cout << "White won!" << std::endl;
			break;
		}
		//clearAllMovesForColor(*board, Piece::Color::BLACK);
		board->clearAllMovesForColor(Piece::Color::BLACK);

		updateDisplay(&window, *board, *boardGui);

		system("pause");
	}



	delete board;
	board = nullptr;

	system("pause");
	return 0;
}

#pragma region unused

//while (window.isOpen())
//{
//	Vector2i mousePos = Mouse::getPosition(window);
//	sf::Event event;

//	while (window.pollEvent(event))
//	{
//		if (event.type == sf::Event::Closed)
//			window.close();

//		if (event.type == Event::MouseButtonPressed)
//			if (event.key.code == Mouse::Right)
//			{
//				if (board->getPositions().length() > 6)
//					board->getPositions().erase(board->getPositions().length() - 6, 5);

//				else if (board->getPositions().length() == 5)
//					board->getPositions().erase();
//								
//				board->loadPosition();


//			}


//		if (event.type == Event::MouseButtonPressed)
//			if (event.key.code == Mouse::Left)
//				for (int i = 0; i < 24; i++)
//					if (board->getPiece(i).getSprite().getGlobalBounds().contains(float(mousePos.x), float(mousePos.y)))
//					{
//						isMove = true; n = i;
//						dx = mousePos.x - board->getPiece(i).getPosition().x;
//						dy = mousePos.y - board->getPiece(i).getPosition().y;
//						oldPos = board->getPiece(i).getPosition();
//					}

//		if (event.type == Event::MouseButtonReleased)
//			if (event.key.code == Mouse::Left)
//			{
//				isMove = false;
//				Vector2f p = board->getPiece(n).getSprite().getPosition();
//				Vector2f newPos = Vector2f(float(pieceSize * int(p.x / pieceSize)),
//					float(pieceSize*int(p.y / pieceSize))) + board->getOffset();
//				if(board->isInPlayfield(newPos) && board->isLegalMove(oldPos,newPos,n))
//				{
//					str = board->toChessNote(oldPos) + board->toChessNote(newPos);
//					board->getPiece(n).move(newPos);
//					board->getPositions() += str + " ";
//					std::cout << board->getPositions() << std::endl;
//				}
//				else board->getPiece(n).getSprite().setPosition(oldPos);
//			}
//	}
//	if (isMove) board->getPiece(n).getSprite().setPosition(mousePos.x - dx, mousePos.y - dy);

//	window.clear();
//	window.draw(board->getSprite());
//	for (int i = 0; i < 24; i++) window.draw(board->getPiece(i).getSprite());
//	window.display();
//}

#pragma endregion


void playerMove(sf::RenderWindow* window, Board &board, BoardGUI &boardGui)
{
	int selectedPieceIdx = -1;
	bool isSelected = true;


	//updateDisplay(window, board);
	updateDisplay(window, board, boardGui);
	while (window->isOpen())
	{
		Vector2i mousePos = Mouse::getPosition(*window);
		sf::Event event;

		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();

			if (event.type == Event::MouseButtonReleased)
			{
				if (event.key.code == Mouse::Left)
				{
					int i = boardGui.getPieceIdxBySprite(mousePos); // get the index of piece clicked by mouse
					if (i != -1) // if we clicked on piece 
					{
						if (selectedPieceIdx != i && board.getPieceByIdx(i).getColor() == Piece::Color::WHITE) // if the new piece was clicked while having selected piece and this is player's piece
						{
							selectedPieceIdx = i;
							isSelected = true;
							std::cout << "Selected piece = " << selectedPieceIdx << std::endl;
							board.getPieceByIdx(selectedPieceIdx).printPossibleMoves();
						}
						else // if we clicked on the selected piece or not player's piece
						{
							selectedPieceIdx = -1;
							isSelected = false;
							std::cout << "isSelected = " << isSelected << std::endl;
						}
					}
					else if (selectedPieceIdx != -1) // else if we clicked on empty field having selected piece
					{
						sf::Vector2f snapedPos = sf::Vector2f((float)(mousePos.x - (mousePos.x - int(board.getOffset().x)) % board.getPieceSize()),
							(float)(mousePos.y - ((mousePos.y - int(board.getOffset().y)) % board.getPieceSize()))); // get position of mouse click snaped to the upper left corner of square

						std::string finalPos = board.toChessNote(snapedPos);

						int m = board.getPieceByIdx(selectedPieceIdx).getPossibleMoveIdx(finalPos); // get index of move containing final position
						if (m != -1) // if we found move with final position
						{
							//std::cout << "isSelected = " << isSelected << std::endl;
							std::cout << "Selected move: <" << board.getPieceByIdx(selectedPieceIdx).getPossibleMove(m).first
								<< ", " << board.getPieceByIdx(selectedPieceIdx).getPossibleMove(m).second << ">" << std::endl;

							sf::Vector2f oldPos = board.toCoord(board.getPieceByIdx(selectedPieceIdx).getPossibleMove(m).second[0],
								board.getPieceByIdx(selectedPieceIdx).getPossibleMove(m).second[1]);

							sf::Vector2f newPos = board.toCoord(board.getPieceByIdx(selectedPieceIdx).getPossibleMove(m).second[2],
								board.getPieceByIdx(selectedPieceIdx).getPossibleMove(m).second[3]);

							if (board.isMoveforOneField(oldPos, newPos))
							{
								board.getPieceByIdx(selectedPieceIdx).move(newPos);
								boardGui.getPieceByIdx(selectedPieceIdx).move(newPos);
								board.addPosition(board.getPieceByIdx(selectedPieceIdx).getPossibleMove(m));
								std::cout << "Move to position " << std::endl;

								if (board.getPieceByIdx(selectedPieceIdx).getPosition().y == board.getOffset().y && !board.getPieceByIdx(selectedPieceIdx).isKing())
								{
									std::cout << "Promoted to king " << std::endl;
									board.getPieceByIdx(selectedPieceIdx).promote();
									boardGui.getPieceByIdx(selectedPieceIdx).promote(board.getPieceByIdx(selectedPieceIdx).getColor());

								}
							}
							else
							{
								for (int idx = 0; idx < board.getPieceByIdx(selectedPieceIdx).getPossibleMove(m).first; idx++)
								{

									sf::Vector2f newPos = board.toCoord(board.getPieceByIdx(selectedPieceIdx).getPossibleMove(m).second[idx * 5 + 2],
										board.getPieceByIdx(selectedPieceIdx).getPossibleMove(m).second[idx * 5 + 3]);

									sf::Vector2f attackPos = Vector2f((newPos.x + oldPos.x)/2, (newPos.y + oldPos.y) / 2);
									//sf::Vector2f attackPos = Vector2f((newPos.x + board.getPieceByIdx(selectedPieceIdx).getPosition().x) / 2,
									//	(newPos.y + board.getPieceByIdx(selectedPieceIdx).getPosition().y) / 2);

									int index = board.getPieceIdxOnPosition(attackPos);
									board.getPieceByIdx(selectedPieceIdx).capture(newPos, board.getPieceByIdx(index));
									boardGui.getPieceByIdx(selectedPieceIdx).capture(newPos, boardGui.getPieceByIdx(index));
								}
								board.addPosition(board.getPieceByIdx(selectedPieceIdx).getPossibleMove(m));
								std::cout << "Attacked position " << std::endl;

								if (board.getPieceByIdx(selectedPieceIdx).getPosition().y == board.getOffset().y && !board.getPieceByIdx(selectedPieceIdx).isKing())
								{
									std::cout << "Promoted to king " << std::endl;
									board.getPieceByIdx(selectedPieceIdx).promote();
									boardGui.getPieceByIdx(selectedPieceIdx).promote(board.getPieceByIdx(selectedPieceIdx).getColor());
								}
							}

							board.printPositions();
							//updateDisplay(window, board);
							updateDisplay(window, board, boardGui);
							selectedPieceIdx = -1;
							isSelected = false;
							return;

						}
						else
						{
							//std::cout << "isSelected = " << isSelected << std::endl;
							std::cout << "Selected empty field" << std::endl;
						}

					}

				}

				// Undo move (undo 2 moves - one for computer and one for player)
				if (event.key.code == Mouse::Right)
					if (board.getPositionsSize() > 0)
					{
						board.deleteLastPosition();
						//board.deleteLastPosition();
						board.loadPosition();
						boardGui.loadPosition(board.getPieces());
						board.printPositions();
						//clearAllMovesForColor(board, Piece::Color::WHITE);
						//calculateAllMovesForColor(board, Piece::Color::WHITE);
						board.clearAllMovesForColor(Piece::Color::WHITE);
						board.calculateAllMovesForColor(Piece::Color::WHITE);

						//updateDisplay(window, board);
						updateDisplay(window, board, boardGui);
					}
			}
		}
	}
}


void AIMove(sf::RenderWindow* window, Board &board, BoardGUI &boardGui, Piece::Color agentColor)
{
	//intialize AI agent
	Agent *a = new Agent(aiDepth, board, agentColor);
	
	// get best possible move
	std::pair<int, std::vector<std::pair<int, std::string>>> bestm = a->alphaBeta(a->getRootNode(), a->getTreeDepth(), true);
	//std::pair<int, std::pair<int, std::string>> bestm = a->alphaBeta(a->getRootNode(), a->getTreeDepth(), true);

	std::pair<int, std::string> bestMove = bestm.second.front();
	
	// delete agent
	delete a;
	a = nullptr;

	std::cout << "Selected move: <" << bestMove.first
		<< ", " << bestMove.second << ">" << std::endl;
	
	sf::Vector2f oldPos = board.toCoord(bestMove.second[0], bestMove.second[1]);
	
	sf::Vector2f newPos = board.toCoord(bestMove.second[2], bestMove.second[3]);
	
	if (board.isMoveforOneField(oldPos, newPos))
	{
		int idx = board.getPieceIdxOnPosition(oldPos);
		board.getPieceByIdx(idx).move(newPos);
		boardGui.getPieceByIdx(idx).move(newPos);
		board.addPosition(bestMove);
		std::cout << "Move to position " << std::endl;

		
	
		if ((board.getPieceByIdx(board.getPieceIdxOnPosition(newPos)).getPosition().y == (boardGui.getYBound() - board.getOffset().y - board.getPieceSize()) || board.getPieceByIdx(board.getPieceIdxOnPosition(newPos)).getPosition().y == board.getOffset().y)
			&& !board.getPieceByIdx(board.getPieceIdxOnPosition(newPos)).isKing())
		{
			std::cout << "Promoted to king " << std::endl;
			board.getPieceByIdx(board.getPieceIdxOnPosition(newPos)).promote();
			boardGui.getPieceByIdx(board.getPieceIdxOnPosition(newPos)).promote(board.getPieceByIdx(board.getPieceIdxOnPosition(newPos)).getColor());
		}
	}
	else
	{
		sf::Vector2f attackOldPos;
		sf::Vector2f attackNewPos;

		for (int idx = 0; idx < bestMove.first; idx++)
		{
			attackOldPos = board.toCoord(bestMove.second[idx * 5], bestMove.second[idx * 5 +1]);
			attackNewPos = board.toCoord(bestMove.second[idx * 5 + 2], bestMove.second[idx * 5 + 3]);
			
			sf::Vector2f attackPos = Vector2f((attackNewPos.x + attackOldPos.x) / 2, (attackNewPos.y + attackOldPos.y) / 2);
			
			int oindex = board.getPieceIdxOnPosition(attackOldPos);
			int index = board.getPieceIdxOnPosition(attackPos);
			board.getPieceByIdx(oindex).capture(attackNewPos, board.getPieceByIdx(index));
			boardGui.getPieceByIdx(oindex).capture(attackNewPos, boardGui.getPieceByIdx(index));
		}

		board.addPosition(bestMove);
		std::cout << "Attacked position " << std::endl;

		if ((board.getPieceByIdx(board.getPieceIdxOnPosition(attackNewPos)).getPosition().y == (boardGui.getYBound() - board.getOffset().y - board.getPieceSize()) || board.getPieceByIdx(board.getPieceIdxOnPosition(newPos)).getPosition().y == board.getOffset().y)
			&& !board.getPieceByIdx(board.getPieceIdxOnPosition(attackNewPos)).isKing())
		{
			std::cout << "Promoted to king " << std::endl;
			board.getPieceByIdx(board.getPieceIdxOnPosition(attackNewPos)).promote();
			boardGui.getPieceByIdx(board.getPieceIdxOnPosition(newPos)).promote(board.getPieceByIdx(board.getPieceIdxOnPosition(newPos)).getColor());
		}
	}
	

	
	board.printPositions();
	//updateDisplay(window, board);
	updateDisplay(window, board, boardGui);
}

void updateDisplay(RenderWindow* window, Board &board, BoardGUI &boardGui)
{
	window->clear();
	window->draw(boardGui.getSprite());
	//for (int i = 0; i < board.getPiecesNum(); i++) window->draw(board.getPieceByIdx(i).getSprite());
	for (int i = 0; i < boardGui.getPieces().size(); i++)
	{
		sf::Sprite sp = boardGui.getPieces().at(i).getSprite();
		window->draw(sp);
	}
	window->display();
}


