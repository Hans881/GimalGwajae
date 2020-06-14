#define _CRT_SECURE_NO_WARNINGS
#include<bangtal.h>
#include <stdio.h> 
#include<stdlib.h>
#include<time.h>

#define bulletterm 1.0f
#define bulletmove 0.01f
#define bulframe 1.0f
#define zombiespeed -2.0f
#define randomspeed 0.8f

//장면 생성
SceneID scene0, scene1;
//배경, 터렛 오브젝트,탄약 갯수 오브젝트 생성
ObjectID startButton, endButton, playButton, background, turret[2][3], turretimg, ammo[2][3], bullet[2][3][3]
, castle
//좀비 오브젝트 생성
, zombie[4][3];

//총알사이의 딜레이 생성
bool delay[2][3];
//나가는 총알의 번호
int bulletnum[2][3];
//터렛 이미지가 생성될 랜덤 x, y좌표
int x_turret, y_turret,
//터렛이 짝수이면 보이고, 홀수이면 안보임
randomkey;
//sprintf를 위한
char ammoimg[10];


//각 총알마다의 움직임 타이머 
TimerID timerbullet[2][3][3],
//총알 발사 간격 타이머
timerdelay[2][3],
//게임 내 계속 체크 하는 타이머
timerframe,
//각 좀비의 움직임 타이머
timerzombie[4][3],
//랜덤 좌표 생성 타이머
timerrandom;

//각 총알의 x좌표
float x_bullet[2][3][3];
float x_zombie[4][3];




//시작함수 
void startGame() {
	hideObject(startButton);
	hideObject(endButton);

	enterScene(scene1);

}

//총알
//x, y는 현재 위치, step_x는 타이머 끝날때 마다 움직이는 x값
void moveObject(ObjectID object, float* x, float y, float step_x) {

	*x = *x + step_x;
	locateObject(object, scene1, *x, y);





}

//좀비 생성
void createzombie() {
	for (int i = 3; i >= 0; i--) {
		for (int k = 2; k >= 0; k--) {
			locateObject(zombie[i][k], scene1, 1290 + i * 190, 15 + k * 90);
			x_zombie[i][k] = 1290 + i * 190;
			showObject(zombie[i][k]);
			setTimer(timerzombie[i][k], bulletmove);
			startTimer(timerzombie[i][k]);
		}

	}

}




//오브젝트 콜백 함수
void objectCallback(ObjectID object, EventID event) {
	if (object == turretimg) {
		//물체를 떨궜으면 랜덤키를 0으로 지정하고 타이머 랜덤을 실행시켜 나머지가 0이면 안보이고 1이면 보이게 한다.
		if (event == EventID::EVENT_DROP_OBJECT) {
			randomkey = 0;
			setTimer(timerrandom, 0.5f);
			startTimer(timerrandom);

		}

	}
}


//마우스 콜백 함수
void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == endButton) {
		endGame();
	}
	else if (object == startButton) {
		startGame();
		startTimer(timerframe);
	}

	//배경화면의 특정 지점 클릭시 그 지점에 숨겨져있던 터렛이 보임
	if (object == background) {

		//타워 배치 타워가 배치 되면 총알 타이머 시작
		if (getHandObject() == turretimg) {
			//좀비 생성

			for (int i = 0; i < 2; i++) {
				for (int k = 0; k < 3; k++) {
					//바닥의 칸을 클릭하면
					if (220 + i * 90 <= x && x <= 310 + i * 90 && k * 90 <= y && y <= 90 + k * 90) {

						//포탑 보이기
						showObject(turret[i][k]);
						//0,0 0,1에 장애가 있다
						//포탑의 잔여 탄약수 보이기
						showObject(ammo[i][k]);
						//불렛 딜레이타이머 시작
						setTimer(timerdelay[i][k], bulletterm);


						for (int t = 2; t >= 0; t--) {
							//총알 출발 시작, 생성된 포탑에서 총알 1개 출발, 그리고 1초뒤에 다시 
							setTimer(timerbullet[i][k][t], bulletmove);
							//총알 초기 좌표
							x_bullet[i][k][t] = 315 + i * 90;
						}
						startTimer(timerdelay[i][k]);
						//그리고 타워를 떨어뜨린다
						dropObject(turretimg);

					}
				}
			}
		}
	}
	//만약 바닥에 떨어진 터렛을 주우면 가방에 들어감
	if (object == turretimg) {
		pickObject(turretimg);
		//왜 use 상태로 바뀌는 것인가.
	}

}


//타이머 함수
void timerCallback(TimerID timer) {


	//게임내에서 지속적으로 0.01초마다 한번씩 체크하는 타이머
	if (timer == timerframe) {
		//같은 레인에서만 판별
		for (int k = 0; k < 3; k++) {
			//먼저 총알이 닿은 것인지 판별
			for (int i = 0; i < 2; i++) {
				//좀비에 닿았는지 판별
				for (int j = 0; j < 4; j++) {
					//몇번째 총알에 닿았는 지 판별
					for (int t = 0; t < 3; t++) {
						//만약 좀비와 총알이 닿으면
						if (x_zombie[j][k] <= x_bullet[i][k][t] + 90 && x_zombie[j][k] + 20 >= x_bullet[i][k][t] + 90) {
							//좀비의 x좌표를 1500으로 옮겨 주어 닿지 않게한다
							x_zombie[j][k] = 1800;
							//총알의 x좌표를 옮겨 재장전 조건에 맞게한다.
							x_bullet[i][k][t] = 1500;
							stopTimer(timerzombie[j][k]);
							stopTimer(timerbullet[i][k][t]);
							locateObject(zombie[j][k], scene1, -200, -200);
							locateObject(bullet[i][k][t], scene1, -200, -200);


						}
					}
				}
			}

		}


		//총알 다썼는지 체크
		for (int i = 1; i >= 0; i--) {
			for (int k = 2; k >= 0; k--) {


				if (bulletnum[i][k] == 3) {
					stopTimer(timerdelay[i][k]);

					//재장전 조건 : 
					if (x_bullet[i][k][0] > 1280 && x_bullet[i][k][1] > 1280 && x_bullet[i][k][2] > 1280) {
						for (int t = 0; t < 3; t++) {
							hideObject(bullet[i][k][t]);
							stopTimer(timerbullet[i][k][t]);
							//locateObject(bullet[i][k][t], scene1, 315 + i * 90, 50 + k * 90);
							x_bullet[i][k][t] = 315 + i * 90;
							setTimer(timerdelay[i][k], bulletterm);
							startTimer(timerdelay[i][k]);
						}
						bulletnum[i][k] = 0;
					}

				}
				setTimer(timerframe, 0.01f);
				startTimer(timerframe);
			}
		}
	}


	//타이머 랜덤
	if (timer == timerrandom) {
		randomkey++;
		//터렛 이미지의 random 좌표를 정한다. 
		x_turret = rand() % 1210;
		y_turret = rand() % 650;
		//randomkey를 2로 나누고 나머지 0이면 안보이고, 나머지가 1이면 보인다.
		if (randomkey % 2 == 0) {
			locateObject(turretimg, scene1, x_turret, y_turret);
			showObject(turretimg);

		}
		else {
			hideObject(turretimg);
			locateObject(turretimg, scene1, x_turret, y_turret);


		}
		setTimer(timerrandom, randomspeed);
		startTimer(timerrandom);
	}


	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 3; k++) {
			if (timer == timerzombie[i][k]) {
				moveObject(zombie[i][k], &x_zombie[i][k], 15 + k * 90, zombiespeed);
				setTimer(timerzombie[i][k], bulletmove);
				startTimer(timerzombie[i][k]);
			}
		}
	}

	for (int i = 0; i <= 1; i++) {
		for (int k = 0; k <= 2; k++) {

			for (int t = 0; t < 3; t++) {
				//타이머 <불렛(움직임)>

				if (timer == timerbullet[i][k][t]) {
					//110에서 나오는 총알 ->50
					locateObject(bullet[i][k][t], scene1, 315 + i * 90, 110 + k * 90);

					moveObject(bullet[i][k][t], &x_bullet[i][k][t], 110 + k * 90, bulframe);

					//printf("%.1f\n", x_bullet[i][k][t]);	
					startTimer(timerbullet[i][k][t]);


				}

			}



			//타이머 <딜레이>가 종료되었을 때

			if (timer == timerdelay[i][k]) {

				//해당 포탑의 불렛 넘이 0부터 1씩 증가하며, bullet[i][k][bulletnum] 오브젝트가 보여지고 타이머불렛[i][k][bulletnum]가 시작한다.(총알이 발사된다)
				showObject(bullet[i][k][bulletnum[i][k]]);
				startTimer(timerbullet[i][k][bulletnum[i][k]]);



				//그리고 현재 아모 이미지를 ammo(3 - 불렛넘버).png로 바꾼다.
				sprintf(ammoimg, "images/ammo%d.png", 2 - bulletnum[i][k]);
				setObjectImage(ammo[i][k], ammoimg);

				//불렛넘버가 3이 되었을 때는 해당 포탑의 타이머 딜레이 함수를 잠시 멈춘다. 그리고 모든 총알이 x좌표 1500을 넘어간 곳에 존재할경우(좀비 피격시 혹은 맵끝 피격시) 재장전을 한다.





				//아니면 딜레이 타이머를 다시 시작한다.
				if (bulletnum[i][k] <= 2) {
					//그리고 불렛넘버가 1더해진다. 


					setTimer(timerdelay[i][k], bulletterm);
					startTimer(timerdelay[i][k]);
					bulletnum[i][k]++;
				}
			}





		}
	}
}











int main() {
	//난수 생성
	srand(time(0));

	setMouseCallback(mouseCallback);
	setObjectCallback(objectCallback);
	setTimerCallback(timerCallback);
	//장면 생성
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
	//터렛 생성후 각 칸에 배치 6개
	for (int i = 1; i >= 0; i--) {
		for (int k = 2; k >= 0; k--) {

			//터렛 생성
			turret[i][k] = createObject("images/turret1.png");
			locateObject(turret[i][k], scene1, 235 + i * 90, 15 + k * 90);

			//터렛 위 탄약 갯수 이미지 생성
			ammo[i][k] = createObject("images/ammo3.png");
			locateObject(ammo[i][k], scene1, 220 + i * 90, 15 + k * 90);

			//딜레이 변수 생성
			delay[i][k] = true;

			//총알 딜레이 타이머 생성
			timerdelay[i][k] = createTimer(bulletterm);

			//각 포탑의 총알 초기화
			bulletnum[i][k] = { 0 };

			for (int t = 0; t <= 2; t++) {
				//각 터렛마다 총알 3개 생성
				bullet[i][k][t] = createObject("images/bullet.png");
				//원래 여기서 총알 위치 나열


				//각 총알의 x좌표 값 저장



				//총알마다 타이머 생성
				timerbullet[i][k][t] = createTimer(3.14f);
			}
		}
	}
	//좀비 생성 타이머 생성
	for (int i = 3; i >= 0; i--) {
		for (int k = 2; k >= 0; k--) {
			zombie[i][k] = createObject("images/zombie1.png");
			timerzombie[i][k] = createTimer(bulletmove);
		}

	}



	//터렛 클릭용 이미지 생성
	turretimg = createObject("images/turret1.png");
	locateObject(turretimg, scene1, 500, 400);
	showObject(turretimg);
	//본진 생성
	castle = createObject("images/castle.png");
	locateObject(castle, scene1, 0, 90);
	showObject(castle);
	//타이머 프레임 생성
	timerframe = createTimer(0.01f);
	//랜덤 타이머 생성
	timerrandom = createTimer(0.5f);

	createzombie();
	startGame(scene0);
}