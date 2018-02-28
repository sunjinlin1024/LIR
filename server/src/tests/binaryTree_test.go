package tests

import (
	"time"
	"math/rand"
	"github.com/davyxu/cellnet/util"
)

type Player struct{
	id int
	x int
}

// var countMap map[uint]uint=make(map[uint]uint)
var playerCount int=10000
var testCount int=10000
var playerList []*Player=make([]*Player,playerCount)

func main() {
	depth:=uint(5)
	maxInx:=(1<<depth)-1
	bTree:=util.NewBinaryTree(depth)
	// for k:=uint(1);k<=depth;k++{
	// 	log.Infof("depth %d child Node count %d",k,countMap[k])
	// }

	for k:=1;k<playerCount;k++{
		p:=&Player{id:k,x:rand.Intn(maxInx)}
		bTree.Append(k,p,uint(p.x))
		playerList[k]=p
		// log.Infof("")
	}

	findNum:=0
	findX:=15
	findOffset:=0

	var findHandle func(p *Player)
	findHandle=func(p *Player)  {
		findNum++
	}

	
	curOffset:=0
	now:=time.Now()
	startT:=float64(now.Unix())+float64(now.UnixNano())/1e9
	for a:=0;a<testCount;a++{
		for k:=1;k<len(playerList);k++{
			curOffset=playerList[k].x-findX
			if curOffset<0 {
				curOffset=-curOffset
			}
			if curOffset<=findOffset{
				findHandle(playerList[k])
			}
		}
	}
	now=time.Now()
	endT:=float64(now.Unix())+float64(now.UnixNano())/1e9
	log.Infof("array findNum %d cost:%.9f-%.9f=%.9f",findNum,endT,startT,endT-startT)



	findNum=0
	now=time.Now()
	startT=float64(now.Unix())+float64(now.UnixNano())/1e9


	var realTest func(interface{},bool)
	realTest=func(p interface{},needRecheck bool){
		p1:=p.(*Player)
		if needRecheck{
			curOffset:=p1.x-findX
			if curOffset<0{
				curOffset=-curOffset
			}
			if curOffset<=findOffset{
				findHandle(p1)
			}
		}else{
			findHandle(p1)
		}
	}
	for a:=0;a<testCount;a++{
		bTree.FindInArea(uint(findX-findOffset),uint(findX+findOffset),realTest)
	}
	now=time.Now()
	endT=float64(now.Unix())+float64(now.UnixNano())/1e9
	log.Infof("bTree findNum %d cost:%.9f-%.9f=%.9f",findNum,endT,startT,endT-startT)

	
	for id,obj:= range bTree.All(){
		player:=obj.(*Player)
		bTree.Remove(id,player.x)
	}
}