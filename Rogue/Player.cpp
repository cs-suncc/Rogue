#include "Player.h"
#include "TextureManager.h"
#include "Game.h"
#include "InputHandler.h"
#include "MapManager.h"
#include "playing.h"
#include "UI.h"
#include <cmath>
#ifdef _DEBUG
#include <iostream>
#endif

enum PlayerState {
	MOVING,
	JUMPING,
	ATTACKING,
	DEFENDING
};

Player::Player():
	SDLGameObject(), velocity(0.0, 0.0), accelerate(0.0, 0.0) {
}

void Player::load(const LoaderParams &param) {
	SDLGameObject::load(param);
	UI::Instance().newUI("PlayerHP", "Bar", 0, 0, 350, 15, hitpoint);
	UI::Instance().setUIBarColor("PlayerHP", 200, 50, 50);
	UI::Instance().newUI("PlayerMana", "Bar", 0, 15 + 1, 200, 10, mana);
	UI::Instance().setUIBarColor("PlayerMana", 100, 50, 200);
}

void Player::draw() {
	// 绘制基点(x, y) -> (-1, 0)
	//|
	//+--
	//|
	TextureManager::Instance().drawFrame(textureID, x, y - 32, width, height, currentRow, currentFrame, 
										 Game::Instance().getRenderer(), faceflip?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE);
#ifdef _DEBUG
	//std::cerr << "PLAYER AT "<< x << " " << y - height << " " << std::endl;
#endif
}

void Player::update() {
	static PlayerState currentState = MOVING;
	int xaxis = InputHandler::Instance().xValue(0, XBoxInputNodes::LEFT_JOYSTICK);
	int yaxis = InputHandler::Instance().yValue(0, XBoxInputNodes::LEFT_JOYSTICK);
	auto currentMap = MapManager::Instance().getMap(
		((PlayingState *)Game::Instance().getGameStateMachine()->currentState())->getCurrentMap()
	);
	switch (currentState) {
	case MOVING:
#ifdef _DEBUG
		//std::cerr << "In MOVING " <<velocity.getX()<<" "<<velocity.getY()<< std::endl;
#endif // _DEBUG
		if (yaxis > 0) {	//=> DEFENDING
			setCurrentRow(3);
			setCurrentFrame(0);
			velocity.setX(0);
			currentState = DEFENDING;
			break;
		}
		if (yaxis < 0) {	//=> JUMPING
			setCurrentRow(1);
			setCurrentFrame(0);
			velocity.setY(-10);
			accelerate.setY(1);
			currentState = JUMPING;
			break;
		}
		if (xaxis > 0) {
			faceflip = false;
			setCurrentFrame((currentFrame + 1) % 4);
			velocity.setX(7.0);
		} else if (xaxis < 0) {
			faceflip = true;
			setCurrentFrame((currentFrame + 1) % 4);
			velocity.setX(-7.0);
		} else if (xaxis == 0) {
			velocity.setX(0);
			setCurrentFrame(0);
		}
		//坠落
		if (currentMap->getTile((y + 32) / 32, (x + 11) / 32)->passable()) {
			accelerate.setY(1);
		} else {
			accelerate.setY(0);
		}
		break;
	case JUMPING:
#ifdef _DEBUG
		//std::cerr << "In JUMPING " << velocity.getX() << " " << velocity.getY() << std::endl;
#endif // _DEBUG
		if (currentMap->getTile((y + 32) / 32, (x + 11) / 32)->passable() == false) { //跳跃正常完成 => MOVING
			accelerate.setY(0);
			setCurrentRow(0);
			currentState = MOVING;
		}

		if (xaxis > 0) {
			faceflip = false;
			velocity.setX(3.0);
		} else if (xaxis < 0) {
			faceflip = true;
			velocity.setX(-3.0);
		} else if (xaxis == 0) {
			//velocity.setX(0);
		}
		break;
	case DEFENDING:
		if (yaxis > 0) {
			setCurrentFrame((currentFrame + 1) % 4);
		} else {
			setCurrentRow(0);
			setCurrentFrame(0);
			currentState = MOVING;
		}
		if (currentMap->getTile((y + 32) / 32, (x + 11) / 32)->passable()) {
			accelerate.setY(1);
		} else {
			accelerate.setY(0);
		}
		break;
	}
	velocity += accelerate;
	auto dx = x + (int)velocity.getX();
	auto dy = y + (int)velocity.getY();
	//碰撞检查+归位
	bool bonceX = false;
	bool bonceY = false;
	int ckp[][2] = {
		{ (dx + 11) / 32, (dy - 32) / 32},	{(dx + 20) / 32, (dy - 32) / 32},
		{ (dx + 11) / 32, dy / 32},			{(dx + 20) / 32, dy / 32},
		{ (dx + 11) / 32, (dy + 31) / 32},	{(dx + 20) / 32, (dy + 31) / 32},
	};//检测点
	//y有正速度(向下
	// <注意getTile的参数顺序>
	if (velocity.getY() > 0) {
		if (!currentMap->getTile(ckp[4][1], ckp[4][0])->passable() ||
			!currentMap->getTile(ckp[5][1], ckp[5][0])->passable()) {
			dy = ((dy) / 32) * 32;
			bonceY = true;
#ifdef _DEBUG
			std::cerr << "BOUNCE 1:" << std::endl;
#endif // _DEBUG
		}
	}
	//y有负速度(向上
	if (velocity.getY() < 0) {
		if (!currentMap->getTile(ckp[0][1], ckp[1][0])->passable() ||
			!currentMap->getTile(ckp[1][1], ckp[1][0])->passable()) {
			dy = ((dy + 10) / 32) * 32;
			bonceY = true;
#ifdef _DEBUG
			std::cerr << "BOUNCE 2:" << std::endl;
#endif // _DEBUG
		}
	}
	//y轴调整完成，重新计算检测点
	ckp[0][0] = (dx + 11) / 32; ckp[0][1] = (dy - 32) / 32;
	ckp[2][0] = (dx + 11) / 32; ckp[2][1] = dy / 32;
	ckp[4][0] = (dx + 11) / 32; ckp[4][1] = (dy + 31) / 32;
	ckp[1][0] = (dx + 20) / 32; ckp[1][1] = (dy - 32) / 32;
	ckp[3][0] = (dx + 20) / 32; ckp[3][1] = dy / 32;
	ckp[5][0] = (dx + 20) / 32; ckp[5][1] = (dy + 31) / 32;
	//x有正速度（向右
	if (velocity.getX() > 0) {
		if (!currentMap->getTile(ckp[1][1], ckp[1][0])->passable() ||
			!currentMap->getTile(ckp[3][1], ckp[3][0])->passable() ||
			!currentMap->getTile(ckp[5][1], ckp[5][0])->passable()) { //x正向无法通过，移动到边界
			dx = (dx / 32) * 32 + 9;	//假设：速度不大于32px/frame，下同
			bonceX = true;
		}
	}
	//x有负速度(向左
	if (velocity.getX() < 0) {
		if (!currentMap->getTile(ckp[0][1], ckp[0][0])->passable() ||
			!currentMap->getTile(ckp[2][1], ckp[2][0])->passable() ||
			!currentMap->getTile(ckp[4][1], ckp[4][0])->passable()) { //x正向无法通过，移动到边界
			dx = ((dx + 31) / 32) * 32 - 9;
			bonceX = true;
#ifdef _DEBUG
			std::cerr << "BOUNCE 4:" << std::endl;
#endif // _DEBUG
		}
	}
	x = dx;
	y = dy;
	if (bonceX)
		velocity.setX(0);
	if (bonceY)
		velocity.setY(0);
#ifdef _DEBUG
	//std::cerr << "Player " << x << ' ' << y << ' ' << currentFrame << std::endl;
#endif
}

void Player::clean() {
	UI::Instance().destroyUI("PlayerHP");
	UI::Instance().destroyUI("PlayerMana");
}
