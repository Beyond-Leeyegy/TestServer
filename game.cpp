#include "game.h"
#include "message.h"
#include "packet.h"
#include "Bullet.h"
#include "utils.h"
#include "Shape.h"
#include "Setting.h"
#include "EnemyTank.h"
#include "network.h"
#include "def.h"
#include <unistd.h>
/*
发送all_pos数据包

Game::Game(sPCLIENT_DATA Client1 , sPCLIENT_DATA Client2)
{
        pClient1 = Client1;
        pClient2 = Client2;
}

Game::~Game()
{
}


ALL_POS数据包结构：size word->tag word->主战坦克1数据{id:byte,x:int,y:int.dir:int,坦克生命值:byte}
					->友军主战坦克数据{数据同上}
					->子弹类数据{子弹个数:byte,loop{id:byte,x:int,y:int,dir:int,type:byte0代表我军集合，1代表敌军集合}}
					->电脑坦克数据{坦克数:byte,loop{id:byte,x:int,y:int,dir:int}}

要放松 的数据暂时写到数据的缓存区里面
*/
Game::Game(sPCLIENT_DATA Client1 , sPCLIENT_DATA Client2)
{
        pClient1 = Client1;
        pClient2 = Client2;
}

Game::~Game()
{
}

Game::Game(){}
void Game::onSend_ALL_POS() {
	char content_to_1[dMAX_SOCK_BUFF];
	char content_to_2[dMAX_SOCK_BUFF];
	
	//封装用户1的头部

	int pos_1 = 2;
	PutWord(content_to_1, (WORD)dPACKET_ALL_POS, pos_1);
	/*
		MYSELF
	*/
	//id
	PutByte(content_to_1, pClient1->mainTank.get_id(), pos_1);

	//x:int
	PutInteger(content_to_1, pClient1->mainTank.get_pos().GetX(), pos_1);

	//y:int
	PutInteger(content_to_1, pClient1->mainTank.get_pos().GetY(), pos_1);

	//dir:int
	PutInteger(content_to_1, pClient1->mainTank.GetDir(), pos_1);

	//life:byte
	PutByte(content_to_1, pClient1->mainTank.GetLife(), pos_1);

	/*
		FRIEND
	*/
	//id
	PutByte(content_to_1, pClient2->mainTank.get_id(), pos_1);

	//x:int
	PutInteger(content_to_1, pClient2->mainTank.get_pos().GetX(), pos_1);

	//y:int
	PutInteger(content_to_1, pClient2->mainTank.get_pos().GetY(), pos_1);

	//dir:int
	PutInteger(content_to_1, pClient2->mainTank.GetDir(), pos_1);

	//life:byte
	PutByte(content_to_1, pClient2->mainTank.GetLife(), pos_1);



	//封装用户2的头部
	int pos_2 = 2;
	PutWord(content_to_2, (WORD)dPACKET_ALL_POS, pos_2);

	/*
	MYSELF
	*/
	//id
	PutByte(content_to_2, pClient2->mainTank.get_id(), pos_2);

	//x:int
	PutInteger(content_to_2, pClient2->mainTank.get_pos().GetX(), pos_2);

	//y:int
	PutInteger(content_to_2, pClient2->mainTank.get_pos().GetY(), pos_2);

	//dir:int
	PutInteger(content_to_2, pClient2->mainTank.GetDir(), pos_2);

	//life:byte
	PutByte(content_to_2, pClient2->mainTank.GetLife(), pos_2);

	/*
		FRIEND
	*/
	//id
	PutByte(content_to_2, pClient1->mainTank.get_id(), pos_2);

	//x:int
	PutInteger(content_to_2, pClient1->mainTank.get_pos().GetX(), pos_2);

	//y:int
	PutInteger(content_to_2, pClient1->mainTank.get_pos().GetY(), pos_2);

	//dir:int
	PutInteger(content_to_2, pClient1->mainTank.GetDir(), pos_2);

	//life:byte
	PutByte(content_to_2, pClient1->mainTank.GetLife(), pos_2);


	BYTE num;
	BYTE id;
	int x;
	int y;
	int dir;


	// bullet
	PutByte(content_to_1, lstBullets.size() + lstMainTankBullets.size(), pos_1);
	PutByte(content_to_2, lstBullets.size() + lstMainTankBullets.size(), pos_2);

	//enemy bullet
	for (list<Object*>::iterator it = lstBullets.begin(); it != lstBullets.end(); it++)
	{
		//id:BYTE
		PutByte(content_to_1, (*it)->get_id(),pos_1);
		PutByte(content_to_2, (*it)->get_id(), pos_2);


		//x:INT
		PutInteger(content_to_1, (*it)->get_pos().GetX(), pos_1);
		PutInteger(content_to_2, (*it)->get_pos().GetX(), pos_2);

		//y:INT
		PutInteger(content_to_1, (*it)->get_pos().GetY(), pos_1);
		PutInteger(content_to_2, (*it)->get_pos().GetY(), pos_2);

		//dir:INT
		PutInteger(content_to_1, (*it)->GetDir(), pos_1);
		PutInteger(content_to_2, (*it)->GetDir(), pos_2);

		//type:BYTE
		PutByte(content_to_1, 1, pos_1);
		PutByte(content_to_2, 1, pos_2);
	}

	//our bullet
	for (list<Object*>::iterator it = lstMainTankBullets.begin(); it != lstMainTankBullets.end(); it++)
	{
		//id:BYTE
		PutByte(content_to_1, (*it)->get_id(), pos_1);
		PutByte(content_to_2, (*it)->get_id(), pos_2);


		//x:INT
		PutInteger(content_to_1, (*it)->get_pos().GetX(), pos_1);
		PutInteger(content_to_2, (*it)->get_pos().GetX(), pos_2);

		//y:INT
		PutInteger(content_to_1, (*it)->get_pos().GetY(), pos_1);
		PutInteger(content_to_2, (*it)->get_pos().GetY(), pos_2);

		//dir:INT
		PutInteger(content_to_1, (*it)->GetDir(), pos_1);
		PutInteger(content_to_2, (*it)->GetDir(), pos_2);

		//type:BYTE
		PutByte(content_to_1, 0, pos_1);
		PutByte(content_to_2, 0, pos_2);
	}

	// enemyTank{坦克数:byte,loop{id:byte,x:int,y:int,dir:int}
	//num
	PutByte(content_to_1, lstTanks.size(), pos_1);
	PutByte(content_to_2, lstTanks.size(), pos_2);



	//enemyTank
	for (list<Tank*>::iterator it = lstTanks.begin(); it != lstTanks.end(); it++)
	{
		//id:BYTE
		PutByte(content_to_1, (*it)->get_id(), pos_1);
		PutByte(content_to_2, (*it)->get_id(), pos_2);


		//x:INT
		PutInteger(content_to_1, (*it)->get_pos().GetX(), pos_1);
		PutInteger(content_to_2, (*it)->get_pos().GetX(), pos_2);

		//y:INT
		PutInteger(content_to_1, (*it)->get_pos().GetY(), pos_1);
		PutInteger(content_to_2, (*it)->get_pos().GetY(), pos_2);

		//dir:INT
		PutInteger(content_to_1, (*it)->GetDir(), pos_1);
		PutInteger(content_to_2, (*it)->GetDir(), pos_2);

	}
	
	//putsize
	PutSize(content_to_1, (WORD)pos_1);
	PutSize(content_to_2, (WORD)pos_2);


	if (pClient1 != NULL&& pClient1->m_sendSize + pos_1 > dMAX_SOCK_BUFF) {
		log("overflow in client [%d] 's m_sendBuff may resulting in data omitting! \r\n", pClient1->m_scok);
	}
	else if(pClient1 != NULL){
		memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], content_to_1, pos_1);
		pClient1->m_sendSize += pos_1;
		log("[onSend_ALL_POS::cached::%d::%s]\r\n\r\n",pClient1->m_scok,pClient1->m_IP);
	}


	if (pClient2 != NULL && pClient2->m_sendSize + pos_2 > dMAX_SOCK_BUFF) {
		log("overflow in client [%d] 's m_sendBuff may resulting in data omitting! \r\n", pClient2->m_scok);
	}
	else if(pClient2 != NULL)
	{
		memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], content_to_2, pos_2);
		pClient2->m_sendSize += pos_2;
		log("[onSend_ALL_POS::cached::%d::%s]\r\n\r\n",pClient2->m_scok,pClient2->m_IP);
	}
}


/*
发送BOOM_ACT数据包

//boom_act数据包结构：size:word->tag:word->id:byte->id:byte

*/
void Game::onSend_BOOM_ACT(BYTE id_1,BYTE id_2) {
	int pos_1= 2;
	int pos_2 = 2;

	char content_to_1[dMAX_SOCK_BUFF];
	char content_to_2[dMAX_SOCK_BUFF];

	//tag:word
	PutWord(content_to_1, (WORD)dPACKET_BOOM_ACT, pos_1);
	PutWord(content_to_2, (WORD)dPACKET_BOOM_ACT, pos_2);


	//id1:byte
	PutByte(content_to_1, id_1, pos_1);
	PutByte(content_to_2, id_1, pos_2);

	//id2:byte
	PutByte(content_to_1, id_2, pos_1);
	PutByte(content_to_2, id_2, pos_2);

	//putsize
	PutSize(content_to_1, (WORD)pos_1);
	PutSize(content_to_2, (WORD)pos_2);

	if (pClient1->m_sendSize + pos_1 > dMAX_SOCK_BUFF) {
		log("overflow in client [%d] 's m_sendBuff may resulting in data omitting! \r\n", pClient1->m_scok);
	}
	else {
		memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], content_to_1, pos_1);
		pClient1->m_sendSize += pos_1;
		log("[onSend_BOOM_ACT::cached::%d::%s\r\n\r\n]",pClient1->m_scok,pClient1->m_IP);

	}


	if (pClient2->m_sendSize + pos_2 > dMAX_SOCK_BUFF) {
		log("overflow in client [%d] 's m_sendBuff may resulting in data omitting! \r\n", pClient2->m_scok);
	}
	else
	{
		memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], content_to_2, pos_2);
		pClient2->m_sendSize += pos_2;
		log("[onSend_BOOM_ACT::cached::%d::%s]\r\n\r\n",pClient2->m_scok,pClient2->m_IP);
	}

}

/*
发送 FRIEND数据包

FRIEND数据包结构：size:word->tag:word->ip:string->nickName:string
分别为client1加入client2的数据包、为client2加入client1的数据包
*/
void Game::onSend_FRIEND() {
	//client
	int pos_1 = 2;
	int pos_2 = 2;

	char content_to_1[dMAX_SOCK_BUFF];
	char content_to_2[dMAX_SOCK_BUFF];

	/*
		client1
	*/	
	//tag:word
	PutWord(content_to_1, dPACKET_FRIEND, pos_1);

	//ip:string
	PutString(content_to_1, pClient2->m_IP, pos_1);

	//nickName:string
	PutString(content_to_1, pClient2->m_Name, pos_1);

	//put size
	PutSize(content_to_1, (WORD)pos_1);

	/*
		client2
	*/
	//tag:word
	PutWord(content_to_2, dPACKET_FRIEND, pos_2);

	//ip:string
	PutString(content_to_2, pClient1->m_IP, pos_2);

	//nickName:string
	PutString(content_to_2, pClient1->m_Name, pos_2);

	//put size
	PutSize(content_to_2, (WORD)pos_2);

	/*
		cache
	*/

	if (pClient1->m_sendSize + pos_1 > dMAX_SOCK_BUFF) {
		log("overflow in client [%d] 's m_sendBuff may resulting in data omitting! \r\n", pClient1->m_scok);
	}
	else {
		memcpy(&pClient1->m_sendBuff[pClient1->m_sendSize], content_to_1, pos_1);
		pClient1->m_sendSize += pos_1;
		log("[onSend_FRIEND::cached::%d::%s]\r\n\r\n",pClient1->m_scok,pClient1->m_IP);

	}


	if (pClient2->m_sendSize + pos_2 > dMAX_SOCK_BUFF) {
		log("overflow in client [%d] 's m_sendBuff may resulting in data omitting! \r\n", pClient2->m_scok);
	}
	else
	{
		memcpy(&pClient2->m_sendBuff[pClient2->m_sendSize], content_to_2, pos_2);
		pClient2->m_sendSize += pos_2;
		log("[onSend_FRIEND::cached::%d::%s\r\n\r\n]",pClient2->m_scok,pClient2->m_IP);
	}

}


/*
初始化，设置两个client都是inGame+发送一次OnSendFriend包
*/
void Game::init() {
	pClient1->in_Game = 1;
	pClient2->in_Game = 1;

	//sending onSendFriend
	onSend_FRIEND();
}


void Game::Game_Logic() {
	/*
	检查碰撞，如果有碰撞就置disappear
	*/
	CheckCrash();

	// New Game Level
	if (Setting::m_bNewLevel)
	{
		Setting::m_bNewLevel = false;

		Setting::NewGameLevel();


		//should add a new 数据包来传送游戏的信息
		//Graphic::ShowGameLevel(Setting::GetGameLevel());

		for (int i = 0; i < Setting::GetTankNum(); i++)
		{
			EnemyTank* p = new EnemyTank(width, height);
			lstTanks.push_back(p);
		}

		// 设置暂停，按Enter开始
		//skip = true;
		//continue;
		return;
	}

	/*
	用户1游戏结束
	//不论是哪个用户由于游戏结束或者网络断开连接，服务器都必须保护另一个仍在游戏中的用户的游戏体验

	//
	//*/
	//if (pClient1 == NULL||pClient1->mainTank.IsDisappear())
	//{
	//	//skip = true;
	//	//bGameOver = true;

	//	//Graphic::ShowGameOver();

	//	//continue;
	//}

	///*
	//用户2游戏结束
	//omitting
	//*/

	//if (pClient2->mainTank.IsDisappear()) {

	//	continue;
	//}

	////按照单机版的逻辑：如果主战坦克消失了，那么下面的代码就不应该执行了


	//move one step
	/*
		没有消失 && 网络连接正常
	*/
	if (pClient1 != NULL && !pClient1->mainTank.IsDisappear()) {
		log("[DEBUG::%d::%s]mainTank MOVE\r\n\r\n\r\n",pClient1->m_scok,pClient1->m_IP);
		pClient1->mainTank.Move(start_x,end_x,start_y,end_y);
	}

	if (pClient2 != NULL && !pClient2->mainTank.IsDisappear()) {
		log("[DEBUG::%d::%s]mainTank MOVE\r\n\r\n\r\n",pClient2->m_scok,pClient2->m_IP);

		pClient2->mainTank.Move(start_x, end_x, start_y, end_y);
	}


	//mainTank.Display();

	/* Move Tanks and Send BOOM_ACT */
	for (list<Tank*>::iterator it = lstTanks.begin(); it != lstTanks.end();)
	{
	log("[DEBUG:ENEMYMOVE]\r\n\r\n");
		(*it)->Move(start_x, end_x, start_y, end_y);

		if ((*it)->IsDisappear())
		{
			/*
				计算在本关中剩余的坦克数、总共所得分、击毁的坦克总量；
				根据现有的坦克数设计是否需要开启新的一关
			*/
			Setting::TankDamaged();

			// Add a bomb
			//(*it)->Boom(lstBombs);

			// Delete the tank
			delete *it;
			it = lstTanks.erase(it);
			continue;
		}

		//(*it)->Display();

		//如果敌军坦克射击子弹
		if ((*it)->NeedShoot())
		{
			EnemyTank* p = (EnemyTank*)*it;
			p->Shoot(lstBullets);
		}
		it++;
	}

	/* Draw Bullets */
	for (list<Object*>::iterator it = lstMainTankBullets.begin(); it != lstMainTankBullets.end();)
	{
		(*it)->Move(start_x, end_x, start_y, end_y);

		if ((*it)->IsDisappear())
		{
			// Add a bomb
			//(*it)->Boom(lstBombs);

			// Delete the bullet
			delete *it;
			it = lstMainTankBullets.erase(it);
			continue;
		}

		//(*it)->Display();
		it++;
	}

	for (list<Object*>::iterator it = lstBullets.begin(); it != lstBullets.end();)
	{
		(*it)->Move(start_x, end_x, start_y, end_y);

		if ((*it)->IsDisappear())
		{
			// Add a bomb
			//(*it)->Boom(lstBombs);

			// Delete the bullet
			delete *it;
			it = lstBullets.erase(it);
			continue;
		}

		//(*it)->Display();
		it++;
	}

	///* Draw Bombs */
	//for (list<Object*>::iterator it = lstBombs.begin(); it != lstBombs.end();)
	//{
	//	(*it)->Move();

	//	if ((*it)->IsDisappear())
	//	{
	//		delete *it;
	//		it = lstBombs.erase(it);
	//		continue;
	//	}

	//	(*it)->Display();
	//	it++;
	//}
}


void Game::Handling_Info() {
	fd_set read_set;
	fd_set write_set;
	fd_set exc_set;


	
	/*
		FD_SET INIT
	*/

	FD_ZERO(&read_set);
	FD_ZERO(&write_set);
	FD_ZERO(&exc_set);

	/*
		binding
	*/
	if (pClient1 != NULL) {
		FD_SET(pClient1->m_scok, &read_set);
		FD_SET(pClient1->m_scok, &write_set);
		FD_SET(pClient1->m_scok, &exc_set);
	}

	if (pClient2 != NULL) {
		FD_SET(pClient2->m_scok, &read_set);
		FD_SET(pClient2->m_scok, &write_set);
		FD_SET(pClient2->m_scok, &exc_set);
	}
	
	/*
		GET NFDS
	*/
	SOCKET nfds;
	if(pClient1!=NULL&&pClient2!=NULL){
	 nfds = pClient1->m_scok>pClient2->m_scok ? pClient1->m_scok : pClient2->m_scok;
	}else if(pClient1 != NULL){
		nfds = pClient1->m_scok;
	}else if(pClient2 != NULL){
		nfds = pClient2->m_scok;
	}else{
		log("[DISCONNECT::BOTH]\r\n");
		return;
	}
	/*
		SET TIME
	*/
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	/*
		CALL SELECT
	*/
	if(select(nfds+1,&read_set,&write_set,&exc_set,&tv)<-1){
		log("[Error::select::subThread]\r\n");
		return;
	}

	/*
		CHECK
	*/
	if (pClient1 != NULL && (FD_ISSET(pClient1->m_scok, &exc_set)|| FD_ISSET(pClient2->m_scok, &exc_set))) {
		//OMITTING

		DisconnectClient(pClient1);
		//LIST_SKIP(client, next_client);
	}

	/*
		NO RESPONSE OVER 1 MIN
		OMITTING

	*/
	if (pClient1 != NULL && (pClient1->m_lastRecvTime + 60000 <= timeGetTime()|| pClient2->m_lastRecvTime + 60000 <= timeGetTime())) {
		DisconnectClient(pClient1);
		//LIST_SKIP(client, next_client);
	}


	/*
		try to recv
	*/

	//client1
	if (pClient1 != NULL && FD_ISSET(pClient1->m_scok, &read_set)) {

		if (!RecvFromClient(pClient1)) {
			DisconnectClient(pClient1);
			//LIST_SKIP(client, next_client);
		}
	}

	//client2
	if (pClient2 != NULL && FD_ISSET(pClient2->m_scok, &read_set)) {

		if (!RecvFromClient(pClient2)) {
			DisconnectClient(pClient2);
			//LIST_SKIP(client, next_client);
		}
	}

	/*
		handle all recv
	*/
	//client1
	if (pClient1 != NULL && pClient1->m_recvSize) {
		if (!ReadRecvBuff(pClient1,*this))
		{
			DisconnectClient(pClient1);
			//LIST_SKIP(client, next_client);
		}
	}

	//client2
	if (pClient2 != NULL && pClient2->m_recvSize) {
		if (!ReadRecvBuff(pClient2 ,*this))
		{
			DisconnectClient(pClient2);
			//LIST_SKIP(client, next_client);
		}
	}

	/*
		SEND
	*/
//	//client1
	if (pClient1 != NULL && pClient1->m_sendSize && FD_ISSET(pClient1->m_scok, &write_set)) {

		if (FlushSendBuff(pClient1) < 0) {
			DisconnectClient(pClient1);
			//LIST_SKIP(client, next_client);
		}
	}
	
	//client2
	if (pClient2 != NULL && pClient2->m_sendSize && FD_ISSET(pClient2->m_scok, &write_set)) {
		if (FlushSendBuff(pClient2) < 0) {
			DisconnectClient(pClient2);
			//LIST_SKIP(client, next_client);
		}
	}
}



void Game::runGame() {
	while (true) {
		if (pClient1 == NULL && pClient2 == NULL) {
			return;
		}
		
		//game logic
		Game_Logic();

		//sending all_pos
		onSend_ALL_POS();

		//handling_info
		Handling_Info();
		
		usleep(1000000);

		if (pClient1 != NULL && pClient2!=NULL && pClient1->mainTank.IsDisappear() && pClient2->mainTank.IsDisappear()) {
			pClient1->is_Ready = 0;
			pClient1->in_Game = 0;
			pClient1->is_Ready = 0;
			pClient1->in_Game = 0;

			DisconnectClient(pClient1);
			DisconnectClient(pClient2);

			break;
		}
	}
}



void Game::CheckCrash()
{
	// Check enermy tank damage
	for (list<Object*>::iterator it = lstMainTankBullets.begin(); it != lstMainTankBullets.end(); it++)
	{
		for (list<Tank*>::iterator itt = lstTanks.begin(); itt != lstTanks.end(); itt++)
		{
			if (Shape::CheckIntersect((*it)->GetSphere(), (*itt)->GetSphere()))
			{
				
				(*itt)->SetDisappear();
				(*it)->SetDisappear();
				onSend_BOOM_ACT((*itt)->get_id(), (*it)->get_id());
			}
		}
	}

	// Check main tank damage
	for (list<Object*>::iterator it = lstBullets.begin(); it != lstBullets.end(); it++)
	{
		if (Shape::CheckIntersect((*it)->GetSphere(), pClient1->mainTank.GetSphere()))
		{
			pClient1->mainTank.SetLife(pClient1->mainTank.GetLife() - 1);

			if (Setting::GetLife() > 0)
			{
				(*it)->SetDisappear();
				onSend_BOOM_ACT((*it)->get_id(), (*it)->get_id());
			}
			else
			{
				pClient1->mainTank.SetDisappear();
				onSend_BOOM_ACT(pClient1->mainTank.get_id(), pClient1->mainTank.get_id());
			}
		}

		if (Shape::CheckIntersect((*it)->GetSphere(), pClient2->mainTank.GetSphere()))
		{

			pClient2->mainTank.SetLife(pClient2->mainTank.GetLife() - 1);

			if (Setting::GetLife() > 0)
			{
				(*it)->SetDisappear();
				onSend_BOOM_ACT((*it)->get_id(), (*it)->get_id());
			}
			else
			{
				pClient2->mainTank.SetDisappear();
				onSend_BOOM_ACT(pClient2->mainTank.get_id(), pClient2->mainTank.get_id());

			}
		}
	}
}


void Game::Dispose()
{
	for (list<Tank*>::iterator it = lstTanks.begin(); it != lstTanks.end(); it++)
	{
		delete *it;
	}
	lstTanks.clear();

	for (list<Object*>::iterator it = lstMainTankBullets.begin(); it != lstMainTankBullets.end(); it++)
	{
		delete *it;
	}

	for (list<Object*>::iterator it = lstBullets.begin(); it != lstBullets.end(); it++)
	{
		delete *it;
	}
	lstBullets.clear();

	for (list<Object*>::iterator it = lstBombs.begin(); it != lstBombs.end(); it++)
	{
		delete *it;
	}
	lstBombs.clear();

	//Graphic::Destroy();
}
