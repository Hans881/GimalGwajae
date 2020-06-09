#include<bangtal.h>
#include <stdio.h> 

SceneID scene0, scene1;

ObjectID startButton, endButton, playButton;

void startGame() {
	hideObject(startButton);
	hideObject(endButton);

	enterScene(scene1);
}
//확인
void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == endButton) {
		endGame();
	}
	else if (object == startButton) {
		startGame();
	}
}

int main() {

	setMouseCallback(mouseCallback);

	scene0 = createScene("대기화면", "images/배경1.jpg");
	scene1 = createScene("게임시작", "images/배경1.jpg");


	startButton = createObject("images/start.png");
	locateObject(startButton, scene0, 590, 300);
	showObject(startButton);

	endButton = createObject("images/end.png");
	locateObject(endButton, scene0, 590, 250);
	showObject(endButton);

	startGame(scene0);
}