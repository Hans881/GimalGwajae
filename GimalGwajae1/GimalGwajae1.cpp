#include<bangtal.h>
#include <stdio.h> 

SceneID scene0, scene1;
//배경, 터렛 오브젝트 생성
ObjectID startButton, endButton, playButton, background, turret[2][3];

void startGame() {
	hideObject(startButton);
	hideObject(endButton);

	enterScene(scene1);
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == endButton) {
		endGame();
	}
	else if (object == startButton) {
		startGame();
	}
//완성아님, 배경화면의 특정 지점 클릭시 그 지점에 숨겨져있던 터렛이 보임.
	if (object == background) {
		//타워 배치
		for (int i = 0; i < 2; i++) {
			for (int k = 0; k < 3; k++) {
				if (220 + i * 90 <= x && x <= 310 + i * 90 && k * 90 <= y && y <= 90 + k * 90) {
					showObject(turret[i][k]);
				}
			}
		}
	}

}













int main() {

	setMouseCallback(mouseCallback);

	scene0 = createScene("대기화면", "images/배경1.jpg");
	scene1 = createScene("게임시작", "images/background5.png");

	//배경화면 이미지에 마우스 콜백 적용 위해 배경화면 오브젝트 처리
	background = createObject("images/background5.png");
	locateObject(background, scene1, 0, 0);
	showObject(background);

	//가방에 타워 집어넣기
	



	startButton = createObject("images/start.png");
	locateObject(startButton, scene0, 590, 300);
	showObject(startButton);

	endButton = createObject("images/end.png");
	locateObject(endButton, scene0, 590, 250);
	showObject(endButton);
	//[x칸][y칸]
	for (int i = 1; i>=0; i--) {
		for (int k = 2; k >= 0; k--) {
			turret[i][k] = createObject("images/turret1.png");
			locateObject(turret[i][k], scene1, 235 + i * 90, 15 + k * 90);
			pickObject(turret[i][k]);
		}
	}




	startGame(scene0);
}