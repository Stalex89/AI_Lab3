// AI_Lab3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Piece.h"
#include "Board.h"

using namespace sf;

//
//bool isLegalMove(Vector2f &oldPos, Vector2f &newPos, int n)
//{
//	Vector2f p = newPos - oldPos;
//
//	// the move is legal if
//	// 1) Move only by diagonal for one field forward (+ backward for queen) 
//	if (pieces[n].getTextureRect() == IntRect(0, 0, size, size)) // if piece is black
//	{
//		if ((p.x < 0 && p.y > 0) || (p.x > 0 && p.y > 0)) // if southwest or southeast 
//		{
//			if (sqrt(p.x*p.x + p.y*p.y) <= size*2) // if moving less than two fields
//				return true;
//		}
//
//	}
//	else if (pieces[n].getTextureRect() == IntRect(size, 0, size, size)) // if piece is white
//	{
//		if ((p.x < 0 && p.y < 0) || (p.x > 0 && p.y < 0)) // if northwest or northeast
//		{
//			if (sqrt(p.x*p.x + p.y*p.y) <= size*2) // if moving less than two fields
//				return true;
//		}
//	}
//	else if (pieces[n].getTextureRect() == IntRect(0, size, size, size) ||
//		     pieces[n].getTextureRect() == IntRect(size, size, size, size)) // if piece is queen
//	{
//		if ((p.x < 0 && p.y > 0) || (p.x > 0 && p.y > 0) || 
//			(p.x < 0 && p.y < 0) || (p.x > 0 && p.y < 0)) // in any diagonal direction
//		{
//			if (sqrt(p.x*p.x + p.y*p.y) <= size*2) // if moving less than two fields
//				return true;
//		}
//	}
//		
//
//	// 2) 
//
//
//	return false;
//}

int main()
{
	RenderWindow window(VideoMode(504, 504), "AI Draughts");
	int pieceSize = 56;
	bool isMove = false;
	float dx = 0, dy = 0;
	Vector2f oldPos, newPos;
	int n = 0;

	std::string str = "";

	Board *board = new Board();
	board->loadPosition();

	while (window.isOpen())
	{
		Vector2i mousePos = Mouse::getPosition(window);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == Event::MouseButtonPressed)
				if (event.key.code == Mouse::Right)
				{
					if (board->getPositions().length() > 6)
						board->getPositions().erase(board->getPositions().length() - 6, 5);

					else if (board->getPositions().length() == 5)
						board->getPositions().erase();
									
					board->loadPosition();


				}


			if (event.type == Event::MouseButtonPressed)
				if (event.key.code == Mouse::Left)
					for (int i = 0; i < 24; i++)
						if (board->getPiece(i).getSprite().getGlobalBounds().contains(float(mousePos.x), float(mousePos.y)))
						{
							isMove = true; n = i;
							dx = mousePos.x - board->getPiece(i).getPosition().x;
							dy = mousePos.y - board->getPiece(i).getPosition().y;
							oldPos = board->getPiece(i).getPosition();
						}

			if (event.type == Event::MouseButtonReleased)
				if (event.key.code == Mouse::Left)
				{
					isMove = false;
					Vector2f p = board->getPiece(n).getSprite().getPosition();
					Vector2f newPos = Vector2f(float(pieceSize * int(p.x / pieceSize)),
						float(pieceSize*int(p.y / pieceSize))) + board->getOffset();
					if(board->isInPlayfield(newPos) && board->isLegalMove(oldPos,newPos,n))
					{
						str = board->toChessNote(oldPos) + board->toChessNote(newPos);
						board->getPiece(n).move(newPos);
						board->getPositions() += str + " ";
						std::cout << board->getPositions() << std::endl;
					}
					else board->getPiece(n).getSprite().setPosition(oldPos);
				}
		}
		if (isMove) board->getPiece(n).getSprite().setPosition(mousePos.x - dx, mousePos.y - dy);

		window.clear();
		window.draw(board->getSprite());
		for (int i = 0; i < 24; i++) window.draw(board->getPiece(i).getSprite());
		window.display();
	}


	//for (int i = 0; i < 24; i++) pieces[i].setTexture(tPieces);
	//loadPosition();

	//bool isMove = false;
	//float dx = 0, dy = 0;
	//Vector2f oldPos, newPos;
	//std::string str;
	//int n = 0;

	//while (window.isOpen())
	//{
	//	Vector2i pos = Mouse::getPosition(window);

	//	sf::Event event;
	//	while (window.pollEvent(event))
	//	{
	//		if (event.type == sf::Event::Closed)
	//			window.close();

	//		// move back
	//		if (event.type == Event::KeyPressed)
	//			if (event.key.code == Keyboard::BackSpace)
	//			{
	//				if (position.length() > 6)
	//					position.erase(position.length() - 6, 5);

	//				else if (position.length() == 5)
	//					position.erase();
	//				
	//				loadPosition();
	//			}

	//		// drag and drop
	//		if (event.type == Event::MouseButtonPressed)
	//			if (event.key.code == Mouse::Left)
	//				for (int i = 0; i < 24; i++)
	//					if (pieces[i].getGlobalBounds().contains(float(pos.x), float(pos.y)))
	//					{
	//						isMove = true; n = i;
	//						dx = pos.x - pieces[i].getPosition().x;
	//						dy = pos.y - pieces[i].getPosition().y;
	//						oldPos = pieces[i].getPosition();
	//					}

	//		if (event.type == Event::MouseButtonReleased)
	//			if (event.key.code == Mouse::Left)
	//			{
	//				isMove = false;
	//				Vector2f p = pieces[n].getPosition();
	//				Vector2f newPos = Vector2f(float(size * int(p.x / size)),
	//					float(size*int(p.y / size))) + offset;

	//				if (oldPos != newPos && isInPlayfield(newPos, sBoard) && isLegalMove(oldPos, newPos, n))
	//				{
	//					str = toChessNote(oldPos) + toChessNote(newPos);
	//					move(str);
	//					position += str + " ";
	//					std::cout << position << std::endl;
	//					pieces[n].setPosition(newPos);
	//				}
	//				else pieces[n].setPosition(oldPos);
	//			}
	//	
	//	}

	//	// computer move
	//	if (Keyboard::isKeyPressed(Keyboard::Space))
	//	{
	//		str = "d6d5";

	//		oldPos = toCoord(str[0], str[1]);
	//		newPos = toCoord(str[2], str[3]);

	//		for (int i = 0; i < 24; i++)
	//			if (pieces[i].getPosition() == oldPos) n = i;

	//		// animation
	//		time(&now);

	//		for (int k = 0; k < 50; k++)
	//		{
	//			oldTime = now;
	//			time(&now);
	//			Vector2f p = newPos - oldPos;
	//			pieces[n].move(p.x / 50, p.y / 50);
	//			window.draw(sBoard);
	//			for (int i = 0; i < 24; i++)
	//				window.draw(pieces[i]);
	//			window.draw(pieces[n]);
	//			window.display();
	//		}

	//		move(str);
	//		position += str + " ";
	//		pieces[n].setPosition(newPos);
	//	}

	//	if (isMove) pieces[n].setPosition(pos.x - dx, pos.y - dy);

	//window.clear();
	//window.draw(sBoard);
	//for (int i = 0; i < 24; i++) window.draw(pieces[i]);
	//window.display();
	//}

	return 0;
}